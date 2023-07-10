/************************************
*Matricola --> VR457793
*Nome e cognome --> Vittorio Maria Stano
*Data di realizzazione --> 14/06/2023
*************************************/

/// @file shared_memory.c
/// @brief functions for shared memory management

#include "err_exit.h"
#include "shared_memory.h"

#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>

int errnoValue;

/// @brief shared memory creation
int createSharedMemory(size_t size){
    printf(YELLOW "> shared memory creation ... <\n" RESET);

    // shared memory allocation
    int shmid = shmget(IPC_PRIVATE, size, S_IWUSR | S_IRUSR);

    // allocation error management
    if(shmid == -1) {
        errnoValue = errno;
        printf(BOLDRED "> shared memory creation failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    printf(GREEN "> shared memory created successfully <\n" RESET);

    return shmid;
}

/// @brief shared memory attach
void *attachSharedMemory(int shmid, int column){
    printf(YELLOW "> shared memory attaching ... <\n" RESET);

    // shared memory attach
    char (*playingField)[column] = shmat(shmid, NULL, 0);

    // attach error management
    if(playingField == (void *) -1) {
        errnoValue = errno;
        printf(BOLDRED "> shared memory attach failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    printf(GREEN "> shared memory attached successfully <\n" RESET);

    return playingField;
}

/// @brief shared memory detach
void detachSharedMemory(int column, char (*playingField)[column]){
    printf(YELLOW "> shared memory detaching ... <\n" RESET);

    // shared memory detach and error management
    if(shmdt(playingField) == -1){
        errnoValue = errno;
        printf(BOLDRED "> shared memory detach failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    printf(GREEN "> shared memory detached successfully <\n" RESET);
}

/// @brief shared memory removal
void removeSharedMemory(int shmid){
    printf(YELLOW "> shared memory removal ... <\n" RESET);

    // shared memory detach and error management
    if(shmctl(shmid, IPC_RMID, NULL)){
        errnoValue = errno;
        printf(BOLDRED "> shared memory remove failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    printf(GREEN "> shared memory removed successfully <\n" RESET);
}