#ifndef _IENCODER_H_
#define _IENCODER_H_

#include <stdint.h>

struct EncodeSetting
{
    ImageResolution SourceResolution;
    ImageResolution DestResolution;


    struct
    {
        uint32_t Framerate;
        uint32_t TargetBitrate;
        uint32_t MaxKeyInterval;
    }Stream;
};


class IEncoder
{
public:
    virtual Status Init( EncodeSetting& encodeSetting ) = 0;
    virtual void Deinit() = 0;
    virtual Status EncqueueFrame( void* pFrame, size_t frameSize ) = 0;
    virtual size_t IsDequeueFrameReady(); //return size of frame, blocked
    virtual Status DequeueFrame( void* pBitstreamBuff, size_t bitstreamSize ) = 0;
    virtual void ForceKeyFrame( void* pBitstreamBuff, size_t bitstreamSize ) = 0;
};

#endif //_IENCODER_H_
