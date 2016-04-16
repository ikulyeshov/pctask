#ifndef _IRTP_PACKETIZER_H_
#define _IRTP_PACKETIZER_H_

#include <stdio.h>
#include "Types.hpp"

class IRtpPacketizer
{
public:
    virtual void InitPacket( const void* pData, size_t dataLength ) = 0; //Init packetizer by h264 bitstream buffer
    virtual size_t GetNextPacketSize() = 0; // 0 if there is no next packet
    virtual Status GetNextPacket( void* pData, size_t dataBuffMaxSize ) = 0; //Get next RTP packet buffer
};

#endif //_IRTP_PACKETIZER_H_
