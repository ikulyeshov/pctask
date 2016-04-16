/*
 * Cube.hpp
 *
 *  Created on: Apr 16, 2016
 *      Author: ihor
 */

#ifndef INCLUDES_CUBE_HPP_
#define INCLUDES_CUBE_HPP_

#include "Logger.hpp"
#include "ICamera.hpp"
#include "INetwork.hpp"
#include "IRtpPacketizer.hpp"
#include "IEncoder.hpp"
#include "Thread.hpp"

class Cube: public Thread::User
{
public:
	typedef std::vector<ICamera*> CamerasCollection;
	typedef std::vector<IEncoder*> EncodersCollection;

public:
	Cube( 	INetwork* network,
			const CamerasCollection& camears,
			IRtpPacketizer* packetizer,
			const EncodersCollection& encoders
		);
	~Cube();

	virtual void OnThread();

private:
	static void NetworkConnectionEventCallBack( void* pContext, NetworkConnectionEvent event );
	static void NetworkMediaEventCallBack( void* pContext, NetworkEventStruct* pNetworkEvent, NetworkMediaEvent event );
	static void CaptureFrameCallback0( void* pContext, FrameContext* pFrameContext );
	static void CaptureFrameCallback1( void* pContext, FrameContext* pFrameContext );

	void OnNetworkCallback(NetworkConnectionEvent event);
	void OnMediaCallback(NetworkEventStruct* pNetworkEvent);
	void OnCameraData(int camera, FrameContext* pFrameContext);

	INetwork* mNetwork;
	IRtpPacketizer* mPacketizer;
	CamerasCollection mCameras;
	EncodersCollection mEncoders;
	int mCurrentBitrate;
	int mTickCounter;
	std::vector<int> mStartTime;
	Thread mThread;

	void GetEncoderSetting(int camera, EncodeSetting& encodeSetting);
	void SendCapabilities();


};


#endif /* INCLUDES_CUBE_HPP_ */
