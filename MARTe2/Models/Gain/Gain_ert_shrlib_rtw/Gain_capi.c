#include <stddef.h>
#include "rtw_capi.h"
#ifdef HOST_CAPI_BUILD
#include "Gain_capi_host.h"
#define sizeof(s)                      ((size_t)(0xFFFF))
#undef rt_offsetof
#define rt_offsetof(s,el)              ((uint16_T)(0xFFFF))
#define TARGET_CONST
#define TARGET_STRING(s)               (s)
#else
#include "builtin_typeid_types.h"
#include "Gain.h"
#include "Gain_capi.h"
#include "Gain_private.h"
#ifdef LIGHT_WEIGHT_CAPI
#define TARGET_CONST
#define TARGET_STRING(s)               ((NULL))
#else
#define TARGET_CONST                   const
#define TARGET_STRING(s)               (s)
#endif
#endif

static rtwCAPI_BlockParameters rtBlockParameters[] = {

  {
    0, (NULL), (NULL), 0, 0, 0
  }
};

static rtwCAPI_Signals rtRootInputs[] = {

  { 0, 0, TARGET_STRING("Gain/Inport"),
    TARGET_STRING("Input1"), 1, 0, 0, 0, 0 },

  {
    0, 0, (NULL), (NULL), 0, 0, 0, 0, 0
  }
};

static rtwCAPI_Signals rtRootOutputs[] = {

  { 1, 0, TARGET_STRING("Gain/Outport"),
    TARGET_STRING("Output1"), 1, 0, 0, 0, 0 },

  {
    0, 0, (NULL), (NULL), 0, 0, 0, 0, 0
  }
};

static rtwCAPI_ModelParameters rtModelParameters[] = {

  { 2, TARGET_STRING("Param1"), 0, 0, 0 },

  { 0, (NULL), 0, 0, 0 }
};

static TARGET_CONST rtwCAPI_DataTypeMap rtDataTypeMap[] = {

  { "float", "real32_T", 0, 0, sizeof(real32_T), (uint8_T)SS_SINGLE, 0, 0, 0 }
};

#ifdef HOST_CAPI_BUILD
#undef sizeof
#endif

static TARGET_CONST rtwCAPI_ElementMap rtElementMap[] = {

  { (NULL), 0, 0, 0, 0 },
};

static rtwCAPI_DimensionMap rtDimensionMap[] = {

  { rtwCAPI_SCALAR, 0, 2, 0 }
};

static uint_T rtDimensionArray[] = {
  1,
  1
};

static const real_T rtcapiStoredFloats[] = {
  0.2, 0.0
};

static rtwCAPI_FixPtMap rtFixPtMap[] = {

  { (NULL), (NULL), rtwCAPI_FIX_RESERVED, 0, 0, (boolean_T)0 },
};

static rtwCAPI_SampleTimeMap rtSampleTimeMap[] = {

  { (const void *) &rtcapiStoredFloats[0], (const void *) &rtcapiStoredFloats[1],
    (int8_T)0, (uint8_T)0 }
};

static rtwCAPI_ModelMappingStaticInfo mmiStatic = {

  { (NULL), 0,
    rtRootInputs, 1,
    rtRootOutputs, 1 },

  { rtBlockParameters, 0,
    rtModelParameters, 1 },

  { (NULL), 0 },

  { rtDataTypeMap, rtDimensionMap, rtFixPtMap,
    rtElementMap, rtSampleTimeMap, rtDimensionArray },
  "float",

  { 3243015861U,
    1808538051U,
    4093236672U,
    3186360495U },
  (NULL), 0,
  (boolean_T)0
};

const rtwCAPI_ModelMappingStaticInfo*
  Gain_GetCAPIStaticMap(void)
{
  return &mmiStatic;
}

#ifndef HOST_CAPI_BUILD

void Gain_InitializeDataMapInfo(RT_MODEL_Gain_T *const Gain_M)
{
  ExtU_Gain_T *Gain_U = (ExtU_Gain_T *) Gain_M->inputs;
  ExtY_Gain_T *Gain_Y = (ExtY_Gain_T *) Gain_M->outputs;
  void* *rtDataAddrMap;
  int32_T* *rtVarDimsAddrMap;
  rt_FREE( rtwCAPI_GetDataAddressMap( &(Gain_M->DataMapInfo.mmi) ) );
  rtDataAddrMap = (void* *) malloc(3 * sizeof(void* ));
  if ((rtDataAddrMap) == (NULL)) {
    ((void) 0);
    return;
  }

  rtDataAddrMap[0] = (void* )(&Gain_U->Input1);
  rtDataAddrMap[1] = (void* )(&Gain_Y->Outport);
  rtDataAddrMap[2] = (void* )(&Gain_P.Param1);
  rt_FREE( rtwCAPI_GetVarDimsAddressMap( &(Gain_M->DataMapInfo.mmi) ) );
  rtVarDimsAddrMap = (int32_T* *) malloc(1 * sizeof(int32_T* ));
  if ((rtVarDimsAddrMap) == (NULL)) {
    ((void) 0);
    return;
  }

  rtVarDimsAddrMap[0] = (int32_T* )((NULL));
  rtwCAPI_SetVersion(Gain_M->DataMapInfo.mmi, 1);
  rtwCAPI_SetStaticMap(Gain_M->DataMapInfo.mmi, &mmiStatic);
  rtwCAPI_SetLoggingStaticMap(Gain_M->DataMapInfo.mmi, (NULL));
  rtwCAPI_SetDataAddressMap(Gain_M->DataMapInfo.mmi, rtDataAddrMap);
  rtwCAPI_SetVarDimsAddressMap(Gain_M->DataMapInfo.mmi, rtVarDimsAddrMap);
  rtwCAPI_SetInstanceLoggingInfo(Gain_M->DataMapInfo.mmi, (NULL));
  rtwCAPI_SetChildMMIArray(Gain_M->DataMapInfo.mmi, (NULL));
  rtwCAPI_SetChildMMIArrayLen(Gain_M->DataMapInfo.mmi, 0);
}

#else
#ifdef __cplusplus

extern "C" {

#endif

  void Gain_host_InitializeDataMapInfo(Gain_host_DataMapInfo_T *dataMap, const
    char *path)
  {
    rtwCAPI_SetVersion(dataMap->mmi, 1);
    rtwCAPI_SetStaticMap(dataMap->mmi, &mmiStatic);
    rtwCAPI_SetDataAddressMap(dataMap->mmi, (NULL));
    rtwCAPI_SetVarDimsAddressMap(dataMap->mmi, (NULL));
    rtwCAPI_SetPath(dataMap->mmi, path);
    rtwCAPI_SetFullPath(dataMap->mmi, (NULL));
    rtwCAPI_SetChildMMIArray(dataMap->mmi, (NULL));
    rtwCAPI_SetChildMMIArrayLen(dataMap->mmi, 0);
  }

#ifdef __cplusplus

}
#endif
#endif

