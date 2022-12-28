#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
  int shmid = shmget(IPC_PRIVATE, 64, IPC_CREAT | 0644);
  if(shmid == -1) {
    perror("IPC error: shmget");
    exit(1);
  }

  /***
   * void *shmat(int shmid, const void *shmaddr, int shmflg)
   * If shmaddr is a null pointer, the segment is attached at the first
   * available address as selected by the system
   ***/
  switch (fork()) {
    case -1:
      perror("fork");
      exit(1);
      break;
    case 0: // child process
      printf("Child process is ready\n");
      char* shmaddr1 = (char*) shmat(shmid, (char*) NULL, 0);
      for (int i = 0; i < 10; ++i)
        shmaddr1[i] = 'a' + i;
      // system("ipcs -m");
      shmdt((char*) shmaddr1);
      // system("ipcs -m");
      break;
    default:
      wait(0); // wait until the child process is terminated
      printf("Parent process is ready\n");
      char* shmaddr2 = (char*) shmat(shmid, (char*) NULL, 0);
      for (int i = 0; i < 10; ++i)
        putchar(shmaddr2[i]);
      puts("");
      system("ipcs -m");
      sleep(1); /* to use ipcs to check memory status */
      shmdt((char*) shmaddr2);
      shmctl(shmid, IPC_RMID, (struct shmid_ds*) NULL);
      system("ipcs -m");
      break;
  }

  return 0;
}

