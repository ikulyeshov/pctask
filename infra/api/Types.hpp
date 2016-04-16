
#ifndef API_TYPES_HPP_
#define API_TYPES_HPP_

#include <stdint.h>

typedef int Handle;

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
	ST_OK,
	ST_ERROR,
	ST_SELECT_ERROR,
	ST_BINDING_ERROR,
	ST_ACCEPT_ERROR,
	ST_OPEN_ERROR,
	ST_WRITE_ERROR,
	ST_PARAM_ERROR,
	ST_CONNECT_ERROR
};

struct CaptureMode
{
    ImageResolution Resolution;
    uint32_t        BitrateL;
    uint32_t        BitrateH;
};

#endif //API_TYPES_HPP_
