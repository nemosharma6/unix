#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXLINE 4096

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE];

    if (argc != 5) {
        perror("Usage: TCPClient <IP address of the server> <port> <cmd_file> <log_file>");
        exit(1);
    }

    if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
        perror("Problem in creating the socket");
        exit(2);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr= inet_addr(argv[1]);
    servaddr.sin_port =  htons(atoi(argv[2])); //convert to big-endian order

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
        perror("Problem in connecting to the server");
        exit(3);
    }

    FILE *fp = fopen(argv[3], "r");
    char * line = NULL;
    ssize_t read;
    size_t len = 0;

    FILE *f = fopen(argv[4], "a");
    if (f == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

    while ((read = getline(&line, &len, fp)) != -1) {

        printf("%s\n", line);
        send(sockfd, line, strlen(line), 0);
        char recvline[MAXLINE] = "";

        if (recv(sockfd, recvline, MAXLINE,0) == 0){
            perror("The server terminated prematurely");
            exit(4);
        }

        fprintf(f, "%s", recvline);
        fprintf(f, "%s", "\n---------------\n\n");
        printf("%s", "String received from the server: ");
        fputs(recvline, stdout);
    }

    exit(0);
}
