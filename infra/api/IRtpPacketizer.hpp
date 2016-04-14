#ifndef _IRTP_PACKETIZER_H_
#define _IRTP_PACKETIZER_H_

class IRtpPacketizer
{
public:
    virtual void InitPacket( void* pData, size_t dataLength ); //Init packetizer by h264 bitstream buffer
    virtual size_t GetNextPacketSize(); // 0 if there is no next packet
    virtual Status GetNextPacket( void* pData, size_t dataBuffMaxSize ); //Get next RTP packet buffer
};

#endif //_IRTP_PACKETIZER_H_
