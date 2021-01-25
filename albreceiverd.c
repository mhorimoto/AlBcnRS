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
#include "alb.h"

#define False 0
#define True  1
#define CFGFILENAME "/usr/local/etc/host.cfg"
#define LOGFILENAME "/var/log/albreceiver.log"

extern ALB_cfghost *cptr,*ptr,*start_cfghost ;
extern void init_table(void);

volatile sig_atomic_t stopflag = 0;
void abrt_handler(int sig);

int main(int argc, char* argv[]) {
  int  c,opt_a,opt_p,unknown;
  char ipa[30];    // IP Address
  int  port;       // PORT
  const char* optstring = "va:p:";
  opt_a = False;
  opt_p = False;
  unknown = True;

  while((c=getopt(argc,argv,optstring))!=-1) {
  //    printf("opt=%c ",c);
    if (c=='a') {
      opt_a = True;
      sprintf(ipa,"%s",optarg);
    } else if (c=='p') {
      opt_p = True;
      port = atoi(optarg);
    } else if (c=='v') {
      printf("albreceiver Version 0.30\n");
      exit(0);
    }
  }

  //ここで子プロセスを生成し親は終了
  if(daemon(0, 1) == 0) {
    mainLoop(port,&ipa[0]);
  } else {
    printf("error\n");
  }
  return 0;
}

int mainLoop(int port, char *ipa) {
  int sd;
  struct sockaddr_in addr;
 
  socklen_t sin_size;
  struct sockaddr_in from_addr,bcn_addr;

  char buf[2048];  // 受信バッファ
  char dbuf[2048]; // 表示バッファ
  char *ptrdbuf;
  //char ipa[30];    // IP Address
  char ddd[11],tod[9];
  //  int  port;       // PORT
  int  rc;
  int  rtcnt;
  int  unknown;
  int  opt_a,opt_p;
  u_int yes = 1;
  time_t now,past;
  struct tm *tm_now;
  FILE *lfp;

  opt_a = False;
  opt_p = False;
  unknown = True;

  //  while((c=getopt(argc,argv,optstring))!=-1) {
  //    //    printf("opt=%c ",c);
  //    if (c=='a') {
  //      opt_a = True;
  //      sprintf(ipa,"%s",optarg);
  //    } else if (c=='p') {
  //      opt_p = True;
  //      port = atoi(optarg);
  //    } else if (c=='v') {
  //      printf("albreceiver Version 0.22\n");
  //      exit(0);
  //    }
  //  }

  /*  init_table(); */
  rtcnt = read_table(CFGFILENAME);
  printf("Host count=%d\n",rtcnt);

  if ( signal(SIGINT, abrt_handler) == SIG_ERR ) {
    exit(1);
  }
  printf("abrt_handler() OK.\n");

  // IPv4 UDP のソケットを作成
  if((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket");
    return -1;
  }
  printf("Create socket OK.\n");
 
  // 待ち受けるIPとポート番号を設定
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ipa); // すべてのアドレス宛のパケットを受信する
  printf("port is %d\n",port);
 
  // バインドする
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  if(bind(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    return -1;
  }
 
  // 受信バッファの初期化
  memset(buf, 0, sizeof(buf));

  while(!stopflag) {
    now = time(NULL);
    if (past!=now) {
      ptr = start_cfghost;
      while(ptr) {
	ptr->cnt--;
	if (ptr->cnt == 0) {
	  tm_now = localtime(&now);
	  ptr->status = 0;
	  sprintf(ddd,"%04d-%02d-%02d",tm_now->tm_year+1900,tm_now->tm_mon+1,tm_now->tm_mday);
	  sprintf(tod,"%02d:%02d:%02d",tm_now->tm_hour,tm_now->tm_min,tm_now->tm_sec);
	  lfp = fopen(LOGFILENAME,"a");
	  if (lfp!=NULL) {
	    fprintf(lfp,"%s %s %d DISCON %-16s %s\n",ddd,tod,now,ptr->ipa,ptr->dir);
	  } else {
	    fprintf(stderr,"%s %s %d DISCON %-16s %s\n",ddd,tod,now,ptr->ipa,ptr->dir);
	  }
	  fclose(lfp);
	}
	ptr = ptr->nextp;
      }
      past = now;
    }
    rc = recvfrom(sd, buf, sizeof(buf), MSG_DONTWAIT,
		  (struct sockaddr *)&from_addr, &sin_size);  // Receive waiting
    if ( rc > 0 ) {
      buf[rc] = (char)NULL;
      strcpy(dbuf,buf);
      ptrdbuf = strtok(dbuf," ");
      // Check Watch Table
      ptr = start_cfghost;
      unknown = True;
      tm_now = localtime(&now);
      while(ptr) {
	if (ptr->ip_addr==from_addr.sin_addr.s_addr) {
	  unknown = False;
	  if (!strcmp(ptrdbuf,ptr->dir)) {
	    if (ptr->status==0) {
	      sprintf(ddd,"%04d-%02d-%02d",tm_now->tm_year+1900,tm_now->tm_mon+1,tm_now->tm_mday);
	      sprintf(tod,"%02d:%02d:%02d",tm_now->tm_hour,tm_now->tm_min,tm_now->tm_sec);
	      lfp = fopen(LOGFILENAME,"a");
	      if (lfp!=NULL) {
		fprintf(lfp,"%s %s %d RECONN %-16s %s\n",ddd,tod,now,ptr->ipa,buf);
	      } else {
		fprintf(stderr,"%s %s %d RECONN %-16s %s\n",ddd,tod,now,ptr->ipa,buf);
	      }
	      fclose(lfp);
	    }
	    ptr->cnt = ptr->delay + 1;
	    ptr->ptime = now;
	    ptr->status = 1;
	    break;
	  }
	}
	ptr = ptr->nextp;
      }
      if (unknown==True) {
	sprintf(ddd,"%04d-%02d-%02d",tm_now->tm_year+1900,tm_now->tm_mon+1,tm_now->tm_mday);
	sprintf(tod,"%02d:%02d:%02d",tm_now->tm_hour,tm_now->tm_min,tm_now->tm_sec);
	lfp = fopen(LOGFILENAME,"a");
	if (lfp!=NULL) {
	  fprintf(lfp,"%s %s %d UNKNOWN %-16s %s\n",ddd,tod,now,inet_ntoa(from_addr.sin_addr),buf);
	} else {
	  fprintf(stderr,"%s %s %d UNKNOWN %-16s %s\n",ddd,tod,now,inet_ntoa(from_addr.sin_addr),buf);
	}
	fclose(lfp);
      }
    } else {
      if ( errno == EAGAIN ) {
	continue;
      }
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

void printout(time_t now,char *keymsg,char *buf) {
  time_t now;
  char ddd[11],tod[9];
  struct tm *tm_now;
  FILE *lfp;

  sprintf(ddd,"%04d-%02d-%02d",tm_now->tm_year+1900,tm_now->tm_mon+1,tm_now->tm_mday);
  sprintf(tod,"%02d:%02d:%02d",tm_now->tm_hour,tm_now->tm_min,tm_now->tm_sec);
  lfp = fopen(LOGFILENAME,"a");
  if (lfp!=NULL) {
    fprintf(lfp,"%s %s %d RECONN %-16s %s\n",ddd,tod,now,ptr->ipa,buf);
  } else {
    fprintf(stderr,"%s %s %d RECONN %-16s %s\n",ddd,tod,now,ptr->ipa,buf);
  }
  fclose(lfp);
}
