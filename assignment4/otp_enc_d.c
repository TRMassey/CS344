/**************************************************
* Author: Tara Massey
* File: otp_enc_d.c
* Citation: Code for sockets struct sockaddr_in
* from Brewster network demo
**************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

// error message function
void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int newsockfd;
    int status;
    char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    int size = 5000;
    char buffer[size];
    char toEncrypt[size];
    char key[size];
    int sockfd, nextSocketfd, portno;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    int n, i, j;
    int val = 1;
    pid_t pid;

    // check arguments
    if (argc != 2) {
        fprintf(stderr,"Usage: %s port\n", argv[0]);
        exit(1);
    }


    // set socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket.\n");
    }

    // fill in the struct
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        error("ERROR on binding.\n");
    }

    // reuse the socket
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));

    // Listen for up to five
    listen(sockfd, 5);

    // while listen loop is going on
    while (1) {

        // accepted
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            error("ERROR on accept.\n");
        }

        // Per instructions, fork after connection
        pid = fork();

        // check for errors
        if (pid < 0) {
            error("ERROR forking");
            exit(1);
        }

        // no errors, child
        if(pid == 0){


            //read message
            bzero(buffer, size);
            n = read(newsockfd, buffer, size);
            if (n < 0) {
                error("ERROR reading from socket.\n");
            }

            // open
            FILE *fp;
            fp = fopen(buffer, "r");
            if (fp == NULL) {
                exit(1);
            }

            // store
            char toEncrypt[size];
            fgets(toEncrypt, size, fp);
            for (i = 0; i < strlen(toEncrypt); i++) {
                toEncrypt[i] = toupper(toEncrypt[i]);
            }
            fclose(fp);

            //read the key
            bzero(buffer, size);
            n = read(newsockfd, buffer, size);
            if (n < 0) {
                error("ERROR reading from socket.\n");
            }

            // open
            fp = fopen(buffer, "r");
            if (fp == NULL) {
                error("ERROR opening key file.\n");
            }

            // store
            char key[size];
            fgets(key, size, fp);
            fclose(fp);

            // encrypt
            char done[strlen(toEncrypt)];
            int num;    // position in alphabet for message
            int num2;   // position in alphabet for key
            char letter;
            int newNum;

            // for the length of the message, encrypt
            for(i = 0; i < strlen(toEncrypt); i++) {

                // message
                for(j = 0; j < strlen(alpha); j++){
                    if(toEncrypt[i] == alpha[j]){
                        num = j;
                    }
                }

                // key
                for(j = 0; j < strlen(alpha); j++){
                    if(key[i] == alpha[j]){
                        num2 = j;
                    }
                }

                // new char and store
                letter = (num + num2) % 27;
                done[i] = alpha[letter];
            }
            done[strlen(toEncrypt) -1] = '\0';

            // send response
            bzero(buffer, size);
            snprintf(buffer, size, "%s", done);
            n = write(newsockfd, buffer, strlen(buffer));
            if (n < 0) {
                error("ERROR writing to socket.\n");
            }
        } // end child
        close(newsockfd);
    }// end while

    // parent
    close(sockfd);


    return 0;
}
