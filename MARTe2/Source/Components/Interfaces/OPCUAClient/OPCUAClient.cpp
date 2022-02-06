/**
 * @file OPCUAClient.cpp
 * @brief Source file for class OPCUAClient
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

 * @details This source file contains the definition of all the methods for
 * the class OPCUAClient (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "OPCUAClient.h"
#include "RegisteredMethodsMessageFilter.h"
#include "OPCUAVariable.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

OPCUAClient::OPCUAClient() :
        ReferenceContainer(),
        EmbeddedServiceMethodBinderI(),
        MessageI(),
        executor(*this) {
    stackSize = THREADS_DEFAULT_STACKSIZE * 4u;
    cpuMask = 0xffu;
    connected = false;
    retryInterval = 0.5F;
    retryNumber = 5;

    ReferenceT<RegisteredMethodsMessageFilter> filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
//    config = UA_ClientConfig_default;
//    opcuaClient = UA_Client_new(config);
    opcuaClient = UA_Client_new();
    UA_ClientConfig_setDefault(UA_Client_getConfig(opcuaClient));
    #if LOCALDEB
    REPORT_ERROR(ErrorManagement::Information, "Variable opcuaClient initialized"); // <<< PM
    #endif
    serverAddress.Seek(0LLU);
    // monitoredItemsContainer=this;
    OPCUAVariable::CreateLock();
    OPCUAVariable::SetContainer(this);
}

OPCUAClient::~OPCUAClient() {
    UA_StatusCode code = UA_Client_disconnect(opcuaClient);
    if (code == 0x00U) { /* UA_STATUSCODE_GOOD */
        UA_Client_delete(opcuaClient);
    }
}

void OPCUAClient::Purge(ReferenceContainer &purgeList) {
    if (!executor.Stop()) {
        if (!executor.Stop()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
        }
    }
    ReferenceContainer::Purge(purgeList);
}

bool OPCUAClient::Initialise(StructuredDataI & data) {
    bool ok = ReferenceContainer::Initialise(data);
    if (ok) {
        #if LOCALDEB
        REPORT_ERROR(ErrorManagement::Information, "OPCUAClient::Initialise() Entering Initialise"); // <<< CP  // <<< PM
        if (!data.Read("CPUs", cpuMask)) {
            REPORT_ERROR(ErrorManagement::Information, "OPCUAClient::Initialise() No CPUs defined. Using default = %d", cpuMask); // <<< PM
        }
        if (!data.Read("StackSize", stackSize)) {
            REPORT_ERROR(ErrorManagement::Information, "OPCUAClient::Initialise() No StackSize defined. Using default = %d", stackSize); // <<< PM
        }
        if (!data.Read("RetryInterval", retryInterval)) {
            REPORT_ERROR(ErrorManagement::ParametersError, "OPCUAClient::Initialise() No RetryInterval defined. Using default = %f", retryInterval);  // <<< PM
        }
        if (!data.Read("RetryNumber", retryNumber)) {
            REPORT_ERROR(ErrorManagement::ParametersError, "OPCUAClient::Initialise() No RetryNumber defined. Using default = %d", retryNumber); // <<< PM
        }
        #endif
        ok = data.Read("Address", serverAddress);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "OPCUAClient::Initialise() Cannot read the Address attribute");
        }
        else {
            #if LOCALDEB
            REPORT_ERROR(ErrorManagement::Information, "OPCUAClient::Initialise() CPUs = %d", cpuMask); // <<< CP // <<< PM
            REPORT_ERROR(ErrorManagement::Information, "OPCUAClient::Initialise() StackSize = %d", stackSize); // <<< CP // <<< PM
            #endif
            executor.SetStackSize(stackSize);
            executor.SetCPUMask(cpuMask);
            uint32 autoStart = 1u;
            (void) (data.Read("AutoStart", autoStart));
            if (autoStart == 1u) {
                #if LOCALDEB
                REPORT_ERROR(ErrorManagement::Information, "OPCUAClient::Initialise()  Initialise before ConnectTry"); // <<< PM
                #endif
                OPCUAVariable::Lock();
                ok = ConnectTry(retryInterval, retryNumber);
                OPCUAVariable::UnLock();
                #if LOCALDEB
                REPORT_ERROR(ErrorManagement::Information, "OPCUAClient::Initialise()  Initialise after ConnectTry"); // <<< PM
                #endif
                if (ok)
                    ok = (Start() == ErrorManagement::NoError);
                #if LOCALDEB
                else
                    REPORT_ERROR(ErrorManagement::FatalError, "OPCUAClient::Initialise()  Cannot connect to OPCUA server"); // <<< PM
                #endif
            }
        }
    }
    #if LOCALDEB
    else {
        REPORT_ERROR(ErrorManagement::Information, "OPCUAClient::Initialise()  ReferenceContainer is NOT initialised"); // <<< CP // <<< PM
    }
    #endif
    #if LOCALDEB
    REPORT_ERROR(ErrorManagement::Information, "OPCUAClient::Initialise()  Exiting initialise()"); // <<< CP // <<< PM
    #endif
    return ok;
}

bool OPCUAClient::Connect() {
    #if LOCALDEB
    REPORT_ERROR(ErrorManagement::Information, "OPCUAClient::Connect() Before UA_Client_connect"); // <<< PM
    #endif
    UA_StatusCode retval = UA_Client_connect(opcuaClient, const_cast<char8*>(serverAddress.Buffer()));
    #if LOCALDEB
    REPORT_ERROR(ErrorManagement::Information, "OPCUAClient::Connect() Before UA_Client_connect retval: %x - %s",retval,UA_StatusCode_name(retval)); // <<< CP
    #endif
    return ((retval & 0xFF000000) == UA_STATUSCODE_GOOD); /* UA_STATUSCODE_GOOD */
}

ErrorManagement::ErrorType OPCUAClient::Start() {
    #if LOCALDEB
    REPORT_ERROR(ErrorManagement::Information, "OPCUAClient::Start() Entering Start()"); // <<< PM
    #endif
    ErrorManagement::ErrorType err = executor.Start();
    #if LOCALDEB
    REPORT_ERROR(ErrorManagement::Information, "OPCUAClient::Start() Exiting Start()"); // <<< PM
    #endif
    return err;
}

EmbeddedThreadI::States OPCUAClient::GetStatus() {
    return executor.GetStatus();
}

ErrorManagement::ErrorType OPCUAClient::Execute(ExecutionInfo& info) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (info.GetStage() == ExecutionInfo::StartupStage) {
        #if LOCALDEB
        REPORT_ERROR(ErrorManagement::Information, "OPCUAClient::Execute() Entering Execute (StartupStage)"); // <<< PM
        #endif
        bool ok = true;
        if (!connected) {
            #if LOCALDEB
            REPORT_ERROR(ErrorManagement::Information, "OPCUAClient::Execute() !connected"); // <<< PM
            #endif
            ok = false;
        }
        if (ok) {
            uint32 j;
		    OPCUAVariable::Lock();
            for (j = 0u; j < Size(); j++) {
                ReferenceT<OPCUAVariable> child = Get(j);
                if (child.IsValid()) {
                    ok = child->PostInit(opcuaClient);
                    if (!ok)
                        err = ErrorManagement::FatalError;
                }
            }
		OPCUAVariable::UnLock();
        }
        else {
            err = ErrorManagement::FatalError;
            #if LOCALDEB
            REPORT_ERROR(err, "OPCUAClient::Execute() Connection failed"); // <<< PM
            #endif
            executor.Stop();
        }
    }
    else if (info.GetStage() != ExecutionInfo::BadTerminationStage) {
        //  REPORT_ERROR(ErrorManagement::Information, "Sleeping - Stage != ExecutionInfo::BadTerminationStage"); // <<< PM
        Sleep::Sec(0.1F);
        OPCUAVariable::Lock();
//        UA_Client_runAsync(opcuaClient,100);//changed from run_iterate
        if (connected)
            UA_Client_run_iterate(opcuaClient, 100);
        OPCUAVariable::UnLock();
    }
    #if LOCALDEB
    else {
        REPORT_ERROR(ErrorManagement::Information, "OPCUAWrite::Execute()  ExecutionInfo::BadTerminationStage"); // <<< PM
    }
    #endif
    return err;
}

uint32 OPCUAClient::GetStackSize() const {
    return stackSize;
}

uint32 OPCUAClient::GetCPUMask() const {
    return cpuMask;
}

bool OPCUAClient::ConnectTry(uint32 interval, uint32 retry) {
    bool ok = true;
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    do {
        #if LOCALDEB
        REPORT_ERROR(ErrorManagement::Information, "OPCUAClient::ConnectTry() before connect - retry %d",retry); // <<< PM
        #endif
        ok = Connect();
        #if LOCALDEB
        REPORT_ERROR(ErrorManagement::Information, "OPCUAClient::ConnectTry() after connect"); // <<< PM
        #endif
        if (ok) {
            #if LOCALDEB
            REPORT_ERROR(ErrorManagement::Information, "OPCUAClient::ConnectTry() The connection with the OPCUA Server has been established successfully!");  // <<< PM
            #endif
            connected = true;
        }
        else {
            err = ErrorManagement::FatalError;
            #if LOCALDEB
            REPORT_ERROR(err, "OPCUAClient::ConnectTry() Connection failed");  // <<< PM
            #endif
        }
        Sleep::Sec(interval);
        retry--;
    } while (!connected && retry > 0);
    if (!connected)
        ok = false;
    return ok;
}

CLASS_REGISTER(OPCUAClient, "1.0")
CLASS_METHOD_REGISTER(OPCUAClient, Start)
}
