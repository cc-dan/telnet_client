#pragma once
#include <string>
//#include "network.h"
#include <socket.h>
#include "libtelnet.h"
#include <vector>
#include <string_view>

namespace network
{
    class telnet_client
    {
    public:
        telnet_client();
        ~telnet_client();
        void connect(std::string ip, network::port port, std::string_view username, std::string_view password);
        std::string receive(unsigned quota);
        void send(std::string_view expression);
        void execute(std::string command);
        bool is_logged_in() const noexcept;
        
        /* telnet_init de libtelnet recibe un puntero al event handler, este método se usa para ello */
        static void static_event_handler(telnet_t *telnet, telnet_event_t *event, void *user_data)
        {
            telnet_client *client = reinterpret_cast<telnet_client*>(user_data);
            client->handle_event(event);
        }
    private:
        network::socket server;
        telnet_t *telnet;
        telnet_telopt_t telopts[5] = {
            { TELNET_TELOPT_ECHO,       TELNET_WONT, TELNET_DO   },
            { TELNET_TELOPT_TTYPE,      TELNET_WILL, TELNET_DONT },
            { TELNET_TELOPT_COMPRESS2,  TELNET_WONT, TELNET_DO   },
            { TELNET_TELOPT_MSSP,       TELNET_WONT, TELNET_DO   },
            { -1, 0, 0 }
        };
        bool logged_in = false;
		std::string username, password;
        
        void handle_event(telnet_event_t *ev);
    };
}
