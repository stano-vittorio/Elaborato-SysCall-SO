/************************************
*Matricola --> VR457793
*Nome e cognome --> Vittorio Maria Stano
*Data di realizzazione --> 14/06/2023
*************************************/

/// @file F4autoPlayer.c
/// @brief contains FORZA4 game auto-client

#include "err_exit.h"
#include "function.h"
#include "autoPlayer_function.h"

#include "semaphore.h"
#include "shared_memory.h"

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {

    system("touch F4Client2Run");

    char *playerName = "CPU\0";

    // -------------------------------------------------------------------------------------------------------------- //

    printf("> welcome "
           BOLDCYAN   "%s"
           RESET  " <\n", playerName);

    // -------------------------------------------------------------------------------------------------------------- //


    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // BEGIN // ----------------------------------- /* SIGNAL MANAGEMENT */ -------------------------------- // BEGIN //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //

    int errnoValue;

    if (signal(SIGINT, sendWarning) == SIG_ERR) {
        errnoValue = errno;
        printf(BOLDRED "> SIGINT failed <\n" RESET);
    }

    if (signal(SIGUSR1, clientWin) == SIG_ERR) {
        errnoValue = errno;
        printf(BOLDRED "> SIGALRM failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    if (signal(SIGUSR2, trackSignal) == SIG_ERR) {
        errnoValue = errno;
        printf(BOLDRED "> SIGALRM failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    if (signal(SIGHUP, opponentAbandoned) == SIG_ERR) {
        errnoValue = errno;
        printf(BOLDRED "> SIGALRM failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    if (signal(SIGABRT, serverClosure) == SIG_ERR) {
        errnoValue = errno;
        printf(BOLDRED "> SIGALRM failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    if (signal(SIGTERM, clientLose) == SIG_ERR) {
        errnoValue = errno;
        printf(BOLDRED "> SIGALRM failed <\n" RESET);
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

    int pidClient = getpid();

    int fifoFileDescriptor;

    // begin // ---------------------------------------- // fifo read // ----------------------------------- // begin //
    fifoFileDescriptor = open(FIFO, O_RDONLY);
    if (fifoFileDescriptor == -1) {
        errnoValue = errno;
        printf(BOLDRED "> fifo open failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    if (read(fifoFileDescriptor,&parameter, sizeof(parameter)) == -1) {
        errnoValue = errno;
        printf(BOLDRED "> fifo read failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    if (close(fifoFileDescriptor) == -1) {
        errnoValue = errno;
        printf(BOLDRED "> fifo close failed <\n" RESET);
        errnoCheck(errnoValue);
    }
    // end // ------------------------------------------ // fifo read // ------------------------------------- // end //


    // begin // --------------------------------------- // fifo write // ----------------------------------- // begin //
    fifoFileDescriptor = open(FIFO, O_WRONLY);

    if (fifoFileDescriptor == -1) {
        errnoValue = errno;
        printf(BOLDRED "> fifo open failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    if (write(fifoFileDescriptor,&pidClient, sizeof(pidClient)) == -1) {
        errnoValue = errno;
        printf(BOLDRED "> fifo write failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    if (close(fifoFileDescriptor) == -1) {
        errnoValue = errno;
        printf(BOLDRED "> fifo close failed <\n" RESET);
        errnoCheck(errnoValue);
    }
    // end // ----------------------------------------- // fifo write // ------------------------------------- // end //

    printf(BOLDGREEN "> you are now synchronized on network ! <\n" RESET);

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // END // ---------------------------------- /* SYNCHRONIZATION PHASE */ --------------------------------- // END //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //

    

    // ---------------------------------------------------------------------------------------------------------------//

    printf(YELLOW "\n> environment setting in progress ... <\n" RESET);

    char (*playingField)[parameter.column];

    playingField = attachSharedMemory(parameter.shmid, parameter.column);

    int validToken;

    // signalArray   ->   [0]SIGINT, [1]SIGUSR1, [2]SIGUSR2, [3]SIGALRM, [4]SIGTERM, [5]SIGABRT, [6]SIGHUP} //
    signalArray[0] = true;
    signalArray[1] = true;
    signalArray[2] = true;
    signalArray[3] = true;
    signalArray[4] = true;
    signalArray[5] = true;
    signalArray[6] = true;

    setSignalSet(signalArray);

    printf(GREEN "> environment set successfully <\n" RESET);

    // -------------------------------------------------------------------------------------------------------------- //



    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // BEGIN // ----------------------------------------- /* GAME */ --------------------------------------- // BEGIN //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //
    while (1) {
        receivedSignal = -1;

        // wait // --------------------------------------- // turn // --------------------------------------- // wait //
        while (1) {
            if (receivedSignal == SIGUSR2)
                break;
        }

        // begin // --------------------------------------- // turn // ------------------------------------- // begin //

        validToken = 0;

        printf(BOLDGREEN "\n> it's your turn <\n" RESET);

        setSemaphore(parameter.semid, BLOCK_SHARED_MEMORY, -1);
        showPlayingField(playingField);
        setSemaphore(parameter.semid, UNBLOCK_SHARED_MEMORY, 1);

        setSemaphore(parameter.semid, BLOCK_SHARED_MEMORY, -1);
        validToken = insertToken(playingField);
        setSemaphore(parameter.semid, UNBLOCK_SHARED_MEMORY, 1);

        if(validToken == 1){
            if(kill(parameter.pid, 0) == 0) {
                if(kill(parameter.pid, SIGALRM) == -1){
                    errnoValue = errno;
                    printf(BOLDRED "> kill failed <\n" RESET);
                    errnoCheck(errnoValue);
                }
            }
        }

        setSemaphore(parameter.semid, BLOCK_SHARED_MEMORY, -1);
        showPlayingField(playingField);
        setSemaphore(parameter.semid, UNBLOCK_SHARED_MEMORY, 1);

        printf(YELLOW "> waiting for opponent move <\n" RESET);

        // end // ----------------------------------------- // turn // --------------------------------------- // end //
    }

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // END // ------------------------------------------- /* GAME */ ----------------------------------------- // END //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //
}