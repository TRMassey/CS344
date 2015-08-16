/**************************************************
* Author: Tara Massey
* File: otp_dec.c
* Citation: Code for sockets struct sockaddr_in
* from Brewster network demo
**************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


// error message function
void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int sockfd;
    int portno;
    int strSize;
    int size = 5000;
    int n;
    int i;
    int val = 1;
    int encryptSize;
    char toEncrypt[size];           // for arg 1
    char key[size];                 // for arg 2
    char buffer[size];              // for reading and writing

    // check for correct input
    if (argc != 4) {
       fprintf(stderr,"Usage: %s plaintext key port\n", argv[0]);
       exit(1);
    }

    // passed, make sockets
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket.\n");
    }

    // reuse the socket
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));

    // fill in the struct
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[3]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // connect
    if (connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("ERROR otp_dec cannot find otp_dec_d\n");
        exit(2);
    }

    //Working with argument one:
    // check command line for bad input
    snprintf(buffer, size, "%s", argv[1]);
    for(i = 0; i < strlen(buffer) -1; i++){
        // check to see if it's correct characters
        if(!(isalpha(buffer[i]))){

        //if not alpha, check for space, cause space is allowed
            if(buffer[i] != 32){
                error("ERROR : input contains bad characters");
                exit(1);
            }
        }
    }

    // now check to see if the inner contents are bad
    FILE *fp = fopen(argv[1], "r");
    if(fp == 0){
        error("ERROR ");  // will display message about bad file
        exit(1);
    }

    // store it in an array for processing
    fgets(toEncrypt, size, fp);
    encryptSize = strlen(toEncrypt);
    toEncrypt[encryptSize -1] = '\0';
    fclose(fp);



    // working with argument two:
    // check that it's valid
    fp = fopen(argv[2], "r");

    if(fp == 0){
        error("ERROR: key file doesn't exist");
    }

    // store it in an array for processing
    fgets(key, size, fp);
    strSize = strlen(key);
    key[strSize -1] = '\0';
    fclose(fp);


    // now writing:
    // passed all for valid input, so now write One
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        error("ERROR writing to socket.\n");
    }
    snprintf(buffer, size, "%s", argv[2]);

    // passed all, so now write two
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        error("ERROR writing to socket.\n");
    }

    // receive response
    bzero(buffer, size);
    n = read(sockfd, buffer, size);
    if (n < 0) {
        error("ERROR reading from socket.\n");
    }

    // print response and close
    printf("%s\n", buffer);
    close(sockfd);

    return 0;
}
