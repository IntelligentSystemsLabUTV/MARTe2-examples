/**
 * @file WaveformChirp.h
 * @brief Header file for class WaveformChirp
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

 * @details This header file contains the declaration of the class WaveformChirp
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef WAVEFORMCHIRP_H_
#define WAVEFORMCHIRP_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Waveform.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief GAM which synthesise a chirp signal.
 * @details Computes a chirp using the following expression:
 *
 * \f$
 *  outputFloat64[i] = (amplitude * sin((w1 * currentTime + w12 * currentTime^2 / cD2) + phase)) + offset
 * \f$
 *
 * Where\n
 * w1 = 2 *pi * frequency1\n
 * w12 = 2*pi* (frequency2-frequency1)\n
 * cD2 = 2 * timeChirp\n
 * phase in radians\n
 * frequency1 initial frequency\n
 * frequency2 final frequency \n
 *
 *The configuration syntax is (names and signal quantity are only given as an example):
 *<pre>
 * +waveformChirp1 = {
 *     Class = WaveformChirp
 *     Amplitude = 10.0
 *     Frequency1 = 1.0
 *     Frequency2 = 3.0
 *     Phase = 0.0
 *     Offset = 1.1
 *     StartTriggerTime = {0.1 0.3 0.5 1.8}
 *     StopTriggerTime = {0.2 0.4 0.6} //the StopTriggerTime has one time less, it means that after the sequence of output on and off, the GAM will remain on forever
 *     Time = {
 *         InputSignal1 = {
 *             DataSource = "DDB1"
 *             Type = uint32 //Supported type uint32 (int32 also valid since time cannot be negative. uint64 && int64 valid types)
 *         }
 *     }
 *     OutputSignals = {
 *         OutputSignal1 = {
 *             DataSource = "LCD"
 *             Type = float32
 *         }
 *         OutputSignal2 = {
 *             DataSource = "LCD"
 *             Type = float64
 *         }
 *     }
 * }
 * </pre>
 *
 * Note that when Frequency1 = Frequency2 the resultant chirp is a sinusoidal waveform with a constant frequency.
 */
class WaveformChirp: public Waveform {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief default constructor
     * @post
     * amplitude = 0.0\n
     * phase = 0.0\n
     * offset = 0.0\n
     * frequency1 = 0.0\n
     * frequency2 = 0.0\n
     * w1 = 0.0\n
     * w2 = 0.0\n
     * w12 = w2 - w1\n
     * chirpDuration = 0.0\n
     * cD2 = chirpDuration * 2\n
     */
WaveformChirp    ();

    /**
     * @brief default destructor
     *
     */
    virtual ~WaveformChirp();

    /**
     * @brief Load and initialize the variables to synthesise the chirp signal.
     * @details Load the following parameters:\n
     * f1\n
     * f2\n
     * chirpDration\n
     * phase\n
     * offset\n
     * amplitude\n
     *
     * And calculate the following ones:\n
     * w1\n
     * w12\n
     * cD2\n
     * Moreover it checks that the parameters are correct and consistent.
     *
     * @return true if the parameters are loaded correctly and the the verifications are positive.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief computes the chirp signal in uint8.
     * @details this function calls the template function GetValue() with the uint8 type
     * @return true always
     */
    virtual bool GetUInt8Value();

    /**
     * @brief computes the chirp signal in int8.
     * @details this function calls the template function GetValue() with the int8 type
     * @return true always
     */
    virtual bool GetInt8Value();

    /**
     * @brief computes the chirp signal in uint16.
     * @details this function calls the template function GetValue() with the uint16 type
     * @return true always
     */
    virtual bool GetUInt16Value();

    /**
     * @brief computes the chirp signal in int16.
     * @details this function calls the template function GetValue() with the int16 type
     * @return true always
     */
    virtual bool GetInt16Value();

    /**
     * @brief computes the chirp signal in uint32.
     * @details this function calls the template function GetValue() with the uint32 type
     * @return true always
     */
    virtual bool GetUInt32Value();

    /**
     * @brief computes the chirp signal in int32.
     * @details this function calls the template function GetValue() with the int32 type
     * @return true always
     */
    virtual bool GetInt32Value();

    /**
     * @brief computes the chirp signal in uin64.
     * @details this function calls the template function GetValue() with the uint64 type
     * @return true always
     */
    virtual bool GetUInt64Value();

    /**
     * @brief computes the chirp signal in int64.
     * @details this function calls the template function GetValue() with the int64 type
     * @return true always
     */
    virtual bool GetInt64Value();

    /**
     * @brief computes the chirp signal in float32.
     * @details this function calls the template function GetValue() with the float32 type
     * @return true always
     */
    virtual bool GetFloat32Value();

    /**
     * @brief computes the chirp signal in float64.
     * @details this function calls the template function GetValue() with the float64 type
     * @return true always
     */
    virtual bool GetFloat64Value();

protected:

    /**
     * @brief computes the chirp signal in float64
     * @details computes the following operations:
     *
     * \f$
     * outputFloat64[i] = (amplitude * sin((w1 * currentTime + w12 * currentTime^2 / cD2) + phase)) + offset
     * \f$
     *
     * and save the data in #MARTe#Waveform::outputFloat64
     * @return true always
     */
    virtual bool PrecomputeValues();

    /**
     * @brief check that increment between consecutive samples have enough resolution to not violate the sample frequency theorem.
     */
    virtual bool TimeIncrementValidation();

private:

    /**
     * @brief Cast the chirp signal to the specified type.
     * @details Template method which cast the chirp signal saved in #MARTe#Waveform::outputFloat64
     * @return true always
     */
    template<typename T>
    bool GetValue();

    /**
     * Amplitude of the chirp signal
     */
    float64 amplitude;

    /**
     * phase of the signal
     */
    float64 phase;

    /**
     * offset of the chirp signal
     */
    float64 offset;

    /**
     * frequency 1
     */
    float64 frequency1;

    /**
     * frequency 2
     */
    float64 frequency2;

    /**
     * w1 = 2 * PI * f1
     */
    float64 w1;

    /**
     * w2 = 2 * PI * f2
     */
    float64 w2;

    /**
     * w2-w1
     */
    float64 w12;

    /**
     * Duration of the chirp. When the time is = chripDuration the instantaneous frequency is f2
     */
    float64 chirpDuration;

    /**
     * 2*chirpDuration
     */
    float64 cD2;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {

template<typename T>
bool WaveformChirp::GetValue() {
for (uint32 i = 0u; (i < numberOfOutputElements); i++) {
    static_cast<T *>(outputValue[indexOutputSignal])[i] = static_cast<T>(outputFloat64[i]);
}
return true;
}
}
#endif /* WAVEFORMCHIRP_H_ */

