#ifndef RTW_HEADER_Gain_cap_host_h__
#define RTW_HEADER_Gain_cap_host_h__
#ifdef HOST_CAPI_BUILD
#include "rtw_capi.h"
#include "rtw_modelmap.h"

typedef struct {
  rtwCAPI_ModelMappingInfo mmi;
} Gain_host_DataMapInfo_T;

#ifdef __cplusplus

extern "C" {

#endif

  void Gain_host_InitializeDataMapInfo(Gain_host_DataMapInfo_T *dataMap, const
    char *path);

#ifdef __cplusplus

}
#endif
#endif
#endif

