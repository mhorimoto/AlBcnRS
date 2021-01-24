#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>

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
