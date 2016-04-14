
template < typename CallBackType, typename ContextType >
struct CallBackEvent
{
    CallBackType Callback;
    ContextType  Context;
};

struct ImageResolution
{
    uint32_t Horizontal;
    uint32_t Vertical;
};

enum Status
{
    SUCCESS_STATUS,
    FAIL_STATUS
};

struct CaptureMode
{
    ImageResolution Resolution;
    uint32_t        Bitrate;
};

