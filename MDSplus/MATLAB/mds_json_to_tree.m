function mds_json_to_tree(file_path)
%MDS_JSON_TO_TREE
% ----------  BEGIN CODE  ----------
% Get file parts
[~, file_name, file_ext] = fileparts(file_path);

% Check file extension
if ~strcmp(file_ext, '.json')
    fprintf('ERROR: Input file must be a JSON\n')
end

% Read file line by line
fd = fopen(file_path);
if (fd == -1)
    fprintf('ERROR: Input file `%s` not found\n', file_path)
    return
end
fprintf('INFO: Reading `%s`... ', file_path)
raw = char(fread(fd,inf)');
tree = jsondecode(raw);
fclose(fd);
fprintf('Done\n')

% Chek tree name
if ~strcmp(file_name, tree.name)
    fprintf('ERROR: Input file name and tree name does not match\n')
end

% Check if 'name'_path exist
ret = getenv([tree.name '_path']);
if isempty(ret)
    % Error
    fprintf('ERROR: Unable to find %s\n', tree.name)
    fprintf('\tPlease export %s_path = /path/to/tree\n', tree.name)
    return
end

% Create tree
mdstcl(['edit ' tree.name '/new'])
fprintf('INFO: %s created\n', tree.name)

% Elaborate data and add nodes
if isstruct(tree.subnodes)
    tree.subnodes = num2cell(tree.subnodes);
end
open = tree.subnodes;
while (numel(open) ~= 0)
    curr = open{1};
    open(1) = [];
    % Append path to subnodes
    if isstruct(curr.subnodes)
        curr.subnodes = num2cell(curr.subnodes);
    end
    for i = 1 : numel(curr.subnodes)
        curr.subnodes{i}.name = [curr.name ':' curr.subnodes{i}.name];
        open{end+1} = curr.subnodes{i};
    end
    % Check if exist field name
    if ~isfield(curr, 'name')
        fprintf('ERROR: Missing key `name`\n')
        return
    end
    % Check if field name is empty
    if isempty(curr.name)
        fprintf('ERROR: Missing value `name`\n')
        return
    end
    % Check if exist field usage
    if ~isfield(curr, 'usage')
        fprintf('ERROR: Missing key usage\n')
        return
    end
    % Check if field usage is empty
    if isempty(curr.usage)
        fprintf('ERROR: Missing value usage\n')
        return
    end
    % Add node
    mdstcl(['add node ' curr.name '/usage=' curr.usage])
    fprintf('INFO: Added node %s as %s\n', curr.name, curr.usage)

    switch curr.usage
        case 'signal'
            % Check if field x exists or if it is empty
            if ~isfield(curr, 'x') || isempty(curr.x)
                continue
            end
            % Check if field y exists ot if it is empty
            if ~isfield(curr, 'y') || isempty(curr.y)
                continue
            end
            % Set data
            x = eval(curr.x);
            y = eval(curr.y);
            mdsput(curr.name, 'BUILD_SIGNAL($1,,$2)', y, x);
            fprintf('INFO: Put in node %s data %s\n', curr.name, curr.y)
        case 'numeric'
            % Check if field data exists of if it is empty
            if ~isfield(curr, 'value') || isempty(curr.value)
                continue
            end
            % Set data
            mdsput(curr.name, '$1', curr.value);
            fprintf('INFO: Put in node %s data %s\n', curr.name, curr.value)
    end    
end

% Write and close
mdstcl('write')
mdstcl('close')
% -----------  END CODE  -----------