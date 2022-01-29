%% MDSplus
setenv('MDSPLUS_DIR', [getenv('HOME') '/MDSplus'])
setenv('PATH', [getenv('PATH') ':' ...
            getenv('MDSPLUS_DIR') '/bin'])
setenv('LD_LIBRARY_PATH', [getenv('LD_LIBRARY_PATH') ':' ...
            getenv('MDSPLUS_DIR') '/lib'])
setenv('MDS_PATH', [getenv('MDSPLUS_DIR') '/tdi'])
setenv('CLASSPATH', [getenv('CLASSPATH') ':' ...
            getenv('MDSPLUS_DIR') '/java/classes/jScope.jar' ':' ...
            getenv('MDSPLUS_DIR') '/java/classes/jTraverser.jar' ':' ...
            getenv('MDSPLUS_DIR') '/java/classes/jDevices.jar' ':' ...
            getenv('MDSPLUS_DIR') '/java/classes/mdsobjects.jar' ':' ...
            getenv('MDSPLUS_DIR') '/java/classes/jDispatcher.jar'])