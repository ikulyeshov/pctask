/*
 * Encoder.hpp
 *
 *  Created on: Apr 14, 2016
 *      Author: ihor.kulyeshov
 */

#ifndef INCLUDES_ENCODER_HPP_
#define INCLUDES_ENCODER_HPP_

#include "IEncoder.hpp"

namespace infra
{

class Encoder: public IEncoder
{
public:
	Encoder();
	virtual ~Encoder();

private:
	//not copyable
	Encoder(const Encoder& rop);
	const Encoder& operator=(const Encoder& rop);

    virtual Status Init( EncodeSetting& encodeSetting );
    virtual void Deinit();
    virtual Status EncqueueFrame( const void* pFrame, size_t frameSize );
    virtual size_t IsDequeueFrameReady();
    virtual Status DequeueFrame( void* pBitstreamBuff, size_t bitstreamSize );
    virtual void ForceKeyFrame( void* pBitstreamBuff, size_t bitstreamSize );

    const void* mCurrentFrame;
	size_t mCurrentFrameSize;
	EncodeSetting mEncodeSetting;

};

} //namespace infra




#endif /* INCLUDES_ENCODER_HPP_ */
