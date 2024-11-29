#include <iostream>
#include <string_view>
#include <exception>
#include <poll.h>
#include <socket.h>
#include "libtelnet.h"
#include "telnet_client.h"
#include <cstring>
#include <vector>
#include <string_view>

network::telnet_client::telnet_client()
{
    telnet = telnet_init(telopts, static_event_handler, 0, this);
}

network::telnet_client::~telnet_client()
{
    telnet_free(telnet);
    std::cout << "closed telnet session" << std::endl;
}

void
network::telnet_client::connect(std::string ip, network::port port, std::string_view _username, std::string_view _password)
{
	username = _username;
	password = _password;
    server.connect(ip, port);
}

std::string 
network::telnet_client::receive(unsigned quota)
{
    std::string data = server.receive(quota);
    
    if (!data.size())
        logged_in = false;
    
    telnet_recv(telnet, data.c_str(), data.size());

    return data;
}

void
network::telnet_client::execute(std::string command)
{
    command += "\r\n";
    telnet_send(telnet, command.c_str(), command.size());
}

void 
network::telnet_client::handle_event(telnet_event_t *ev)
{
    switch (ev->type) 
    {
    /* data received */
    case TELNET_EV_DATA: {
        if (!ev->data.size)
            break;
        
        std::string_view data = ev->data.buffer;
   
        if (!logged_in)
        {
            if (data.rfind(" login: ") != std::string::npos)
            {
                execute(username);
            }
            else if (data.rfind("Password: ") != std::string::npos)
            {
                execute(password);
                logged_in = true;
            }
        }
            
        break;
    }
    /* data must be sent */
    case TELNET_EV_SEND:
        server.send(ev->data.buffer);
        break;
    case TELNET_EV_TTYPE:
		/* respond with our terminal type, if requested */
		if (ev->ttype.cmd == TELNET_TTYPE_SEND)
			telnet_ttype_is(telnet, getenv("TERM"));
		break;
    }
}

bool
network::telnet_client::is_logged_in() const noexcept
{
    return logged_in;
}
