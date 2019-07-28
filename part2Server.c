#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096
#define LISTENQ 8

int main (int argc, char **argv) {
    int listenfd, connfd, n;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    pid_t childpid;

    listenfd = socket (AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(listenfd, LISTENQ);

    printf("%s\n","Server running...waiting for connections.");
    clilen = sizeof(cliaddr);

    while (1) {

        connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
        if(connfd < 0) {
            exit(1);
        }
        printf("%s\n","Received request...");

        char buf[MAXLINE] = "";

        if((childpid = fork()) == 0){
            printf("%s\n", "child created to handle client requests");
            close(listenfd);
            printf("child process : %u\n", getpid());
            while((n = recv(connfd, buf, MAXLINE,0)) > 0)  {
                if (n < 0) {
                    perror("Read error");
                    exit(1);
                }

                printf("from client to server:pid = %u, command = %s\n", (intmax_t) getpid(), buf);

                char str[MAXLINE] = "";
                buf[strlen(buf)-1]=' ';
                sprintf(str, "%s> tmp_output%d", buf, connfd);

                system(str);

                char path[200];
                sprintf(path, "/home/011/n/nx/nxs180035/cs5375/a3/tmp_output%d", connfd);
                FILE *fp = fopen(path, "r");
                char * line = NULL;
                ssize_t read;
                size_t len = 0;

                int t=1;
                char out[MAXLINE] = "";
                while ((read = getline(&line, &len, fp)) != -1) {
                    strcat(out, line);
                }

                fclose(fp);
                remove(path);

                send(connfd, out, strlen(out), 0);
                bzero(buf, sizeof(buf));
            }
        }
    }

    close(connfd);
    close(listenfd);
    return 0;
}
