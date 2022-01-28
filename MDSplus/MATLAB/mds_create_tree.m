function mds_create_tree(name)
%MDS_CREATE_TREE Summary of this function goes here
%   Detailed explanation goes here

% ---------- BEGIN CODE ----------
% Check if 'name'_path exist
ret = getenv([name '_path']);
if isempty(ret)
    % Error
    fprintf('ERROR: Unable to find %s\n', name)
    fprintf('Please export %s_path = /path/to/tree\n', name)
else
    % Create tree
    mdstcl(['edit ' name '/new'])
    fprintf('INFO: %s created\n', name)
end
% ----------- END CODE -----------
