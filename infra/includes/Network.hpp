/*
 * Network.hpp
 *
 *  Created on: Apr 14, 2016
 *      Author: ihor.kulyeshov
 */

#ifndef INCLUDES_NETWORK_HPP_
#define INCLUDES_NETWORK_HPP_


#include "INetwork.hpp"
#include "TextMessageClient.hpp"

namespace infra
{


class Network: public INetwork, public INetwork::ISender, public infra::msgserver::Client::Observer
{
public:
	Network(const char* addr, int port);
	virtual ~Network();

    virtual Status Init();
    virtual void DeInit();
    virtual void NetworkProcessing();
    virtual void RegisterNetworkConnectionEventHandler( NetworkCallback callback );
    virtual void RegisterNetworkMediaEventHandler( MediaCallback callback );
    virtual INetwork::ISender* GetISender();

    virtual Status SendRtpBuffer( uint32_t ipAddr, uint32_t port, void* pPacket, size_t packetSize );
    virtual Status SendCaptureCapabilities( int camera, CaptureModeCollection& captureModes );
    virtual Status SendStreamState( int camera, NetworkStreamState );

private:
	//not copyable
	Network(const Network& rop);
	const Network& operator=(const Network& rop);

	infra::msgserver::Client mClient;
	const char* mAddr;
	int mPort;
	NetworkCallback mConCallback;
	MediaCallback mMediaCallback;

	void OnMessage(const char* message);

};

} //namespace infra




#endif /* INCLUDES_NETWORK_HPP_ */
