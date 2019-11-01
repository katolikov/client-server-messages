#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MSGSZ 2047

typedef struct msgbuf{
    long mtype;
    char mtext[MSGSZ];
}message_buf;
int main()
{
    struct msqid_ds info;
    int msqid,fd;
    key_t key;
    message_buf  buf2;
    char buf[MSGSZ];
    key = 15;
    if((msqid=msgget(key,0))==-1){//возвращает дискриптор очереди сообщений (0 успешно)
        printf("msgget error\n");
        exit(1);
    }

    if(msgctl(msqid, IPC_STAT, &info)==-1){//выполняет контрольную операцию над сообщением (IPC_STAT - скопировать информацию из структуры)
      printf("msqctl error");
      exit(1);
    }

    printf("Message id: %d\n", msqid);
    if(msgrcv(msqid,&buf2,MSGSZ,info.msg_qnum,IPC_NOWAIT)==-1){//IPC_NOWAIT не ждать
        printf("msgrcv error\n");
        exit(1);
    }

    memcpy(buf,buf2.mtext, sizeof(buf2.mtext));


    if(access("haram", F_OK)==-1){

         fd = creat("haram",0666);
         write(fd,buf,strlen(buf));
         printf("Messages count: %ld\n", info.msg_qnum);
         printf("-|PID|-|RSS|-\n");
         system("rev haram | awk '$2' | cut -c 9- | rev");
         close(fd);
         system("rm haram");

    }

    else printf("Rename your file: haram\n");
    if(msgctl(msqid,IPC_RMID,NULL)==-1){//IPC_RMID - немедленно удалить очередь сообщений
            printf("Deleting error!\n");
            exit(1);
    }

    return 0;
}
