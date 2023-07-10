/************************************
*Matricola --> VR457793
*Nome e cognome --> Vittorio Maria Stano
*Data di realizzazione --> 14/06/2023
*************************************/

/// @file serverFunction.c
/// @brief server functions implementation

#include "err_exit.h"
#include "function.h"
#include "server_function.h"

#include "semaphore.h"
#include "shared_memory.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

/// @brief check the server startup string
void serverCommandLineCheck(int argc, char *argv[]){
    printf(YELLOW "> command line correctness checking ... <\n" RESET);

    // -------------------------------------------------------------------------------------------------------------- //

    /* number of arguments check */
    if(argc != ARGC_SERVER) {
        errExit("> command line correctness failed <\n"
                " > invalid input string format <\n"
                " > format must contain 5 parameters as follow <\n"
                " > ./F4Server playingFieldRowSize playingFieldColumnSize player1Symbol player2Symbol <\n"
                " >            ------type int----- -------type int------- --type char-- --type char-- <\n");
    }

    // -------------------------------------------------------------------------------------------------------------- //



    // begin // -------------------------------------- // CHECK ROW // ------------------------------------- // begin //

    /* argv length variable declaration */
    int argvLength;

    /* playing field row size check */
    argvLength = strlen(argv[1]);

    if(((int) *argv[1] > 48 && (int) *argv[1] < 53) && argvLength == 1){
        errExit("> playing field row size must be at least 5 <\n");
    }

    for(int i = 0; i < argvLength; i++){
        if((int) *argv[1] < 48 || (int) *argv[1] > 57){
            errExit("> playing field row size must be int <\n");
        }
        argv[1]++;
    }

    /* argv[1] pointer restore */
    for(int i = argvLength; i > 0; i--)
        argv[1]--;

    // end // ---------------------------------------- // CHECK ROW // --------------------------------------- // end //



    // begin // ------------------------------------- // CHECK COLUMN // ----------------------------------- // begin //

    /* playing field column size check */
    argvLength = strlen(argv[2]);

    if(((int) *argv[2] > 48 && (int) *argv[2] < 53) && argvLength == 1){
        errExit("> playing field column size must be at least 5 <\n");
    }

    for(int i = 0; i < argvLength; i++){
        if((int) *argv[2] < 48 || (int) *argv[2] > 57){
            errExit("> playing field column size must be int <\n");
        }
        argv[2]++;
    }


    /* argv[2] pointer restore */
    for(int i = argvLength; i > 0; i--)
        argv[2]--;

    // end // --------------------------------------- // CHECK COLUMN // ------------------------------------- // end //



    // begin // ------------------------------------- // CHECK TOKENS // ----------------------------------- // begin //

    /* player 1 symbol check */
    argvLength = strlen(argv[3]);

    if(argvLength != 1){
        errExit("> player 1 symbol must be a char <\n");
    }


    /* player 2 symbol check */
    argvLength = strlen(argv[4]);

    if(argvLength != 1){
        errExit("> player 2 symbol must be a char <\n");
    }

    // end // --------------------------------------- // CHECK TOKENS // ------------------------------------- // end //

    printf(GREEN "> command line correctness checked successfully <\n" RESET);
}

/// @brief alert the user after the CTRL+C key is pressed
void sendWarning(int sig){
    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // BEGIN // ----------------------------------- /* SIGNAL MANAGEMENT */ -------------------------------- // BEGIN //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //

    int errnoValue;

    if(signal(SIGALRM, continueGame) == SIG_ERR){
        errnoValue = errno;
        errExit("> sigalrm failed \n<");
        errnoCheck(errnoValue);
    }

    if(signal(SIGINT, endGame) == SIG_ERR){
        errnoValue = errno;
        errExit("> sigint failed <\n");
        errnoCheck(errnoValue);
    }

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // END // ------------------------------------- /* SIGNAL MANAGEMENT */ ---------------------------------- // END //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //

    printf(RED "\n> if you press ctrl-c again the game will end <\n" RESET);
    printf(YELLOW "> detecting ctr+C ... <\n" RESET);

    // -------------------------------------------------------------------------------------------------------------- //

    // signalArray   ->   [0]SIGINT, [1]SIGUSR1, [2]SIGUSR2, [3]SIGALRM, [4]SIGTERM, [5]SIGABRT, [6]SIGHUP} //
    bool signalArray[] = {true, false, false, true, false, true, false};

    setSignalSet(signalArray);

    // -------------------------------------------------------------------------------------------------------------- //

    alarm(TIME_CTRL_C);

    // waiting for signal
    if(pause() == -1){
        // ////////////////////////////////////////////////////////////////////////////////////////////////////////// //
        // BEGIN // ------------------------------ /* RESET SIGNAL MANAGEMENT */ --------------------------- // BEGIN //
        // /////////////////////////////////////////////////////////////////////////////////////////////////////////  //

        if (signal(SIGALRM, trackSignal) == SIG_ERR) {
            errnoValue = errno;
            printf("> SIGALRM failed <\n");
            errnoCheck(errnoValue);
        }

        // set
        if(signal(SIGINT, sendWarning) == SIG_ERR){
            errExit("> sigint failed <\n");
        }

        // ////////////////////////////////////////////////////////////////////////////////////////////////////////// //
        // END // -------------------------------- /* RESET SIGNAL MANAGEMENT */ ----------------------------- // END //
        // /////////////////////////////////////////////////////////////////////////////////////////////////////////  //
    }
}

/// @brief continues the game after the time is up
void continueGame(int sig) {
    printf(GREEN "> ctr+C not detected game will continue <\n" RESET);
}

/// @brief ends the game after a double CTRL+C press
void endGame(int sig) {
    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // BEGIN // ------------------------------------ /* SERVER CLOSURE */ ---------------------------------- // BEGIN //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //

    printf(BOLDRED "\n> game ended <\n" RESET);
    printf(BOLDYELLOW "\n> server closure ... <\n" RESET);

    int errnoValue;

    if (searchFile(COUNTDOWN) == 0) {
        if (kill(readPid(COUNTDOWN), 0) == 0) {
            if (kill(readPid(COUNTDOWN), SIGKILL) == -1) {
                errnoValue = errno;
                printf(BOLDRED "> kill failed <\n" RESET);
                errnoCheck(errnoValue);
            }
        }
    }

    if (searchFile(READ_TOKEN) == 0) {
        if (kill(readPid(READ_TOKEN), 0) == 0) {
            if (kill(readPid(READ_TOKEN), SIGKILL) == -1) {
                errnoValue = errno;
                printf(BOLDRED "> kill failed <\n" RESET);
                errnoCheck(errnoValue);
            }
        }
    }

    if(kill(pidClient1, 0) == 0) {
        if (kill(pidClient1, SIGABRT) == -1) {
            errnoValue = errno;
            printf(BOLDRED "> kill failed <\n" RESET);
            errnoCheck(errnoValue);
        }
    }

    if(kill(pidClient2, 0) == 0) {
        if (kill(pidClient2, SIGABRT) == -1) {
            errnoValue = errno;
            printf(BOLDRED "> kill failed <\n" RESET);
            errnoCheck(errnoValue);
        }
    }

    // remove fifo
    removeFile(FIFO);

    // remove shared memory
    removeSharedMemory(parameter.shmid);

    // remove semaphore
    removeSemaphoreSet(parameter.semid);

    // remove file
    removeFile(COUNTDOWN);
    removeFile(READ_TOKEN);

    // remove sentinel file
    removeFile(F4SERVER);
    removeFile(F4CLIENT1);
    removeFile(F4CLIENT2);

    printf(BOLDGREEN "\n> server closed successfully <\n" RESET);

    exit(0);

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // END // -------------------------------------- /* SERVER CLOSURE */ ------------------------------------ // END //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //
}

/// @brief check if a player won the game
winIndex winCheck(char playerToken, char(*playingField)[parameter.column]){
    int counter;

    // checking row
    for(int r = 0; r < parameter.row; r++) {
        counter = 0;
        for (int c = 0; c < parameter.column; c++) {
            if (playingField[r][c] == playerToken) {
                cell.row[counter] = r;
                cell.column[counter] = c;
                counter++;
                if (counter == 4) {
                    cell.returnValue = 1;
                    return cell;
                }
            }else {
                counter = 0;
            }
        }
    }

    // checking column
    for (int c = 0; c < parameter.column; c++) {
        counter = 0;
        for(int r = 0; r < parameter.row; r++) {
            if (playingField[r][c] == playerToken) {
                cell.row[counter] = r;
                cell.column[counter] = c;
                counter++;
                if (counter == 4) {
                    cell.returnValue = 1;
                    return cell;
                }
            }else {
                counter = 0;
            }
        }
    }

    // checking diagonal sx to dx
    for (int r = 0; r <= parameter.row-4; r++) {
        for(int c = 0; c <= parameter.column-4; c++) {
            counter = 0;
            while(counter < 4 && playingField[r+counter][c+counter] == playerToken) {
                cell.row[counter] = r+counter;
                cell.column[counter] = c+counter;
                counter ++;
            }
            if (counter == 4) {
                cell.returnValue = 1;
                return cell;
            }
        }
    }

    // checking diagonal dx to sx
    for (int r = 0; r <= parameter.row-4; r++) {
        for(int c = parameter.column-1; c > 2; c--) {
            counter = 0;
            while (counter < 4 && playingField[r+counter][c-counter] == playerToken) {
                cell.row[counter] = r+counter;
                cell.column[counter] = c-counter;
                counter++;
            }
            if (counter == 4) {
                cell.returnValue = 1;
                return cell;
            }
        }
    }

    // checking full playing field
    counter = 0;

    for (int c = 0; c < parameter.column; c++) {
        if(playingField[0][c] == ' ') {
            counter ++;
        }
    }

    if(counter == 0) {
        cell.returnValue = -1;
        return cell;
    }

    cell.returnValue = 0;
    return cell;
}

/// @brief used to set the signal received with SIGALRM
void trackSignal(int sig){
    receivedSignal = SIGALRM;
}

/// @brief remove a specific file
void removeFile(char *name){
    int errnoValue;

    if(searchFile(name) == 0) {
        printf(YELLOW "> %s removal ... <\n" RESET, name);

        if (unlink(name) == -1) {
            errnoValue = errno;
            printf(BOLDRED "> %s removal failed <\n" RESET, name);
            errnoCheck(errnoValue);
        }
        printf(GREEN "> %s removed successfully <\n" RESET, name);
    }
}

/// @brief displays the win playing field
void showWinPlayingField(char (*playingField)[parameter.column], winIndex cell){
    for(int r = 0; r < parameter.row; r++){
        printf("\n");
        for(int c = 0; c < parameter.column; c++){
            if( cell.returnValue == 1 &&
                ((r == cell.row[0] && c == cell.column[0]) ||
                 (r == cell.row[1] && c == cell.column[1]) ||
                 (r == cell.row[2] && c == cell.column[2]) ||
                 (r == cell.row[3] && c == cell.column[3]))
                    ){
                printf("| "
                       BOLDGREEN  "%c"
                       RESET  " |", playingField[r][c]);
            }else {
                printf("| %c |", playingField[r][c]);
            }
        }
        printf("\n");
        for(int s = 0; s < parameter.column * 5; s++){
            printf("-");
        }
    }

    printf("\n");

    for(int c = 0; c < parameter.column; c++){
        printf("  %d  ", c+1);
    }

    printf("\n\n");
}

/// @brief game termination after the player leaves
void clientClosure(int sig){
    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // BEGIN // ------------------------------------ /* SERVER CLOSURE */ ---------------------------------- // BEGIN //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //

    int errnoValue;

    printf(BOLDYELLOW"\n> player abandoned <\n"RESET);
    printf(BOLDYELLOW "\n> server closure ... <\n\n" RESET);

    if (searchFile(COUNTDOWN) == 0) {
        if (kill(readPid(COUNTDOWN), 0) == 0) {
            if (kill(readPid(COUNTDOWN), SIGKILL) == -1) {
                errnoValue = errno;
                printf(BOLDRED "> kill failed <\n" RESET);
                errnoCheck(errnoValue);
            }
        }
    }

    if(searchFile(READ_TOKEN) == 0) {
        if(kill(readPid(READ_TOKEN), 0) == 0) {
            if (kill(readPid(READ_TOKEN), SIGKILL) == -1) {
                errnoValue = errno;
                printf(BOLDRED "> kill failed <\n" RESET);
                errnoCheck(errnoValue);
            }
        }
    }

    if(kill(pidClient1, 0) == 0) {
        if (kill(pidClient1, SIGHUP) == -1) {
            errnoValue = errno;
            printf(BOLDRED "> kill failed <\n" RESET);
            errnoCheck(errnoValue);
        }
    }


    if(kill(pidClient2, 0) == 0) {
        if (kill(pidClient2, SIGHUP) == -1) {
            errnoValue = errno;
            printf(BOLDRED "> kill failed <\n" RESET);
            errnoCheck(errnoValue);
        }
    }

    // remove fifo
    removeFile(FIFO);

    // remove shared memory
    removeSharedMemory(parameter.shmid);

    // remove semaphore
    removeSemaphoreSet(parameter.semid);

    // remove file
    removeFile(COUNTDOWN);
    removeFile(READ_TOKEN);

    // remove sentinel file
    removeFile(F4SERVER);
    removeFile(F4CLIENT1);
    removeFile(F4CLIENT2);

    printf(BOLDGREEN "\n> server closed successfully <\n" RESET);

    exit(0);

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // END // -------------------------------------- /* SERVER CLOSURE */ ------------------------------------ // END //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //
}
