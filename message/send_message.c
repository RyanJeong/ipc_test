#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  long type_;
  char text_[64];
} MsgBuf;

int main(void) {
  key_t semkey = ftok("./hello", 'a');
  if (semkey == (key_t) -1) {
    perror("IPC error: ftok");
    exit(1);
  }

  int msgid = msgget(semkey, IPC_CREAT | 0644);
  if (msgid == -1) {
    perror("IPC error: msgget");
    exit(1);
  }

  MsgBuf msg;
  msg.type_ = 1234;
  strcpy(msg.text_, "Message queue test");
  int res = msgsnd(msgid, (void*) &msg, sizeof(msg.text_), IPC_NOWAIT);
  if (res == -1) {
    perror("IPC error: msgsnd");
    exit(1);
  }

  return 0;
}

