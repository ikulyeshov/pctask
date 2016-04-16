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
#include "Camera.hpp"
#include "ClientServerDefines.hpp"

const int DEF_SERVER_PORT =			4007;
const char DEF_SERVER_ADDR[] =		"127.0.0.1";


struct Client: infra::msgserver::Client::Observer
{
	Client(const char* addr, int port):
			mClient(this)
	{
	    mClient.Start(addr, port);

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

	    mCameras[0] = new infra::Camera(modes);

	    modes.clear();

	    mode.BitrateH = 20;

	    modes.push_back(mode);
	    modes.back().Resolution.Horizontal = 1280;
	    modes.back().Resolution.Vertical = 720;

	    modes.push_back(mode);
	    modes.back().Resolution.Horizontal = 1920;
	    modes.back().Resolution.Vertical = 1080;

	    mCameras[1] = new infra::Camera(modes);
	}

	~Client()
	{
		delete mCameras[0];
		delete mCameras[1];
	}

	static void CaptureFrameCallback0( void* pContext, FrameContext* pFrameContext )
	{
		static_cast<Client*>(pContext)->OnCameraData(0, pFrameContext);
	}
	static void CaptureFrameCallback1( void* pContext, FrameContext* pFrameContext )
	{
		static_cast<Client*>(pContext)->OnCameraData(1, pFrameContext);
	}

	void OnCameraData(int camera, FrameContext* pFrameContext)
	{
		int buflen = infra::msgserver::TEXT_MESSAGE_MAX_MESSAGE_LEN;
		char buffer[buflen];

		snprintf(buffer, buflen, "%s cam: %i res: %i:%i",
					(const char*)pFrameContext->pData,
					camera,
					pFrameContext->Resolution.Horizontal,
					pFrameContext->Resolution.Vertical
				);

		mClient.Message(buffer);
	}

	virtual void OnMessage(const char* message)
	{
		ps_log_debug("Server data: %s", message);

		/*find first space*/
		int i = 0;
		while (message[i] != ' ' && message[i] != '\0') ++i;

		if (!strncmp(message, infra::MSG_START, i))
		{
			int cam;
			sscanf(message, infra::MSG_START, &cam);
			ps_log_debug("Cam start %i", cam);

			CaptureParam params;
			params.FrameRate = 10;
			params.Resolution.Horizontal = 1024;
			params.Resolution.Vertical = 768;
			params.Callback.Callback = cam ? CaptureFrameCallback1 :CaptureFrameCallback0;
			params.Callback.Context = this;

			mCameras[cam]->Start(params);
		}
		else if (!strncmp(message, infra::MSG_STOP, i))
		{
			int cam;
			sscanf(message, infra::MSG_STOP, &cam);
			ps_log_debug("Cam stop %i", cam);
			mCameras[cam]->Stop();
		}
		else
		{

		}
	}

    infra::msgserver::Client mClient;
    infra::Camera* mCameras[2];
};


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

    Client client(addr, portno);

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
