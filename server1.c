#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>


#define MAX 1024
#define PORT 8080 /*verify this is a good port with ss -tulnp */
