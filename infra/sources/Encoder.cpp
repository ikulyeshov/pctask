/*
 * Encoder.cpp
 *
 *  Created on: Apr 14, 2016
 *      Author: ihor.kulyeshov
 */

#include <string.h>
#include "Encoder.hpp"

using namespace infra;

Encoder::Encoder():
	mCurrentFrame(0),
	mCurrentFrameSize(0)
{

}

Encoder::~Encoder()
{

}

Status Encoder::Init( EncodeSetting& encodeSetting )
{
	Status st = ST_OK;

	mEncodeSetting = encodeSetting;

	return st;
}

void Encoder::Deinit()
{

}

Status Encoder::EncqueueFrame( const void* pFrame, size_t frameSize )
{
	Status st = ST_OK;

	mCurrentFrame = pFrame;
	mCurrentFrameSize = frameSize;

	return st;
}

size_t Encoder::IsDequeueFrameReady()
{
	return mCurrentFrame ? strlen((const char*)mCurrentFrame) + 50/*extra data*/ : 0;
}

Status Encoder::DequeueFrame( void* pBitstreamBuff, size_t bitstreamSize )
{
	if (!mCurrentFrame)
		return ST_EMPTY;

	Status st = ST_OK;

	snprintf((char*)pBitstreamBuff, bitstreamSize, "Encoded res: %i:%i rate: %i data: %s",
			mEncodeSetting.DestResolution.Horizontal,
			mEncodeSetting.DestResolution.Vertical,
			mEncodeSetting.Stream.Framerate,
			(const char*)mCurrentFrame
			);

	mCurrentFrame = 0;

	return st;
}

void Encoder::ForceKeyFrame( void* pBitstreamBuff, size_t bitstreamSize )
{

}

