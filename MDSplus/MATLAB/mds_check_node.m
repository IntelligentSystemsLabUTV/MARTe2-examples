function [ret] = mds_check_node(node, key)
%MDS_CHECK_NODE

% ----------  BEGIN CODE  ----------
ret = 1;
% Check if field 'name' exist
if ~isfield(node, key)
    fprintf('ERROR: Missing key `%s`\n', key)
    ret = 0;
end
% Check if field 'name' is empty
if isempty(node.(key))
    fprintf('ERROR: Missing value `%s`\n', key)
    ret = 0;
end
% -----------  END CODE  -----------