#ifndef DATASTRUCT_H
#define DATASTRUCT_H

typedef struct datagram datastruct;

struct datagram {
	unsigned short version_id;
	char data[1426];
	int checksum;
};


#endif
