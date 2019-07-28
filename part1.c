#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

void compute(int port){

    // printf("Given port : %d\n", port);
    int i;
    for(i=1;i<=10;i++){
        int newPort = port + i;
        char str[100] = "";
        char np[100] = "";
        sprintf(np, "%d", newPort);
        strcat(str, "netstat -aont | grep ");
        strcat(str, "\"`hostname -i`:");
        strcat(str, np);
        strcat(str, "\" | wc -l >> tmp_output.txt");
        // printf("%s\n", str);

        system(str);
    }

    FILE *fp = fopen("/home/011/n/nx/nxs180035/cs5375/a3/tmp_output.txt", "r");
    char * line = NULL;
    ssize_t read;
    size_t len = 0;

    int t=1;

    while ((read = getline(&line, &len, fp)) != -1) {
        int value = atoi(line);
        int p = port + t;
        if(value==0) printf("%d : %s", p, "free\n");
        else printf("%d : %s", p, "not free\n");
        t++;
    }

    fclose(fp);
    remove("/home/011/n/nx/nxs180035/cs5375/a3/tmp_output.txt");
}

int main(int argc, char** argv){
    compute(atoi(argv[1]));
}