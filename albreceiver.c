#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define False 0
#define True  1

volatile sig_atomic_t stopflag = 0;
void abrt_handler(int sig);

extern int rep(char *, const char *, const char *, const char *);

int main(int argc, char* argv[]) {
  const char* optstring = "vi:p:";
  int sd;
  struct sockaddr_in addr;
 
  socklen_t sin_size;
  struct sockaddr_in from_addr;
 
  char buf[2048];  // 受信バッファ
  char dbuf[2048]; // 表示バッファ
  char ipa[30];    // IP Address
  char ddd[11],tod[9];
  int  port;       // PORT
  int  cnt;        // 受信カウンタ
  int  rc;
  int  c;
  int  opt_i,opt_p;
  u_int yes = 1;
  time_t now;
  struct tm *tm_now;

  opt_i = False;
  opt_p = False;
  
  while((c=getopt(argc,argv,optstring))!=-1) {
    //    printf("opt=%c ",c);
    if (c=='i') {
      opt_i = True;
      sprintf(ipa,"%s",optarg);
    } else if (c=='p') {
      opt_p = True;
      port = atoi(optarg);
    } else if (c=='v') {
      printf("albreceiver Version 0.01\n");
      exit(0);
    }
  }
  
  if ( signal(SIGINT, abrt_handler) == SIG_ERR ) {
    exit(1);
  }

  // IPv4 UDP のソケットを作成
  if((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    return -1;
  }
 
  // 待ち受けるIPとポート番号を設定
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ipa); // すべてのアドレス宛のパケットを受信する
 
  // バインドする
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  if(bind(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    return -1;
  }
 
  // 受信バッファの初期化
  memset(buf, 0, sizeof(buf));

  while(!stopflag) {
    // 受信 パケットが到着するまでブロック
    // from_addr には、送信元アドレスが格納される
    rc = recvfrom(sd, buf, sizeof(buf), MSG_DONTWAIT,
		  (struct sockaddr *)&from_addr, &sin_size);
    if ( rc > 0 ) {
      buf[rc] = (char)NULL;
      now = time(NULL);
      tm_now = localtime(&now);
      sprintf(ddd,"%04d-%02d-%02d",tm_now->tm_year+1900,tm_now->tm_mon+1,tm_now->tm_mday);
      sprintf(tod,"%02d:%02d:%02d",tm_now->tm_hour,tm_now->tm_min,tm_now->tm_sec);
      strcpy(dbuf,buf);
      // 受信データの出力
      printf("%s %s %s\n",ddd,tod,dbuf);
    } else {
      if ( errno == EAGAIN ) continue;
      perror("recvfrom");
    }
  }
  // ソケットのクローズ
  close(sd);
  return 0;
}

void abrt_handler(int sig) {
  stopflag = 1;
}
