#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "Cube.hpp"
#include "Camera.hpp"
#include "Network.hpp"
#include "RtpPacketizer.hpp"
#include "Encoder.hpp"

const int DEF_SERVER_PORT =			4000;
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

    printf("Use 'q' to exit\n");

    Cube::CamerasCollection cameras;

	CaptureModeCollection modes;

	CaptureMode mode;
	mode.BitrateL = 2;
	mode.BitrateH = 15;

	modes.push_back(mode);
	modes.back().Resolution.Horizontal = 640;
	modes.back().Resolution.Vertical = 480;

	modes.push_back(mode);
	modes.back().Resolution.Horizontal = 1024;
	modes.back().Resolution.Vertical = 576;

	modes.push_back(mode);
	modes.back().Resolution.Horizontal = 1280;
	modes.back().Resolution.Vertical = 720;

	modes.push_back(mode);
	modes.back().Resolution.Horizontal = 1920;
	modes.back().Resolution.Vertical = 1080;

	cameras.push_back(new infra::Camera(modes, 10));

	modes.clear();

	mode.BitrateH = 20;

	modes.push_back(mode);
	modes.back().Resolution.Horizontal = 1280;
	modes.back().Resolution.Vertical = 720;

	modes.push_back(mode);
	modes.back().Resolution.Horizontal = 1920;
	modes.back().Resolution.Vertical = 1080;

	cameras.push_back(new infra::Camera(modes, 0/*no overheat*/));

	infra::Network network(addr, portno);

	infra::RtpPacketizer packetizer;

	Cube::EncodersCollection encoders;
	encoders.push_back(new infra::Encoder());
	encoders.push_back(new infra::Encoder());

    Cube cube(&network, cameras, &packetizer, encoders);

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
