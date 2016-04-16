/*
 * RtpPacketizer.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: ihor.kulyeshov
 */

#include "RtpPacketizer.hpp"
#include <string.h>

using namespace infra;

RtpPacketizer::RtpPacketizer():
			mCurrentPacket(0),
			mDataLen(0)

{

}

RtpPacketizer::~RtpPacketizer()
{

}

void RtpPacketizer::InitPacket( const void* pData, size_t dataLength )
{
	mCurrentPacket = pData;
	mDataLen = dataLength;
}

size_t RtpPacketizer::GetNextPacketSize()
{
	//be transparent: just return incoming data
	return mCurrentPacket ? strlen((char*)mCurrentPacket) : 0;
}

Status RtpPacketizer::GetNextPacket( void* pData, size_t dataBuffMaxSize )
{
	if (!mCurrentPacket)
		return ST_EMPTY;

	//be transparent: just return incoming data
	if (pData != mCurrentPacket)
		strcpy((char*)pData, (char*)mCurrentPacket);

	mCurrentPacket = 0; //no more packets

	return ST_OK;
}
