#pragma once
#include "lwipopts.h"
#include "lwip/api.h"
#include "lwip/tcp.h"

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
    char name[32] = {0};
    char value[64] = {0};
};

struct Parameter {
    enum Type {POST, GET};

    char name[32] = {0};
    char value[256] = {0};
    Type type;
};

static const char* to_char(Parameter::Type type) {
    return type == Parameter::GET? "GET": "POST";
}

struct Request {
    Version version;
    MethodType method;
    char uri[64] = {0};
    int usedHeaders = 0;
    Header headers[16];
    int usedParameters = 0;
    Parameter parameters[32];
};

static void debug_print(const Request& request) {
    printf("Request:\n");
    printf("Version: %s\n", to_char(request.version));
    printf("Method: %s\n", to_char(request.method));
    printf("Uri: %s\n", request.uri);

    printf("Headers: %d\n", request.usedHeaders);
    for(int i = 0; i < request.usedHeaders; ++i){
        printf("\t%s:%s\n", request.headers[i].name, request.headers[i].value);
    }

    printf("Parameters: %d\n", request.usedParameters);
    for(int i = 0; i < request.usedParameters; ++i){
        printf("\t[%s]%s:%s\n", to_char(request.parameters[i].type), request.parameters[i].name, request.parameters[i].value);
    }
}

class WebServer {
public:
    bool init(uint16_t port);

private:
    tcp_pcb* socket;
};