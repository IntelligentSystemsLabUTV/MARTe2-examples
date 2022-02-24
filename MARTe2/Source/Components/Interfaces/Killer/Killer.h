/**
 * @file Killer.h
 * @brief Header file for class Killer
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

#ifndef INTERFACES_KILLER_KILLER_H
#define INTERFACES_KILLER_KILLER_H

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <csignal>
#include <unistd.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "ConfigurationDatabase.h"
#include "MessageFilter.h"
#include "MessageI.h"
#include "Object.h"
#include "ObjectRegistryDatabase.h"
#include "RegisteredMethodsMessageFilter.h"
#include "Sleep.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe2Tutorial {

/**
 * @brief A MARTe::Object class that will receive messages.
 */
class Killer : public MARTe::Object, public MARTe::MessageI {
public:
  CLASS_REGISTER_DECLARATION()

  /**
   * @brief Install the message filter.
   */
  Killer();

  virtual ~Killer();

  virtual void Purge(MARTe::ReferenceContainer &purgeList);

  MARTe::ErrorManagement::ErrorType Kill();

private:
  MARTe::ReferenceT<MARTe::RegisteredMethodsMessageFilter> filter;
};

} // namespace MARTe2Tutorial

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* INTERFACES_KILLER_KILLER_H */
