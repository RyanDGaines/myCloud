#include "csapp.h"

int main(int argc, char **argv)
{
    int listenfd, connfd, port;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    char client_hostname[MAXLINE], client_port[MAXLINE], retVal[MAXLINE];
    unsigned int key, clientkey;
       
    if (argc != 3) {
        fprintf(stderr, "usage: %s <port> <key>\n", argv[0]);
        exit(0);
    }
    
    port = atoi(argv[1]);
    key = atoi(argv[2]);
    
    
    listenfd = Open_listenfd(argv[1]);
    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE,
                    client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        char buf[MAXLINE];
        
        //concat key
        Rio_readn(connfd, &buf, MAXLINE);
        memcpy(&clientkey, &buf[4], 4);
        if (key != clientkey){
            printf("Failure: invalid key!\n");
            retVal[0] = '9';
            retVal[1] = '9';
            retVal[2] = '9';
            retVal[3] = '9';
        }
        else{
            if(buf[0] == '1'){
                printf("Secret key: %d\tcput\t", key);
                int size;
                memcpy(&size, &buf[87], 4);
                
                char data[MAXLINE];
                memcpy(&data, &buf[92], size);
                
                char filename[80];
                memcpy(&filename, &buf[8], 80);
                
                FILE *write;
                write = fopen(filename, "w+");
                if (fputs(data, write) != EOF){
                    retVal[0] = '0';
                    printf("%s\tSuccess\n", filename);
                }
                else{
                    int mem = -1;
                    memcpy(&retVal[0], &mem, 1);
                    printf("%s\tFailure\n", filename);
                }
                fclose(write);
                
            }
            else if(buf[0] == '2'){
                printf("Secret key: %d\tcget\t", key);
                char filename[80];
                memcpy(&filename, &buf[8], 80);
            
                FILE *get;
                get = fopen(filename, "r");
                if (get != NULL){
                    fseek(get, 0L, SEEK_END);
                    unsigned int size = ftell(get);
                    rewind(get);
                    char filecontents[MAXLINE];
                    Fread(&filecontents, sizeof(filecontents), 1, get);
                    memcpy(&retVal[4], &size, 4);
                    memcpy(&retVal[8], &filecontents, size);
                    printf("%s\tSuccess\n", filename);
                    retVal[0] = '0';
                }
                else{
                    printf("%s\tFailure\n", filename);
                    int mem = -1;
                    memcpy(&retVal[0], &mem, 1);
                }
                fclose(get);
                
            }
            else if (buf[0] == '3'){
                printf("Secret key: %d\tcdelete\t", key);
                char filename[80];
                memcpy(&filename, &buf[8], 80);
                if (strcmp(filename, "Server") == 0){
                    printf("%s\tFailure\n", filename);
                    int mem = -1;
                    memcpy(&retVal[0], &mem, 1);
                }
                else if (remove(filename) == 0){
                    printf("%s\tSuccess\n", filename);
                    retVal[0] = '0';
                }
                else{
                    printf("%s\tFailure\n", filename);
                    int mem = -1;
                    memcpy(&retVal[0], &mem, 1);
                }
            }
            else if (buf[0] == '4'){
                printf("Secret key: %d\tclist\t", key);
                DIR *d;
                struct dirent *dir;
                d = opendir(".");
                int num = 0;
                if (d){
                    while ((dir = readdir(d)) != NULL){
                        if (strcmp(dir->d_name, "Server") != 0){
                            char file[80];
                            memcpy(file, dir->d_name, 80);
                            memcpy(&retVal[num*80+8], file, 80);
                            num++;
                        }
                        
                    }
                    memcpy(&retVal[4], &num, 4);
                    printf("\tSuccess\n");
                    retVal[0] = '0';
                }
                else{
                    printf("\tFailure\n");
                    int mem = -1;
                    memcpy(&retVal[0], &mem, 1);
                }
                
            }
        }
        Rio_writen(connfd, retVal, sizeof(retVal));
    }
}


