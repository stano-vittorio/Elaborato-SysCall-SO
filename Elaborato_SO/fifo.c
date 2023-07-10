/************************************
*Matricola --> VR457793
*Nome e cognome --> Vittorio Maria Stano
*Data di realizzazione --> 14/06/2023
*************************************/

/// @file fifo.c
/// @brief functions for fifo management

#include "err_exit.h"
#include "fifo.h"

#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>

/// @brief fifo memory creation
void createFifo(char *pathFifo){
    int errnoValue;

    // fifo creation
    int file_Fifo = mkfifo(pathFifo, S_IRUSR | S_IWUSR | S_IWGRP);

    // fifo creation error management
    if(file_Fifo == -1) {
        errnoValue = errno;
        printf(BOLDRED "> fifo create failed <\n" RESET);
        errnoCheck(errnoValue);
    }
}