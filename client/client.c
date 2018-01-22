#include "csapp.h"

int main(int argc, char **argv)
{
    int clientfd;
    int test = 0;
    char *host, *port, buf[MAXLINE], choice[MAXLINE], serverBuf[MAXLINE], filename[80];
    rio_t rio;
    unsigned int key;
        
    if (argc != 4) {
        fprintf(stderr, "usage: %s <host> <port> <key>\n", argv[0]);
        exit(0);
    }
    
    host = argv[1];
    port = argv[2];
    key = atoi(argv[3]);
    buf[0] = '\0';
    buf[1] = '\0';
    buf[2] = '\0';
    buf[3] = '\0';
    memcpy(&buf[4], &key, 4);
    
    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);
    Rio_writen(clientfd, buf, sizeof(buf));
    Rio_readnb(&rio, serverBuf, sizeof(serverBuf));
    while (1){
        test = 0;
        if (serverBuf[0] == '9' && serverBuf[1] == '9'){
            printf("Invalid key!\n");
            exit(2);
        }
        printf(">");
        Fgets(choice, MAXLINE, stdin);
        int i = 0;
        char intial[MAXLINE];
        while (choice[i] != ' ' && choice[i] != '\n'){
            intial[i] = choice[i];
            i++;
        }
        intial[i] = '\0';
    
        if (strcmp(intial, "quit") == 0){
            exit(0);
        }
        else if (strcmp(intial, "cput") == 0){
            if (choice[i] == '\n'){
                printf("Invalid command: Needs a filename!\n");
            }
            else{
                buf[0] = '1';
                buf[1] = '\0';
                buf[2] = '\0';
                buf[3] = '\0';
                int fi = 0;
                i++;
                while (choice[i] != '\n'){
                    filename[fi] = choice[i];
                    i++; fi++;
                }
                filename[fi] = '\0';
                FILE *get;
                get = fopen(filename, "r");
                if (get != NULL){
                    fseek(get, 0L, SEEK_END);
                    unsigned int size = ftell(get);
                    rewind(get);
                    char filecontents[MAXLINE];
                    Fread(&filecontents, sizeof(filecontents), 1, get);
                    memcpy(&buf[8], filename, 80);
                    memcpy(&buf[87], &size, 4);
                    memcpy(&buf[92], filecontents, size);
                }
                else{
                    printf("Error: No file found!\n");
                    buf[0] = '\0';
                    buf[1] = '\0';
                    buf[2] = '\0';
                    buf[3] = '\0';
                }
                fclose(get);
            }
        }
        else if (strcmp(intial, "cget") == 0){
            if (choice[i] == '\n'){
                printf("Invalid command: Needs a filename!\n");
            }
            else{
                buf[0] = '2';
                buf[1] = '\0';
                buf[2] = '\0';
                buf[3] = '\0';
                int fi = 0;
                i++;
                while (choice[i] != '\n'){
                    filename[fi] = choice[i];
                    i++; fi++;
                }
                filename[fi] = '\0';
                memcpy(&buf[8], &filename, 80);
                
            }
        }
        else if (strcmp(intial, "cdelete") == 0){
            if (choice[i] == '\n'){
                printf("Invalid command: Needs a filename!\n");
            }
            else {
                buf[0] = '3';
                buf[1] = '\0';
                buf[2] = '\0';
                buf[3] = '\0';
                int fi = 0;
                i++;
                while (choice[i] != '\n'){
                    filename[fi] = choice[i];
                    i++; fi++;
                }
                filename[fi] = '\0';
                memcpy(&buf[8], &filename, 80);
            }
        }
        else if (strcmp(intial, "clist") == 0){
            buf[0] = '4';
            buf[1] = '\0';
            buf[2] = '\0';
            buf[3] = '\0';
        }
        else{
            printf("Invalid command!\n");
            buf[0] = '\0';
            buf[1] = '\0';
            buf[2] = '\0';
            buf[3] = '\0';
            test = 1;
        }
        
        clientfd = Open_clientfd(host, port);
        Rio_readinitb(&rio, clientfd);
        Rio_writen(clientfd, buf, sizeof(buf));
        Rio_readnb(&rio, serverBuf, sizeof(serverBuf));
        Close(clientfd); //line:netp:echoclient:close
        int returned;
        memcpy(&returned, &serverBuf[0], 1);
        if (returned == 0xFF && test != 1 && buf[0] == '1'){
            printf("Error with file upload!\n");
        }
        else if (returned == 0xFF && test != 1 && buf[0] == '2'){
            printf("Error with file retrieval!\n");
        }
        if (serverBuf[0] == '0' && buf[0] == '1'){
            printf("File successfully uploaded!\n");
        }
        if (serverBuf[0] == '0' && buf[0] == '2'){
            int size;
            memcpy(&size, &serverBuf[4], 4);
            char data[MAXLINE];
            memcpy(&data, &serverBuf[8], size);
            FILE *write;
            write = fopen(filename, "w+");
            fputs(data, write);
            fclose(write);
            printf("%s is now in this directory!\n", filename);
        }
        if (serverBuf[0] == '0' && buf[0] == '3'){
            printf("%s has been deleted!\n", filename);
        }
        if (serverBuf[0] == '0' && buf[0] == '4'){
            int entries = 0;
            memcpy(&entries, &serverBuf[4], 4);
            int i = 0;
            while (entries != i){
                char file[80];
                memcpy(file, &serverBuf[i*80+8], 80);
                printf("%s\n", file);
                i++;
            }
        }
    }
}

