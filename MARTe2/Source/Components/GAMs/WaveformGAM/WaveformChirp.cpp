/**
 * @file WaveformChirp.cpp
 * @brief Source file for class WaveformChirp
 * @date 02/06/2017
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
 * the class WaveformChirp (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <math.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "FastMath.h"
#include "WaveformChirp.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

WaveformChirp::WaveformChirp() :
        Waveform() {
    amplitude = 0.0;
    phase = 0.0;
    offset = 0.0;
    frequency1 = 0.0;
    frequency2 = 0.0;
    w1 = 0.0;
    w2 = 0.0;
    w12 = w2 - w1;
    chirpDuration = 0.0;
    cD2 = chirpDuration * 2.0;
}

WaveformChirp::~WaveformChirp() {
}

bool WaveformChirp::Initialise(StructuredDataI &data) {
    bool ok = Waveform::Initialise(data);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "Waveform class has returned an initialization error");
    }
    if (ok) {
        ok = data.Read("Amplitude", amplitude);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Failed to read <Amplitude>");
        }
    }
    if (ok) {
        ok = !IsEqual(amplitude, 0.0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "amplitude must be different from 0");
        }
    }
    if (ok) {
        ok = data.Read("Phase", phase);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Failed to read <Phase>");
        }
    }
    if (ok) {
        ok = data.Read("Offset", offset);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Failed to read <Offset>");
        }
    }
    if (ok) {
        ok = data.Read("Frequency1", frequency1);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Failed to read <Frequency1>");
        }
    }
    if (ok) {
        ok = data.Read("Frequency2", frequency2);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Failed to read <Frequency2>");
        }
    }
    if (ok) {
        w1 = 2.0 * FastMath::PI * frequency1;
        w2 = 2.0 * FastMath::PI * frequency2;
        w12 = w2 - w1;
    }
    if (ok) {
        ok = data.Read("ChirpDuration", chirpDuration);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Failed to read <ChirpDuration>");
        }
    }
    if (ok) {
        ok = (chirpDuration > 0.0);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "ChirpDuration must be positive");
        }
    }
    if (ok) {
        cD2 = chirpDuration * 2.0;
    }

    return ok;
}

bool WaveformChirp::PrecomputeValues() {
    for (uint32 i = 0u; i < numberOfOutputElements; i++) {
        TriggerMechanism();
        if (signalOn && triggersOn) {
            float64 aux = ((w1 * currentTime) + ((w12 * currentTime * currentTime) / cD2)) + phase;
            float64 aux2 = sin(aux);
            outputFloat64[i] = (amplitude * aux2) + offset;
        }
        else {
            outputFloat64[i] = 0.0;
        }
        currentTime += timeIncrement;
    }
    return true;
}

bool WaveformChirp::TimeIncrementValidation() {
    bool ok;
    if (frequency2 > frequency1) {
        ok = ((1.0 / timeIncrement) / 2.0) >= frequency2;
    }
    else {
        ok = ((1.0 / timeIncrement) / 2.0 )>= frequency1;
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, "%s::sample frequency /2 < maxFrequency", GAMName.Buffer());
    }
    return ok;
}

bool WaveformChirp::GetUInt8Value() {
    return WaveformChirp::GetValue<uint8>();
}

bool WaveformChirp::GetInt8Value() {
    return WaveformChirp::GetValue<int8>();
}

bool WaveformChirp::GetInt16Value() {
    return WaveformChirp::GetValue<int16>();
}

bool WaveformChirp::GetUInt16Value() {
    return WaveformChirp::GetValue<uint16>();
}

bool WaveformChirp::GetInt32Value() {
    return WaveformChirp::GetValue<int32>();
}

bool WaveformChirp::GetUInt32Value() {
    return WaveformChirp::GetValue<uint32>();
}

bool WaveformChirp::GetInt64Value() {
    return WaveformChirp::GetValue<int64>();
}

bool WaveformChirp::GetUInt64Value() {
    return WaveformChirp::GetValue<uint64>();
}

bool WaveformChirp::GetFloat32Value() {
    return WaveformChirp::GetValue<float32>();
}

bool WaveformChirp::GetFloat64Value() {
    return WaveformChirp::GetValue<float64>();
}
CLASS_REGISTER(WaveformChirp, "1.0")
}

