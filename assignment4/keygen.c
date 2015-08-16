#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int i;
    int size;

    // random seed once
    srand(time(NULL));

    // check for correct input
    if( argc != 2){
        printf("Error: Incorrect number of arguments.");
        exit(1);
    }
    else{
        // make array of possible characters
        char alphabet[] =" ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        // make the array for the code
        size = atoi(argv[1]);
        char array[size];

        // fill the array
        for(i = 0; i < size; i++){
                array[i] = alphabet[rand() % 27];
        }

        array[size] = '\0';

        //print it
        printf("%s\n", array);
    }

return 0;
}
