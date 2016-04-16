/*
 * Network.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: ihor.kulyeshov
 */

#include <string.h>

#include "Network.hpp"
#include "ClientServerDefines.hpp"
#include "Logger.hpp"

using namespace infra;

Network::Network(const char* addr, int port):
		mClient(this),
		mAddr(addr),
		mPort(port)
{

}

Network::~Network()
{

}

Status Network::Init()
{
    mClient.Start(mAddr, mPort);

    //simulate connect event
    if (mConCallback.Callback)
    	mConCallback.Callback(mConCallback.Context, NET_CONNECTED_EVENT);

}

void Network::DeInit()
{
    //simulate disconnect event
    if (mConCallback.Callback)
    	mConCallback.Callback(mConCallback.Context, NET_DISCONNECTED_EVENT);
}

void Network::NetworkProcessing()
{

}

void Network::RegisterNetworkConnectionEventHandler( NetworkCallback callback )
{
	mConCallback = callback;
}

void Network::RegisterNetworkMediaEventHandler( CallBackEvent< fpNetworkNetworkMediaEventCallBack, void* > callback )
{
	mMediaCallback = callback;
}

INetwork::ISender* Network::GetISender()
{
	return this;
}

Status Network::SendRtpBuffer( uint32_t ipAddr, uint32_t port, void* pPacket, size_t packetSize )
{
	Status st = ST_OK;

	mClient.Message((char*)pPacket);

	return st;
}

Status Network::SendCaptureCapabilities( int camera, CaptureModeCollection& captureModes )
{
	Status st = ST_OK;
	int buflen = infra::msgserver::TEXT_MESSAGE_MAX_MESSAGE_LEN;
	char buffer[buflen];

	for (int i = 0; i < captureModes.size(); ++i)
	{
		snprintf(buffer, buflen, infra::MSG_CAPS,
				camera,
				i,
				captureModes[i].Resolution.Horizontal,
				captureModes[i].Resolution.Vertical
			);

		mClient.Message(buffer);
	}

	return st;
}

Status Network::SendStreamState( int camera, NetworkStreamState  streamState)
{
	Status st = ST_OK;

	int buflen = infra::msgserver::TEXT_MESSAGE_MAX_MESSAGE_LEN;
	char buffer[buflen];

	snprintf(buffer, buflen, infra::MSG_OVERHEAT, camera);

	mClient.Message(buffer);

	return st;
}

void Network::OnMessage(const char* message)
{
	ps_log_debug("Server data: %s", message);

	if (!mMediaCallback.Callback)
		return;

	NetworkEventStruct networkEvent;
	networkEvent.StreamParams.Framerate = 10;
	networkEvent.StreamParams.Resolution.Horizontal = 1024;
	networkEvent.StreamParams.Resolution.Vertical = 768;
	networkEvent.StreamParams.TargetBitrate = 10;

	/*find first space*/
	int i = 0;
	while (message[i] != ' ' && message[i] != '\0') ++i;

	if (!strncmp(message, infra::MSG_START, i))
	{
		int cam;
		sscanf(message, infra::MSG_START, &cam);
		ps_log_debug("Stream start %i", cam);

		networkEvent.Event = NET_VIDEO_START_STREAM_EVENT;
		networkEvent.StreamParams.Camera = cam;

		mMediaCallback.Callback(mMediaCallback.Context, &networkEvent, networkEvent.Event);
	}
	else if (!strncmp(message, infra::MSG_STOP, i))
	{
		int cam;
		sscanf(message, infra::MSG_STOP, &cam);
		ps_log_debug("Stream stop %i", cam);

		networkEvent.Event = NET_VIDEO_STOP_STREAM_EVENT;
		networkEvent.StreamParams.Camera = cam;

		mMediaCallback.Callback(mMediaCallback.Context, &networkEvent, networkEvent.Event);
	}
	else if (!strncmp(message, infra::MSG_CAPS_REQ, i))
	{
		networkEvent.Event = NET_VIDEO_REQ_CAPABILITIES_EVENT;
		mMediaCallback.Callback(mMediaCallback.Context, &networkEvent, networkEvent.Event);
	}
	else if (!strncmp(message, infra::MSG_QOS, i))
	{
		int bitrate;
		sscanf(message, infra::MSG_QOS, &bitrate);
		ps_log_debug("QOS changed %iMbit", bitrate);

		networkEvent.Event = NET_VIDEO_NETWORK_PARAMETERS_CHANGED_EVENT;
		networkEvent.NetworkParams.TargetBitrate = bitrate;

		mMediaCallback.Callback(mMediaCallback.Context, &networkEvent, networkEvent.Event);
	}
	else
	{

	}
}
