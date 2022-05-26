function tc = raspberrypi_tc
%RASPBERRY_TC Toolchain to cross compile on Raspberry Pi using gcc-arm-linux-gnueabihf

% -----------------------------
% Toolchain Overview & init
% -----------------------------
tc = coder.make.ToolchainInfo('BuildArtifact', 'gmake makefile', 'SupportedLanguages', {'Asm/C/C++'});
tc.Name = 'Raspberry Pi (64-bit Linux)';
tc.Platform = computer('arch');

armCompilerPath = '/opt/cross-pi-gcc/bin';
setenv('ARM_GCC_BIN_DIR', armCompilerPath);
tc.addMacro('ARM_GCC_BIN_DIR', armCompilerPath);

% --------------
% Attributes
% --------------
tc.addAttribute('TransformPathsWithSpaces');
tc.addAttribute('SupportsUNCPaths',     false);
tc.addAttribute('SupportsDoubleQuotes', false);

% ------------------------------
% Macros
% ------------------------------
tc.addIntrinsicMacros({'TARGET_LOAD_CMD_ARGS'});
tc.addIntrinsicMacros({'TARGET_LOAD_CMD'});
tc.addMacro('MW_EXTERNLIB_DIR',     ['$(MATLAB_ROOT)\extern\lib\' tc.Platform]);
tc.addMacro('MW_LIB_DIR',           ['$(MATLAB_ROOT)\lib\' tc.Platform]);
tc.addMacro('CFLAGS_ADDITIONAL',    '');
tc.addMacro('CPPFLAGS_ADDITIONAL',  '');
tc.addMacro('LIBS_TOOLCHAIN',       '');
tc.addMacro('CVARSFLAG',            '');
tc.addIntrinsicMacros({'ANSI_OPTS', 'CPP_ANSI_OPTS'});

% ----------------
% Inline Commands
% ----------------
tc.InlinedCommands{1} = 'ALL_DEPS:=$(patsubst %$(OBJ_EXT),%$(DEP_EXT),$(ALL_OBJS))';
tc.InlinedCommands{2} = 'all:';
tc.InlinedCommands{3} = '';

% -----------
% Assembler
% -----------
assembler = tc.getBuildTool('Assembler');
assembler.setName('RaspberryPi Assembler');
assembler.setPath('$(ARM_GCC_BIN_DIR)');
assembler.setCommand('arm-linux-gnueabihf-as');

assembler.setDirective('IncludeSearchPath', '-I');
assembler.setDirective('PreprocessorDefine', '-D');
assembler.setDirective('OutputFlag', '-o');
assembler.setDirective('Debug', '-g');

assembler.setFileExtension('Source','.s');
assembler.addFileExtension('ASMType1Source', coder.make.BuildItem('ASM_Type1_Ext', '.S'));
assembler.setFileExtension('Object', '.o');
assembler.addFileExtension('DependencyFile', coder.make.BuildItem('DEP_EXT', '.d'));

assembler.InputFileExtensions = {'Source', 'ASMType1Source'};

% -----------------------------
% C Compiler
% -----------------------------
compiler = tc.getBuildTool('C Compiler');
compiler.setName('RaspberryPi C Compiler');
compiler.setPath('$(ARM_GCC_BIN_DIR)');
compiler.setCommand('arm-linux-gnueabihf-gcc');

compiler.setDirective('CompileFlag', '-c');
compiler.setDirective('IncludeSearchPath', '-I');
compiler.setDirective('PreprocessorDefine', '-D');
compiler.setDirective('OutputFlag', '-o');
compiler.setDirective('Debug', '-g');

compiler.setFileExtension('Source', '.c');
compiler.setFileExtension('Header', '.h');
compiler.setFileExtension('Object', '.o');
compiler.addFileExtension('DependencyFile', coder.make.BuildItem('DEP_EXT', '.d'));

compiler.setCommandPattern('|>TOOL<| |>TOOL_OPTIONS<| |>OUTPUT_FLAG<| |>OUTPUT<|');

% -----------------------------
% C++ compiler
% -----------------------------
cppcompiler = tc.getBuildTool('C++ Compiler');
cppcompiler.setName('RaspberryPi C++ Compiler');
cppcompiler.setPath('$(ARM_GCC_BIN_DIR)');
cppcompiler.setCommand('arm-linux-gnueabihf-g++');

cppcompiler.setDirective('CompileFlag', '-c');
cppcompiler.setDirective('IncludeSearchPath', '-I');
cppcompiler.setDirective('PreprocessorDefine', '-D');
cppcompiler.setDirective('OutputFlag', '-o');
cppcompiler.setDirective('Debug', '-g');

cppcompiler.setFileExtension('Source', '.cpp');
cppcompiler.setFileExtension('Header', '.h');
cppcompiler.setFileExtension('Object', '.o');
cppcompiler.addFileExtension('DependencyFile', coder.make.BuildItem('DEP_EXT', '.d'));

cppcompiler.setCommandPattern('|>TOOL<| |>TOOL_OPTIONS<| |>OUTPUT_FLAG<| |>OUTPUT<|');

% -----------------------------
% C Linker
% -----------------------------
linker = tc.getBuildTool('Linker');
linker.setName('RaspberryPi C Linker');
linker.setPath('$(ARM_GCC_BIN_DIR)');
linker.setCommand('arm-linux-gnueabihf-gcc');

linker.setDirective('Library', '-l');
linker.setDirective('LibrarySearchPath', '-L');
linker.setDirective('OutputFlag', '-o');
linker.setDirective('Debug', '--debug');

linker.setFileExtension('Executable', '.elf');
linker.setFileExtension('Shared Library', '.so');

linker.setCommandPattern('|>TOOL<| |>TOOL_OPTIONS<| |>OUTPUT_FLAG<| |>OUTPUT<|');

% -----------------------------
% C++ Linker
% -----------------------------
linker = tc.getBuildTool('C++ Linker');
linker.setName('RaspberryPi C++ Linker');
linker.setPath('$(ARM_GCC_BIN_DIR)');
linker.setCommand('arm-linux-gnueabihf-g++');

linker.setDirective('Library', '-l');
linker.setDirective('LibrarySearchPath', '-L');
linker.setDirective('OutputFlag', '-o');
linker.setDirective('Debug', '-g');

linker.setFileExtension('Executable', '.elf');
linker.setFileExtension('Shared Library', '.so');

linker.setCommandPattern('|>TOOL<| |>TOOL_OPTIONS<| |>OUTPUT_FLAG<| |>OUTPUT<|');

% -----------------------------
% Archiver
% -----------------------------
archiver = tc.getBuildTool('Archiver');
archiver.setName('RaspberryPi C/C++ Archiver');
archiver.setPath('$(ARM_GCC_BIN_DIR)');
archiver.setCommand('arm-linux-gnueabihf-ar');

archiver.setDirective('OutputFlag', '');

archiver.setFileExtension('Static Library', '.a');

archiver.setCommandPattern('|>TOOL<| |>TOOL_OPTIONS<| |>OUTPUT_FLAG<| |>OUTPUT<|');

% ------------------------------
% Builder
% ------------------------------
tc.setBuilderApplication(tc.Platform);

% --------------------------------------------
% Build Configuration
% --------------------------------------------
% Compiler optimization levels
optimsOffOpts = {'-O0'};
optimsOnOpts = {'-O3 -fno-loop-optimize'};
profileOpts.compiler = {'-fprofile-arcs' '-ftest-coverage' '-pg'};
profileOpts.linker   = {'-fprofile-arcs' '-ftest-coverage' '-pg'};
profileOpts.archiver = {};

% Baseline options
cCompilerOpts    = {'-c $(ANSI_OPTS)'};
cppCompilerOpts  = {'-c $(CPP_ANSI_OPTS) -fno-exceptions -fno-rtti'};
linkerOpts       = {'-Wl,-rpath,"$(MATLAB_ARCH_BIN)",-L"$(MATLAB_ARCH_BIN)"'};
sharedLinkerOpts = {'-shared -Wl,-rpath,"$(MATLAB_ARCH_BIN)",-L"$(MATLAB_ARCH_BIN)" -Wl,--no-undefined'};
archiverOpts     = {'-ruvs'};

% Get the debug flag per build tool
debugFlag.CCompiler   = '$(CDEBUG)';   
debugFlag.CppCompiler = '$(CPPDEBUG)';
debugFlag.Linker      = '$(LDDEBUG)';  
debugFlag.CppLinker   = '$(CPPLDDEBUG)';  
debugFlag.Archiver    = '$(ARDEBUG)';  

% Set the toolchain flags for 'Faster Builds' build configuration
cfg = tc.getBuildConfiguration('Faster Builds');
cfg.setOption( 'C Compiler',                horzcat(cCompilerOpts,   optimsOffOpts));
cfg.setOption( 'C++ Compiler',              horzcat(cppCompilerOpts, optimsOffOpts));
cfg.setOption( 'Linker',                    linkerOpts);
cfg.setOption( 'C++ Linker',                linkerOpts);
cfg.setOption( 'Shared Library Linker',     sharedLinkerOpts);
cfg.setOption( 'Archiver',                  archiverOpts);

% Set the toolchain flags for 'Faster Runs' build configuration
cfg = tc.getBuildConfiguration('Faster Runs');
cfg.setOption( 'C Compiler',                horzcat(cCompilerOpts,   optimsOnOpts));
cfg.setOption( 'C++ Compiler',              horzcat(cppCompilerOpts, optimsOnOpts));
cfg.setOption( 'Linker',                    linkerOpts);
cfg.setOption( 'C++ Linker',                linkerOpts);
cfg.setOption( 'Shared Library Linker',     sharedLinkerOpts);
cfg.setOption( 'Archiver',                  archiverOpts);

% Set the toolchain flags for 'Debug' build configuration
cfg = tc.getBuildConfiguration('Debug');
cfg.setOption( 'C Compiler',              	horzcat(cCompilerOpts,    optimsOffOpts, debugFlag.CCompiler));
cfg.setOption( 'C++ Compiler',          	horzcat(cppCompilerOpts,  optimsOffOpts, debugFlag.CppCompiler));
cfg.setOption( 'Linker',                	horzcat(linkerOpts,       debugFlag.Linker));
cfg.setOption( 'C++ Linker',               	horzcat(linkerOpts,       debugFlag.CppLinker));
cfg.setOption( 'Shared Library Linker',  	horzcat(sharedLinkerOpts, debugFlag.Linker));
cfg.setOption( 'Archiver',              	horzcat(archiverOpts,     debugFlag.Archiver));

% --------------
% Make
% --------------
tc.setBuildConfigurationOption('all', 'Make Tool', '-f $(MAKEFILE)');
makeTool = tc.BuilderApplication;
makeTool.setDirective('DeleteCommand', '@rm -rf');
