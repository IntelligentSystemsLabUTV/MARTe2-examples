/**
 * @file StrobeFun.cpp
 * @brief Source file for class StrobeFun
 * @date 19/05/2017
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
 * the class StrobeFun (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <math.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "FastMath.h"
#include "StrobeFun.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

StrobeFun::StrobeFun() :
        Strobe() {
    	amplitude = 0.0;
    	frequency = 0.0;
    	w = 2.0 * FastMath::PI * frequency;
    	phase = 0.0;
    	offset = 0.0;
    	val = 0;
}

StrobeFun::~StrobeFun() {

}

bool StrobeFun::Initialise(StructuredDataI& data) {
    bool ok = Strobe::Initialise(data);
    if (!ok) { //Waveform only fails if GAM::initialize fails (a wrong trigger configurations is a warning, not a InitialisationError)
        REPORT_ERROR(ErrorManagement::InitialisationError, "Error. Waveform returns an initialization error");
    }
    if (ok) {
        ok = data.Read("Amplitude", amplitude);
        if (ok) {
            ok = !(IsEqual(amplitude, 0.0));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "amplitude must be different from 0");
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading Amplitude");
        }
    }
    if (ok) {
        ok = data.Read("Frequency", frequency);
        if (ok) {
            if (IsEqual(frequency, 0.0)) {
                REPORT_ERROR(ErrorManagement::Warning, "The frequency is 0. If phase = 0 the output will be always 0");
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading Frequency");
        }
    }
    if (ok) {
        w = 2.0 * FastMath::PI * frequency;
    }
    if (ok) {
        ok = data.Read("Phase", phase);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading Phase");
        }
    }
    if (ok) {
        ok = data.Read("Offset", offset);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading offset");
        }
    }
    if (ok) {
        ok = data.Read("Val", val);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading val");
        }
    }
    return ok;
}

bool StrobeFun::PrecomputeValues() {
    for (uint32 i = 0u; i < numberOfOutputElements; i++) {
        TriggerMechanism();
        if (signalOn && triggersOn) {
 	    // set val
            outputFloat64[i] = val;
        }
        else {
            outputFloat64[i] = 0.0;
        }
        currentTime += timeIncrement;
    }
    return true;
}

bool StrobeFun::TimeIncrementValidation() {
    bool ok = ((1.0 / timeIncrement) / 2.0) >= frequency;
    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, "sample frequency /2 < frequency");
    }
    return ok;
}

bool StrobeFun::GetUInt8Value() {
    return StrobeFun::GetValue<uint8>();
}

bool StrobeFun::GetInt8Value() {
    return StrobeFun::GetValue<int8>();
}

bool StrobeFun::GetInt16Value() {
    return StrobeFun::GetValue<int16>();
}

bool StrobeFun::GetUInt16Value() {
    return StrobeFun::GetValue<uint16>();
}

bool StrobeFun::GetInt32Value() {
    return StrobeFun::GetValue<int32>();
}

bool StrobeFun::GetUInt32Value() {
    return StrobeFun::GetValue<uint32>();
}

bool StrobeFun::GetInt64Value() {
    return StrobeFun::GetValue<int64>();
}

bool StrobeFun::GetUInt64Value() {
    return StrobeFun::GetValue<uint64>();
}

bool StrobeFun::GetFloat32Value() {
    return StrobeFun::GetValue<float32>();
}

bool StrobeFun::GetFloat64Value() {
    return StrobeFun::GetValue<float64>();
}
CLASS_REGISTER(StrobeFun, "1.0")

}
