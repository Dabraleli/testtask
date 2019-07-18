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
#include <thread>
#include <zconf.h>


using namespace seasocks;

class EchoHandler : public WebSocket::Handler {
public:
    void onConnect(WebSocket * connection) override {
        m_connection = connection;
    }

    void onDisconnect(WebSocket* connection) override {

    }

    void tick() {
        if(m_connection != nullptr)
            m_connection->server().execute([&](){
            m_connection->send(R"({"Type": 1, "Data": "ping"})");
        });
    }

private:
    WebSocket * m_connection;
};

int main(int argc, const char* argv[]) {
    auto logger = std::make_shared<PrintfLogger>(Logger::Level::Debug);

    Server server(logger);
    auto handler = std::make_shared<EchoHandler>();
    server.addWebSocketHandler("/", handler);
    std::thread tick([&]{
        for (;;) { sleep(3); handler->tick(); }
    });
    tick.detach();
    server.serve("", 8000);
    return 0;
}
