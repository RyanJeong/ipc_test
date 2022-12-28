```
$ ./sem 
Lock : Process 30829
** Lock Mode : Critical Section
Unlock : Process 30829
Lock : Process 30830
** Lock Mode : Critical Section
Unlock : Process 30830
Lock : Process 30831
** Lock Mode : Critical Section
Unlock : Process 30831
Lock : Process 30832
** Lock Mode : Critical Section
Unlock : Process 30832
Lock : Process 30833
** Lock Mode : Critical Section
Unlock : Process 30833

$ ipcs -s

------ Semaphore Arrays --------
key        semid      owner      perms      nsems     
0x00000001 5          munseongje 600        1         

$ ipcrm -s5
$ ipcs -s

------ Semaphore Arrays --------
key        semid      owner      perms      nsems     

```
