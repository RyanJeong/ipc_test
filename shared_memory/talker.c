#define MEM_SIZE 1024

#include <sys/types.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  key_t semkey = ftok("./hello", 'a');
  if (semkey == (key_t) -1) {
    perror("IPC error: ftok");
    exit(1);
  }

  /* If a shared memory identifier exists already, shmflg is 0 */
  int shmid = shmget(semkey, MEM_SIZE, 0);
  if (shmid == -1) {
    perror("IPC error: shmget");
    exit(1);
  }

  void *shmaddr;
  shmaddr = shmat(shmid, NULL, 0);
  strcpy(shmaddr, "Hello, I'm a talker\n");

  // send any signal to any process group or process
  kill(atoi(argv[1]), SIGUSR1);
  sleep(1);  // wait to be able to receive a response from the listener side

  char buf[MEM_SIZE];
  strcpy(buf, shmaddr);
  printf("Listener said : %s\n", buf);

  system("ipcs -m");
  shmdt(shmaddr);
  shmctl(shmid, IPC_RMID, NULL);

  return 0;
}

