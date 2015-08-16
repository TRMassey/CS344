/*****************************************************************************************
 * Author: Tara Massey
 * Assignment: Assignment 2
 * File: masseyta.adventure.c
 * Description: Word based maze game with the following components:
 * 	- 10 rooms are hard coded. Seven rooms are randomly selected, with 3 - 6 randomly
 * 	selected connections. If a room has a path, there is a path back.
 * 	- When the exit is found, the game automatically ends
 * 	- Formatting for output follows teacher's specifications
 * 	- Rooms are written to a file in the masseyta.rooms.[pid] directory.
 * 	- Room information is read from the file, and used for the game
 * *************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>


// Declarations
struct Room{
    char name[20];
    char type[20];
    int connectNums;
    char connect1[20];
    char connect2[20];
    char connect3[20];
    char connect4[20];
    char connect5[20];
    char connect6[20];
};

// Steps in the game
void createDir(char newDirName[]);                // create directectory
void createMaze(char newDirName[], char curFile[], char startMaze[]);               // generate the random rooms
void readMaze(char curFile[], int *start, char startMaze[]);


int main(){
    int start = 0;
    char curFile[100];
    char startMaze[20];
    char *newDirName = malloc(sizeof(char) * 50);



    /* for randomization. Only call once per program */
    srand(time(NULL));

    // creating directory
    createDir(newDirName);

    // Create the random maze
    createMaze(newDirName, curFile, startMaze);

    // Read maze files until user is at end room
    readMaze(curFile, &start, startMaze);


    // You beat it. Woot.
    return 0;
}






/********************************************************************************
*           Create Directories
* void createDir(char *newDirName)
* Purpose: Create the directory for the gameplay with aunique pid
* Entry: Pointer to an array that serves as directory for game
* Exit: Array has unique directory
********************************************************************************/
void createDir(char *newDirName){

    /* Create the directory */
    int buffer= 50;

    // get the two pieces of info
    int dirPid= getpid();
    char *dirName= malloc(sizeof(char) * 15);
    strcpy(dirName, "masseyta.rooms.");

    /* combine the two pieces of info into one and store in newDirName */
    snprintf(newDirName, buffer, "%s%d", dirName, dirPid);
}




/**********************************************************************************
*           Create Rooms
* void createMaze(char *newDirName, char curFile[], char startMaze[])
* Purpose: Randomizes a new maze for a game. Writes the maze to appropraite files.
* Entry: Directory name, file path, and an array to hold the name of the first room
* in the randomized sequence
* Exit: the startMaze array has the name of the first randomized maze element. The
* maze is written to files in the appropriate directory.
**********************************************************************************/
void createMaze(char *newDirName, char curFile[], char startMaze[]){

    // position placeholders
    int j;
    int i;
    int k;
    int num;

    /**********************************
    *   Create the base list
    *   ******************************/
    char *mazeRooms[10];    	       // file names
    struct Room randomRooms[7];       // random array for connection
    
    mazeRooms[0]="One";
    mazeRooms[1]= "Two";
    mazeRooms[2]= "Three";
    mazeRooms[3]= "Four";
    mazeRooms[4]= "Five";
    mazeRooms[5]= "Six";
    mazeRooms[6]= "Seven";
    mazeRooms[7]= "Eight";
    mazeRooms[8]= "Nine";
    mazeRooms[9]= "Ten";

  

     /**********************************************
     * Randomize the rooms for a fresh maze each time
     * **********************************************/
    int found = 0;
    j=0;
    while(j < 7){
        num= rand() % 10; 

	// check to see if the current room is already taken
	k=0;
	while(k < j){
		if(strcmp(randomRooms[k].name, mazeRooms[num])== 0){
			found = 1;
		}
		k++;					// not found yet, so keep looking	        	
	}

	// room not already taken, so store it in random room array
	if(found != 1){
		strcpy(randomRooms[j].name, mazeRooms[num]);
		randomRooms[j].connectNums=0;
		if(j == 0){
			strcpy(startMaze, mazeRooms[num]);
		}
		j++;
	}
	found = 0;
    }
   

    /***********************************************
    * Save the name, type, and connections for each
    * room in the random play. Save to arry and
    * write to file in the pid unique directory
    * *******************************************/

    // make the directory and permissions
     strcpy(curFile, newDirName);
     mkdir(newDirName, 0770);
     chmod(newDirName, 0770);
     opendir(newDirName);

    // Write name, type, and connections to file
    for(i = 0; i < 7; i++){

        // create the file of the room at position i in the array
        strcpy(curFile, newDirName);
        strcat(curFile, "/");   
        strcat(curFile, randomRooms[i].name);

      
        FILE *fin = fopen(curFile, "w");
      

	// WRITE NAME
        fprintf(fin, "ROOM NAME: %s\n", randomRooms[i].name);


   
	// BUILD CONNECTIONS
	int found = 0;
    	k=0; 

	// build minimum three connections, max six, no duplicates allowed
	while(randomRooms[i].connectNums < 3){
        	num= rand() % 7;
		if(strcmp(randomRooms[i].connect1, randomRooms[num].name) == 0){
			found = 1;
		}		
		if(strcmp(randomRooms[i].connect2, randomRooms[num].name) == 0){
			found = 1;
		}
		if(strcmp(randomRooms[i].connect3, randomRooms[num].name) == 0){
			found = 1;
		}
		if(strcmp(randomRooms[i].connect4, randomRooms[num].name) == 0){
			found = 1;
		}		
		if(strcmp(randomRooms[i].connect5, randomRooms[num].name) == 0){
			found = 1;
		}
		if(strcmp(randomRooms[i].connect6, randomRooms[num].name) == 0){
			found = 1;
		}
		// check to make sure it's not connected to itself
		if(strcmp(randomRooms[i].name, randomRooms[num].name)==0){
			found = 1;
		}

		// check to make sure there is still connection room avaiilable w/out overwrite
		if(randomRooms[i].connectNums == 6){
			found = 1;
		}
		if(randomRooms[num].connectNums == 6){
			found = 1;
		}
				// if room is new and there is room, store it appropriately
		if(found != 1){
				// store in the appropraite connection
				if(randomRooms[i].connectNums == 0){
					strcpy(randomRooms[i].connect1, randomRooms[num].name);
				}	

				else if(randomRooms[i].connectNums == 1){
					strcpy(randomRooms[i].connect2, randomRooms[num].name);
				}
				
				else if(randomRooms[i].connectNums == 2){
					strcpy(randomRooms[i].connect3, randomRooms[num].name);
				}
			
				else if(randomRooms[i].connectNums == 3){
					strcpy(randomRooms[i].connect4, randomRooms[num].name);
				}
			
				else if(randomRooms[i].connectNums == 4){
					strcpy(randomRooms[i].connect5, randomRooms[num].name);
				}
				else if(randomRooms[i].connectNums == 5){
					strcpy(randomRooms[i].connect6, randomRooms[num].name);
				}

				// connect back
				if(randomRooms[num].connectNums == 0){
					strcpy(randomRooms[num].connect1, randomRooms[i].name);
				}
				else if(randomRooms[num].connectNums == 1){
					strcpy(randomRooms[num].connect2, randomRooms[i].name);
				}
				else if(randomRooms[num].connectNums == 2){
					strcpy(randomRooms[num].connect3, randomRooms[i].name);
				}
				else if(randomRooms[num].connectNums == 3){
					strcpy(randomRooms[num].connect4, randomRooms[i].name);
				}
				else if(randomRooms[num].connectNums == 4){
					strcpy(randomRooms[num].connect5, randomRooms[i].name);
				}
				else if(randomRooms[num].connectNums == 5){
					strcpy(randomRooms[num].connect6, randomRooms[i].name);
				}
				
				// increase position and connecction amounts
				randomRooms[i].connectNums++;
				randomRooms[num].connectNums++;
				k++;
		}
		found=0;
	}


	// WRITE THE CONNECTIONS
	if(randomRooms[i].connectNums == 3){
		fprintf(fin, "CONNECTION1: %s\n", randomRooms[i].connect1);
		fprintf(fin, "CONNECTION2: %s\n", randomRooms[i].connect2);
		fprintf(fin, "CONNECTION3: %s\n", randomRooms[i].connect3);
	}
	if(randomRooms[i].connectNums == 4){
		fprintf(fin, "CONNECTION1: %s\n", randomRooms[i].connect1);
		fprintf(fin, "CONNECTION2: %s\n", randomRooms[i].connect2);
		fprintf(fin, "CONNECTION3: %s\n", randomRooms[i].connect3);
		fprintf(fin, "CONNECTION4: %s\n", randomRooms[i].connect4);

	}
	if(randomRooms[i].connectNums == 5){
		fprintf(fin, "CONNECTION1: %s\n", randomRooms[i].connect1);
		fprintf(fin, "CONNECTION2: %s\n", randomRooms[i].connect2);
		fprintf(fin, "CONNECTION3: %s\n", randomRooms[i].connect3);
		fprintf(fin, "CONNECTION4: %s\n", randomRooms[i].connect4);
		fprintf(fin, "CONNECTION5: %s\n", randomRooms[i].connect5);
	}
	if(randomRooms[i].connectNums == 6){
		fprintf(fin, "CONNECTION1: %s\n", randomRooms[i].connect1);
		fprintf(fin, "CONNECTION2: %s\n", randomRooms[i].connect2);
		fprintf(fin, "CONNECTION3: %s\n", randomRooms[i].connect3);
		fprintf(fin, "CONNECTION4: %s\n", randomRooms[i].connect4);
		fprintf(fin, "CONNECTION5: %s\n", randomRooms[i].connect5);
		fprintf(fin, "CONNECTION6: %s\n", randomRooms[i].connect6);
	}

	
        
	//  WRITE TYPE. Already randomized.
        if(i == 0){					// first room in random array
            fprintf(fin, "ROOM TYPE: %s\n", "START");
            strcpy(randomRooms[i].type, "START");
        } 	
        else if(i == 6){				// last room in random array
            fprintf(fin, "ROOM TYPE: %s\n", "END");
      	    strcpy(randomRooms[i].type,"END");
        }
        else if(i != 0 && i !=6){			// all others
            fprintf(fin, "ROOM TYPE: %s\n", "MID-ROOM");
	    strcpy(randomRooms[i].type,"MID-ROOM");
        }
 


	fclose(fin);
    }
    closedir(newDirName);
}


/**************************************************************************************
 * 			Read the Maze
 * void readMaze(char curFile[], int *start, char startMaze[]
 * Purpose: Game loop. Reads from current room file and outputs information about the
 * room to the user.
 * Entry: Current directory, a boolean indicator if this is the start of the maze or
 * not, and the name of the first room in the maze if it is the start.
 * Exit: Loops reading rooms until the room type is determine dto be "exit." Winning
 * message displayed including steps and path.
 * Additional User Input: User inputs next room/file to be read. User notified if it
 * is not a valid choice.
 * ***********************************************************************************/
void readMaze(char curFile[], int *start, char startMaze[]){
	char input[50];							// user room choice
	char buffer[100];						// read from FILE
	char roomFile[100];						// name of file w/directory
	char smallBuffer[50];						// read line from FILE
	char roomName[20];						// store room name from FILE
	char *connections[6];						// store connections from FILE
	char *path[100];						// store path user takes. Per instructor, can be static array 100
	char connectName[20];						// store name from FILE
	char connectOne[20];						// store first connection from FILE
	char connectTwo[20];						// store second connection from FILE
	char connectThree[20];						// store third connection from FILE
	char connectFour[20];						// store fourth connection from FILE
	char connectFive[20];						// store fifth connection from FILE
	char connectSix[20];						// store sixth connection from FILE
	char endGame[50];						// store room type from FILE
	char *pch;							// breaking up a string	
	int pos;							// position
	int steps = 0;							// user steps, errors not included
	int count = 0;							// accumulator
	int i = 0;							// position
	int k = 0;							// position
	int j = 0;							// position
	int ch = 0;							// removing newlines
	int lines=0;							// lines in the file being read
	int wonGame =0;							// boolean - game over
	int moveOn = 0;							// boolean - valid or invalid
  
	
	/*******************************************************************
 	* 		Set up room names for FILE cocmparison
 	* *****************************************************************/
	char *rooms[10];
   
        rooms[0]="One";
        rooms[1]= "Two";
        rooms[2]= "Three";
        rooms[3]= "Four";
        rooms[4]= "Five";
        rooms[5]= "Six";
        rooms[6]= "Seven";
        rooms[7]= "Eight";
        rooms[8]= "Nine";
        rooms[9]= "Ten";


	do{
	
		/********************************************************
 		*		Set up the current game state
 		*******************************************************/
		lines = 0;
		count = 0;
		moveOn = 0;
		i = 0;

		// clear the old contents, update with new to make room file
		// Citation: cplusplus.com/reference/cstring/strchr
		// get the root directory with no additions		
		memset(&roomFile[0], 0, sizeof(roomFile));		// was simply adding to old name, so cleared it
		pch=strchr(curFile, '/');
		while(pch != NULL){
			pos= pch-curFile+1;
			break;
		}
        	strncpy(roomFile, curFile, pos);




		// if not a starting loop, validated user input will be the room name
		if(*start ==1){
			strcat(roomFile, input);
		}

		// if start game, the very first randomized room will be room name
		else{
    			strcat(roomFile, startMaze);
  		}


		/****************************************************
 		*	Information about Room from File
 		****************************************************/
		FILE* fout= fopen(roomFile, "r");
		if(fout == NULL){
			printf("THere was an error opening");
		}

		// get number of lines in file (will differ due to connection amount). Need for proper
		// formatting to match instructor's output
		// ciration: stackoverflow.com/questions/12733105/c-functions-that-counts-lines-in-file
		// author: Lundin, October 4, 2012
		while(!feof(fout)){
			ch = fgetc(fout);
			if (ch == '\n'){
				lines++;
			}
		}
	
		
		// get connections names only, format w/out space, and store
		// Will read by line so I can store and format properly to match teacher
		// rewind so I don't pull garbage next loop through. Seg faults suck.
		rewind(fout);
		while(fgets(smallBuffer, 50, fout)!= NULL){				// read from the FILE
			count++;
			if(count == 1){
				strcpy(connectName, smallBuffer+11);			// store name, newline doesn't matter
			}
			if(count > 1 && count < lines){
				if( count == 2){
					strcpy(buffer, smallBuffer+13);			// just connection name
					int strAmount = strlen(buffer);
					buffer[strAmount - 1] = 0;			// get rid of the newline
					strcpy(connectOne, buffer);
					connections[i]=connectOne;			// store
					i++;
				}
				if( count == 3){
					strcpy(buffer, smallBuffer+13);			// just connection name
					int strAmount = strlen(buffer);
					buffer[strAmount - 1] = 0;			// get rid of the newline
					strcpy(connectTwo, buffer);				
					connections[i]=connectTwo;			// store
					i++;
				}
				if( count == 4){
					strcpy(buffer, smallBuffer+13);			// just connection name
					int strAmount = strlen(buffer);
					buffer[strAmount - 1] = 0;			// get rid of the newline
					strcpy(connectThree, buffer);
					connections[i]=connectThree;			// store
					i++;
				}
				if( count == 5){
					strcpy(buffer, smallBuffer+13);			// just connection name
					int strAmount = strlen(buffer);
					buffer[strAmount - 1] = 0;			// get rid of the newline
					strcpy(connectFour, buffer);				
					connections[i]=connectFour;			// store
					i++;
				}
				if( count == 6){
					strcpy(buffer, smallBuffer+13);			// just connection name
					int strAmount = strlen(buffer);
					buffer[strAmount - 1] = 0;			// get rid of the newline
					strcpy(connectFive, buffer);
					connections[i]=connectFive;			// store
					i++;
				}
				if( count == 7){
					strcpy(buffer, smallBuffer+13);			// just connection name
					int strAmount = strlen(buffer);
					buffer[strAmount - 1] = 0;			// get rid of the newline
					strcpy(connectSix, buffer);
					connections[i]=connectSix;			// store
					i++;
				}
																
			}
			if(count == lines){
				strcpy(endGame, smallBuffer+11);			// store type, newline doesn't matter
			}
		}
		fclose(fout);
		fout = NULL;
		if(fout != NULL){
			printf("File close failed..");
		}
		
		/****************************************************************
                *	Interaction with User
                ***************************************************************/

		// User did NOT select the end room
		if(strcmp(endGame, "END\n") != 0){

			while(moveOn != 1){			
				// print room and connections
				printf("\nCURRENT LOCATION: %s", connectName);
				printf("POSSIBLE CONNECTIONS: ");

				i = 0;
				for(k= lines-3; k < lines; k++){
					if(k != lines-1){
						printf("%s, ", connections[i]);
						i++;
					}
					else{
						printf("%s.\n", connections[i]);
					}
				}

				// get user input for next room
				printf("WHERE TO? >");
				scanf("%s", input);

				// check for validity, and use choice to update FILE if valid
				i = 0;
				for(k=lines-3; k < lines; k++){
					if(strcmp(input, connections[i]) == 0){
						for(j=0; j <10; j++){
							if(strcmp(connections[i], rooms[j])==0){
								path[steps]=rooms[j];
								steps++;
							}
						}
						moveOn = 1;
						*start = 1;
					}
					i++;
				}
				if(moveOn == 0){
					printf("\n\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n ");
				}


			} // end while moveOn

		} //end if endGame != end
		else{
			printf("\n\nYOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
			printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", steps);
			for(i=0; i < steps; i++){
				printf("%s\n", path[i]);
			}
			return;
		}
	
	}while(wonGame != 1); // end of game loop
	
} // end of function



