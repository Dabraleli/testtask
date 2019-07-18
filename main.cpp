#include "seasocks/PrintfLogger.h"
#include "seasocks/Server.h"
#include "seasocks/StringUtil.h"
#include "seasocks/WebSocket.h"

#include <cstring>
#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <map>

using namespace seasocks;

class EchoHandler : public WebSocket::Handler {
public:
    virtual void onConnect(WebSocket * connection){

    }
    virtual void onData(WebSocket* connection, const uint8_t* data, size_t length) override {
        connection->send(data, length);
    }

    virtual void onData(WebSocket* connection, const char* data) override {
        connection->send(data);
    }

    virtual void onDisconnect(WebSocket* connection) override {
    }
};

int main(int argc, const char* argv[]) {
    auto logger = std::make_shared<PrintfLogger>(Logger::Level::Debug);

    Server server(logger);
    auto handler = std::make_shared<EchoHandler>();
    server.addWebSocketHandler("/", handler);
    server.serve("", 8000);
    return 0;
}
