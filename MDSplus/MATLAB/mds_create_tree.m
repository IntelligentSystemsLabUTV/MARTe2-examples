function mds_create_tree(nodes)
%MDS_CREATE_TREE Summary of this function goes here
%   Detailed explanation goes here

% ---------- BEGIN CODE ----------
tree_name = nodes{1}.name;
nodes(1) = [];
% Check if 'name'_path exist
ret = getenv([tree_name '_path']);
if isempty(ret)
    % Error
    fprintf('ERROR: Unable to find %s\n', tree_name)
    fprintf('Please export %s_path = /path/to/%s\n', tree_name, tree_name)
else
    % Create tree
    mdstcl(['edit ' tree_name '/new'])
    fprintf('INFO: %s created\n', tree_name)
    % Add nodes
    for node = nodes
        mdstcl(['add node ' node.name '/usage=' node.usage])
    end
end
% ----------- END CODE -----------
