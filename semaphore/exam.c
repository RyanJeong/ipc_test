#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

union semun {
  int val_;
  struct semid_ds* buf_;
  unsigned short* array_;
};

int init_sem(key_t semkey) {
  int status = 0;

  /***
   * int semget(key_t key, int nsems, int semflg)
   ***/
  int semid = semget(semkey, 1, IPC_CREAT | IPC_EXCL | 0600);
  if (semid == -1) {
    if (errno == EEXIST)
      /***
       * get semaphore is already created to set semflg to 0
       ***/
      semid = semget(semkey, 1, 0);
  } else {
    union semun semun_arg;

    /***
     * int semctl(int semid, int semnum, int cmd, ...)
     * modify semaphore value to 1 using semctl
     ***/
    semun_arg.val_ = 1;
    status = semctl(semid, 0, SETVAL, semun_arg);
  }

  if (semid == -1 || status == -1) {
    perror("init_sem");
    return (-1);
  }

  return semid;
}

int sem_lock(int semid){
  struct sembuf buf;

  /***
   * Each operation is performed on the sem_num-th semaphore of the semaphore
   * set, where the first semaphore of the set is numbered 0
   ***/
  buf.sem_num = 0;  // to lock 0-th sem

  /***
   * to use shared resource and semval is going to 0
   * (can't use from other process)
   ***/
  buf.sem_op = -1;

  /***
   * if an error occurred, undo the lock process
   ***/
  buf.sem_flg = SEM_UNDO;

  /***
   * int semop(int semid, struct sembuf *sops, size_t nsops)
   * if the buf size is 2 -> semop(semid, &buf, 2)
   ***/
  if (semop(semid, &buf, 1) == -1) {
    perror("sem_lock failed");
    exit(1);
  }

  return 0;
}

int sem_unlock(int semid) {
  struct sembuf buf;

  /***
   * Each operation is performed on the sem_num-th semaphore of the semaphore
   * set, where the first semaphore of the set is numbered 0
   ***/
  buf.sem_num = 0;  // to lock 0-th sem

  /***
   * return the using shared resource and semval is going to 1
   * (now other process can use thos resource as 1 unit)
   ***/
  buf.sem_op = 1;

  /***
   * if an error occurred, undo the lock process
   ***/
  buf.sem_flg = SEM_UNDO;

  /***
   * int semop(int semid, struct sembuf *sops, size_t nsops)
   * if the buf size is 2 -> semop(semid, &buf, 2)
   ***/
  if (semop(semid, &buf, 1) == -1) {
    perror("sem_unlock failed");
    exit(1);
  }

  return 0;
}

void sem_handle() {
  pid_t pid = getpid();

  int semid = init_sem(1);
  if (semid < 0)
    exit(1);

  sem_lock(semid);

  printf("Lock : Process %d\n", (int) pid);
  printf("** Lock Mode : Critical Section\n");
  sleep(1);
  printf("Unlock : Process %d\n", (int) pid);

  sem_unlock(semid);

  exit(0);
}

int main(void) {
  for (int i = 0; i < 5; ++i) {
    if (!fork())
      sem_handle();
  }

  return 0;
}

