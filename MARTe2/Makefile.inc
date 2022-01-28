#############################################################
#
# Copyright 2015 F4E | European Joint Undertaking for ITER 
# and the Development of Fusion Energy ('Fusion for Energy') 
#
# Licensed under the EUPL, Version 1.1 or - as soon they 
# will be approved by the European Commission - subsequent 
# versions of the EUPL (the "Licence"); 
# You may not use this work except in compliance with the 
# Licence. 
# You may obtain a copy of the Licence at: 
#
# http://ec.europa.eu/idabc/eupl 
#
# Unless required by applicable law or agreed to in 
# writing, software distributed under the Licence is 
# distributed on an "AS IS" basis, 
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
# express or implied. 
# See the Licence for the specific language governing 
# permissions and limitations under the Licence. 
#
#############################################################

SPB = Source/Components/Interfaces.x

MAKEDEFAULTDIR = $(MARTe2_DIR)/MakeDefaults

ROOT_DIR = .

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

all: $(OBJS) $(SUBPROJMAIN) check-env
	echo  $(OBJS)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

check-env:
ifndef MARTe2_DIR
    $(error MARTe2_DIR is undefined)
endif
ifndef MARTe2_Components_DIR
    $(error MARTe2_Componenets_DIR is undefined)
endif
ifndef TARGET
    $(error TARGET is undefined)
endif
