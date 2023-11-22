#include "WebServer.hpp"
#include "lwipopts.h"

struct httpState {
    WebServer* webserer;
    tcp_pcb* socket;
    int tries;
};

enum class ParseResult {
    DONE,
    IN_PROGRESS,
    ERROR
};

const char* to_char(ParseResult result){
    switch(result) {
        case ParseResult::DONE:
            return "DONE";
        case ParseResult::IN_PROGRESS:
            return "IN_PROGRESS";
        case ParseResult::ERROR:
            return "ERROR";
        default:
            return "???";
    }
}

struct ParseProgress {
    ParseResult result;
    uint32_t end_at = 0;
};

static err_t close_connection(tcp_pcb* pcb, httpState * hs, bool abort = false);

ParseProgress parseHttpVersion(Request& request, char* data, uint32_t data_size) {
    if(data[0] != 'H'
       || data[1] != 'T'
       || data[2] != 'T'
       || data[3] != 'P'
       || data[4] != '/')
        return {ParseResult::ERROR, 0};

    if(data[5] == '1'
       && data[6] == '.'
       && data[7] == '1') {
        request.version = Version::HTTP_1_1;
    }
    else if(data[5] == '1'
            && data[6] == '.'
            && data[7] == '0'){
        request.version = Version::HTTP_1_1;
    }
    else{
        request.version = Version::OTHER;
    }

    return {ParseResult::DONE, 7};
}

ParseProgress parseUri(Request& request, char* data, uint32_t data_size) {
    auto max_size = data_size < sizeof(request.uri) ? data_size : sizeof(request.uri);

    uint32_t i = 0;
    for(i; i < max_size && data[i] != ' ' && data[i] != '?'; ++i) {
        request.uri[i] = data[i];
    }
    request.uri[i] = 0;

    return {ParseResult::DONE, i};
}

ParseProgress parseMethod(Request& request, char* data, uint32_t data_size) {
    if(data[0] == 'G' && data[1] == 'E' && data[2] == 'T'){
        request.method = MethodType::GET;
        return {ParseResult::DONE, 3};
    }
    else if(data[0] == 'P' && data[1] == 'O' && data[2] == 'S' && data[3] == 'T'){
        request.method = MethodType::POST;
        return {ParseResult::DONE, 4};
    }
    else if(data[0] == 'H' && data[1] == 'E' && data[2] == 'A' && data[3] == 'D'){
        request.method = MethodType::HEAD;
        return {ParseResult::DONE, 4};
    }
    else if(data[0] == 'P' && data[1] == 'U' && data[2] == 'T'){
        request.method = MethodType::PUT;
        return {ParseResult::DONE, 3};
    }
    else if(data[0] == 'D' && data[1] == 'E' && data[2] == 'L' && data[3] == 'E' && data[4] == 'T' && data[5] == 'E'){
        request.method = MethodType::DELETE;
        return {ParseResult::DONE, 6};
    }
    else if(data[0] == 'O' && data[1] == 'P' && data[2] == 'T' && data[3] == 'I' && data[4] == 'O' && data[5] == 'N' && data[6] == 'S'){
        request.method = MethodType::OPTIONS;
        return {ParseResult::DONE, 7};
    }
    else if(data[0] == 'T' && data[1] == 'R' && data[2] == 'A' && data[3] == 'C' && data[4] == 'E'){
        request.method = MethodType::TRACE;
        return {ParseResult::DONE, 5};
    }
    else if(data[0] == 'C' && data[1] == 'O' && data[2] == 'N' && data[3] == 'N' && data[4] == 'E' && data[5] == 'C' && data[6] == 'T'){
        request.method = MethodType::CONNECT;
        return {ParseResult::DONE, 7};
    }
    else if(data[0] == 'P' && data[1] == 'A' && data[2] == 'T' && data[3] == 'C' && data[4] == 'H'){
        request.method = MethodType::PATCH;
        return {ParseResult::DONE, 5};
    }
    else {
        request.method = MethodType::OTHER_WRONG;
        return {ParseResult::ERROR, 0};
    }
}

ParseProgress parseHeader(Request& request, char* data, uint32_t data_size) {
    //printf("parseHeader  request.usedHeaders %d sizeof %d \n", request.usedHeaders, (sizeof(Request::headers) / sizeof(Header)));
    if(request.usedHeaders >= (sizeof(Request::headers) / sizeof(Header)))
        return {ParseResult::DONE, 0};

    //printf("parseHeader  data[0] %d data[1] %d\n",data[0], data[1]);
    if(data[0] == '\r' && data[1] == '\n')
        return {ParseResult::DONE, 2};

    //Find and copy name
    auto separator_pos = 0u;
    for(separator_pos; separator_pos < data_size && data[separator_pos] != ':' && data[separator_pos] != '\n'; ++separator_pos){}
    //printf("separator_pos %d\n", separator_pos);

    if(separator_pos == data_size)
        return {ParseResult::ERROR, 0};

    auto name_length = sizeof(Header::name) < separator_pos ? sizeof(Header::name): separator_pos;
    for(int i = 0; i < name_length; ++i)
        request.headers[request.usedHeaders].name[i] = data[i];
    request.headers[request.usedHeaders].name[sizeof(Header::name) -1] = 0;

    //Find and copy value
    data += separator_pos + 2;
    data_size -= separator_pos + 2;
    auto value_pos = 0u;
    for(value_pos; value_pos < data_size && data[value_pos] != '\n'; ++value_pos){}

    if(value_pos == data_size)
        return {ParseResult::ERROR, 0};

    auto value_length = sizeof(Header::value) < value_pos ? sizeof(Header::value): value_pos;
    for(int i = 0; i < value_length; ++i)
        request.headers[request.usedHeaders].value[i] = data[i];
    request.headers[request.usedHeaders].value[sizeof(Header::value) -1] = 0;

    ++request.usedHeaders;
    return {ParseResult::IN_PROGRESS, separator_pos+ value_pos + 3};
}

ParseProgress parseParameters(Request& request, char* data, uint32_t data_size, Parameter::Type type) {
    //printf("parseHeader  request.usedHeaders %d sizeof %d \n", request.usedHeaders, (sizeof(Request::headers) / sizeof(Header)));
    if(request.usedParameters >= (sizeof(Request::parameters) / sizeof(Parameter)))
        return {ParseResult::DONE, 0};

    if(data[0] == ' ')
        return {ParseResult::DONE, 1};

    if(data[0] == '&')
        return {ParseResult::IN_PROGRESS, 1};

    //Find and copy name
    auto separator_pos = 0u;
    for(separator_pos; separator_pos < data_size && data[separator_pos] != '='  && data[separator_pos] != ' '; ++separator_pos){}
    //printf("separator_pos %d\n", separator_pos);

    if(separator_pos == data_size)
        return {ParseResult::ERROR, 0};

    auto name_length = sizeof(Parameter::name)-1 < separator_pos ? sizeof(Parameter::name)-1: separator_pos;
    for(int i = 0; i < name_length; ++i)
        request.parameters[request.usedParameters].name[i] = data[i];
    request.parameters[request.usedParameters].name[name_length] = 0;

    //Find and copy value
    data += separator_pos + 1;
    data_size -= separator_pos + 1;
    auto value_pos = 0u;
    for(value_pos; value_pos < data_size && data[value_pos] != '&' && data[value_pos] != ' '; ++value_pos){}

    if(value_pos == data_size)
        return {ParseResult::ERROR, 0};

    auto value_length = sizeof(Parameter::value) -1 < value_pos ? sizeof(Parameter::value)-1: value_pos;
    for(int i = 0; i < value_length; ++i)
        request.parameters[request.usedParameters].value[i] = data[i];
    request.parameters[request.usedParameters].value[value_length] = 0;
    request.parameters[request.usedParameters].type = type;

    ++request.usedParameters;
    return {ParseResult::IN_PROGRESS, separator_pos+ value_pos+1};
}

ParseResult parseRequest(char* data, Request& request) {
    constexpr int CLFR_SIZE = 2;
    auto offset = 0;
    auto result = parseMethod(request, data, 1024);
    if(result.result == ParseResult::ERROR)
        return ParseResult::ERROR;
    offset += result.end_at + 1;

    result = parseUri(request, data + offset, 1024 - offset);
    if(result.result == ParseResult::ERROR)
        return ParseResult::ERROR;
    offset += result.end_at;

    if(data[offset] == '?'){
        result.result == ParseResult::IN_PROGRESS;
        ++offset;
        auto maxParameters = sizeof(Request::parameters) / sizeof(Parameter);
        for(int i = 0; i < maxParameters; ++i) {
            result = parseParameters(request, data + offset, 1024 - offset, Parameter::Type::GET);
            if(result.result != ParseResult::IN_PROGRESS)
                break;

            offset += result.end_at;
        }
    }
    ++offset;

    result = parseHttpVersion(request, data + offset, 1024 - offset);
    if(result.result == ParseResult::ERROR)
        return ParseResult::ERROR;
    offset += result.end_at + 1 + CLFR_SIZE;

    result.result == ParseResult::IN_PROGRESS;
    auto maxHeaders = sizeof(Request::headers) / sizeof(Header);
    for(int i = 0; i < maxHeaders ; ++i){
        result = parseHeader(request, data + offset, 1024 - offset);
        if(result.result != ParseResult::IN_PROGRESS)
            break;

        offset += result.end_at;
    }
    return result.result;
}

uint32_t strlen(const char* str) {
    uint32_t len = 0;
    for(len; str[len] != 0; ++len){}
    return len;
}

static Response prepareResponse(tcp_pcb* pcb, const Request& request) {
    Response response;

    response.write("OK", strlen("OK"));

    return response;
}

static err_t sendHeaders(tcp_pcb* pcb, const Response& response) {
    const char* headers = "HTTP/1.1 200 OK\r\n"
        "Server: YALC webserver\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 2\r\n"
        "\r\n"
        "OK";

    tcp_write(pcb, headers, strlen(headers), TCP_WRITE_FLAG_COPY);
    tcp_output(pcb);

    return ERR_OK;
}

static err_t sendResponse(tcp_pcb* pcb, const Response& response){
    tcp_write(pcb, response.payload, response.totalSize, TCP_WRITE_FLAG_COPY);
    tcp_output(pcb);

    return ERR_OK;
}

static err_t http_recv(void *arg, tcp_pcb *pcb, pbuf *p, err_t err)
{
    httpState *hs = (httpState *)arg;
    printf("http_recv: len %d, tot_len %d \n", p->len, p->tot_len);

    if ((err != ERR_OK) || (p == NULL) || (hs == NULL)) {
        /* error or closed by other side? */
        if (p != NULL) {
            /* Inform TCP that we have taken the data. */
            tcp_recved(pcb, p->tot_len);
            pbuf_free(p);
        }
        if (hs == NULL) {
            /* this should not happen, only to be robust */
        }

        close_connection(pcb, hs);
        return ERR_OK;
    }

    char part[1024] = {0};
    pbuf_copy_partial(p, part, sizeof(part), 0);
    part[1023] = 0;

    Request request;
    parseRequest(part, request);
    debug_print(request);
    auto response = prepareResponse(pcb, request);

    sendHeaders(pcb, response);
    sendResponse(pcb, response);

    tcp_recved(pcb, p->tot_len);
    pbuf_free(p);

    return ERR_OK;
}

static void http_err(void *arg, err_t err)
{
    httpState *hs = (httpState *)arg;
    printf("http_err\n");

    if (hs != NULL) {
        mem_free(hs);
    }
}

static err_t close_connection(tcp_pcb* pcb, httpState * hs, bool abort) {
    tcp_arg(pcb, NULL);
    tcp_recv(pcb, NULL);
    tcp_err(pcb, NULL);
    tcp_poll(pcb, NULL, 0);
    tcp_sent(pcb, NULL);
    if(!abort){
        tcp_close(pcb);
    }
    else{
        tcp_abort(pcb);
    }

    mem_free(hs);

    return ERR_OK;
}

static err_t http_poll(void *arg, tcp_pcb *pcb)
{
    httpState *hs = (httpState *)arg;
    printf("http_poll\n");

    if(hs == NULL) {
        return close_connection(pcb, hs);
    }

    if(hs->tries == 5){
        printf("Max tries. Closing\n");
        return close_connection(pcb, hs);
    }

    ++hs->tries;

    return ERR_OK;
}

static err_t http_sent(void *arg, tcp_pcb *pcb, u16_t len)
{
    httpState *hs = (httpState *)arg;
    printf("http_sent\n");

    if (hs == NULL) {
        return ERR_OK;
    }

    close_connection(pcb, hs, false);

    return ERR_OK;
}

static err_t tcp_accept(void *arg, tcp_pcb *newpcb, err_t err)
{
    WebServer* server = reinterpret_cast<WebServer*>(arg);

    if ((err != ERR_OK) || (newpcb == NULL) || (arg == NULL)) {
        return ERR_VAL;
    }

    httpState* state = (httpState*)mem_malloc(sizeof(httpState));
    if(!state){
        printf("Failed to allocated state\n");
        return ERR_MEM;
    }

    state->socket = newpcb;
    state->webserer = server;
    state->tries = 0;
    tcp_arg(newpcb, state);

    tcp_recv(newpcb, http_recv);
    tcp_err(newpcb, http_err);
    tcp_poll(newpcb, http_poll, 4);
    tcp_sent(newpcb, http_sent);

    return ERR_OK;
}

bool WebServer::init(uint16_t port)
{
    socket = tcp_new();
    if(!socket) {
        printf("Failed tcp_new\n");
        return false;
    }

    if(tcp_bind(socket, IP_ANY_TYPE, port) != ERR_OK){
        printf("Failed tcp_bind\n");
        return false;
    }

    socket =  tcp_listen(socket);
    if(!socket) {
        printf("Failed tcp_listen\n");
        return false;
    }

    tcp_arg(socket, this);
    tcp_accept(socket, tcp_accept);

    printf("Setup webserver on port %d\n", port);
    return true;
}

Response &Response::write(const char *data, uint32_t size)
{
    for(int i = 0; i < size; ++i)
        payload[totalSize + i] = data[i];

    totalSize += size;

    return *this;
}
