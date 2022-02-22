/**
 * @file WaveformPointsDef.h
 * @brief Header file for class WaveformPointsDef
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

 * @details This header file contains the declaration of the class WaveformPointsDef
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef WAVEFORMPOINTSDEFGAM_H_
#define WAVEFORMPOINTSDEFGAM_H_

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
 * @brief GAM which generates a signal linearly interpolating a configurable list of points.
 * @details Given an array of values and another with times, this GAM interpolates the values.
 * When the sequence is finished, the next value is the first value of the sequence (no interpolation is done between the last and the first value),
 * i.e. the sequence is repeated from the beginning.
 * The configuration syntax is (names and signal quantity are only given as an example):
 *<pre>
 * +waveformPointsDef1 = {
 *     Class = WaveformPointsDef
 *     Points = {10.0 5.1 0.3 3}
 *     Times = {0.0 2.1 3.2 4.5} //time in seconds (if the times are not multiple of the sampling time the interpolation will be done but the output
 *     values may not match with the Points array).
 *     StartTriggerTime = {0.1 0.3 0.5 1.8}
 *     StopTriggerTime = {0.2 0.4 0.6} //the StopTriggerTime has one time less, it means that after the sequence of output on and off, the GAM will remain on forever
 *     InputSignals = {
 *         Time = {
 *             DataSource = "DDB1"
 *             Type = uint32 //Supported type uint32 (int32 | int64 | uint64 are also valid types)
 *         }
 *     }
 *     OutputSignals = {
 *         OutputSignal1 = {
 *             DataSource = "LCD"
 *             Type = float32
 *         }
 *         OutputSignal2 = {
 *             DataSource = "LCD"
 *             Type = int8
 *         }
 *     }
 * }
 * </pre>
 *
 * The minimum number of points must be two, otherwise it is impossible to interpolated and the GAM exits with an initialisation error
 */
class WaveformPointsDef: public Waveform {
public:CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor
     * @post
     * points = NULL_PTR(float64 *)\n
     * times = NULL_PTR(float64 *)\n
     * numberOfPointsElements = 0u\n
     * numberOfTimesElements = 0u\n
     * lastOutputValue = 0.0\n
     * refVal = 0.0\n
     * timeRefVal = 0.0\n
     * indexSlopes = 0u\n
     * beginningSequence = true\n
     * lastTimeValue = 0.0\n
     */
    WaveformPointsDef ();
    /**
     * @brief default destructor
     * @details frees the memory allocated by this GAM and points the pointers to NULL
     * @post
     *  points = NULL_PTR(float64 *)\n
     *  times = NULL_PTR(float64 *)\n
     */
    virtual ~WaveformPointsDef();

    /**
     * @brief Initialise the GAM from a configuration file.
     * @details Loads the parameters \a Points and \a Times and verify its correctness and consistency.
     * Moreover, from Points and Times vector, the slope for each segment is calculated.
     * @return true if all parameters are valid
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief computes the waveform interpolating points in uint8.
     * @details this function calls the template function GetValue() with the uint8 type
     * @return true always
     */
    virtual bool GetUInt8Value();

    /**
     * @brief computes the waveform interpolating points in int8.
     * @details this function calls the template function GetValue() with the int8 type
     * @return true always
     */
    virtual bool GetInt8Value();

    /**
     * @brief computes the waveform interpolating points in uint16.
     * @details this function calls the template function GetValue() with the uint16 type
     * @return true always
     */
    virtual bool GetUInt16Value();

    /**
     * @brief computes the waveform interpolating points in int16.
     * @details this function calls the template function GetValue() with the int16 type
     * @return true always
     */
    virtual bool GetInt16Value();

    /**
     * @brief computes the waveform interpolating points in uint32.
     * @details this function calls the template function GetValue() with the uint32 type
     * @return true always
     */
    virtual bool GetUInt32Value();

    /**
     * @brief computes the waveform interpolating points in int32.
     * @details this function calls the template function GetValue() with the int32 type
     * @return true always
     */
    virtual bool GetInt32Value();

    /**
     * @brief computes the waveform interpolating points in uint64.
     * @details this function calls the template function GetValue() with the uint64 type
     * @return true always
     */
    virtual bool GetUInt64Value();

    /**
     * @brief computes the waveform interpolating points in int64.
     * @details this function calls the template function GetValue() with the int64 type
     * @return true always
     */
    virtual bool GetInt64Value();

    /**
     * @brief computes the waveform interpolating points in float32.
     * @details this function calls the template function GetValue() with the float32 type
     * @return true always
     */
    virtual bool GetFloat32Value();

    /**
     * @brief computes the waveform interpolating points in float64.
     * @details this function calls the template function GetValue() with the float64 type
     * @return true always
     */
    virtual bool GetFloat64Value();

protected:
    /**
     * @brief computes the signal defined by points in in float64
     * @details computes the following operations:
     *
     * \f$
     * outputFloat64[i] = pointRef1 + ((currentTime - timeRef1) * slope)
     * \f$
     *
     * and save the data in #MARTe#Waveform::outputFloat64
     * @return true always
     */
    virtual bool PrecomputeValues();

    /**
     * @brief Validates that the time between samples is small enough.
     * @details check that the time increment between samples is smaller than the times interval specified in the configuration file.
     * @returns true if timeIncrement is smaller than the largest times.
     */
    virtual bool TimeIncrementValidation();
private:

    /**
     * @brief Cast the signal defined by points to the specified type.
     * @details Template method which cast the signal defined by points computed with GetFloat64OutputValues()
     * @return true always
     */
    template<typename T>
    bool GetValue();

    /**
     * Binary search for preq in p.
     * @param[in] preq the value to search.
     * @param[in] p the array to be searched.
     * @param[in] sizeOfArray the size of p.
     * @param[out] index the index of preq in p, if found.
     * @return true if preq is found.
     */
    bool SearchIndex(const float64 preq, const float64 * const p, const uint32 sizeOfArray, uint32& index) const;

    /**
     * Array to store the values.
     */
    float64 *points;

    /**
     * times array to indicate at which time there is a new point. The first time must be 0
     */
    float64 *times;

    /**
     * Indicates the current applied slope
     */
    uint32 indexSlopes;

    /**
     * number of points elements. The minimum number must be 2
     */
    uint32 numberOfPointsElements;

    /**
     * numberOfTImesElements. It must be equal than numberOfPointsElements
     */
    uint32 numberOfTimesElements;

    /**
     * remember the last output value
     */
    float64 lastOutputValue;

    /**
     * Indicates if it is the beginning of the points sequence
     */
    bool beginningSequence;

    /**
     * Used to move the pattern for the next iteration
     */
    float64 lastTimeValue;

    /**
     * Used to interpolate
     */
    float64 pointRef1;

    /**
     * Used to interpolate
     */
    float64 pointRef2;

    /**
     * Used to interpolate
     */
    float64 timeRef1;

    /**
     * Used to interpolate
     */
    float64 timeRef2;

    /**
     * Used to interpolate. slope = (pointRef2 - pointRef1)/(timeRef2 - timeRef1)
     */
    float64 slope;

    /*remind the last cycle time*/
    float64 remindTime;

    /**
     * @brief Verifies if the times values are correctly configured.
     * @details It checks that the first element is equal than 0.0 and the time is increasing.
     * @return true if the time is consistent with the previous listed conditions.
     */
    bool VerifyTimes() const;

    /**
     * @brief Using the time decides between which points the interpolation must be done.
     * @details given the times array looks the nearest points to currentTime. If currentTime is beyond
     * the array the function updates times for the next interval and tries to find again.
     */
    void FindNearestPoints();

    /**
     * @brief computes the slope to be applied.
     * @details slope = (pointRef2 - pointRef1)/(timeRef2 - timeRef1);
     */
    void Slope();

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {

template<typename T>
bool WaveformPointsDef::GetValue() {
    for (uint32 i = 0u; (i < numberOfOutputElements); i++) {
        static_cast<T*>(outputValue[indexOutputSignal])[i] = static_cast<T>(outputFloat64[i]);
    }
    return true;
}

}

#endif /* WAVEFORMPOINTSDEFGAM_H_ */

