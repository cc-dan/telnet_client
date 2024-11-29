#include <iostream>
#include <telnet_client.h>
#include <exception>
#include <future>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
	   std::cout << "usage: " << argv[0] << " " << "ip_address" << std::endl;
	   return 1;
    }

    network::telnet_client telnet;

    try {
        telnet.connect(argv[1], 23, "admin", "admin");
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "error in telnet connection: " << e.what() << std::endl;
        return 1;
    }
    
    /* mantener el telnet leyendo en un thread paralelo */
    auto read_task = std::async([&telnet]()
    {
        while (true)
        {
            std::string data;
            
            try {
                data = telnet.receive(512);
            }         
            catch (const std::runtime_error &e)
            {
                std::cerr << "read error: " << e.what() << std::endl;
                break;
            }
            
            if (!data.size())
                break;
        }
    });
    
    /* mantenerse esperando a que se loguee */ 
    while (true)
    {   
        if (telnet.is_logged_in())
        {
            try {
                telnet.execute("ls");
                telnet.execute("exit");
            }
            catch (const std::runtime_error &e)
            {
                std::cerr << "write error: " << e.what() << std::endl;
                break;
            }
            
            break;
        }
    }
    
    read_task.wait();

	return 0;
}
