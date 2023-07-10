/************************************
*Matricola --> VR457793
*Nome e cognome --> Vittorio Maria Stano
*Data di realizzazione --> 14/06/2023
*************************************/

/// @file client_function.c
/// @brief client functions implementation

#include "function.h"
#include "client_function.h"

#include "err_exit.h"

#include <dirent.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/// @brief check the client startup string
int clientCommandLineCheck(int argc, char *argv[]){
    printf(YELLOW "> command line correctness checking ... <\n" RESET);

    // -------------------------------------------------------------------------------------------------------------- //

    if (argc == 1) {
        errExit("> invalid input string format <\n"
                "> format must contain 2 parameters for human player as follow <\n"
                "> ./F4Client userName  <\n"
                "> or <\n"
                "> format must contain 3 parameters for automatic player as follow <\n"
                "> ./F4Client playerName *<\n");
    }

    // -------------------------------------------------------------------------------------------------------------- //



    // -------------------------------------------------------------------------------------------------------------- //

    if(argc == 2){
        printf(GREEN "> command line correctness checked successfully <\n" RESET);
        return 0;
    }

    // -------------------------------------------------------------------------------------------------------------- //



    // -------------------------------------------------------------------------------------------------------------- //

    if (argc > 2) {

        // ---------------------------------------------------------------------------------------------------------- //
        int errnoValue;

        DIR *directory;
        struct dirent *directoryPath;

        char cwd[1024];

        getcwd(cwd, sizeof(cwd));

        if ((directory = opendir(cwd)) == NULL) {
            errnoValue = errno;
            printf(BOLDRED "open failed" RESET);
            errnoCheck(errnoValue);
        }

        int nFile = 0;

        while ((directoryPath = readdir(directory)) != NULL) {
            nFile ++;
        }

        if (closedir(directory) == -1) {
            errnoValue = errno;
            printf(BOLDRED "open failed" RESET);
            errnoCheck(errnoValue);
        }
        // ---------------------------------------------------------------------------------------------------------- //

        if(argc-nFile > 2)
            errExit("> invalid input string format <\n"
                    "> format must contain 2 parameters for human player as follow <\n"
                    "> ./F4Client userName  <\n"
                    "> or <\n"
                    "> format must contain 3 parameters for automatic player as follow <\n"
                    "> ./F4Client playerName *<\n");
    }

    // -------------------------------------------------------------------------------------------------------------- //

    printf(GREEN "> command line correctness checked successfully <\n" RESET);
    return 1;
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

    if(signal(SIGINT, youAbandoned) == SIG_ERR){
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
    bool signalArray[] = {true, true, true, true, true, true, true};

    setSignalSet(signalArray);

    // -------------------------------------------------------------------------------------------------------------- //

    alarm(TIME_CTRL_C);

    // waiting for signal
    if(pause() == -1){
        // ////////////////////////////////////////////////////////////////////////////////////////////////////////// //
        // BEGIN // ------------------------------ /* RESET SIGNAL MANAGEMENT */ --------------------------- // BEGIN //
        // /////////////////////////////////////////////////////////////////////////////////////////////////////////  //

        if (signal(SIGALRM, nothing) == SIG_ERR) {
            errnoValue = errno;
            printf("> SIGALRM failed <\n");
            errnoCheck(errnoValue);
        }

        if(signal(SIGINT, sendWarning) == SIG_ERR){
            errnoValue = errno;
            errExit("> sigint failed <\n");
            errnoCheck(errnoValue);
        }

        // ////////////////////////////////////////////////////////////////////////////////////////////////////////// //
        // END // -------------------------------- /* RESET SIGNAL MANAGEMENT */ ----------------------------- // END //
        // /////////////////////////////////////////////////////////////////////////////////////////////////////////  //
    }
}

/// @brief continues the game after the time is up
void continueGame(int sig) {
    printf(GREEN "> ctr+C not detected game will continue <\n" RESET);
    printf("> Insert column < ");
    fflush(stdout);
}

/// @brief inserts a token into playing field
int insertToken(char (*playingField)[parameter.column]){
    printf("> Insert column: ");
    fflush(stdout);

    int errnoValue;
    int playerColumn;

    pipeInfo infoPipe;

    for(int i = 0; i < 100; i++){
        infoPipe.cPlayerColumn[i] = '\0';
    }


    // begin // ---------------------------------- // CREATE PIPE // --------------------------------------- // begin //

    int fd[2];
    ssize_t nBytes;

    if(pipe(fd) == -1) {
        errnoValue = errno;
        printf("> pipe creation failed <\n");
        errnoCheck(errnoValue);
    }

    // end // ------------------------------------ // CREATE PIPE // ----------------------------------------- // end //



    // ----------------------------------------------- / READ TOKEN / ------------------------------------------------//
    pid_t readToken = fork();

    if(readToken == -1){
        errnoValue = errno;
        printf(RED "> fork failed <\n" RESET);
    }

    if(readToken == 0){

        // ---------------------------------------------------------------------------------------------------------- //

        // signalArray   ->   [0]SIGINT, [1]SIGUSR1, [2]SIGUSR2, [3]SIGALRM, [4]SIGTERM, [5]SIGABRT, [6]SIGHUP} //
        bool signalArray[] = {false, false, false, false, false, false, false};

        setSignalSet(signalArray);

        // ---------------------------------------------------------------------------------------------------------- //


        writePid(READ_TOKEN, getpid());


        // ------------------------------------------- // READ COLUMN // ---------------------------------------------//

        if(read(0, &infoPipe.cPlayerColumn, sizeof(infoPipe.cPlayerColumn)) == -1){
            errnoValue = errno;
            printf("> read failed <\n");
            errnoCheck(errnoValue);
        }

        // ------------------------------------------- // READ COLUMN // ---------------------------------------------//


        infoPipe.readCheck = true;


        // begin // -------------------------------- // WRITE PIPE // -------------------------------------- // begin //

        if(close(fd[0]) == -1){
            errnoValue = errno;
            printf("> pipe close failed <\n");
            errnoCheck(errnoValue);
        }

        nBytes = write(fd[1], &infoPipe, sizeof(infoPipe));

        if(nBytes != sizeof(infoPipe)){
            errnoValue = errno;
            printf("> pipe close failed <\n");
            errnoCheck(errnoValue);
        }

        if(close(fd[1]) == -1){
            errnoValue = errno;
            printf("> pipe close failed <\n");
            errnoCheck(errnoValue);
        }

        // end // ---------------------------------- // WRITE PIPE // ---------------------------------------- // end //

        exit(0);
        // --------------------------------------------- / READ TOKEN / ----------------------------------------------//
    }else{
        waitpid(readToken, NULL, WUNTRACED | WCONTINUED);


        // begin // -------------------------------- // READ PIPE // --------------------------------------- // begin //

        if(close(fd[1]) == -1){
            errnoValue = errno;
            printf("> pipe close failed <\n");
            errnoCheck(errnoValue);
        }

        nBytes = read(fd[0], &infoPipe, sizeof(infoPipe));

        if(nBytes > 0){

        }else{
            infoPipe.readCheck = false;
        }

        if(close(fd[0]) == -1){
            errnoValue = errno;
            printf("> pipe close failed <\n");
            errnoCheck(errnoValue);
        }

        // end // ----------------------------------- // READ PIPE // ---------------------------------------- // end //
    }



    if(infoPipe.readCheck == false){
        return -1;
    }

    int len = strlen(infoPipe.cPlayerColumn);

    for(int index = 0; index < len-1; index ++){
        if((int) infoPipe.cPlayerColumn[index] < 48 || (int) infoPipe.cPlayerColumn[index] > 57) {
            printf("\n> type must be int <\n");
            return 0;
        }
    }

    playerColumn = atoi(infoPipe.cPlayerColumn);

    if (playerColumn < 1 || playerColumn > parameter.column) {
        printf("\n> column does not exist <\n");
        return 0;
    }

    for (int r = 0; r < parameter.row; r++) {
        if (playingField[parameter.row - r - 1][playerColumn-1] == ' ') {
            playingField[parameter.row - r - 1][playerColumn-1] = parameter.playerToken;
            return 1;
        }
    }

    printf("\n> column is full <\n");
    return 0;
}

/// @brief displays playing field
void showPlayingField(char (*playingField)[parameter.column]){
    for(int r = 0; r < parameter.row; r++){
        printf("\n");
        for(int c = 0; c < parameter.column; c++){
            printf("| %c |", playingField[r][c]);
        }
        printf("\n");
        for(int s = 0; s < parameter.column * 5; s++){
            printf("-");
        }
    }

    printf("\n");

    for(int c = 0; c < parameter.column; c++){
        if(c < 10) {
            printf("  %d  ", c + 1);
        }
        else{
            printf(" %d  ", c + 1);
        }
    }

    printf("\n\n");
}

/// @brief alerts the opponent's abandonment
void opponentAbandoned(int sig){ //clientWin
    printf(BOLDGREEN "\n> opponent abandoned >>> you win ! <\n" RESET);
    printf(BOLDYELLOW "\n> client closure ... <\n" RESET);
    printf(BOLDGREEN "\n> client closed successfully <\n" RESET);

    exit(0);
}

/// @brief warns loss by abandonment
void youAbandoned(int sig){
    int errnoValue;

    printf(BOLDRED "\n> game abandoned >>> you lose ! <\n" RESET);

    printf(BOLDYELLOW "\n> client closure ... <\n" RESET);

    if(kill(parameter.pid, 0) == 0) {
        if (kill(parameter.pid, SIGABRT) == -1) {
            errnoValue = errno;
            printf(BOLDRED "> kill failed <\n" RESET);
            errnoCheck(errnoValue);
        }
    }

    printf(BOLDGREEN "\n> client closed successfully <\n" RESET);

    exit(0);
}

/// @brief terminates the client when the server sends a close signal
void serverClosure(int sig){
    printf(BOLDYELLOW "\n> external closure from server <\n" RESET);
    printf(BOLDYELLOW "\n> nobody won <\n" RESET);

    printf(BOLDYELLOW "\n> client closure ... <\n" RESET);
    printf(BOLDGREEN "\n> client closed successfully <\n" RESET);

    exit(0);
}

/// @brief used to set the signal received with SIGUSR2
void trackSignal(int sig){
    receivedSignal = SIGUSR2;
}

/// @brief notifies the client win
void clientWin(int sig){
    printf(BOLDGREEN "> you win ! <\n" RESET);

    exit(0);
}

/// @brief notifies client loss
void clientLose(int sig){
    printf(BOLDRED "> you lose ! <\n" RESET);

    exit(0);
}