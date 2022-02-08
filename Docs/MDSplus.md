# MDSplus 

- Export tree path

``` bash
export <tree_name>_path=/path/to/tree
```

- Create a new tree

``` bash
edit <tree_name>/new
add node NODE/usage=structure
add node NUM/usage=numeric
add node SIG/usage=signal
add node 
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
