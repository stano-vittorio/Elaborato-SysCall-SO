/************************************
*Matricola --> VR457793
*Nome e cognome --> Vittorio Maria Stano
*Data di realizzazione --> 14/06/2023
*************************************/

/// @file semaphore.c
/// @brief functions for semaphore management

#include "err_exit.h"
#include "semaphore.h"

#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/sem.h>

#define NUMSEM 1

/// @brief semaphore creation
int createSemaphoreSet(void){
    int errnoValue;
    printf(YELLOW "> semaphore set creation ... <\n" RESET);

    int semid = semget(IPC_PRIVATE, NUMSEM, S_IRUSR | S_IWUSR);

    if(semid == -1){
        errnoValue = errno;
        errnoCheck(errnoValue);
    }

    union semun arg;

    unsigned short semaphoreSet[] = {1};

    arg.semaphoreSet = semaphoreSet;

    if(semctl(semid, 0, SETALL, arg) == -1){
        errnoValue = errno;
        printf(BOLDRED "> semaphore creation failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    printf(GREEN "> semaphore set created successfully <\n" RESET);

    return semid;
}

/// @brief semaphore settings
void setSemaphore(int semid, unsigned short numSem, short semOp){
    int errnoValue;

    struct sembuf sop = {.sem_num = numSem, .sem_op = semOp, .sem_flg = 0};

    if(semop(semid, &sop, 1) == -1){
        errnoValue = errno;
        printf(BOLDRED "> set semaphore failed <\n" RESET);
        errnoCheck(errnoValue);
    }
}

/// @brief semaphore removal
void removeSemaphoreSet(int semid) {
    int errnoValue;

    printf(YELLOW "> semaphore set removal ... <\n" RESET);

    if (semctl(semid, 0, IPC_RMID, NULL) == -1) {
        errnoValue = errno;
        printf(BOLDRED "> set semaphore failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    printf(GREEN "> set semaphore removed successfully <\n" RESET);
}