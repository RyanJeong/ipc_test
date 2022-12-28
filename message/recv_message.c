#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>

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

  int msgid = msgget(semkey, 0);
  if (msgid == -1) {
    perror("IPC error: msgget");
    exit(1);
  }

  MsgBuf recv_msg;
  /***
   * no message shall be received, msgrcv() shall return -1 
   * 
   * ssize_t msgrcv(int msqid,
   *                void *msgp,
   *                size_t msgsz,
   *                long msgtyp,
   *                int msgflg)
   * The following example receives the first message on the queue
   * (based on the value of the msgtyp argument, 0)
   ***/
  int len = msgrcv(msgid,
                   (void*) &recv_msg,
                   sizeof(recv_msg.text_),
                   0,
                   MSG_NOERROR | IPC_NOWAIT);
  if (len < 0) {
    perror("IPC error: msgrcv");
    exit(1);
  }
  printf("Received Msg = %s, Len= %d\n", recv_msg.text_, len);

  return 0;
}

