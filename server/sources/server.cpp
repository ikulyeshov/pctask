#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "Logger.hpp"
#include "TextMessageServer.hpp"
#include "ClientServerDefines.hpp"

const int DEF_SERVER_PORT =			4000;


struct ServerObserver: infra::msgserver::Server::Observer
{
	virtual void OnMessage(Handle connect, const char* message)
	{
		ps_log_debug("Cam %i data: %s", connect, message);
	}
};


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

    ServerObserver observer;
    infra::msgserver::Server server(&observer);
    int curClient = 0;
    int curCamera = 0;
    int curBitrate = 10;//Mbit
    int buflen = infra::msgserver::TEXT_MESSAGE_MAX_MESSAGE_LEN;
    char buffer[buflen];

    printf("Use 'q' to exit\n");

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
				snprintf(buffer, buflen, infra::MSG_START, curCamera);
				server.Message(curClient, buffer);
				break;
			case 't':
				ps_log_info("Stop stream");
				server.Message(curClient, buffer);
				snprintf(buffer, buflen, infra::MSG_STOP, curCamera);
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
			case '>':
			case '<':
				curCamera ^= 1;
				ps_log_info("Current camera: %i", curCamera);
				break;
			case 'a':
				server.Message(curClient, infra::MSG_CAPS_REQ);
				break;
			case 'o':/*QOS*/
				ps_log_info("Current bitrate: %iMbit", curBitrate);
				snprintf(buffer, buflen, infra::MSG_QOS, curBitrate);
				server.Message(curClient, buffer);
				--curBitrate;
				if (curBitrate < 2)
					curBitrate = 10;
				break;
			}
		}
		while (ch != 'q');

		system("stty cooked echo");
    }

    ps_log_info("Exiting server");

    return 0;
}
