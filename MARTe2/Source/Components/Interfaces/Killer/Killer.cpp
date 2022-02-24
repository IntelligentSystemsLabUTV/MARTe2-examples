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

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Killer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe2Tutorial {

Killer::Killer() : MARTe::Object(), MARTe::MessageI() {
  using namespace MARTe;
  filter = ReferenceT<RegisteredMethodsMessageFilter>(
      GlobalObjectsDatabase::Instance()->GetStandardHeap());
  filter->SetDestination(this);
  MessageI::InstallMessageFilter(filter);
}

Killer::~Killer() {
  if (GetName() != NULL) {
    REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information,
                        "No more references pointing at %s [%s]. "
                        "The Object will be safely deleted.",
                        GetName(), GetClassProperties()->GetName());
  }
}

void Killer::Purge(MARTe::ReferenceContainer &purgeList) {
  RemoveMessageFilter(filter);
}

MARTe::ErrorManagement::ErrorType Killer::Kill() {
  REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "Kill called.");
  kill(0, SIGTERM);
  return MARTe::ErrorManagement::NoError;
}

CLASS_REGISTER(Killer, "")
CLASS_METHOD_REGISTER(Killer, Kill)

} // namespace MARTe2Tutorial

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
