#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
  key_t semkey = ftok("./hello", 'a');
  if (semkey == (key_t) -1) {
    perror("IPC error: ftok");
    exit(1);
  }

  int msgid = msgget(semkey, 0);
  if (msgid == -1) {
    perror("IPC error: msgget");
    exit(1);
  }

  printf("Before IPC_RMID\n");
  system("ipcs -q");
  /* remove based on msgid */
  msgctl(msgid, IPC_RMID, (struct msqid_ds*) NULL);
  printf("After IPC_RMID\n");
  system("ipcs -q");

  return 0;
}

