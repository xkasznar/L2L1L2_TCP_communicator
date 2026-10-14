#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
    
#define PORTNO 52002
#define MAX_MSG_SIZE 300
#define PREAMBLE 8
static const char COMMENTS[] = "N/A";
    
void error(const char *msg)
{
    perror(msg);
    exit(1);
}
    
void altprint(char buffer[]){
    for(int i = 0; i < MAX_MSG_SIZE; i++){
        unsigned char c = (unsigned char)buffer[i];
        if((c>=0 && c<=31 || c==127)==0)
            putchar(c);
        else
            putchar('X');
        if(i != 0 && (i+1)%50 == 0)
            putchar('\n');
    }
}
    
int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr,"Correct format: %s DEBUG SYNC_SKIP\n", argv[0]);
        exit(0);
    }
    int DEBUG = atoi(argv[1]);
    int SYNC_SKIP = atoi(argv[2]);
    
    int sockfd, newsockfd;
    socklen_t clilen;
    
    char buffer[MAX_MSG_SIZE];
    
    struct sockaddr_in serv_addr, cli_addr;
    
    int n;
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    int optval = 1; //hotfix bind failure
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)); //hotfix bind failure
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    
    serv_addr.sin_port = htons(PORTNO);
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
        sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    
    printf("Terminal TCP Server started! (PORTNO=%i, MAX_MSG_SIZE=%i, DEBUG=%i, SYNC_SKIP=%i, COMMENTS=%s)\n", PORTNO, MAX_MSG_SIZE,DEBUG,SYNC_SKIP,COMMENTS);
    LISTEN_AGAIN:
    
    newsockfd = accept(sockfd, 
        (struct sockaddr *) &cli_addr, 
        &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");
    
    bzero(buffer,MAX_MSG_SIZE);
    n = read(newsockfd,buffer,MAX_MSG_SIZE-1);
    
    if (n < 0) error("ERROR reading from socket");
    
    if(DEBUG == 0)
        printf("Here is the message:%s\n",buffer+PREAMBLE+SYNC_SKIP);
    else{
        printf("Here is the message:\n");
        altprint(buffer);
        printf("First 16 bytes: [");
        for(int i = 0; i<16;i++){
            printf("%d,", (unsigned char)buffer[i] );
            if((i+1)%4 == 0)
                printf(" ");
        }
        printf("]\n");
        printf("Last 16 bytes: [");
        for(int i = MAX_MSG_SIZE-16; i<MAX_MSG_SIZE;i++){
            printf("%d,", (unsigned char)buffer[i] );
            if((i+1)%4 == 0)
                printf(" ");
        }
        printf("]\n");

        int lie=0;
        int loe=0;
        for(int i = PREAMBLE; i<PREAMBLE+SYNC_SKIP; i++)
            if((unsigned char)buffer[i] != 255)
                lie++;
        for(int i = PREAMBLE+SYNC_SKIP+strlen(buffer+PREAMBLE+SYNC_SKIP); i<MAX_MSG_SIZE; i++)
            if((unsigned char)buffer[i] != 0)
                loe++;
        long int guar = MAX_MSG_SIZE-strlen(buffer+PREAMBLE+SYNC_SKIP)-PREAMBLE;
        printf("Leadin Errors-Lead out errors-Errors-Guar. Bytes-BER: %i-%i-%i-%li-%.2f%%\n",lie,loe,lie+loe,guar,100*((float)(lie+loe)/guar));
    }
    
    close(newsockfd);
    goto LISTEN_AGAIN;
    
    close(sockfd);
    return 1; 
}
