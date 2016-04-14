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

private:
	//not copyable
	RtpPacketizer(const RtpPacketizer& rop);
	const RtpPacketizer& operator=(const RtpPacketizer& rop);

};

} //namespace infra




#endif /* INCLUDES_RTPPACKETIZER_HPP_ */
