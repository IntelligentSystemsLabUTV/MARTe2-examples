#!/bin/bash

#Arguments -l LOADER -f FILENAME -m MESSAGE | -s STATE [-d cgdb|strace]
#-l LOADER=The Loader to use
#-f FILENAME=MARTe configuration file
#-m MESSAGE=Start message
#-s STATE=RealTimeApplication first state 
#-d cgdb=Run with cgdb
#-d strace=Run with strace

export MARTe2_F4E=/home/pi/MARTe2-F4E
export MARTe2_DIR=$MARTe2_F4E/MARTe2_armv8
export MARTe2_Componenets_DIR=$MARTe2_F4E/MARTe2-components
export MARTe2_EXAMPLES_DIR=/home/pi/MARTe2-examples

#Run with cgdb or strace?
DEBUG=""

#Consume input arguments
while [[ $# -gt 1 ]]
do
key="$1"

case $key in
    -l|--loader)
    LOADER="$2"
    shift # past argument
    ;;
    -f|--file)
    FILE="$2"
    shift # past argument
    ;;
    -m|--message)
    MESSAGE="$2"
    shift # past argument
    ;;
    -s|--state)
    STATE="$2"
    shift # past argument
    ;;
    -d|--debug)
    DEBUG="$2"
    shift # past argument
    ;;
    --default)
    DEFAULT=YES
    ;;
    *)
          # unknown option
    ;;
esac
shift # past argument or value
done

if [ -z ${MARTe2_DIR+x} ]; then echo "Please set the MARTe2_DIR environment variable"; exit; fi
if [ -z ${MARTe2_Components_DIR+x} ]; then echo "Please set the MARTe2_Components_DIR environment variable"; exit; fi

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
# MARTe2 Core
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_DIR/Build/$TARGET/Core/
# MARTe2 Components
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/DataSources/EPICSCA/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/DataSources/LinuxTimer/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/DataSources/LoggerDataSource/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/DataSources/DAN/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/DataSources/NI6259/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/DataSources/NI6368/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/DataSources/SDN/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/DataSources/UDP/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/DataSources/MDSWriter/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/DataSources/RealTimeThreadSynchronisation/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/DataSources/FileDataSource/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/DataSources/OPCUADataSource/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/DataSources/EPICSPVA/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/GAMs/ConstantGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/GAMs/IOGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/GAMs/PIDGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/GAMs/CRCGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/GAMs/BaseLib2GAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/GAMs/ConversionGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/GAMs/FilterGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/GAMs/SimulinkWrapperGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/GAMs/StatisticsGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/GAMs/WaveformGAM/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/Interfaces/BaseLib2Wrapper/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/Interfaces/SysLogger/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/Interfaces/EPICS/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/Interfaces/OPCUA/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_Components_DIR/Build/$TARGET/Components/Interfaces/EPICSPVA/
# MDSplus
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MDSPLUS_DIR/lib/
# MARTe2 Examples
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_EXAMPLES_DIR/Build/$TARGET/Components/DataSources/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_EXAMPLES_DIR/Build/$TARGET/Components/GAMs/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_EXAMPLES_DIR/Build/$TARGET/Components/Interfaces/TCPSocketMessageProxy/
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MARTe2_EXAMPLES_DIR/Models/

echo $LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH

MARTeAPP=$MARTe2_DIR/Build/$TARGET/App/MARTeApp.ex

#Start with cgdb or with strace
if [ "$DEBUG" = "cgdb" ]
then
    if [ -z ${STATE+x} ]; then
        cgdb --args $MARTeAPP -l $LOADER -f $FILE -m $MESSAGE
    else
        cgdb --args $MARTeAPP -l $LOADER -f $FILE -s $STATE
    fi
elif [ "$DEBUG" = "strace" ]
then
    if [ -z ${STATE+x} ]; then
        strace -o/tmp/strace.err $MARTeAPP -l $LOADER -f $FILE -m $MESSAGE
    else
        strace -o/tmp/strace.err $MARTeAPP -l $LOADER -f $FILE -s $STATE
    fi
else
    if [ -z ${STATE+x} ]; then
        $MARTeAPP -l $LOADER -f $FILE -m $MESSAGE
    else
        $MARTeAPP -l $LOADER -f $FILE -s $STATE 
    fi
fi
