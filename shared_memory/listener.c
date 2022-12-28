#define _XOPEN_SOURCE 500
#define MEM_SIZE 1024

#include <sys/types.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void handler(int dummy) {
  ;
}

int main(void) {
  key_t semkey = ftok("./hello", 'a');
  if (semkey == (key_t) -1) {
    perror("IPC error: ftok");
    exit(1);
  }

  int shmid = shmget(semkey, MEM_SIZE, IPC_CREAT | 0666);
  if (shmid == -1) {
    perror("IPC error: shmget");
    exit(1);
  }

  printf("Listenser wait for Talker\n");
  sigset_t mask;

  /***
   * int sigfillset(sigset_t *set)
   * initialize the signal set pointed to by set
   ***/
  sigfillset(&mask);

  /***
   * int sigdelset(sigset_t *set, int signo)
   * deletes the individual signal specified by signo from the signal set
   * pointed to by set
   ***/
  sigdelset(&mask, SIGUSR1);

  /***
   * sigset(int sig, void (*disp)(int))
   * modify signal dispositions
   ***/
  sigset(SIGUSR1, handler);

  /***
   * int sigsuspend(const sigset_t *sigmask)
   * replace the current signal mask of the calling thread with the set of
   * signals pointed to by sigmask and then suspend the thread until delivery
   * of a signal whose action is either to execute a signal-catching function
   * or to terminate the process
   ***/
  // <- wait here until the SIGUSR1 signal is received
  sigsuspend(&mask);
  printf("Listener Start !\n");

  /***
   * void *shmat(int shmid, const void *shmaddr, int shmflg)
   * If shmaddr is a null pointer, the segment is attached at the first
   * available address as selected by the system
   ***/
  void* shmaddr = shmat(shmid, NULL, 0);
  char buf[MEM_SIZE];
  strcpy(buf, shmaddr);
  printf("Listener recieved : %s\n", buf);

  strcpy(shmaddr, "Have a nice day!\n");
  sleep(3); // to be able to call ipcs on talker side
  shmdt(shmaddr);

  return 0;
}

