#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "Logger.hpp"
#include "TextMessageClient.hpp"

const int DEF_SERVER_PORT =			4007;
const char DEF_SERVER_ADDR[] =		"127.0.0.1";

int main(int argc, char* argv[])
{
	int portno = 0;
    if (argc < 3) {
    	ps_log_info("No port provided, using default: %i", DEF_SERVER_PORT);
        portno = DEF_SERVER_PORT;
    }
    else
    {
    	portno = atoi(argv[2]);
    }

    const char* addr = 0;
    if (argc < 2) {
    	ps_log_info("No address provided, using default: %s", DEF_SERVER_ADDR);
    	addr = DEF_SERVER_ADDR;
    }
    else
    {
        addr = argv[1];
    }

    infra::msgserver::Client client;
    client.Start(addr, portno);

	system("stty raw -echo");

	int ch = 0;
	do
	{
		ch = getchar();
	}
	while (ch != 'q');

	system("stty cooked echo");


	return 0;
}
