#ifndef _INETWORK_H_
#define _INETWORK_H_

#include <stdio.h>
#include <stdint.h>
#include "Types.hpp"

enum NetworkConnectionEvent
{
    NET_CONNECTED_EVENT,
    NET_DISCONNECTED_EVENT,
};


enum NetworkMediaEvent
{
    NET_VIDEO_REQ_CAPABILITIES_EVENT,
    NET_VIDEO_START_STREAM_EVENT,
    NET_VIDEO_STOP_STREAM_EVENT,
    NET_VIDEO_REQUEST_KEY_FRAME_EVENT,
    NET_VIDEO_NETWORK_PARAMETERS_CHANGED_EVENT,
};

enum NetworkStreamState
{
    NET_VIDEO_STREAM_STOPPED_STATE,
};


struct NetworkEventStruct
{

    NetworkMediaEvent Event;

    union
    {

        struct
        {
        	int             Camera;
            ImageResolution Resolution;
            uint32_t        TargetBitrate;
            uint32_t        Framerate;
            uint32_t        IpAddr;
            uint32_t        TcpPort;
        }StreamParams;

        struct
        {
            uint32_t        TargetBitrate;
            uint32_t        IpAddr;
            uint32_t        TcpPort;
        }NetworkParams;

    };

};

typedef void( *fpNetworkConnectionEventCallBack )( void* pContext, NetworkConnectionEvent event );
typedef void( *fpNetworkNetworkMediaEventCallBack )( void* pContext, NetworkEventStruct* pNetworkEvent, NetworkMediaEvent event );

class INetwork
{
public:

	typedef CallBackEvent< fpNetworkConnectionEventCallBack, void* > NetworkCallback;
	typedef CallBackEvent< fpNetworkNetworkMediaEventCallBack, void* > MediaCallback;

public:
    class ISender
    {
    public:
        virtual Status SendRtpBuffer( uint32_t ipAddr, uint32_t port, void* pPacket, size_t packetSize ) = 0;
        virtual Status SendCaptureCapabilities( int camera, CaptureModeCollection& captureModes ) = 0;
        virtual Status SendStreamState( int camera, NetworkStreamState ) = 0;
    };

    virtual Status Init() = 0;
    virtual void DeInit() = 0;
    virtual void NetworkProcessing() = 0;
    virtual void RegisterNetworkConnectionEventHandler( NetworkCallback callback ) = 0;
    virtual void RegisterNetworkMediaEventHandler( MediaCallback callback ) = 0;
    virtual ISender* GetISender() = 0;
};

#endif
