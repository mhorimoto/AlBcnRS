#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>
/*
2020-09-06 17:46:37 61.114.108.19 CHUN2/IMR/STBY4 7815
2020-09-06 17:46:37 211.124.5.34 ACN/K/OYN 9189
2020-09-06 17:46:37 180.148.184.131 ICV/NS2 4568
2020-09-06 17:46:37 211.124.5.34 ACN/K/HMD 11188
2020-09-06 17:46:38 211.124.5.34 ACN/K/MTS 14284
2020-09-06 17:46:38 211.124.5.34 ACN/H/AMK 11655
2020-09-06 17:46:38 211.124.5.34 ACN/H/ARK 12779
2020-09-06 17:46:38 211.124.5.20 ACN/H/TUP3 26179
2020-09-06 17:46:38 211.124.5.34 ACN/K/YSM 20798
2020-09-06 17:46:38 211.124.5.41 ACN/K/TUP4 31288
2020-09-06 17:46:38 61.114.108.19 CHUN2/IMR/STBY4 7816
2020-09-06 17:46:38 211.124.5.34 ACN/K/OYN 9190
*/

/*
  struct sockaddr_in bcn_addr;
    bcn_addr.sin_addr.s_addr <<== IP Address
*/

struct ALB_CONFIG_HOST {
  int status;         // WDT status
  time_t ptime;       // Receive time
  int cnt;            // Counter
  int delay;          // ALART delay
  char name[31];      // beacon site name
  char dir[31];       // subdirectory name by text
  char ipa[16];       // IP address by text
  in_addr_t ip_addr;  // IP address by binary
  struct ALB_CONFIG_HOST *nextp;
};

typedef struct ALB_CONFIG_HOST ALB_cfghost;



