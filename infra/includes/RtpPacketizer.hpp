/*
 * RtpPacketizer.hpp
 *
 *  Created on: Apr 14, 2016
 *      Author: ihor.kulyeshov
 */

#ifndef INCLUDES_RTPPACKETIZER_HPP_
#define INCLUDES_RTPPACKETIZER_HPP_


#include "IRtpPacketizer.hpp"

namespace infra
{

class RtpPacketizer: public IRtpPacketizer
{
public:
	RtpPacketizer();
	virtual ~RtpPacketizer();

    virtual void InitPacket( const void* pData, size_t dataLength );
    virtual size_t GetNextPacketSize();
    virtual Status GetNextPacket( void* pData, size_t dataBuffMaxSize );

private:
	//not copyable
	RtpPacketizer(const RtpPacketizer& rop);
	const RtpPacketizer& operator=(const RtpPacketizer& rop);

	const void *mCurrentPacket;
	int mDataLen;

};

} //namespace infra




#endif /* INCLUDES_RTPPACKETIZER_HPP_ */
