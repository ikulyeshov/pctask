/*
 * Cube.cpp
 *
 *  Created on: Apr 16, 2016
 *      Author: ihor
 */


#include "Cube.hpp"


Cube::Cube(INetwork* network, const CamerasCollection& cameras, IRtpPacketizer* packetizer, const EncodersCollection& encoders):
		mNetwork(network),
		mCameras(cameras),
		mPacketizer(packetizer),
		mEncoders(encoders)
{
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
	}
}

Cube::~Cube()
{
	mNetwork->DeInit();
}

void Cube::GetEncoderSetting(int camera, EncodeSetting& encodeSetting)
{
	encodeSetting.SourceResolution.Horizontal = 1024;
	encodeSetting.SourceResolution.Vertical = 768;

	encodeSetting.DestResolution.Horizontal = 1024;
	encodeSetting.DestResolution.Vertical = 768;

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
			break;
		}
	case NET_VIDEO_STOP_STREAM_EVENT:
		mCameras[pNetworkEvent->StreamParams.Camera]->Stop();
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

