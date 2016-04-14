#ifndef _ICAMERA_H_
#define _ICAMERA_H_

#include <stdint.h>

struct FrameContext
{
    ImageResolution Resolution;
    uint64_t        Timestamp;
    void*           pData;
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
    virtual int32_t GetCaptureModeList( CaptureMode** ppCaptureModeList, size_t captureModeListSize ) = 0;
    virtual Status Start( CaptureParam& captureParam ) = 0;
    virtual void Stop() = 0;
};

#endif //_ICAMERA_H_
