model_name = 'Plant';
% Solver
set_param(model_name, 'SolverType', 'Fixed-step');
% Code Generation
set_param(model_name, 'SystemTargetFile', 'ert_shrlib.tlc');
set_param(model_name, 'RTWVerbose', 0);
% Optimization
set_param(model_name, 'DefaultParameterBehavior', 'Tunable');
set_param(model_name, 'OptimizationCustomize', 1);
set_param(model_name, 'GlobalVariableUsage', 'None');
% Report
set_param(model_name, 'GenerateReport', 0);
% Comments
set_param(model_name, 'GenerateComments', 0);
% Custom code (MODEL is a coder varialbe for the model name)
set_param(model_name, 'CustomSourceCode', ...
[ ...
'#define CONCAT(str1, str2, str3) CONCAT_(str1, str2, str3)' newline, ...
'#define CONCAT_(str1, str2, str3) str1 ## str2 ## str3' newline, ...
'#define GET_MMI_FUNC CONCAT(MODEL, _GetCAPImmi , )' newline, ...
'#define RT_MODEL_STRUCT CONCAT(RT_MODEL_ , MODEL, _T)' newline, ...
newline, ...
'void* GET_MMI_FUNC(void* voidPtrToRealTimeStructure)' newline, ...
'{' newline, ...
'rtwCAPI_ModelMappingInfo* mmiPtr = &(rtmGetDataMapInfo( ( RT_MODEL_STRUCT *)(voidPtrToRealTimeStructure) ).mmi);' newline, ...
'return (void*) mmiPtr;' newline, ...
'}' ...
] ...
);
% Interface
set_param(model_name, 'SupportComplex', 0);
set_param(model_name, 'SupportAbsoluteTime', 0);
set_param(model_name, 'SuppressErrorStatus', 1);
set_param(model_name, 'CodeInterfacePackaging', 'Reusable function');
set_param(model_name, 'RootIOFormat', 'Part of model data structure');
set_param(model_name, 'RTWCAPIParams', 1);
set_param(model_name, 'RTWCAPIRootIO', 1);
set_param(model_name, 'GenerateAllocFcn', 1);
set_param(model_name, 'IncludeMdlTerminateFcn', 0);
set_param(model_name, 'CombineSignalStateStructs', 1);
set_param(model_name, 'ArrayLayout', 'Column-major');
% Templates
set_param(model_name, 'GenerateSampleERTMain', 0);

% Toolchain
% register_tc;
% cfg = coder.config('exe');
% cfg.Toolchain = 'Raspberry Pi (64-bit Linux)';
