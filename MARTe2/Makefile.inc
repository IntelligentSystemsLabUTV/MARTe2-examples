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

OBJSX = 

SPBM = Source/Components/Interfaces.x

SUBPROJMAIN=$(SPBM:%.x=%.spb)
SUBPROJMAINCLEAN=$(SPBM:%.x=%.spc)

ROOT_DIR = .

MAKEDEFAULTDIR = $(MARTe2_DIR)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

all: $(OBJS) core
	echo  $(OBJS)

core: $(SUBPROJMAIN) check-env
    echo  $(SUBPROJMAIN)

clean: $(SUBPROJMAINCLEAN) clean_wipe_old

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

check-env:
ifndef MARTe2_DIR
    $(error MARTe2_DIR is undefined)
endif
ifndef TARGET
    $(error TARGET is undefined)
endif
