function [nodes] = mds_json_tree(file_path)
%MDS_JSON_TO_TREE

% ----------  BEGIN CODE  ----------
nodes = mds_read_json(file_path);
if numel(nodes) == 0
    return
end
mds_create_tree(nodes);
% -----------  END CODE  -----------
