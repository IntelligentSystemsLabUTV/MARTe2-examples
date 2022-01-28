function mds_open_tree(name, shot)
%MDS_OPEN_TREE Summary of this function goes here
%   Detailed explanation goes here

% ---------- BEGIN CODE ----------
% Check if 'name'_path exist
ret = getenv([name '_path']);
if isempty(ret)
    % Error
    fprintf('ERROR: Unable to find %s\n', name)
    fprintf('Please export %s_path = /path/to/tree\n', name)
else
    % Check if tree:shot exist
    ret = mdsopen(name, shot);
    if ret == shot
        % Open tree
        fprintf('INFO: %s opened\n', name)
    elseif shot == -1
        % Create tree
        fprintf('INFO: Creating %s...\n', name)
        mds_create_tree(name)
        fprintf('INFO: %s opened\n', name)
    else
        % Error
        fprintf('ERROR: Shot #%d of %s does not exist\n', shot, name)
    end
end
% ----------- END CODE -----------
