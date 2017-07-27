#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>

#include "../datastruct.h"

#define PORT 4030

int main () {

//Varlist
struct sockaddr_in myaddr, src_addr;
int skt, rc, len;
datagram packageRecv, packageSend;

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
len = sizeof(struct sockaddr_in);

//serverschleife

while(1) {
rc = recvfrom(skt, &packageRecv, sizeof(packageRecv), 0, (struct sockaddr *) &src_addr, &len);
	if(rc) {
		printf("Recieved Message: \n");
		printf("%s\n", packageRecv.data);
		strcpy(packageSend.data, "Merci");
		packageSend.version_id = htons(1);
		(void) sendto(skt, &packageSend, sizeof(packageSend), 0, (struct socketaddr *) &src_addr, sizeof(struct sockaddr_in));
		rc = 0;
		}
	}
}

/*
while (1) {
	rc = recvfrom(skt, &packageRecv, sizeof(packageRecv), 0, (struct socketaddr *) &src_addr, &len);
	printf("\n%s", packageRecv.data);
	if (rc < 0) {
		perror("Empfangsproblem");
		exit(1);
	}else{
		strcpy(packageSend.data, "Danke!");
		packageSend.version_id = htons(1);
		(void) sendto(skt, &packageSend, sizeof(packageSend), 0, (struct sockaddr *) &src_addr, sizeof(struct sockaddr_in));
		}
	}
	close(skt);
}
*/
