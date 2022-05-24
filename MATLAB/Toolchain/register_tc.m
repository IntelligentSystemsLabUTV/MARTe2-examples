%% REGISTER A CUSTOM TOOLCHAIN
% Save path
thispath = fileparts(mfilename('fullpath'));
% Load the toolchain
tc = raspberrypi_tc;
% Validate toolchain
tc.validate;
% Genearate
save(fullfile(thispath,'raspberrypi_tc.mat'), 'tc');
clear tc
% See all valid target
which -all rtwTargetInfo
% Rehash the targets
RTW.TargetRegistry.getInstance('reset');
% Review the list of registered toolchains
coder.make.getToolchains
