#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "Logger.hpp"
#include "TextMessageServer.hpp"

const int DEF_SERVER_PORT =			4005;


int main(int argc, char* argv[])
{
	int portno = 0;
    if (argc < 2)
    {
    	ps_log_info("No port provided, using default: %i\n", DEF_SERVER_PORT);
        portno = DEF_SERVER_PORT;
    }
    else
    {
        portno = atoi(argv[1]);
    }

    infra::msgserver::Server server;
    server.Start(portno);

    return 0;
}
