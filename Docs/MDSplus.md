# MDSplus 

- export tree path

``` bash
export <tree_name>_path=/path/to/tree
```


- create tree

``` bash
set tree <tree_name>
...
write
close
```

- Open a MDS server

``` mdstcl
mdsip -h /usr/mdsplus/etc/mdsip.hosts -p 8005 -m
```

- Create a new pulse with `mdstcl`

``` bash
set tree <tree_name>
set current <tree_nume> /increment
create pulse 0
close
```
