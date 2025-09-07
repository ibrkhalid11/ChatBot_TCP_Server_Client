#include <stdio.h>
#include <stdlib.h>
#include <string.h> /*not strings.h*/ /*remove all mention of bzero with memset*/
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>


#define MAX 1024
#define PORT 8080 /*verify this is a good port with ss -tulnp */
#define SA struct sockaddr

int get_ai_response(const char *input, char *output, int maxlen){
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) return -1;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9090);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sockfd,(struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){
        close(sockfd);
        return -1;
    }
    write(sockfd, input, strlen(input));
    int n = read(sockfd, output, maxlen - 1);
    if (n > 0) output[n] = '\0';
    close(sockfd);
    return n;
}

void func(int connfd){
  char buff[MAX];
  char reply[MAX];
  for(;;){
    memset(buff,0, MAX);
    int n = read(connfd, buff, sizeof(buff) - 1);
    if(n <= 0) break;
    buff[n] = '\0';
    printf("From Client %s\n", buff);
    if(strncmp(buff, "exit", 4) == 0){
        strcpy(reply, "server closing connection\n");
    } 
    else{
        if (get_ai_response(buff, reply, MAX) < 0)
            strcpy(reply, "AI server not available.");
    }
    printf("To Client: %s\n", reply);
    write(connfd, reply, strlen(reply));
    if (strncmp(buff, "exit", 4) == 0) break;
  }
}

int main(void){
  int sockfd, connfd; 
  socklen_t len;
  struct sockaddr_in servaddr, cli;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd == -1){
      printf("Socket Creation Failed... \n");
      exit(0);
  } 
  else 
      printf("Socket Successfully Created...\n");
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);


  if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
      printf("socket bind failed...\n");
      exit(0);
  }
  else
      printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
  if ((listen(sockfd, 5)) != 0) {
      printf("Listen failed...\n");
      exit(0);
  }
  else
    printf("Server listening..\n");
  len = sizeof(cli);
    // Accept the data packet from client and verification
  connfd = accept(sockfd, (SA*)&cli, &len);
  if (connfd < 0) {
      printf("server accept failed...\n");
      exit(0);
  }
  else
    printf("server accept the client...\n");
    // Function for chatting between client and server
  func(connfd);
    // After chatting close the socket
  close(sockfd);
}
