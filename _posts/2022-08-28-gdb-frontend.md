---
published: false
tag:
  - gdb
  - debugging
  - tool
---
# usage gdbfrontend 

GDBFrontend is an easy, flexible and extensionable gui debugger  

Easier for integrate remote debugging.


From target run 
```
gdbserver <binary>
```  

From work station run  
  
  ```
gdbfrontend 
(gdb) set substitute-path /usr/src/include /mnt/include
(gdb) set substitute-path /usr/src /mnt/src
```


Note:

Need to run gdb command for setup environment, configuration before load the binary (and/or connect to target).  

repo : [gdbfrontend](https://github.com/rohanrhu/gdb-frontend)

