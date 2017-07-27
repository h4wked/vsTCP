#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#define PORT 4030

int main () {

//Varlist
struct sockaddr_in myaddr, src_addr;
int skt, rc, len;

//socket erstellen
if ((skt = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	perror("Konnte kein socket erzeugen,\n");
	return -1;
	}
//socketaddress "füllen"
memset(&myaddr, 0, sizeof(struct sockaddr_in));
myaddr.sin_family = AF_INET;
myaddr.sin_port = htons(PORT);
rc = bind(skt, (struct sockaddr *) &myaddr, sizeof(struct sockaddr_in));

}

