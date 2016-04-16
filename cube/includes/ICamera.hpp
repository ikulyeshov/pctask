#ifndef _ICAMERA_H_
#define _ICAMERA_H_

#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include "Types.hpp"

struct FrameContext
{
    ImageResolution Resolution;
    uint64_t        Timestamp;
    const void*     pData;
    size_t          DataLength;
};


typedef void( *fpCaptureFrameCallback )( void* pContext, FrameContext* pFrameContext );

struct CaptureParam
{
    ImageResolution Resolution;
    uint32_t        FrameRate;
    CallBackEvent< fpCaptureFrameCallback, void* > Callback;
};


class ICamera
{
public:
    virtual void GetCaptureModeList( CaptureModeCollection& captureModes) const = 0;
    virtual Status Start( const CaptureParam& captureParam ) = 0;
    virtual Status Stop() = 0;
};

#endif //_ICAMERA_H_
