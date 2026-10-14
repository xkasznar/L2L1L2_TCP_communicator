#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
    
static const char HOSTNAME[] = "localhost";
#define PORTNO 52001
#define MAX_MSG_SIZE 300
static const char COMMENTS[] = "N/A";
    
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
    
int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr,"Correct format: %s AUTO_MSG_HOTFIX SYNC_SKIP\n", argv[0]);
        exit(0);
    }
    int AUTO_MSG_HOTFIX = atoi(argv[1]);
    int SYNC_SKIP = atoi(argv[2]);
    printf("Terminal TCP Client started! (HOSTNAME=%s, PORTNO=%i, MAX_MSG_SIZE=%i, AUTO_MSG_HOTFIX=%i, SYNC_SKIP=%i, COMMENTS=%s)\n", HOSTNAME, PORTNO, MAX_MSG_SIZE, AUTO_MSG_HOTFIX, SYNC_SKIP, COMMENTS);   
    int emptyrun=0;
    EMPTYRUN:
    
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    char buffer[MAX_MSG_SIZE];
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);   
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    server = gethostbyname(HOSTNAME);
    
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }        
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
    
    serv_addr.sin_port = htons(PORTNO);
    
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    
    if(emptyrun==0)
        printf("Please enter the message:");
    bzero(buffer,MAX_MSG_SIZE);
    char finbuffer[MAX_MSG_SIZE];
    bzero(finbuffer,MAX_MSG_SIZE);
    if(emptyrun==0){
        fgets(buffer,MAX_MSG_SIZE-1,stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
    
        memmove(finbuffer+SYNC_SKIP, buffer, strlen(buffer));
        memset(finbuffer, 255, SYNC_SKIP);
    }
    
    n = write(sockfd,finbuffer,MAX_MSG_SIZE);
    
    if (n < 0) 
        error("ERROR writing to socket");
    close(sockfd);
    
    if(emptyrun==1)
        return 0;
    emptyrun=1;
    
    if(AUTO_MSG_HOTFIX == 0){
        printf("Press Enter to apply message hotfix.");
        getchar();
    }
    goto EMPTYRUN; //hotfix to enable arrival of messages to server, TODO: find cause of fails and delete this hotfix
    
    return 1;
}
