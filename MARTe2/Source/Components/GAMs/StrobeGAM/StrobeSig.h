/**
 * @file StrobeSig.h
 * @brief Header file for class StrobeSig
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

 * @details This header file contains the declaration of the class StrobeSig
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef StrobeSig_H_
#define StrobeSig_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Strobe.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief GAM which allows to write a value.
 * @details The configured value (val) must be specified as float64 and the implementation of the trigonometric functions is as follow
 *
 * \f$
 * output[i]= val
 * \f$ 
 *
 * There is no input. The output can be a single array of N elements or multiple equal outputs of N
 * elements with different types (i.e example type output1 = uint8 and type output2 = float64.
 * Note that in the first iteration the output is always 0 due to the fact that a second time is needed to compute the time step (or time increment) for each output sample.
 *
 * The GAM supports the following output types:<b>\n
 * int8\n
 * uint8\n
 * int16\n
 * uint16\n
 * int32\n
 * uint32\n
 * int64\n
 * uint64\n
 * float32\n
 * float64
 * </b>
 *
 * Additionally a trigger mechanism is implemented allowing to specify time intervals (in seconds) in which the output signal is switched off and on.
 * The trigger intervals are specified in two arrays: one with the StartTriggerTime and another with the StopTriggerTime. If the
 * two arrays are not consistent (i.e. start time is later than stop time) a warning is launched, the trigger mechanism is disabled,
 * but the GAM still operates in normal conditions (i.e. always on).
 *
 * The configuration syntax is (names and signal quantities are only given as an example):
 *<pre>
 * +StrobeSig1 = {
 *     Val = 5
 *     StartTriggerTime = {0.1 0.3 0.5 1.8}
 *     StopTriggerTime = {0.2 0.4 0.6} //the StopTriggerTime has one time value less. It means that after the sequence of output on and off, the GAM will remain on forever
 *     InputSignals = {
 *         Time = {
 *             DataSource = "DDB1"
 *             Type = uint32 //Supported type uint32 (int32 | uint64 int64 also valid)
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
 */

class StrobeSig: public Strobe {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor
     * @post
     * val = 0.0\n
     */
     StrobeSig();

    /**
     * @brief Default destructor
     */
    virtual ~StrobeSig();

    /**
     * @brief Initialise the GAM from a configuration file.
     * @details Loads the parameters of for the sin implementation and verify its correctness and consistency
     * @param[in] data is the configuration file.
     * @return true if all parameters are valid.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief computes the Sin in uint8.
     * @details this function calls the template function GetValue() with the uint8 type
     * @return true always.
     */
    virtual bool GetUInt8Value();

    /**
     * @brief computes the Sin in int8.
     * @details this function calls the template function GetValue() with the int8 type
     * @return true always
     */
    virtual bool GetInt8Value();

    /**
     * @brief computes the Sin in uint16.
     * @details this function calls the template function GetValue() with the uint16 type
     * @return true always
     */
    virtual bool GetUInt16Value();

    /**
     * @brief computes the Sin in in16.
     * @details this function calls the template function GetValue() with the int16 type
     * @return true always
     */
    virtual bool GetInt16Value();

    /**
     * @brief computes the Sin in uin32.
     * @details this function calls the template function GetValue() with the uint32 type
     * @return true always
     */
    virtual bool GetUInt32Value();

    /**
     * @brief computes the Sin in int32.
     * @details This function calls the template function GetValue() with the int32 type
     * @return true always
     */
    virtual bool GetInt32Value();

    /**
     * @brief computes the Sin in uin64.
     * @details This function calls the template function GetValue() with the uint64 type
     * @return true always
     */
    virtual bool GetUInt64Value();

    /**
     * @brief computes the Sin in int64.
     * @details This function calls the template function GetValue() with the int64 type
     * @return true always
     */
    virtual bool GetInt64Value();

    /**
     * @brief cast the sin to float32.
     * @details This function calls the template function GetValue() with the float32 type
     * @return true always
     */
    virtual bool GetFloat32Value();

    /**
     * @brief computes the Sin in float64.
     * @details This function calls the template function GetValue() with the float64 type
     * @return true always
     */
    virtual bool GetFloat64Value();

protected:
    /**
     * @brief computes the sin wave in float64
     * @details computes the following operations:
     *
     * \f$
     * outputFloat64[i]= val
     * \f$
     *
     * and save the data in #MARTe#Waveform::outputFloat64
     */
    virtual bool PrecomputeValues();

private:

    /**
     * @brief Cast the val in the specified type.
     * @details Template method which cast the sin wave computed with GetFloat64OutputValues()
     * @return true always
     */
    template<typename T>
    bool GetValue();
    
    /**
     * val to write
     */
    float64 val;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {
template<typename T>
bool StrobeSig::GetValue(){
    for(uint32 i = 0u; (i < numberOfOutputElements); i++){
        static_cast<T *>(outputValue[indexOutputSignal])[i] = static_cast<T>(outputFloat64[i]);
    }
    return true;
}
}

#endif /* StrobeSig_H_ */

