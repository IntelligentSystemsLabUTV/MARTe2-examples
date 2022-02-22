/**
 * @file StrobeSig.cpp
 * @brief Source file for class StrobeSig
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
 * the class StrobeSig (public, protected, and private). Be aware that some
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
#include "StrobeSig.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

StrobeSig::StrobeSig() :
        Strobe() {
    	val = 0;
}

StrobeSig::~StrobeSig() {

}

bool StrobeSig::Initialise(StructuredDataI& data) {
    bool ok = Strobe::Initialise(data);
    if (!ok) { //Waveform only fails if GAM::initialize fails (a wrong trigger configurations is a warning, not a InitialisationError)
        REPORT_ERROR(ErrorManagement::InitialisationError, "Error. Waveform returns an initialization error");
    }
    if (ok) {
        ok = data.Read("Val", val);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading val");
        }
    }
    return ok;
}

bool StrobeSig::PrecomputeValues() {
    for (uint32 i = 0u; i < numberOfOutputElements; i++) {
        TriggerMechanism();
        if (signalOn && triggersOn) {
 	    // set val
            outputFloat64[i] = val;
        }
        else {
            outputFloat64[i] = -1;
        }
    }
    return true;
}

bool StrobeSig::GetUInt8Value() {
    return StrobeSig::GetValue<uint8>();
}

bool StrobeSig::GetInt8Value() {
    return StrobeSig::GetValue<int8>();
}

bool StrobeSig::GetInt16Value() {
    return StrobeSig::GetValue<int16>();
}

bool StrobeSig::GetUInt16Value() {
    return StrobeSig::GetValue<uint16>();
}

bool StrobeSig::GetInt32Value() {
    return StrobeSig::GetValue<int32>();
}

bool StrobeSig::GetUInt32Value() {
    return StrobeSig::GetValue<uint32>();
}

bool StrobeSig::GetInt64Value() {
    return StrobeSig::GetValue<int64>();
}

bool StrobeSig::GetUInt64Value() {
    return StrobeSig::GetValue<uint64>();
}

bool StrobeSig::GetFloat32Value() {
    return StrobeSig::GetValue<float32>();
}

bool StrobeSig::GetFloat64Value() {
    return StrobeSig::GetValue<float64>();
}
CLASS_REGISTER(StrobeSig, "1.0")

}
