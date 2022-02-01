#ifndef RTW_HEADER_rtmodel_h_
#define RTW_HEADER_rtmodel_h_
#include "Gain.h"

#define ROOT_IO_FORMAT                 2

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((void*) 0)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((void) 0)
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((void*) 0)
#endif
#endif

