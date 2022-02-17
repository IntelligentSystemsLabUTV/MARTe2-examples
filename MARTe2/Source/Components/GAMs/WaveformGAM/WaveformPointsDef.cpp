/**
 * @file WaveformPointsDef.cpp
 * @brief Source file for class WaveformPointsDef
 * @date 29/05/2017
 * @author Llorenc Capella
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class WaveformPointsDef (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "WaveformPointsDef.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

WaveformPointsDef::WaveformPointsDef() :
        Waveform() {
    points = NULL_PTR(float64 *);
    times = NULL_PTR(float64 *);
    numberOfPointsElements = 0u;
    numberOfTimesElements = 0u;
    lastOutputValue = 0.0;
    indexSlopes = 0u;
    beginningSequence = true;
    lastTimeValue = 0.0;
    pointRef1 = 0.0;
    pointRef2 = 0.0;
    timeRef1 = 0.0;
    timeRef2 = 0.0;
    slope = 0.0;
    remindTime = 0.0;

}

WaveformPointsDef::~WaveformPointsDef() {
    if (points != NULL_PTR(float64 *)) {
        delete[] points;
        points = NULL_PTR(float64 *);
    }
    if (times != NULL_PTR(float64 *)) {
        delete[] times;
        times = NULL_PTR(float64 *);
    }
}
bool WaveformPointsDef::Initialise(StructuredDataI &data) {
    bool ok = Waveform::Initialise(data);
    if (!ok) { //Waveform::Initialise only fails if GAM::Initialise fails.
        REPORT_ERROR(ErrorManagement::InitialisationError, "Error. Waveform::Initialise");
    }
    AnyType functionArray = data.GetType("Points");
    if (ok) {
        ok = (functionArray.GetDataPointer() != NULL_PTR(void *));
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "Error. Fails to get the pointer to <Points>");
    }
    if (ok) {
        numberOfPointsElements = functionArray.GetNumberOfElements(0u);
        ok = (numberOfPointsElements > 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfPointsElements must be grater than 1 (at least two points must be defined)");
        }
    }
    if (ok) {
        points = new float64[numberOfPointsElements];
    }
    if (ok) {
        Vector<float64> pointsVector(points, numberOfPointsElements);
        ok = data.Read("Points", pointsVector);
        if (!ok) { //If the name Points exist it is difficult to not read the elements
            REPORT_ERROR(ErrorManagement::InitialisationError,
                         "Fails reading <Points> values. A Scalar is not possible, but a Vector of one element is allowed");
        }
    }
    AnyType functionArray1;
    if (ok) {
        functionArray1 = data.GetType("Times");
        ok = (functionArray1.GetDataPointer() != NULL_PTR(void *));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Fails to get the pointer to <Times>");
        }
    }
    if (ok) {
        numberOfTimesElements = functionArray1.GetNumberOfElements(0u);
        ok = (numberOfTimesElements == numberOfPointsElements);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfTimesElements must be equal than numberOfPointsElements");
        }
    }
    if (ok) {
        times = new float64[numberOfTimesElements];
    }
    if (ok) {
        Vector<float64> timesVector(times, numberOfTimesElements);
        ok = data.Read("Times", timesVector);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "fail reading Times values");
        }
    }
    if (ok) {
        ok = VerifyTimes();
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid time sequence. The first time must be 0.0 and time must be monotonous increasing");
        }
    }
    if (ok) {
        //If is due to MISRA are worried about null pointer...
        if (times != NULL_PTR(float64 *)) {
            lastTimeValue = times[numberOfTimesElements - 1u];
            // numberOfSlopeElements = numberOfTimesElements - 1u;
        }
    }
    return ok;
}

bool WaveformPointsDef::PrecomputeValues() {
    for (uint32 i = 0u; (i < numberOfOutputElements); i++) {
        TriggerMechanism();
        FindNearestPoints();
        Slope();
        if (signalOn && triggersOn) {
            if (outputFloat64 != NULL_PTR(float64 *)) {
                //outputFloat64[i] = refVal + ((currentTime - timeRefVal) * slopes[indexSlopes]);
                outputFloat64[i] = pointRef1 + ((currentTime - timeRef1) * slope);
            }
        }
        else {
            outputFloat64[i] = 0.0;
        }
        currentTime += timeIncrement;
    }
    return true;
}

//lint -e{613} Possible use of null pointer. This function only will be called if times is initialised.
bool WaveformPointsDef::TimeIncrementValidation() {
    bool ok = timeIncrement <= times[numberOfTimesElements - 1u];
    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError,
                     "%s::times interval is smaller than timeIncrement. timeIncrement = %.9f, times[numberOfTimesElements - 1u] = %.9f", GAMName.Buffer(),
                     timeIncrement, times[numberOfTimesElements - 1u]);
    }
    return ok;
}

bool WaveformPointsDef::GetInt8Value() {
    return WaveformPointsDef::GetValue<int8>();
}
bool WaveformPointsDef::GetUInt8Value() {
    return WaveformPointsDef::GetValue<uint8>();
}

bool WaveformPointsDef::GetInt16Value() {
    return WaveformPointsDef::GetValue<int16>();
}

bool WaveformPointsDef::GetUInt16Value() {
    return WaveformPointsDef::GetValue<uint16>();
}

bool WaveformPointsDef::GetInt32Value() {
    return WaveformPointsDef::GetValue<int32>();
}

bool WaveformPointsDef::GetUInt32Value() {
    return WaveformPointsDef::GetValue<uint32>();
}

bool WaveformPointsDef::GetInt64Value() {
    return WaveformPointsDef::GetValue<int64>();
}

bool WaveformPointsDef::GetUInt64Value() {
    return WaveformPointsDef::GetValue<uint64>();
}

bool WaveformPointsDef::GetFloat32Value() {
    return WaveformPointsDef::GetValue<float32>();
}

bool WaveformPointsDef::GetFloat64Value() {
    return WaveformPointsDef::GetValue<float64>();
}

/*lint -e{613} times cannot be NULL as otherwise VerifyTimes will not be called.*/
bool WaveformPointsDef::VerifyTimes() const {
    bool ret = (IsEqual(times[0], 0.0));
    for (uint32 i = 0u; (i < (numberOfTimesElements - 1u)) && ret; i++) {
        //Added due to MISRA:Suspicious Truncation in arithmetic expression combining with pointer
        uint32 aux = i + 1u;
        ret = (times[i] < times[aux]);
    }
    return ret;
}

bool WaveformPointsDef::SearchIndex(const float64 preq, const float64 * const p, const uint32 sizeOfArray, uint32& index) const {
    bool found = false;

    uint32 med;
    uint32 inf = 0u;
    uint32 sup = sizeOfArray;
    while (inf != sup) {
        med = (inf + sup) / 2u;
        if (p[med] <= preq) {
            inf = med + 1u;
        }
        else {
            sup = med;
        }
    }
    if (inf < sizeOfArray) {
        found = true;
    }
    index = sup;
    return found;
}

//lint -e{613} Possible use of a null pointer. It is not possible due to this function only is called inside Execute() and Execute() only is called if Setup() and initialise() succeed.
void WaveformPointsDef::FindNearestPoints() {
    bool found;
    uint32 i = 0u;
    found = SearchIndex(currentTime, times, numberOfTimesElements, i);
    if (found) {
        if (i > 0u) {
            uint32 auxIndex = i - 1u;
            pointRef1 = points[auxIndex];
            pointRef2 = points[i];
            timeRef1 = times[auxIndex];
            timeRef2 = times[i];
        }
        else {
            uint32 auxIdx = numberOfPointsElements - 1u;
            pointRef1 = points[auxIdx];
            pointRef2 = points[i];
            timeRef1 = remindTime;
            timeRef2 = times[i];
        }
    }
    else {
        uint32 auxIdx2 = numberOfTimesElements - 1u;
        remindTime = times[auxIdx2];
        for (uint32 m = 0u; m < numberOfTimesElements; m++) { //update times
            if (times != NULL_PTR(float64 *)) {
                times[m] = times[m] + lastTimeValue + timeIncrement;
            }
        }
        i = 0u;
        found = SearchIndex(currentTime, times, numberOfTimesElements, i);
        if (found) {
            if (i == 0u) {
                uint32 auxIdx = numberOfPointsElements - 1u;
                pointRef1 = points[auxIdx];
                pointRef2 = points[i];
                timeRef1 = remindTime;
                timeRef2 = times[i];
            }
            else {
                uint32 auxIndex = i - 1u;
                pointRef1 = points[auxIndex];
                pointRef2 = points[i];
                timeRef1 = times[auxIndex];
                timeRef2 = times[i];
            }
        }
    }
    return;
}

void WaveformPointsDef::Slope() {
    slope = (pointRef2 - pointRef1) / (timeRef2 - timeRef1);
    return;
}

CLASS_REGISTER(WaveformPointsDef, "1.0")

}

