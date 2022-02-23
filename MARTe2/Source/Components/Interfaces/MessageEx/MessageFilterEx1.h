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
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "MessageFilter.h"
#include "MessageI.h"
#include "Object.h"
#include "ObjectRegistryDatabase.h"
#include "Sleep.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe2Tutorial {

/**
 * @brief Message filter for the MessageEx1 below.
 */
class MessageFilterEx1 : public MARTe::Object, public MARTe::MessageFilter {
public:
  CLASS_REGISTER_DECLARATION()

  /**
   * @brief NOOP.
   */
  MessageFilterEx1() : MARTe::Object(), MARTe::MessageFilter(true) {
    using namespace MARTe;
  }

  virtual void Purge(MARTe::ReferenceContainer &purgeList) {
    owner = MARTe::Reference();
  }

  virtual ~MessageFilterEx1() {
    if (GetName() != NULL) {
      REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information,
                          "No more references pointing at %s [%s]. "
                          "The Object will be safely deleted.",
                          GetName(), GetClassProperties()->GetName());
    }
  }

  void SetOwner(MARTe::Reference ownerIn) { owner = ownerIn; }

  virtual MARTe::ErrorManagement::ErrorType
  ConsumeMessage(MARTe::ReferenceT<MARTe::Message> &messageToTest);

private:
  MARTe::Reference owner;
};
CLASS_REGISTER(MessageFilterEx1, "")

} // namespace MARTe2Tutorial

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* INTERFACES_KILLERINTERFACE_KILLERFILTER_H */
