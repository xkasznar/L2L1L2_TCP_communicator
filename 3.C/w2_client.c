#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_MSG_SIZE 10

void error(const char *);

int main(int argc, char *argv[])
{
   int sockfd, servlen,n;
   struct sockaddr_un  serv_addr;
   char buffer[MAX_MSG_SIZE];

   bzero((char *)&serv_addr,sizeof(serv_addr));
   serv_addr.sun_family = AF_UNIX;
   if(argc != 2){
       printf("averted seg fault, please provide socket\n");
       exit(1);
   }
   strcpy(serv_addr.sun_path, argv[1]);
   servlen = strlen(serv_addr.sun_path) + 
                 sizeof(serv_addr.sun_family);
   if ((sockfd = socket(AF_UNIX, SOCK_STREAM,0)) < 0)
       error("Creating socket");
   if (connect(sockfd, (struct sockaddr *) 
                         &serv_addr, servlen) < 0)
       error("Connecting");
   printf("client sending message:");
    bzero(buffer,MAX_MSG_SIZE);
    fgets(buffer,MAX_MSG_SIZE,stdin);
    
    for(int i = 0; i < strlen(buffer); i++){ //editing of text to lowercase
        buffer[i] = tolower(buffer[i]); //editing of text to lowercase
    }
    
    buffer[strcspn(buffer, "\n")] = '\0'; //hotfix to fix trailing '\n'
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,MAX_MSG_SIZE);
    n = read(sockfd,buffer,MAX_MSG_SIZE);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("client received reply from server:%s\n",buffer);
    close(sockfd);
   return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

