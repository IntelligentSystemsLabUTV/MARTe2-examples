/**
 * @file MessageFilterEx1.h
 * @brief Header file for class MessageFilterEx1
 * @date 22/02/2022
 * @author Alessandro Tenaglia
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

 * @details This header file contains the declaration of the class
 TCPSocketMessageProxy
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef INTERFACES_KILLERINTERFACE_KILLERFILTER_H
#define INTERFACES_KILLERINTERFACE_KILLERFILTER_H

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MessageFilterEx1.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe2Tutorial {

/**
 * @brief A MARTe::Object class that will receive messages.
 */
class MessageEx1 : public MARTe::Object, public MARTe::MessageI {
public:
  CLASS_REGISTER_DECLARATION()

  /**
   * @brief Install the message filter.
   */
  MessageEx1() : MARTe::Object(), MARTe::MessageI() {
    using namespace MARTe;
    filter = ReferenceT<MessageFilterEx1>(
        GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetOwner(this);
    MessageI::InstallMessageFilter(filter);
    messageReceived = false;
  }

  virtual ~MessageEx1() {
    if (GetName() != NULL) {
      REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information,
                          "No more references pointing at %s [%s]. "
                          "The Object will be safely deleted.",
                          GetName(), GetClassProperties()->GetName());
    }
  }

  virtual void Purge(MARTe::ReferenceContainer &purgeList) {
    filter->SetOwner(MARTe::Reference());
    RemoveMessageFilter(filter);
  }

  /**
   * @brief Print the message contents in the screen.
   */
  MARTe::ErrorManagement::ErrorType
  CheckMessage(MARTe::ReferenceT<MARTe::Message> &messageToTest) {
    using namespace MARTe;
    const Object *senderObj = messageToTest->GetSender();
    CCString sender = senderObj ? senderObj->GetName() : "anonymous";
    CCString destination = messageToTest->GetDestination();
    CCString function = messageToTest->GetFunction();
    bool expectsReply = messageToTest->ExpectsReply();
    bool expectsIndirectReply = messageToTest->ExpectsIndirectReply();
    bool isReply = messageToTest->IsReply();
    REPORT_ERROR(MARTe::ErrorManagement::Information,
                 "Received message from %s to %s with function %s "
                 "(expectsReply? %d expectsIndirectReply? %d isReply? %d",
                 sender.GetList(), destination.GetList(), function.GetList(),
                 expectsReply, expectsIndirectReply, isReply);

    messageReceived = true;
    return ErrorManagement::NoError;
  }

  MARTe::ErrorManagement::ErrorType Function0() {
    REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information,
                        "Function0 called.");
    return MARTe::ErrorManagement::NoError;
  }

  bool messageReceived;

private:
  MARTe::ReferenceT<MessageFilterEx1> filter;
};

CLASS_REGISTER(MessageEx1, "")

// Delegate to the owner the handling of the message (but could be handled by
// the filter itself as well).
MARTe::ErrorManagement::ErrorType MessageFilterEx1::ConsumeMessage(
    MARTe::ReferenceT<MARTe::Message> &messageToTest) {
  MARTe::ReferenceT<MessageEx1> ownerT = owner;
  MARTe::ErrorManagement::ErrorType err;
  err.fatalError = !owner.IsValid();
  if (err.ErrorsCleared()) {
    err = ownerT->CheckMessage(messageToTest);
  }
  return err;
}

} // namespace MARTe2Tutorial

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* INTERFACES_KILLERINTERFACE_KILLERFILTER_H */
