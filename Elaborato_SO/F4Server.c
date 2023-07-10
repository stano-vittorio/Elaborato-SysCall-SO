/************************************
*Matricola --> VR457793
*Nome e cognome --> Vittorio Maria Stano
*Data di realizzazione --> 14/06/2023
*************************************/

/// @file F4Server.c
/// @brief contains FORZA4 game server

#include "function.h"
#include "server_function.h"

#include "err_exit.h"

#include "fifo.h"
#include "semaphore.h"
#include "shared_memory.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // BEGIN // ---------------------------------- /* ALIVE SERVER CHECK */ -------------------------------- // BEGIN //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //

    if(searchFile(F4SERVER) == 0) {
        printf(BOLDRED "\n> F4Server cannot be started [another F4Server is running] <\n\n" RESET);
        exit(1);
    }else{
        system("touch F4ServerRun");
    }

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // END // ------------------------------------ /* ALIVE SERVER CHECK */ ---------------------------------- // END //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //



    // -------------------------------------------------------------------------------------------------------------- //

    serverCommandLineCheck(argc, argv);

    // -------------------------------------------------------------------------------------------------------------- //


    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // BEGIN // ----------------------------------- /* SIGNAL MANAGEMENT */ -------------------------------- // BEGIN //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //

    int errnoValue;

    if(signal(SIGABRT, clientClosure) == SIG_ERR){
        errnoValue = errno;
        printf(BOLDRED "> SIGALRM failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    if(signal(SIGINT, sendWarning) == SIG_ERR){
        errnoValue = errno;
        printf(BOLDRED "> SIGINT failed <\n" RESET);

        errnoCheck(errnoValue);
    }

    if (signal(SIGALRM, trackSignal) == SIG_ERR) {
        errnoValue = errno;
        printf("> SIGALRM failed <\n");
        errnoCheck(errnoValue);
    }

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // END // ------------------------------------- /* SIGNAL MANAGEMENT */ ---------------------------------- // END //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //



    // -------------------------------------------------------------------------------------------------------------- //

    // signalArray   ->   [0]SIGINT, [1]SIGUSR1, [2]SIGUSR2, [3]SIGALRM, [4]SIGTERM, [5]SIGABRT, [6]SIGHUP} //
    bool signalArray[] = {false, false, false, false, false, false, false};

    // set signal set to ignore all signal
    setSignalSet(signalArray);

    // -------------------------------------------------------------------------------------------------------------- //



    // ///////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // BEGIN // -------------------------------- /* SYNCHRONIZATION PHASE */ ------------------------------- // BEGIN //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //

    int row = atoi(argv[1]);
    int column = atoi(argv[2]);

    int player1Token = *argv[3];
    int player2Token = *argv[4];

    char (*playingField)[column];

    parameter.pid = getpid();
    parameter.shmid = createSharedMemory(sizeof(playingField[row][column]));
    parameter.semid = createSemaphoreSet();
    parameter.row = row;
    parameter.column = column;

    char *path = getenv("PWD");

    // check valid path
    if (opendir(path) == NULL) {
        errnoValue = errno;
        printf(BOLDRED "> open directory failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    // add fifo name to path
    strcat(path, "/");
    strcat(path, FIFO);

    createFifo(path);

    int fifoFileDescriptor;

    printf(YELLOW "\n> waiting for player 1 ... <\n" RESET);

    parameter.playerToken = player1Token;

    // begin // --------------------------------------- // fifo write // ----------------------------------- // begin //
    fifoFileDescriptor = open(FIFO, O_WRONLY);

    if (fifoFileDescriptor == -1) {
        errnoValue = errno;
        printf(BOLDRED "> fifo open failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    if (write(fifoFileDescriptor, &parameter, sizeof(parameter)) == -1) {
        errnoValue = errno;
        printf(BOLDRED "> fifo write failed failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    if (close(fifoFileDescriptor) == -1) {
        errnoValue = errno;
        printf(BOLDRED "> fifo close failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    printf(GREEN "> server <<< player 1 synchronized successfully <\n" RESET);
    // end // ----------------------------------------- // fifo write // ------------------------------------- // end //


    // begin // ---------------------------------------- // fifo read // ----------------------------------- // begin //
    fifoFileDescriptor = open(FIFO, O_RDONLY);

    if (fifoFileDescriptor == -1) {
        errnoValue = errno;
        printf(BOLDRED "> fifo open failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    if (read(fifoFileDescriptor, &pidClient1, sizeof(pidClient1)) == -1) {
        errnoValue = errno;
        printf(BOLDRED "> fifo read failed failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    if (close(fifoFileDescriptor) == -1) {
        errnoValue = errno;
        printf(BOLDRED "> fifo close failed <\n" RESET);
        errnoCheck(errnoValue);
    }
    // end // ------------------------------------------ // fifo read // ------------------------------------- // end //


    printf(GREEN "> server >>> player 1 synchronized successfully <\n" RESET);


    time_t currentTime;
    struct tm *localTime;

    currentTime = time(NULL);

    localTime = localtime(&currentTime);

    printf(BOLDGREEN "> server <<<>>> player 1 connected successfully at "
           "%02d:%02d:%02d "
           "of %02d/%02d/%04d <\n" RESET,
           localTime->tm_hour, localTime->tm_min, localTime->tm_sec,
           localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900
           );


    printf(YELLOW "\n> waiting for player 2 ... <\n" RESET);

    parameter.playerToken = player2Token;

    // begin // --------------------------------------- // fifo write // ----------------------------------- // begin //
    fifoFileDescriptor = open(FIFO, O_WRONLY);

    if (fifoFileDescriptor == -1) {
        errnoValue = errno;
        printf(BOLDRED "> fifo open failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    if (write(fifoFileDescriptor, &parameter, sizeof(parameter)) == -1) {
        errnoValue = errno;
        printf(BOLDRED "> fifo write failed failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    if (close(fifoFileDescriptor) == -1) {
        errnoValue = errno;
        printf(BOLDRED "> fifo close failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    printf(GREEN "> server <<< player 2 synchronized successfully <\n" RESET);
    // end // ----------------------------------------- // fifo write // ------------------------------------- // end //


    // begin // ---------------------------------------- // fifo read // ----------------------------------- // begin //
    fifoFileDescriptor = open(FIFO, O_RDONLY);

    if (fifoFileDescriptor == -1) {
        errnoValue = errno;
        printf(BOLDRED "> fifo open failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    if (read(fifoFileDescriptor, &pidClient2, sizeof(pidClient2)) == -1) {
        errnoValue = errno;
        printf(BOLDRED "> fifo read failed failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    if (close(fifoFileDescriptor) == -1) {
        errnoValue = errno;
        printf(BOLDRED "> fifo close failed <\n" RESET);
        errnoCheck(errnoValue);
    }
    // end // ------------------------------------------ // fifo read // ------------------------------------- // end //

    printf(GREEN "> server >>> player 2 synchronized successfully <\n" RESET);


    currentTime = time(NULL);

    localTime = localtime(&currentTime);

    printf(BOLDGREEN "> server <<<>>> player 2 connected successfully at "
           "%02d:%02d:%02d "
           "of %02d/%02d/%04d <\n" RESET,
           localTime->tm_hour, localTime->tm_min, localTime->tm_sec,
           localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900
    );

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // END // ---------------------------------- /* SYNCHRONIZATION PHASE */ --------------------------------- // END //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //



    // ---------------------------------------------------------------------------------------------------------------//

    printf(YELLOW"\n> setting environment ...<\n" RESET);

    playingField = attachSharedMemory(parameter.shmid, column);

    printf(YELLOW "> creating playing field ...<\n" RESET);

    // fill playing field with empty space
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < column; c++) {
            playingField[r][c] = ' ';
        }
    }

    // signalArray   ->   [0]SIGINT, [1]SIGUSR1, [2]SIGUSR2, [3]SIGALRM, [4]SIGTERM, [5]SIGABRT, [6]SIGHUP} //
    signalArray[0] = true;
    signalArray[1] = false;
    signalArray[2] = false;
    signalArray[3] = true;
    signalArray[4] = false;
    signalArray[5] = true;
    signalArray[6] = false;

    setSignalSet(signalArray);

    printf(GREEN "> environment set successfully <\n" RESET);

    // ---------------------------------------------------------------------------------------------------------------//



    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // BEGIN // ----------------------------------------- /* GAME */ --------------------------------------- // BEGIN //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //
    while (1) {
        // begin // ---------------------------------- // player 1 turn // --------------------------------- // begin //

        if(kill(pidClient1, 0) == 0) {
            if (kill(pidClient1, SIGUSR2) == -1) {
                errnoValue = errno;
                printf(BOLDRED "> kill failed <\n" RESET);
                errnoCheck(errnoValue);
            }
        }

        printf(GREEN "\n> server ·>·>·> player 1 <\n" RESET);

        alarm(TIME_INSERT_TOKEN);

        receivedSignal = -1;

        while(1){
            if(receivedSignal == SIGALRM)
                break;
        }

        if (searchFile(COUNTDOWN) == 0){
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
                if (kill(readPid(READ_TOKEN), SIGKILL) == -1) { //kill read
                    errnoValue = errno;
                    printf(BOLDRED "> kill failed <\n" RESET);
                    errnoCheck(errnoValue);
                }
            }
        }

        printf(GREEN "> server <·<·<· player 1 <\n" RESET);

        // end // ------------------------------------ // player 1 turn // ----------------------------------- // end //


        // begin // ------------------------------------ // check win // ----------------------------------- // begin //
        printf(YELLOW "> victory checking ... <\n" RESET);

        setSemaphore(parameter.semid, BLOCK_SHARED_MEMORY, -1);
        cell = winCheck(player1Token, playingField);
        setSemaphore(parameter.semid, UNBLOCK_SHARED_MEMORY, 1);

        if (cell.returnValue == 1) {
            printf(BOLDGREEN "> player 1 win <\n" RESET);
            if(kill(pidClient1, 0) == 0) {
                if (kill(pidClient1, SIGUSR1) == -1) {
                    errnoValue = errno;
                    printf(BOLDRED "> kill failed <\n" RESET);
                    errnoCheck(errnoValue);
                }
            }
            if(kill(pidClient2, 0) == 0) {
                if (kill(pidClient2, SIGTERM) == -1) {
                    errnoValue = errno;
                    printf(BOLDRED "> kill failed <\n" RESET);
                    errnoCheck(errnoValue);
                }
            }

            showWinPlayingField(playingField, cell);

            break;

        } else if (cell.returnValue == -1) {
            printf(BOLDRED "\n> full matrix <\n" RESET);
            break;
        }

        printf(YELLOW "> no win <\n" RESET);
        // end // -------------------------------------- // check win // ------------------------------------- // end //


        // begin // ---------------------------------- // player 2 turn // --------------------------------- // begin //
        if(kill(pidClient2, 0) == 0) {
            if (kill(pidClient2, SIGUSR2) == -1) {
                errnoValue = errno;
                printf(BOLDRED "> kill failed <\n" RESET);
                errnoCheck(errnoValue);
            }
        }

        printf(GREEN "\n> server >·>·> player 2 <\n" RESET);

        alarm(TIME_INSERT_TOKEN);

        receivedSignal = -1;

        while(1){
            if(receivedSignal == SIGALRM)
                break;
        }

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

        printf(GREEN "> server <·<·<· player 2 <\n" RESET);

        // end // ------------------------------------ // player 2 turn // ----------------------------------- // end //


        // begin // ------------------------------------ // check win // ----------------------------------- // begin //
        printf(YELLOW "> victory checking ... <\n" RESET);

        setSemaphore(parameter.semid, UNBLOCK_SHARED_MEMORY, -1);
        cell = winCheck(player2Token, playingField);
        setSemaphore(parameter.semid, UNBLOCK_SHARED_MEMORY, 1);

        if (cell.returnValue == 1) {
            printf(BOLDGREEN "> player 2 win <\n" RESET);
            if(kill(pidClient2, 0) == 0) {
                if (kill(pidClient2, SIGUSR1) == -1) {
                    errnoValue = errno;
                    printf(BOLDRED "> kill failed <\n" RESET);
                    errnoCheck(errnoValue);
                }
            }
            if(kill(pidClient1, 0) == 0) {
                if (kill(pidClient1, SIGTERM) == -1) {
                    errnoValue = errno;
                    printf(BOLDRED "> kill failed <\n" RESET);
                    errnoCheck(errnoValue);
                }
            }

            showWinPlayingField(playingField, cell);

            break;

        } else if (cell.returnValue == -1) {
            printf(BOLDRED "\n> full matrix <\n" RESET);
            break;
        }

        printf(YELLOW "> no win <\n" RESET);
        // end // -------------------------------------- // check win // ------------------------------------- // end //
    }
    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // END // ------------------------------------------- /* GAME */ ----------------------------------------- // END //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //



    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // BEGIN // ------------------------------------ /* SERVER CLOSURE */ ---------------------------------- // BEGIN //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //

    printf(BOLDYELLOW "\n> server closure ... <\n\n" RESET);

    // remove fifo
    removeFile(FIFO);

    // remove shared memory
    detachSharedMemory(parameter.column, playingField);
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

    return 0;

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // END // -------------------------------------- /* SERVER CLOSURE */ ------------------------------------ // END //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //
}
