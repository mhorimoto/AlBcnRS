#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "alb.h"

ALB_cfghost *cptr,*ptr,*start_cfghost ;

int read_table(char *filename) {
  FILE *fp;
  int  reccnt;
  int  idelay;
  int  r;
  char *iip,*iid,*idir,*iname,buf[80];
  fp = fopen(filename,"r");
  if (fp==NULL) {
    fprintf(stderr,"Config File '%s' not found.\n",filename);
    exit(-1);
  }

  cptr = (ALB_cfghost *)NULL;
  ptr = (ALB_cfghost *)NULL;
  reccnt = 0;

  while(fgets(buf,sizeof(buf),fp)!=NULL) {
    iip = (char *)strtok(buf,",");
    iid = (char *)strtok(NULL,",");
    idelay = atoi((char *)strtok(NULL,","));
    idir = (char *)strtok(NULL,",");
    iname = (char *)strtok(NULL,"\n");
    cptr = (ALB_cfghost *)malloc(sizeof(ALB_cfghost));
    cptr->nextp = (ALB_cfghost *)ptr;
    cptr->delay = idelay;
    cptr->status = 1;
    strncpy(cptr->name,iname,30);
    strncpy(cptr->dir, idir, 30);
    strncpy(cptr->ipa, iip, 16);
    cptr->ip_addr = inet_addr(iip);
    /*    */
    ptr = cptr;
    reccnt++;
  }
  start_cfghost = ptr;
  fclose(fp);
  return(reccnt);
}

char *name[11] = {
    "stby4.chun2.ne.jp",
    "ns2.icv-net.ne.jp",
    "hmd1con.olt.acn-tv.co.jp",
    "mts1con.olt.acn-tv.co.jp",
    "amk1con.olt.acn-tv.co.jp",
    "ark1con.olt.acn-tv.co.jp",
    "tup3.acn-tv.co.jp",
    "ysm1con.olt.acn-tv.co.jp",
    "tup4.acn-tv.co.jp",
    "oyn1con.olt.acn-tv.co.jp",
    "trm.hikoichi-tv.ne.jp"
  };
char *dir[11] = {
    "CHUN2/IMR/STBY4",
    "ICV/NS2",
    "ACN/K/HMD",
    "ACN/K/MTS",
    "ACN/H/AMK",
    "ACN/H/ARK",
    "ACN/H/TUP3",
    "ACN/K/YSM",
    "ACN/K/TUP4",
    "ACN/K/OYN",
    "HIKO1/HE"
  };

char *ipa[11] = {
    "61.114.108.19",
    "180.148.184.131",
    "211.124.5.34",  /* ACN/K/HMD */
    "211.124.5.34",  /* ACN/K/MTS */
    "211.124.5.34",  /* ACN/H/AMK */
    "211.124.5.34",  /* ACN/H/ARK */
    "211.124.5.20",  /* ACN/H/TUP3 */
    "211.124.5.34",  /* ACN/K/YSM */
    "211.124.5.41",  /* ACN/K/TUP4 */
    "211.124.5.34",  /* ACN/K/OYN */
    "210.164.137.4"  /* HIKO1/HE  */
  };


void init_table(void) {
  int i;

  cptr = (ALB_cfghost *)NULL;
  ptr = (ALB_cfghost *)NULL;

  for (i=0;i<11;i++) {
    cptr = (ALB_cfghost *)malloc(sizeof(ALB_cfghost));
    cptr->nextp = (ALB_cfghost *)ptr;
    cptr->delay = 3;
    cptr->status = 1;
    strncpy(cptr->name,name[i],30);
    strncpy(cptr->dir, dir[i], 30);
    strncpy(cptr->ipa, ipa[i], 16);
    cptr->ip_addr = inet_addr(ipa[i]);
    /*    */
    ptr = cptr;
  }
  printf("created struct\n");
  while(ptr) {
    printf("===============\n");
    printf("NAME:%s\n",ptr->name);
    printf("DELAY:%d\n",ptr->delay);
    printf("DIR: %s\n",ptr->dir);
    printf("IP:  %s\n",ptr->ipa);
    printf("IPX: %x\n",ptr->ip_addr);
    ptr = ptr->nextp;
  }
  start_cfghost = cptr;
}
  
