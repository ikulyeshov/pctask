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
    int curClient = 0;

    if (server.Start(portno) == ST_OK)
    {
		system("stty raw -echo");

		int ch = 0;
		do
		{
			ch = getchar();

			switch (ch)
			{
			case 's':
				ps_log_info("Start stream");
				server.Message(curClient, infra::msgserver::MSG_START);
				break;
			case 't':
				ps_log_info("Stop stream");
				server.Message(curClient, infra::msgserver::MSG_STOP);
				break;
			case 'p':
				ps_log_info("List of connections");
				server.List();
				break;
			case '+':
				++curClient;
				ps_log_info("Current client: %i", curClient);
				break;
			case '-':
				--curClient;
				ps_log_info("Current client: %i", curClient);
				break;
			}
		}
		while (ch != 'q');

		system("stty cooked echo");
    }

    ps_log_info("Exiting server");

    return 0;
}
