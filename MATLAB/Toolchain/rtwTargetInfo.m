function rtwTargetInfo(tr)
%RTWTARGETINFO Registration file for custom toolchains.
tr.registerTargetInfo(@loc_createToolchain);
end

function config = loc_createToolchain
%LOC_CREATETOOLCHAIN Create the ToolchainInfoRegistry entries.
config(1) = coder.make.ToolchainInfoRegistry;
config(1).Name = 'Raspberry Pi (64-bit Linux)';
config(1).FileName = fullfile(fileparts(mfilename('fullpath')), 'raspberrypi_tc.mat');
config(1).TargetHWDeviceType = {'*'};
config(1).Platform =  {computer('arch')};
end
