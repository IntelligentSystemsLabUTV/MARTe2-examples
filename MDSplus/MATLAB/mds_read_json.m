function [nodes] = mds_read_json(file_path)
%MDS_READ_JSON

% ----------  BEGIN CODE  ----------
nodes = {}; 

% Get file parts
[~, ~, file_ext] = fileparts(file_path);

% Check file extension
if ~strcmp(file_ext, '.json')
    fprintf('ERROR: Input file must be a JSON\n')
    return
end

% Open file
fd = fopen(file_path);
if (fd == -1)
    fprintf('ERROR: Input file `%s` not found\n', file_path)
    return
end

% Read file line by line
fprintf('INFO: Reading `%s`... ', file_path)
raw = char(fread(fd, inf)');
tree = jsondecode(raw);
fclose(fd);
fprintf('Done\n')

% Add node
node.name = tree.name;
node.usage = tree.usage;
nodes = {node};

% Convert subnodes to struct array
if isstruct(tree.subnodes)
    tree.subnodes = num2cell(tree.subnodes);
end
open = tree.subnodes;
% Loop on nodes
while (numel(open) ~= 0)
    
    % Pop first node
    curr = open{1};
    open(1) = [];

    % Check field 'name'
    ret = mds_check_node(curr, 'name');
    if ret == 0
        return
    end

    % Check field 'usage'
    ret = mds_check_node(curr, 'usage');
    if ret == 0
        return
    end

    % Convert subnodes to struct array
    if isstruct(curr.subnodes)
        curr.subnodes = num2cell(curr.subnodes);
    end

    % Append parent name
    for i = 1:numel(curr.subnodes)
        curr.subnodes{i}.name = [curr.name ':' curr.subnodes{i}.name];
        open{end + 1} = curr.subnodes{i};
    end

    % Add node
    node.name = curr.name;
    node.usage = curr.usage;
    nodes = [nodes, node];

end
% -----------  END CODE  -----------