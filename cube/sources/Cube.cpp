/*
 * Cube.cpp
 *
 *  Created on: Apr 16, 2016
 *      Author: ihor
 */

#include <unistd.h>

#include "Cube.hpp"


Cube::Cube(INetwork* network, const CamerasCollection& cameras, IRtpPacketizer* packetizer, const EncodersCollection& encoders):
		mNetwork(network),
		mCameras(cameras),
		mPacketizer(packetizer),
		mEncoders(encoders),
		mCurrentBitrate(10),
		mThread(this)
{
	mStartTime.resize(mCameras.size());

	INetwork::NetworkCallback networkCallback;
	networkCallback.Callback = NetworkConnectionEventCallBack;
	networkCallback.Context = this;

	INetwork::MediaCallback mediaCallback;
	mediaCallback.Callback = NetworkMediaEventCallBack;
	mediaCallback.Context = this;

	mNetwork->RegisterNetworkConnectionEventHandler(networkCallback);
	mNetwork->RegisterNetworkMediaEventHandler(mediaCallback);

	mNetwork->Init();

	EncodeSetting encodeSetting;

	for (int i = 0; i < mEncoders.size(); ++i)
	{
		GetEncoderSetting(i, encodeSetting);
		mEncoders[i]->Init(encodeSetting);
		mStartTime[i] = 0;
	}

	mThread.Start();
}

Cube::~Cube()
{
	mNetwork->DeInit();
}

void Cube::GetEncoderSetting(int camera, EncodeSetting& encodeSetting)
{
	/*here simplified link adaptation: select resolution base on bitrate*/
	CaptureModeCollection captureModes;
	mCameras[camera]->GetCaptureModeList(captureModes);

	int index = (mCurrentBitrate - 2) * captureModes.size() / 8;
	if (index >= captureModes.size())
		index = captureModes.size() - 1;


	ps_log_debug("Select new resolution index %i", index);

	encodeSetting.SourceResolution.Horizontal = 1024;
	encodeSetting.SourceResolution.Vertical = 768;

	encodeSetting.DestResolution.Horizontal = captureModes[index].Resolution.Horizontal;
	encodeSetting.DestResolution.Vertical = captureModes[index].Resolution.Vertical;

	encodeSetting.Stream.Framerate = 20;
	encodeSetting.Stream.MaxKeyInterval = 5;
	encodeSetting.Stream.TargetBitrate = 10;
}

void Cube::NetworkConnectionEventCallBack( void* pContext, NetworkConnectionEvent event )
{
	static_cast<Cube*>(pContext)->OnNetworkCallback(event);
}

void Cube::NetworkMediaEventCallBack( void* pContext, NetworkEventStruct* pNetworkEvent, NetworkMediaEvent event )
{
	static_cast<Cube*>(pContext)->OnMediaCallback(pNetworkEvent);
}

void Cube::CaptureFrameCallback0( void* pContext, FrameContext* pFrameContext )
{
	static_cast<Cube*>(pContext)->OnCameraData(0, pFrameContext);
}
void Cube::CaptureFrameCallback1( void* pContext, FrameContext* pFrameContext )
{
	static_cast<Cube*>(pContext)->OnCameraData(1, pFrameContext);
}

void Cube::OnNetworkCallback(NetworkConnectionEvent event)
{
	switch (event)
	{
	case NET_CONNECTED_EVENT:
		/*Send capabilities on connect*/
		SendCapabilities();
		break;
	case NET_DISCONNECTED_EVENT:
		break;

	}
}

void Cube::SendCapabilities()
{
	for (int i = 0; i < mCameras.size(); ++i)
	{
		CaptureModeCollection captureModes;
		mCameras[i]->GetCaptureModeList(captureModes);
		mNetwork->GetISender()->SendCaptureCapabilities(i, captureModes);
	}
}

void Cube::OnMediaCallback(NetworkEventStruct* pNetworkEvent)
{
	switch (pNetworkEvent->Event)
	{
	case NET_VIDEO_START_STREAM_EVENT:
		{
			CaptureParam params;
			params.FrameRate = pNetworkEvent->StreamParams.Framerate;
			params.Resolution.Horizontal = pNetworkEvent->StreamParams.Resolution.Horizontal;
			params.Resolution.Vertical = pNetworkEvent->StreamParams.Resolution.Vertical;
			params.Callback.Callback = pNetworkEvent->StreamParams.Camera ? CaptureFrameCallback1 :CaptureFrameCallback0;
			params.Callback.Context = this;

			mCameras[pNetworkEvent->StreamParams.Camera]->Start(params);

			mStartTime[pNetworkEvent->StreamParams.Camera] = mTickCounter;
			break;
		}
	case NET_VIDEO_STOP_STREAM_EVENT:
		mCameras[pNetworkEvent->StreamParams.Camera]->Stop();
		break;
	case NET_VIDEO_REQ_CAPABILITIES_EVENT:
		SendCapabilities();
		break;
	case NET_VIDEO_NETWORK_PARAMETERS_CHANGED_EVENT:
		{
			mCurrentBitrate = pNetworkEvent->NetworkParams.TargetBitrate;

			EncodeSetting encodeSetting;

			for (int i = 0; i < mEncoders.size(); ++i)
			{
				GetEncoderSetting(i, encodeSetting);
				mEncoders[i]->Init(encodeSetting);
			}
		}

		break;
	}
}

void Cube::OnCameraData(int camera, FrameContext* pFrameContext)
{
	mEncoders[camera]->EncqueueFrame(pFrameContext->pData, pFrameContext->DataLength);

	int ressize = mEncoders[camera]->IsDequeueFrameReady();

	if (!ressize)
		return;

	char *data = new char[ressize];

	mEncoders[camera]->DequeueFrame(data, ressize);

	mPacketizer->InitPacket(data, ressize);

	int buflen = 0;
	while ((buflen = mPacketizer->GetNextPacketSize()))
	{
		char *packet = new char[buflen];
		if (mPacketizer->GetNextPacket(packet, buflen) == ST_OK)
		{
			mNetwork->GetISender()->SendRtpBuffer(0, 0, packet, buflen);
		}
	}
}

void Cube::OnThread()
{
	++mTickCounter;

	for (int i = 0; i < mCameras.size(); ++i)
	{
		CameraCaps camCaps;
		mCameras[i]->GetCaps(camCaps);

		if (camCaps.mOverheatTime != 0 && mTickCounter - mStartTime[i] >= camCaps.mOverheatTime && mStartTime[i])
		{
			ps_log_debug("Camera %i is overheated: stop", i);
			mCameras[i]->Stop();
			mNetwork->GetISender()->SendStreamState(i, NET_VIDEO_STREAM_STOPPED_STATE);
			mStartTime[i] = 0;
		}
	}

	sleep(1);

}
