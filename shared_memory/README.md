```
$ ./server &
[5] 24465
$ Listenser wait for Talker
$ ./client 24465
Listener Start =====
Listener recieved : Hello, I'm a talker

Listener said : Have a nice day!


------ Shared Memory Segments --------
0x610216bb 12124189   munseongje 666        1024       2                       
$ ipcs -m

------ Shared Memory Segments --------
key        shmid      owner      perms      bytes      nattch     status      
```
