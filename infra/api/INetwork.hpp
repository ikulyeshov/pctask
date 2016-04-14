#ifndef _INETWORK_H_
#define _INETWORK_H_

#include <stdint.h>

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
    class ISender
    {
    public:
        virtual Status SendRtpBuffer( uint32_t ipAddr, uint32_t port, void* pPacket, size_t packetSize ) = 0;
        virtual Status SendCaptureCapabilities( CaptureMode** ppCaptureModeList, size_t captureModeListSize ) = 0;
        virtual Status SendStreamState( NetworkStreamState ) = 0;
    };

    virtual Status Init();
    virtual void DeInit();
    virtual void NetworkProcessing();
    virtual void RegisterNetworkConnectionEventHandler( fpNetworkConnectionEventCallBack fpCallback, void* pContext ) = 0;
    virtual void RegisterNetworkMediaEventHandler( fpNetworkNetworkMediaEventCallBack fpCallback, void* pContext ) = 0;
    virtual ISender* GetISender();
};

#endif
