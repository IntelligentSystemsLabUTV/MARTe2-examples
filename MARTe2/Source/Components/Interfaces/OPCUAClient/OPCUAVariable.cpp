/**
 * @file OPCUAVariable.cpp
 * @brief Source file for class OPCUAVariable
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
 * the class OPCUAVariable (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "CLASSMETHODREGISTER.h"
#include "RegisteredMethodsMessageFilter.h"
#include "OPCUAVariable.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ReferenceContainer * monitoredItemsContainer;
static FastPollingMutexSem eventCallbackFastMux;

void OPCUAClientDataChange(UA_Client *client,
                           UA_UInt32 subId,
                           void *subContext,
                           UA_UInt32 monId,
                           void *monContext,
                           UA_DataValue *value) {


    uint32 i;
    for (i = 0u; i < monitoredItemsContainer->Size(); i++) {
        ReferenceT<OPCUAVariable> child = monitoredItemsContainer->Get(i);
        if (child.IsValid()) {
            if (child->GetSubscriptionId() == subId) {
                if (child->IsFirstTime()) {
                    child->SetNoMoreFirstTime();
                    #if LOCALDEB
                    REPORT_ERROR_STATIC(ErrorManagement::Information, "FirsTime for subscription %d!", subId); // <<< PM
                    #endif
                }
                else {
                    #if LOCALDEB
                    REPORT_ERROR_STATIC(ErrorManagement::Information, "Value has changed for subscription %d!", subId); // <<< PM
                    #endif
                    child->HandlePVEvent(value);
                }
            }
        }
    }
}

void OPCUAVariable::CreateLock() {
    eventCallbackFastMux.Create();

}
void OPCUAVariable::Lock() {
    (void) eventCallbackFastMux.FastLock();

}

void OPCUAVariable::UnLock() {
    eventCallbackFastMux.FastUnLock();

}
void OPCUAVariable::SetContainer(ReferenceContainer * PVContainer) {
    monitoredItemsContainer = PVContainer;

}

uint32 OPCUAVariable::GetSubscriptionId(void) {
    return response.subscriptionId;

}

OPCUAVariable::OPCUAVariable() :
        Object(),
        MessageI() {

    timeout = 5.0F;
    nodePath = "";
    nameSpaceIndex = 1u;
    path = NULL_PTR(StreamString *);
    pathSize = 0;
    firstTime = true;

    request = UA_CreateSubscriptionRequest_default();
    response.subscriptionId = 0u;
    monitorRequest.itemToMonitor.nodeId.identifier.numeric = 0u;
    monitorResponse.monitoredItemId = 0u;

    pvType = UA_TYPES_INT32;

    eventMode.asUint8 = 0u;
    nOfFunctionMaps = 0u;
    numberOfElements = 1u;
    nOfNodes = 0u;
    functionMap[0u] = NULL_PTR(StreamString *);
    functionMap[1u] = NULL_PTR(StreamString *);

    pvMemory = NULL_PTR(void *);
    memorySize = 0u;
    typeSize = 0u;

    ReferenceT<RegisteredMethodsMessageFilter> filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }

}

/*lint -e{1551} -e{1540} -e{1740} functionMap memory is freed in the destructor. The context, pvChid and pvMemory variables are managed (and thus freed) by the OPCUA framework.*/
OPCUAVariable::~OPCUAVariable() {

    if (functionMap[0u] != NULL_PTR(StreamString *)) {
        delete[] functionMap[0u];
    }
    if (functionMap[1u] != NULL_PTR(StreamString *)) {
        delete[] functionMap[1u];
    }
    if (pvMemory != NULL_PTR(void *)) {
        if (pvAnyType.GetTypeDescriptor().type == SString) {
            if (numberOfElements > 1u) {
                StreamString *str = static_cast<StreamString *>(pvMemory);
                delete[] str;
            }
            else {
                StreamString *str = static_cast<StreamString *>(pvMemory);
                delete str;
            }
        }
        else {
            GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(pvMemory);
        }
    }
    if (path != NULL_PTR(StreamString *))
        delete[] path;

}

bool OPCUAVariable::Initialise(StructuredDataI & data) {
    bool ok = Object::Initialise(data);
    #if LOCALDEB
    REPORT_ERROR(ErrorManagement::Information,"OPCUAVariable: Entering Initialise"); // <<< PM
    #endif
    if (ok) {
        ok = data.Read("Path", nodePath);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Path must be specified");
        }
    }
    if (ok) {
        if (!data.Read("NumberOfElements", numberOfElements)) {
            REPORT_ERROR(ErrorManagement::Warning, "numberOfElements not set. Using default of %d", numberOfElements);
        }
    }
    if (ok) {
        if (!data.Read("NameSpaceIndex", nameSpaceIndex)) {
            REPORT_ERROR(ErrorManagement::Warning, "NameSpaceIndex not set. Using default of %d", nameSpaceIndex);
        }
    }
    TypeDescriptor pvTypeDesc = VoidType;
    if (ok) {
        StreamString pvTypeStr;
        ok = data.Read("Type", pvTypeStr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Type must be specified");
        }
        if (ok) {
            //REPORT_ERROR(ErrorManagement::Information, "Getting type descriptor for  %s", pvTypeStr.Buffer());
            pvTypeDesc = TypeDescriptor::GetTypeDescriptorFromTypeName(pvTypeStr.Buffer());
            typeSize = (static_cast<uint32>(pvTypeDesc.numberOfBits) / 8u);

            REPORT_ERROR(ErrorManagement::Information, "Getting type descriptor for  %s ", pvTypeStr.Buffer());

            memorySize = (typeSize * numberOfElements);
            if (pvTypeDesc == SignedInteger16Bit) {
                pvType = UA_TYPES_INT16;
            }
            else if (pvTypeDesc == UnsignedInteger16Bit) {
                pvType = UA_TYPES_UINT16;
            }
            else if (pvTypeDesc == SignedInteger32Bit) {
                pvType = UA_TYPES_INT32;
                REPORT_ERROR(ErrorManagement::Information, " INT32 Getting type descriptor for  %s ", pvTypeStr.Buffer());

            }
            else if (pvTypeDesc == UnsignedInteger32Bit) {
                pvType = UA_TYPES_UINT32;
                REPORT_ERROR(ErrorManagement::Information, " UINT32 Getting type descriptor for  %s ", pvTypeStr.Buffer());

            }
            else if (pvTypeDesc == Float32Bit) {
                pvType = UA_TYPES_FLOAT;
            }
            else if (pvTypeDesc == Float64Bit) {
                pvType = UA_TYPES_DOUBLE;

            }
            else {
                ok = false;
                REPORT_ERROR(ErrorManagement::ParametersError, "Type %s is not supported", pvTypeStr.Buffer());
            }

            if (ok)
                ok = ParsePath();

            if (ok)
                pvMemory = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(memorySize);
        }
    }

    if (ok) {
        if (!data.Read("Timeout", timeout)) {
            REPORT_ERROR(ErrorManagement::Warning, "opcua subscription timeout not set. Using default of %f", timeout);
        }
        if (timeout < 1e-6) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Timeout shall be > 0s");
            ok = false;
        }
    }

    if (data.MoveRelative("Event")) {
        if (ok) {
            ok = data.Read("Destination", destination);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Destination must be specified");
            }
        }
        if (ok) {
            StreamString modeValueStr;
            ok = data.Read("PVValue", modeValueStr);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "PVValue must be specified");
            }
            else {
                if (modeValueStr == "Function") {
                    eventMode.function = true;
                }
                else if (modeValueStr == "Parameter") {
                    eventMode.parameter = true;
                }
                else if (modeValueStr == "ParameterName") {
                    eventMode.parameterName = true;
                }
                else if (modeValueStr == "Ignore") {
                    eventMode.ignore = true;
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "PVValue %s is not supported", modeValueStr.Buffer());
                    ok = false;
                }
            }
        }
        if (ok) {
            AnyType at = data.GetType("FunctionMap");
            if (!at.IsVoid()) {
                ok = (!(eventMode.parameter.operator bool()));
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "FunctionMap can only be used with PVValue=Function");
                }
                if (ok) {
                    ok = at.GetNumberOfDimensions() == 2u;
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "FunctionMap must be a matrix");
                    }
                }
                if (ok) {
                    nOfFunctionMaps = at.GetNumberOfElements(1u);
                    uint32 nCols = at.GetNumberOfElements(0u);
                    ok = (nCols == 2u);
                    if (ok) {
                        Matrix<StreamString> functionMapMatrix(nOfFunctionMaps, nCols);
                        ok = data.Read("FunctionMap", functionMapMatrix);
                        if (!ok) {
                            REPORT_ERROR(ErrorManagement::ParametersError, "Could not read FunctionMap");
                        }
                        if (ok) {
                            functionMap[0u] = new StreamString[nOfFunctionMaps];
                            functionMap[1u] = new StreamString[nOfFunctionMaps];
                            uint32 i;
                            for (i = 0u; i < nOfFunctionMaps; i++) {
                                functionMap[0u][i] = functionMapMatrix(i, 0u);
                                functionMap[1u][i] = functionMapMatrix(i, 1u);
                            }
                        }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::ParametersError, "FunctionMap must be a matrix with two columns");
                    }
                }
            }
        }
        if (ok) {
            if (data.Read("Function", function)) {
                ok = (!eventMode.function.operator bool());
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError,
                                 "With PVValue=Function the Function to be called is the PV value. Remove this parameter. At most specify a FunctionMap");
                }
            }
        }
        if (ok) {
            //In these cases the Function parameter shall be specified
            bool isIgnore = eventMode.ignore.operator bool();
            bool isParameter = eventMode.parameter.operator bool();
            bool isParameterName = eventMode.parameterName.operator bool();
            if ((isIgnore) || (isParameter) || (isParameterName)) {
                ok = (function.Size() != 0u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "The Function parameter must be specified");
                }
            }
        }
        (void) data.MoveToAncestor(1u);
    }
    else {
        eventMode.notSet = true;
    }
    if (ok) {
        pvAnyType = AnyType(pvTypeDesc, 0u, pvMemory);
        if (numberOfElements > 1u) {
            pvAnyType.SetNumberOfDimensions(1u);
            pvAnyType.SetNumberOfElements(0u, numberOfElements);
        }
    }
    #if LOCALDEB
    REPORT_ERROR(ErrorManagement::Information,"OPCUAVariable: Exiting Initialise"); // <<< PM
    #endif
    return ok;
}

bool OPCUAVariable::PostInit(UA_Client * cli) {
    bool ok = false;
    SetClient(cli);
    ok = BrowseRequest();
    if (ok)
        ok = SetRequest();
    return ok;
}

bool OPCUAVariable::BrowseRequest() {
    bool ok = false;
    #if LOCALDEB
    REPORT_ERROR(ErrorManagement::Information, "Entering OPCUAVariable::BrowseRequest"); // <<< PM
    #endif
    /* Building request for Browse Service */
    UA_BrowseRequest bReq;
    UA_BrowseRequest_init(&bReq);
    bReq.requestedMaxReferencesPerNode = 0u;
    bReq.nodesToBrowse = UA_BrowseDescription_new();
    bReq.nodesToBrowseSize = 1u;

    /* Building request for TranslateBrowsePathsToNodeIds */
    UA_BrowsePath browsePath;
    UA_BrowsePath_init(&browsePath);
    browsePath.startingNode = UA_NODEID_NUMERIC(0u, 85u); /* era  85u --UA_NS0ID_OBJECTSFOLDER */
    browsePath.relativePath.elements = static_cast<UA_RelativePathElement*>(UA_Array_new(static_cast<osulong>(pathSize),
                                                                                         &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]));
    browsePath.relativePath.elementsSize = pathSize;

    uint32 tempNumericNodeId = 85u; /* UA_NS0ID_OBJECTSFOLDER */
    uint16 tempNamespaceIndex = 0u;
    char8* tempStringNodeId;
    uint32* ids;
    ids = new uint32[pathSize];

    if ((path != NULL_PTR(StreamString*)) && (ids != NULL_PTR(uint32*))) {
        for (uint32 j = 0u; j < pathSize; j++) {
            ids[j] = GetReferenceType(bReq, const_cast<char8*>(path[j].Buffer()), tempNamespaceIndex, tempNumericNodeId, tempStringNodeId);
            UA_RelativePathElement *elem = &browsePath.relativePath.elements[j];
            elem->referenceTypeId = UA_NODEID_NUMERIC(0u, ids[j]);
            /*lint -e{1055} -e{64} -e{746} UA_QUALIFIEDNAME is declared in the open62541 library.*/
            elem->targetName = UA_QUALIFIEDNAME_ALLOC(nameSpaceIndex, const_cast<char8*>(path[j].Buffer()));
        }
    }
    UA_TranslateBrowsePathsToNodeIdsRequest tbpReq;
    UA_TranslateBrowsePathsToNodeIdsRequest_init(&tbpReq);
    tbpReq.browsePaths = &browsePath;
    tbpReq.browsePathsSize = 1u;
    UA_TranslateBrowsePathsToNodeIdsResponse tbpResp = UA_Client_Service_translateBrowsePathsToNodeIds(opcuaClient, tbpReq);
    ok = (tbpResp.results[0].statusCode == 0x00U); /* UA_STATUSCODE_GOOD */
    if (ok) {
        UA_BrowsePathTarget *ref = &(tbpResp.results[0].targets[0]);
        nodeId = ref->targetId.nodeId;
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "UA Browse request status code is %d", ok);
    }

    delete[] ids;
    if (ok) {
        UA_TranslateBrowsePathsToNodeIdsResponse_deleteMembers(&tbpResp);
        UA_Array_delete(browsePath.relativePath.elements, static_cast<osulong>(pathSize), &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]);
        UA_BrowseDescription_delete(bReq.nodesToBrowse);
    }
    #if LOCALDEB
    REPORT_ERROR(ErrorManagement::Information, "Exiting OPCUAVariable::BrowseRequest"); // <<< PM
    #endif
    return ok;
}

bool OPCUAVariable::SetRequest() {
    bool ok = false;
    if (!(eventMode.notSet.operator bool())) {
        ok = SetSubscriptionRequest();
    }
    else {
        ok = SetWriteRequest();
    }
    return ok;
}

bool OPCUAVariable::SetSubscriptionRequest() {
    bool ok = false;
    float samplingTime = 500;                                   //to change
    UA_StatusCode code = 1u;
    /* Create a subscription */
    if (response.subscriptionId == 0) {
        request.requestedPublishingInterval = samplingTime;
        request.maxNotificationsPerPublish = 10;
        request.requestedLifetimeCount = 3;
        request.requestedMaxKeepAliveCount = 1;
        response = UA_Client_Subscriptions_create(opcuaClient, request, NULL, NULL, NULL);
        ok = (response.responseHeader.serviceResult == UA_STATUSCODE_GOOD);
    }
    /* Add a MonitoredItem */
    if (monitorRequest.itemToMonitor.nodeId.identifier.numeric == 0 && ok) {
        if (nodeId.identifierType == UA_NODEIDTYPE_NUMERIC) {
            monitorRequest = UA_MonitoredItemCreateRequest_default(UA_NODEID_NUMERIC(nodeId.namespaceIndex, nodeId.identifier.numeric));
        }
        else if (nodeId.identifierType == UA_NODEIDTYPE_STRING) {
            monitorRequest = UA_MonitoredItemCreateRequest_default(
//                    UA_NODEID_STRING(nodeId.namespaceIndex, reinterpret_cast<char*>(nodeId.identifier.string.data)));
                    UA_NODEID_STRING(nodeId.namespaceIndex, (char*)(nodePath.Buffer())));
        }

        monitorRequest.requestedParameters.samplingInterval = samplingTime;
        monitorRequest.requestedParameters.queueSize = 1;
    }
    if (monitorResponse.monitoredItemId == 0) {
        monitorResponse = UA_Client_MonitoredItems_createDataChange(opcuaClient, response.subscriptionId, UA_TIMESTAMPSTORETURN_BOTH, monitorRequest, NULL,
                                                                    OPCUAClientDataChange, NULL);
    }
	if( monitorResponse.statusCode != UA_STATUSCODE_GOOD) {
        ok=false;
        #if LOCALDEB
        REPORT_ERROR(ErrorManagement::Information, " UA_Client_MonitoredItems_createData  NOT GOOD %d",monitorResponse.statusCode); // <<< PM
        #endif
    }
    #if LOCALDEB
	else {
        REPORT_ERROR(ErrorManagement::Information, " UA_Client_MonitoredItems_createData  good %d",monitorResponse.statusCode);
    }
    #endif
    /* Asynchronous call */
//    code = UA_Client_runAsync(opcuaClient,1000);        //changed from run_iterate
    if(ok){
        code = UA_Client_run_iterate(opcuaClient, 500);
        if (code != UA_STATUSCODE_GOOD) {
            ok = false;
            #if LOCALDEB
            REPORT_ERROR(ErrorManagement::Information, " UA_Client_run_iterate NOT GOOD");
            #endif
        }
    }
    #if LOCALDEB
    REPORT_ERROR(ErrorManagement::Information, "SetSubscriptionRequest last ok: %d",ok); // <<< PM
    #endif
    return ok;
}

// /*lint -e{746} -e{1055} -e{534} -e{516} UA_Variant_setScalar is defined in open62541.*/
bool OPCUAVariable::SetWriteRequest() {
    UA_WriteValue_init(&writeValue);
    writeValue.attributeId = UA_ATTRIBUTEID_VALUE;//13u; /* UA_ATTRIBUTEID_VALUE */
    if (nodeId.identifierType == UA_NODEIDTYPE_NUMERIC) {
            writeValue.nodeId = UA_NODEID_NUMERIC(nodeId.namespaceIndex, nodeId.identifier.numeric);
    }
    else if (nodeId.identifierType == UA_NODEIDTYPE_STRING) {

         writeValue.nodeId=  UA_NODEID_STRING(nodeId.namespaceIndex, (char*)(nodePath.Buffer()));
    }

    bool ok = false;
    bool isArray = (numberOfElements > 1u);
    if (pvMemory != NULL_PTR(void*)) {
        if (isArray) {
            UA_Variant_setArray(&tempVariant, pvMemory, static_cast<osulong>(numberOfElements), &UA_TYPES[pvType]);
            ok = true;
        }
        else {
            UA_Variant_setScalar(&tempVariant, pvMemory, &UA_TYPES[pvType]);
            ok = true;
        }
    }
    // writeValue.value.value = static_cast<const UA_Variant>(tempVariant); // <<< PM original line
    // writeValue.value.hasValue = true; // <<< PM original line
    // writeValue.value.value.data = &tempVariant; // <<< PM tempVariant static_cast<const UA_Variant>(tempVariant)
    writeValue.value.value = tempVariant; // <<< PM
    ok = writeValue.value.hasValue;
    return ok;
}

uint32 OPCUAVariable::GetReferenceType(const UA_BrowseRequest &bReq,
                                 const char8* const path,
                                 uint16 &nameSpaceIndex,
                                 uint32 &numericNodeId,
                                 char8* &stringNodeId) {
    uint32 id = 0u;
    if (numericNodeId == 0u) {
        bReq.nodesToBrowse[0].nodeId = UA_NODEID_STRING(nameSpaceIndex, stringNodeId);
    }
    else {
        bReq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(nameSpaceIndex, numericNodeId);
    }
    /*lint -e{1013} -e{63} -e{40} includeSubtypes is a member of struct UA_BrowseDescription.*/
    bReq.nodesToBrowse[0].includeSubtypes = true;
    bReq.nodesToBrowse[0].resultMask = 63u; /* UA_BROWSERESULTMASK_ALL */
    UA_BrowseResponse *bResp = UA_BrowseResponse_new();
    *bResp = UA_Client_Service_browse(opcuaClient, bReq);
    bool ok = (bResp->responseHeader.serviceResult == 0x00U); /* UA_STATUSCODE_GOOD */
    if (ok) {
        bool found = false;
        uint64 initCheckTime = HighResolutionTimer::Counter();
        uint64 finalCheckTime = 0u;
        uint32 count = 0u;
        while ((!found) && (bResp->resultsSize)) {
            for (uint32 i = 0u; i < bResp->resultsSize; ++i) {
                for (uint32 j = 0u; j < bResp->results[i].referencesSize; ++j) {
                    UA_ReferenceDescription *ref = &(bResp->results[i].references[j]);
                    if (StringHelper::Compare(reinterpret_cast<char8*>(ref->browseName.name.data), path) == 0) {
                        id = ref->referenceTypeId.identifier.numeric;
                        nameSpaceIndex = ref->nodeId.nodeId.namespaceIndex;
                        if (ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_NUMERIC) {
                            numericNodeId = ref->nodeId.nodeId.identifier.numeric;
                            ok = true;
                        }
                        else if (ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_STRING) {
                            stringNodeId = new char8[ref->nodeId.nodeId.identifier.string.length];
                            ok = StringHelper::Copy(stringNodeId, reinterpret_cast<char8*>(ref->nodeId.nodeId.identifier.string.data));
                            if (ok) {
                                numericNodeId = 0u;
                            }
                        }
                        else {
                            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "NodeID identifier type not supported.");
                            ok = false;
                        }
                        if (ok) {
                            found = true;
                        }
                    }
                }
            }
            if ((!found) && bResp->results->continuationPoint.length) {
                UA_BrowseNextRequest nextReq;
                UA_BrowseNextRequest_init(&nextReq);
                nextReq.continuationPoints = UA_ByteString_new();
                UA_StatusCode code = UA_ByteString_copy(&(bResp->results->continuationPoint), nextReq.continuationPoints);
                if (code == 0x00U) { /* UA_STATUSCODE_GOOD */
                    nextReq.continuationPointsSize = 1u;
                    /*lint -e{740} The library need the recast of bResp .*/
                    *reinterpret_cast<UA_BrowseNextResponse *>(bResp) = UA_Client_Service_browseNext(opcuaClient, nextReq);
                    UA_BrowseNextRequest_deleteMembers(&nextReq);
                }
            }
            finalCheckTime = HighResolutionTimer::Counter();
            if (HighResolutionTimer::TicksToTime(finalCheckTime, initCheckTime) > 5.0) {
                count++;
                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Browse Service is taking too long. Is the NodeId or the Path right?");
                initCheckTime = HighResolutionTimer::Counter();
            }
            if (count > 1u) {
                break;
            }
        }
        UA_BrowseResponse_delete(bResp);
    }
    return id;
}

bool OPCUAVariable::ParsePath() {
    bool ok = false;

    ok = nodePath.Seek(0LLU);
    StreamString pathTokenized;
    char8 ignore;
    if (ok) {
        /* This cycle is for getting the path size only */
        do {
            ok = nodePath.GetToken(pathTokenized, ".", ignore);
            if (ok) {
                pathSize++;
            }
            pathTokenized = "";
        }
        while (ok);
    }
    if (pathSize > 0u) {
        path = new StreamString[pathSize];
    }
    ok = nodePath.Seek(0LLU);
    if (ok) {
        if (path != NULL_PTR(StreamString*)) {
            for (uint32 k = 0u; k < pathSize; k++) {
                ok = nodePath.GetToken(pathTokenized, ".", ignore);
                if (ok) {
                    StreamString s = pathTokenized;
                    path[k] = s;
                }
                pathTokenized = "";
            }
        }
    }

    return ok;
}

ErrorManagement::ErrorType OPCUAVariable::OPCUAWrite(StructuredDataI & data) {
    ErrorManagement::ErrorType err=ErrorManagement::NoError;
	uint32 tmpint;
    #if LOCALDEB
    REPORT_ERROR(ErrorManagement::Information, "Entering OPCUAWrite");  // <<< PM
    #endif

    UA_SecureChannelState channelState;
    UA_SessionState sessionState;
    UA_StatusCode connectStatus;

    // UA_Client_getState(opcuaClient, &channelState, &sessionState, &connectStatus);
    // REPORT_ERROR(ErrorManagement::Information, "OPCUAWrite connectionStatus: %s", UA_StatusCode_name(connectStatus)); // <<< PM

    // err.parametersError = !data.Read("param1", pvAnyType);
    data.Read("param1", pvAnyType);
    writeRequest.nodesToWrite = &writeValue;
    writeRequest.nodesToWriteSize = 1u;

    // (void) eventCallbackFastMux.FastLock();
    // <<< PM - begin
    StreamString val;
    (void) val.Printf("%!", pvAnyType);
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Trying to write ...%s",val.Buffer());
    /* tmpint=static_cast<uint32>(reinterpret_cast<uintptr_t> (tempVariant.data));
    val.Printf("%d",tmpint);
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Trying to write ...%s",val.Buffer());*/
    // <<< PM - end
    OPCUAVariable::Lock();
    #if LOCALDEB
    REPORT_ERROR_STATIC(ErrorManagement::Information, "OPCUAWrite After Lock()"); // <<< PM
    #endif
    UA_StatusCode retval = UA_Client_writeValueAttribute(opcuaClient, writeValue.nodeId, &tempVariant);
    // UA_WriteResponse wResp = UA_Client_Service_write(opcuaClient, writeRequest); // <<< PM
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Write: done! ");
    // eventCallbackFastMux.FastUnLock();
    OPCUAVariable::UnLock();
    bool ok = ((retval & 0xFF000000) == UA_STATUSCODE_GOOD); // <<< PM
    //bool ok = (wResp.responseHeader.serviceResult == 0x00U); /* UA_STATUSCODE_GOOD */
    //UA_WriteResponse_deleteMembers(&wResp);
    if (!ok) {
        #if LOCALDEB
        REPORT_ERROR(ErrorManagement::Information, "OPCUAWrite return = %08x, %s", retval, UA_StatusCode_name(retval)); // <<< PM
        #endif
        err = ErrorManagement::FatalError;
    }

    return err;
}

void OPCUAVariable::HandlePVEvent(UA_DataValue *value) {
    if (pvMemory != NULL_PTR(void *)) {
        (void) MemoryOperationsHelper::Copy(pvMemory, (value->value.data), typeSize);
        if (!(eventMode.notSet.operator bool())) {
            TriggerEventMessage();
        }
    }
}

void OPCUAVariable::TriggerEventMessage() {
    ConfigurationDatabase cdb;
    bool ok = cdb.Write("Destination", destination.Buffer());
    if (eventMode.function.operator bool()) {
        if (nOfFunctionMaps == 0u) {
            if (ok) {
                ok = cdb.Write("Function", pvAnyType);
            }
        }
        else {
            StreamString newValue;
            AnyType newValueAnyType(TypeDescriptor(false, SString, static_cast<uint16>(sizeof(StreamString) * 8u)), 0u, &newValue);
            if (TypeConvert(newValueAnyType, pvAnyType)) {
                StreamString functionMapValue = GetFunctionFromMap(newValue.Buffer());
                if (functionMapValue.Size() > 0u) {
                    ok = cdb.Write("Function", functionMapValue.Buffer());
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "Could not find a mapping for key: %s", newValue.Buffer());
                    ok = false;
                }
            }
        }
    }
    else if (eventMode.ignore.operator bool()) {
        if (ok) {
            ok = cdb.Write("Function", function.Buffer());
        }
    }
    else if (eventMode.parameterName.operator bool()) {
        if (ok) {
            ok = cdb.Write("Function", function.Buffer());
        }
        if (ok) {
            ok = cdb.CreateAbsolute("+Parameters");
        }
        if (ok) {
            ok = cdb.Write("Class", "ConfigurationDatabase");
        }
        if (ok) {
            ok = cdb.Write("param1", GetName());
        }
        if (ok) {
            ok = cdb.Write("param2", pvAnyType);
        }
        if (ok) {
            ok = cdb.MoveToAncestor(1u);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not create ConfigurationDatabase for message");
        }
    }
    //Must be eventMode.parameter.operator bool()
    else {
        if (ok) {
            ok = cdb.Write("Function", function.Buffer());
        }
        if (ok) {
            ok = cdb.CreateAbsolute("+Parameters");
        }
        if (ok) {
            ok = cdb.Write("Class", "ConfigurationDatabase");
        }
        if (ok) {
            ok = cdb.Write("param1", pvAnyType);
        }
        if (ok) {
            ok = cdb.MoveToAncestor(1u);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not create ConfigurationDatabase for message");
        }
    }
    if (ok) {
        ReferenceT<Message> message(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ok = message->Initialise(cdb);
        if (ok) {
            if (MessageI::SendMessage(message, this) != ErrorManagement::NoError) {
                StreamString val;
                (void) val.Printf("%!", pvAnyType);
                REPORT_ERROR(ErrorManagement::FatalError, "Could not send message to %s with value %s", destination.Buffer(), val.Buffer());
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not Initialise message");
        }
    }
}

/*lint -e{1762} function cannot be made const as it is registered as an RPC*/
ErrorManagement::ErrorType OPCUAVariable::OPCUARead(StructuredDataI & data) {
    ErrorManagement::ErrorType err;
//     ErrorManagement::ErrorType err = data.Write("param1", pvAnyType);
    return err;
}

void OPCUAVariable::SetClient(UA_Client * cli) {
    opcuaClient = cli;
}

OPCUAVariable::EventMode OPCUAVariable::GetMode() const {
    return eventMode;
}

float64 OPCUAVariable::GetTimeout() const {
    return timeout;
}

StreamString OPCUAVariable::GetDestination() const {
    return destination;
}

StreamString OPCUAVariable::GetFunction() const {
    return function;
}

StreamString OPCUAVariable::GetFunctionFromMap(const StreamString &key) const {
    StreamString value;
    uint32 j;
    if (functionMap[0u] != NULL_PTR(StreamString*)) {
        if (functionMap[1u] != NULL_PTR(StreamString*)) {
            bool found = false;
            for (j = 0u; (j < nOfFunctionMaps) && (!found); j++) {
                found = (functionMap[0u][j] == key);
                if (found) {
                    value = functionMap[1u][j];
                }
            }
        }
    }
    return value;
}

uint32 OPCUAVariable::GetMemorySize() const {
    return memorySize;
}

CLASS_REGISTER(OPCUAVariable, "1.0")
/*lint -e{1023} There is no ambiguity on the function to be called as the compiler can distinguish between both template definitions.*/
CLASS_METHOD_REGISTER(OPCUAVariable, OPCUAWrite)
/*lint -e{1023} There is no ambiguity on the function to be called as the compiler can distinguish between both template definitions.*/
CLASS_METHOD_REGISTER(OPCUAVariable, OPCUARead)
}
