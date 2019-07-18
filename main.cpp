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
        m_isClosed = false;
    }

    void onDisconnect(WebSocket* connection) override {
        m_isClosed = true;
    }

    void tick(Server& server) {
        if(m_connection != nullptr)
            try {
            server.execute([&](){
                if(!m_isClosed)
                    m_connection->send(R"({"Type": 1, "Data": "ping"})");

            });
            } catch (std::exception &e){
                std::cout << e.what() << std::endl;
            }
    }

private:
    WebSocket * m_connection;
    bool m_isClosed;
};

int main(int argc, const char* argv[]) {
    auto logger = std::make_shared<PrintfLogger>(Logger::Level::Debug);

    Server server(logger);
    auto handler = std::make_shared<EchoHandler>();
    server.addWebSocketHandler("/", handler);
    std::thread tick([&]{
        for (;;) { sleep(3); handler->tick(server); }
    });
    tick.detach();
    server.serve("", 8000);
    return 0;
}
