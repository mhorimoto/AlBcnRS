#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define False 0
#define True  1

volatile sig_atomic_t stopflag = 0;
void abrt_handler(int sig);

/*
 *  クライアントの接続先サーバ情報
 */
struct client_info {
    unsigned short sv_port;     /* サーバのポート番号 */
    char *sv_ipaddr;            /* サーバのIPアドレス */
    char *msg;                  /* 送信メッセージ */

    int sd;                     /* ソケットディスクリプタ */
    struct sockaddr_in sv_addr; /* サーバのアドレス構造体 */
};
typedef struct client_info cl_info_t;

static int initialize(int c, char *v[], cl_info_t *i, char *er) {
  if ( c != 4 ) {
    sprintf(er, "Usage: %s <ip-address> <port> <beacon message>",v[0]);
    return(-1);
  }
  memset(i,0,sizeof(cl_info_t));
  i->sv_ipaddr = v[1];
  i->sv_port   = atoi(v[2]);
  i->msg       = v[3];

  i->sd = socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
  if ( i->sd < 0 ) {
    sprintf(er,"(line:%d) %s", __LINE__, strerror(errno));
    return(-1);
  }

  i->sv_addr.sin_family  = AF_INET;
  i->sv_addr.sin_addr.s_addr = inet_addr(i->sv_ipaddr);
  i->sv_addr.sin_port = htons(i->sv_port);
  return(0);
}


static int sendBeacon(cl_info_t *i, int c, char *er) {
  int rc = 0;
  int msglen;
  char txt[BUFSIZ];

  sprintf(txt,"%s %d",i->msg,c);
  msglen = strlen(txt)+1;
  rc = sendto(i->sd,txt,msglen,0,
	      (struct sockaddr *)&(i->sv_addr),sizeof(i->sv_addr));
  if (rc != msglen) {
    sprintf(er,"(line:%d) %s", __LINE__, strerror(errno));
    return(-1);
  }
  return(0);
}


static void socket_finalize(cl_info_t *i) {
  if (i->sd != 0) close(i->sd);
  return;
}



int main(int argc, char *argv[]) {
  int rc = 0;
  cl_info_t info = {0};
  int cnt = 0;
  char errmsg[BUFSIZ];

  if ( signal(SIGINT, abrt_handler) == SIG_ERR ) {
    exit(1);
  }

  rc = initialize(argc, argv, &info, errmsg);
  if(rc != 0) {
    fprintf(stderr, "Error: %s\n", errmsg);
    return(-1);
  }

  while(!stopflag) {
    rc = sendBeacon(&info,cnt,errmsg);
    if(rc != 0) {
      fprintf(stderr, "Error: %s\n", errmsg);
      return(-1);
    }
    cnt++;
    sleep(1);
  }
  socket_finalize(&info);
  return(0);
}


void abrt_handler(int sig) {
  stopflag = 1;
}

