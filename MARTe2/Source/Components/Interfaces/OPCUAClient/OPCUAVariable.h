/**
 * @file OPCUAVariable.h
 * @brief Header file for class OPCUAVariable
 * @date 05/04/2019
 * @authors Chiara Piron, Luca Boncagni
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

 * @details This header file contains the declaration of the class OPCUAVariable
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef OPCUAVARIABLE_H_
#define OPCUAVARIABLE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
// #include <cadef.h>
#include "open62541.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MessageI.h"
#include "Object.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief Describes an OPCUA PV.
 * @details This class wraps an OPCUA PV, allowing to caput andOPCUARead values from it.
 * This class is capable of triggering Messages as a response to a PV value change. The new PV value can
 * either be: 1) the name of the Function to be called; 2) or an ID of a Function to be called (see FunctionMap);
 *  3) or the input parameter of a pre-defined Function to be called; or 4) or a pre-defined Function is to be called with no parameters (i.e. the PV value is to be ignored).
 * The configuration syntax is (names are only given as an example):
 *
 * <pre>
 * +PV_1 = {
 *   Class = OPCUAInterface::OPCUAVariable
 *   Path = PV_ONE //Compulsory. Path of the OPCUA PV.
 *   Type = uint32 //Compulsory. The PV type. Supported types are uint16, int16, uint32, int32, float32, float64 and string.
 *   NumberOfElements = 1 //Optional. Number of elements
 *   NameSpaceIndex = 1 // Optional 
 *   Timeout = 5.0 //Optional but if set shall be > 0. The timeout for the ca_pend_io operations in seconds. Default value is 5.0 s.
 *   Event = { //Optional. Information about the message to be triggered every-time the OPCUA PV value changes.
 *     Destination = StateMachine //Compulsory. Destination of the message.
 *     PVValue = Function //Compulsory. Can either be Function, Parameter, ParameterName or Ignore.
 *                        //If Function the PV value will be used as the name of the Function to call.
 *                        //If Parameter the PV value will be used as the parameter of the Function to call. This implies that the Function parameter must be set.
 *                        //If ParameterName the PV value will be used as the second parameter of the Function to call. The first parameter will the Object name. This implies that the Function parameter must be set.
 *                        //If Ignore, the PV value will not be used and the Function will always be called.
 *     Function = STOP //Compulsory if PVValue=Parameter, PVValue=ParameterName or PVValue=Ignore. Shall not be set if FunctionMap is defined or if PVValue=Function.
 *     FunctionMap = {{"1", "RUN"}, {"0", "STOP"}} //Optional Nx2 matrix. Only allowed if PVValue == Function. If defined then the PV value (first column of the matrix) will be used to map the Function name (second column of the matrix).
 *   }
 * }
 * </pre>
 *
 * The OPCUAWrite andOPCUARead class methods are registered as call-backs. The parameter to put/get shall be encoded as "param1" in a StructuredDataI attached to the message.
 *
 * If the Event section is defined the Messages triggered will have the Function defined as above and the parameter (if set) will be written with the key "param1".
 */


class OPCUAVariable: public Object, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. Register the CAPut andOPCUARead function calls for RPC.
     * @post
     *   GetTimeout() == 5.0
     */
    OPCUAVariable();

    /**
     * @brief Frees the FunctionMap memory if needed/
     */
    virtual ~OPCUAVariable();

    /**
     * @brief Calls Object::Initialise and reads the parameters defined in the class description.
     * @param[in] data see Object::Initialise.
     * @return true if Object::Initialise returns true and if the compulsory parameters are correctly set.
     */
    virtual bool Initialise(StructuredDataI & data);



    /**
     * @brief Triggered when this PV value changes.
     * @details The value of the PV will set to the \a dbr value. If the Event section (see class description) was defined
     * it will trigger the sending of a Message.
     * @param[in] args the new PV value.
     */
    void HandlePVEvent(UA_DataValue * value);



    /**
     * @brief Defines the possible EventMode values (Function, Parameter, ParameterName, Ignore).
     */
    /*lint ++flb*/
    union EventMode {
        /**
         * Mode is not set
         */
        BitBoolean<uint8, 0u> notSet;

        /**
         * Value is to be used as the function name
         */
        BitBoolean<uint8, 1u> function;

        /**
         * Value is to be used as the function parameter
         */
        BitBoolean<uint8, 2u> parameter;

        /**
         * Value is to be used as the second function parameter (the first parameter name is this object name).
         */
        BitBoolean<uint8, 3u> parameterName;

        /**
         * Value is to be ignored
         */
        BitBoolean<uint8, 4u> ignore;

        /**
         * Unmapped area
         */
        BitRange<uint8, 5u, 3u> unMapped;

        /**
         * Output as uint16
         */
        uint8 asUint8;
    };
    /*lint --flb*/


    /**
     * @brief Gets the Event mode set for this PV.
     * @return the Event mode set for this PV.
     */
    EventMode GetMode() const;

    /**
     * @brief Gets the ca_pend_io timeout.
     * @return the ca_pend_io timeout.
     */
    float64 GetTimeout() const;

    /**
     * @brief Gets the event message destination.
     * @return the event message destination.
     */
    StreamString GetDestination() const;

    /**
     * @brief Gets the event message destination Function (if set).
     * @return the event message destination Function.
     */
    StreamString GetFunction() const;

    /**
     * @brief Gets the event message destination Function from the FunctionMap.
     * @brief[key] the key to query.
     * @return the event message destination Function assigned to key or an empty StreamString if none was associated.
     */
    StreamString GetFunctionFromMap(const StreamString &key) const;

    /**
     * @brief Gets the nodePath.
     * @return the nodePath.
     */
    StreamString GetNodePath() const;


    /**
     * @brief Updates the value of the PV by calling an OPCUA write.
     * @details This function is registered with CLASS_METHOD_REGISTER and thus available for RPC.
     * @param[in] data shall contain a key named "param1" with the value to caput.
     * @return ErrorManagement::NoError if the value can be successfully caput (which implies that the low-level OPCUA function return no Error).
     */
    ErrorManagement::ErrorType OPCUAWrite(StructuredDataI &data);

    /**
     * @brief Gets the latest value of the PV (which is updated by OPCUA by calling the callback function HandlePVEvent) .
     * @details This function is registered with CLASS_METHOD_REGISTER and thus available for RPC.
     * @param[in] data will be updated with a key named "param1" where the value of the PV will be written into.
     * @return ErrorManagement::NoError if the value can be successfully written into "param1" (i.e. if data.Write succeeds).
     */
    ErrorManagement::ErrorType OPCUARead(StructuredDataI &data);

    /**
     * @brief Updates the value of the PV by calling an OPCUA caput using the value currently held in the memory of GetAnyType.
     * @return ErrorManagement::NoError if the value can be successfully caput (which implies that the low-level OPCUA function return no Error).
     */
    ErrorManagement::ErrorType CAPutRaw();

    /**
     * @brief Gets the AnyType which represents the variable wrapped by this OPCUAVariable.
     * @return the AnyType which represents the variable wrapped by this OPCUAVariable.
     */
    AnyType GetAnyType() const;

    /**
     * @brief Gets the memory size to hold this PV.
     * @return the memory size to hold this PV.
     * TODO test function!
     */
    uint32 GetMemorySize() const;


    /**
     * @brief
     * @return true if.
     */
    bool PostInit(UA_Client * cli);


    /**
     * @brief
     * @return true if.
     */
    static void  SetContainer(ReferenceContainer * PVContainer);


    /**
     * @brief
     * @return true if.
     */
    static void Lock();

    /**
     * @brief
     * @return true if.
     */
    static void UnLock();

    /**
     * @brief
     * @return true if.
     */
    static void CreateLock();

    /**
     * @brief
     * @return true if.
     */
    uint32 GetSubscriptionId();

    /**
     * @brief
     * @return true if.
     */
    bool IsFirstTime() {return firstTime;};

    /**
     * @brief
     * @return true if.
     */
    void SetNoMoreFirstTime() {firstTime=false;};


private:
    /**
     * @brief Triggers the sending of a Message with the rules defined in the class description.
     * @param[in] newValue the value to be sent (either as the Function name or the Function parameter).
     */
    void TriggerEventMessage();

    /**
     * @brief Parses the .cfg file to fill the path and pathSize class variables.
     * @return true if the nodePath is parsed.
     */
    bool ParsePath();

    /**
     * @brief Gets the OPCUA Reference Type Id for each node of the path.
     * @details This method uses the OPCUA Browse and BrowseNext services to obtain the OPCUA Reference Type
     * of the node declared in the path.
     */
    uint32 GetReferenceType(const UA_BrowseRequest &bReq,
                            const char8* const path,
                            uint16 &namespaceIndex,
                            uint32 &numericNodeId,
                            char8* &stringNodeId);

    /**
     * @brief Gets the name of the PV.
     * @return the name of the PV.
     */
    StreamString GetPath() const;


     /**
     * @brief Set the OPCUA client.
     * @return the AnyType which represents the variable wrapped by this OPCUAVariable.
     */
    void SetClient(UA_Client * cli);

     /**
     * @brief
     * @return true if.
     */
    bool BrowseRequest();

    /**
    * @brief Handles the PV subscribe or write request
    * @return true is the PV channel is created.
    */
    bool SetRequest();

    /**
     * The Node identifier.
     */
    UA_NodeId nodeId;

    /**
     * open62541 subscription request structure
     */
    UA_CreateSubscriptionRequest request;

    /**
     * @brief Writes a PV (if Event entry does not exist)
     * @details
     */
    bool SetWriteRequest();

    /**
     * Temporary open62541 void structure for internal operations.
     */
    UA_Variant tempVariant;

    /**
     * @brief Subscribes a PV (if Event entry exists)
     * @details
     */
      bool SetSubscriptionRequest();

    /**
     * open62541 subscription response structure
     */
    UA_CreateSubscriptionResponse response;

    /**
     * open62541 monitored item request structure
     */
    UA_MonitoredItemCreateRequest monitorRequest;

    /**
     * open62541 monitored item result structure
     */
    UA_MonitoredItemCreateResult monitorResponse;

    /**
     * signal the first subscription response received
     */
     bool firstTime;


    /**
     * The ca_pend_io timeout.
     */
    float64 timeout;

    /**
     * The name of the PV.
     */
    StreamString nodePath;

    /**
     *
     * The nameSpaceIndex.
     */
    uint16 nameSpaceIndex;

    /**
     * The size of the path array
     */
    uint16 pathSize;

    /**
     * The path array containing the in-between-dot elements of the PV path
     */
    StreamString* path;


    /**
     * The OPCUAVariable type
     */
    uint32 pvType;

    /**
     * The name of the event message destination object.
     */
    StreamString destination;

    /**
     * The function to be called on the event destination object.
     */
    StreamString function;

    /**
     * Nx2 matrix where the first column contains the keys to be mapped and the second column the value to be associated to each key.
     */
    StreamString *functionMap[2];

    /**
     * Number of rows in functionMap.
     */
    uint32 nOfFunctionMaps;

    /**
     * The type of event to triggered.
     */
    EventMode eventMode;

    /**
     * The memory of the PV (stored using the internal buffer of the pvMemoryStr StreamString)
     */
    void *pvMemory;

    /**
     * The total memory size
     */
    uint32 memorySize;

    /**
     * The type size
     */
    uint32 typeSize;

    /**
     * The number of elements to set
     */
    uint32 numberOfElements;

   /**
     * open62541 client declaration
     */
    UA_Client * opcuaClient;

    /**
     * The OPCUAVariable AnyType representation
     */
    AnyType pvAnyType;

        /**
     * open62541 write request structure
     */
    UA_WriteRequest writeRequest;


    /**
     * The array that stores all the open62541 write values for the nodes to write
     */
    UA_WriteValue writeValue;

    /**
     * Number of nodes to be managed
     */
    uint32 nOfNodes;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* OPCUAVARIABLECONTEXT_H_ */
