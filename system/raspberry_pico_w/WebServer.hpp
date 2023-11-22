#pragma once
#include "lwipopts.h"
#include "lwip/api.h"
#include "lwip/tcp.h"

#include <string>
#include <vector>

enum MethodType {GET = 1 << 0,
                 POST = 1 << 1,
                 HEAD = 1 << 2,
                 PUT = 1 << 3,
                 DELETE = 1 << 4,
                 OPTIONS = 1 << 5,
                 TRACE = 1 << 6,
                 CONNECT = 1 << 7,
                 PATCH = 1 << 8,
                 OTHER_WRONG = 1 << 9};

static const char* to_char(MethodType type) {
    switch(type) {
        case MethodType::GET:
            return "GET";
        case MethodType::POST:
            return "POST";
        case MethodType::HEAD:
            return "HEAD";
        case MethodType::PUT:
            return "PUT";
        case MethodType::DELETE:
            return "DELETE";
        case MethodType::OPTIONS:
            return "OPTIONS";
        case MethodType::TRACE:
            return "TRACE";
        case MethodType::CONNECT:
            return "CONNECT";
        case MethodType::PATCH:
            return "PATCH";
        default:
            return "OTHER_WRONG";
    }
}

enum class Version {
    HTTP_1_1,
    HTTP_1_0,
    OTHER
};

static const char* to_char(Version version){
    switch(version){
        case Version::HTTP_1_0:
            return "HTTP 1.0";
        case Version::HTTP_1_1:
            return "HTTP 1.1";
        default:
            return "OTHER";
    }
}

struct Header {
    std::string name;
    std::string value;
};

namespace Headers {
    namespace ContentType {
        static Header XHTML {"Content-Type", "application/xhtml+xml; charset=utf-8"};
        static Header text_html {"Content-Type", "text/html"};
    }
}

struct Parameter {
    enum Type {POST, GET};

    std::string name;
    std::string value;
    Type type;
};

static const char* to_char(Parameter::Type type) {
    return type == Parameter::GET? "GET": "POST";
}

enum class ResponseCode {
    CONTINUE = 100,
    SWITCHING_PROTOCOLS = 101,
    CONNECTION_TIMED_OUT = 102,
    CONNECTION_REFUSED = 111,
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NON_AUTHORITATIVE_INFORMATION = 203,
    NO_CONTENT = 204,
    RESET_CONTENT = 205,
    PARTIAL_CONTENT = 206,
    MULTIPLE_CHOICES = 300,
    MOVED_PERMANENTLY = 301,
    FOUND = 302,
    SEE_OTHER = 303,
    NOT_MODIFIED = 304,
    USE_PROXY = 305,
    SWITCH_PROXY = 306,
    TEMPORARY_REDIRECT = 307,
    TOO_MANY_REDIRECTS = 310,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    PAYMENT_REQUIRED = 402,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    NOT_ACCEPTABLE = 406,
    PROXY_AUTHENTICATION_REQUIRED = 407,
    REQUEST_TIMEOUT = 408,
    CONFLICT = 409,
    GONE = 410,
    LENGTH_REQUIRED = 411,
    PRECONDITION_FAILED = 412,
    REQUEST_ENTITY_TOO_LARGE = 413,
    REQUEST_URI_TOO_LONG = 414,
    UNSUPPORTED_MEDIA_TYPE = 415,
    REQUEST_RANGE_NOT_SATISFIABLE = 416,
    EXPECTATION_FAILED = 417,
    I_M_TEAPOT = 418,
    MISDIRECTED_REQUEST = 421,
    UNPROCESSABLE_ENTITY = 422,
    LOCKED = 423,
    FAILED_DEPENDENCY = 424,
    TOO_EARLY = 425,
    UPGRADE_REQUIRED = 426,
    PRECONDITION_REQUIRED = 428,
    TOO_MANY_REQUESTS = 429,
    REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    UNAVAILABLE_FOR_LEGAL_REASONS = 451,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505,
    VARIANT_ALSO_NEGOTIATES = 506,
    INSUFFICIENT_STORAGE = 507,
    LOOP_DETECTED = 508,
    BANDWIDTH_LIMIT_EXCEEDED = 509,
    NOT_EXTENDED = 510,
    NETWORK_AUTHENTICATION_REQUIRED = 511
};

struct Request {
    Version version;
    MethodType method;
    std::string uri;
    std::vector<Header> headers;
    std::vector<Parameter> parameters;
};

struct Response {
    ResponseCode code = ResponseCode::OK;
    std::string payload;
    std::vector<Header> headers;

    Response& write(const std::string& data);
};

struct RequestHandler {
    std::string uri;
    Response (*handler)(void* obj, const Request& request);
    void* handlerObject;
};

static void debug_print(const Request& request) {
    printf("Request:\n");
    printf("Version: %s\n", to_char(request.version));
    printf("Method: %s\n", to_char(request.method));
    printf("Uri: %s\n", request.uri.c_str());

    printf("Headers: %d\n", request.headers.size());
    for(int i = 0; i < request.headers.size(); ++i){
        printf("\t%s:%s\n", request.headers[i].name.c_str(), request.headers[i].value.c_str());
    }

    printf("Parameters: %d\n", request.parameters.size());
    for(int i = 0; i < request.parameters.size(); ++i){
        printf("\t[%s]%s:%s\n", to_char(request.parameters[i].type), request.parameters[i].name.c_str(), request.parameters[i].value.c_str());
    }
}

static void printRequestLight(const Request& request) {
    printf("[HTTP][%s]%s\n", to_char(request.method), request.uri.c_str());
}

class WebServer {
public:
    bool init(uint16_t port);
    void registerHandler(RequestHandler handler);
    void registerDefaultHandler(RequestHandler handler);

private:
    tcp_pcb* socket;
    std::vector<RequestHandler> handlers;
    RequestHandler defaultHandler;

public:
    Response prepareResponse(tcp_pcb* pcb, const Request& request);
};