#include "Gain_capi.h"
#include "Gain.h"
#include "Gain_private.h"

#define CONCAT(str1, str2, str3)       CONCAT_(str1, str2, str3)
#define CONCAT_(str1, str2, str3)      str1 ## str2 ## str3
#define GET_MMI_FUNC                   CONCAT(MODEL, _GetCAPImmi , )
#define RT_MODEL_STRUCT                CONCAT(RT_MODEL_ , MODEL, _T)

void* GET_MMI_FUNC(void* voidPtrToRealTimeStructure)
{
  rtwCAPI_ModelMappingInfo* mmiPtr = &(rtmGetDataMapInfo( ( RT_MODEL_STRUCT *)
    (voidPtrToRealTimeStructure) ).mmi);
  return (void*) mmiPtr;
}

void Gain_step(RT_MODEL_Gain_T *const Gain_M)
{
  ExtU_Gain_T *Gain_U = (ExtU_Gain_T *) Gain_M->inputs;
  ExtY_Gain_T *Gain_Y = (ExtY_Gain_T *) Gain_M->outputs;
  Gain_Y->Outport = Gain_P.Param1 * Gain_U->Input1;
}

void Gain_initialize(RT_MODEL_Gain_T *const Gain_M)
{
  UNUSED_PARAMETER(Gain_M);
}

RT_MODEL_Gain_T *Gain(void)
{
  RT_MODEL_Gain_T *Gain_M;
  Gain_M = (RT_MODEL_Gain_T *) malloc(sizeof(RT_MODEL_Gain_T));
  if (Gain_M == (NULL)) {
    return (NULL);
  }

  (void) memset((char *)Gain_M, 0,
                sizeof(RT_MODEL_Gain_T));

  {
    ExtU_Gain_T *Gain_U = (ExtU_Gain_T *) malloc(sizeof(ExtU_Gain_T));
    rt_VALIDATE_MEMORY(Gain_M,Gain_U);
    Gain_M->inputs = (((ExtU_Gain_T *) Gain_U));
  }

  {
    ExtY_Gain_T *Gain_Y = (ExtY_Gain_T *) malloc(sizeof(ExtY_Gain_T));
    rt_VALIDATE_MEMORY(Gain_M,Gain_Y);
    Gain_M->outputs = (Gain_Y);
  }

  Gain_InitializeDataMapInfo(Gain_M);

  {
    ExtU_Gain_T *Gain_U = (ExtU_Gain_T *) Gain_M->inputs;
    ExtY_Gain_T *Gain_Y = (ExtY_Gain_T *) Gain_M->outputs;
    Gain_U->Input1 = 0.0F;
    Gain_Y->Outport = 0.0F;
  }

  return Gain_M;
}
