#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 2047

typedef struct msgbuf {
    long mtype;
    char mtxt[MSGSZ];
}message_buf;


int main()
{
    struct msqid_ds info;
    FILE *g = popen("ps -eAo pid,rss,tty|grep '?'","r");//pipe ->fork()->wait()

    if(!(g))
    {
      printf("\nPopen error");
      exit(1);
    }

    int tail, msqid, msgflg = IPC_CREAT | 0666;
    char m[MSGSZ];
    key_t key = 15;
    message_buf mbuf;
    size_t mbuf_lenght;
    tail = fread(m, 1, MSGSZ, g);
    m[tail] = '\0';

    if((msqid = msgget(key,msgflg))==-1){//возвращает индф очереди сообщений (0 успешно)
        printf("msgget errot\n");
        exit(1);
    }
    if(msgctl(msqid, IPC_STAT, &info)==-1){//выполняет контрольную операцию над сообщением (IPC_STAT - скопировать информацию из структуры)
      printf("msqctl error");
      exit(1);
    }

    mbuf.mtype = info.msg_qnum + 1;
    memcpy(mbuf.mtxt, m, strlen(m));
    mbuf_lenght = strlen(mbuf.mtxt);

    if(msgsnd(msqid, &mbuf, mbuf_lenght, IPC_NOWAIT)==-1){
        printf("Send Error\n");
        exit(1);
    }

    else{
        printf("-|PID|-|RSS|-|TTY|-\n");
        printf("%s\nMessages have sent.\n",mbuf.mtxt);
    }

    pclose(g);
    return 0;

}
