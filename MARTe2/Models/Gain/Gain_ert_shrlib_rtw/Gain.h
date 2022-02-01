#ifndef RTW_HEADER_Gain_h_
#define RTW_HEADER_Gain_h_
#include <stddef.h>
#include <string.h>
#include "rtw_modelmap.h"
#ifndef Gain_COMMON_INCLUDES_
#define Gain_COMMON_INCLUDES_
#include <stdlib.h>
#include "rtwtypes.h"
#endif

#include "Gain_types.h"
#include "rt_defines.h"

#ifndef rtmGetDataMapInfo
#define rtmGetDataMapInfo(rtm)         ((rtm)->DataMapInfo)
#endif

#ifndef rtmSetDataMapInfo
#define rtmSetDataMapInfo(rtm, val)    ((rtm)->DataMapInfo = (val))
#endif

#ifndef rtmGetU
#define rtmGetU(rtm)                   ((rtm)->inputs)
#endif

#ifndef rtmSetU
#define rtmSetU(rtm, val)              ((rtm)->inputs = (val))
#endif

#ifndef rtmGetY
#define rtmGetY(rtm)                   ((rtm)->outputs)
#endif

#ifndef rtmSetY
#define rtmSetY(rtm, val)              ((rtm)->outputs = (val))
#endif

#define Gain_M_TYPE                    RT_MODEL_Gain_T

typedef struct {
  real32_T Input1;
} ExtU_Gain_T;

typedef struct {
  real32_T Outport;
} ExtY_Gain_T;

struct P_Gain_T_ {
  real32_T Param1;
};

struct tag_RTM_Gain_T {
  ExtU_Gain_T *inputs;
  ExtY_Gain_T *outputs;
  struct {
    rtwCAPI_ModelMappingInfo mmi;
    void* dataAddress[3];
    int32_T* vardimsAddress[3];
    RTWLoggingFcnPtr loggingPtrs[3];
  } DataMapInfo;
};

extern P_Gain_T Gain_P;
extern const char *RT_MEMORY_ALLOCATION_ERROR;
extern P_Gain_T Gain_P;
extern RT_MODEL_Gain_T *Gain(void);
extern void Gain_initialize(RT_MODEL_Gain_T *const Gain_M);
extern void Gain_step(RT_MODEL_Gain_T *const Gain_M);
extern const rtwCAPI_ModelMappingStaticInfo*
  Gain_GetCAPIStaticMap(void);

#endif

