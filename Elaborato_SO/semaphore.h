/************************************
*Matricola --> VR457793
*Nome e cognome --> Vittorio Maria Stano
*Data di realizzazione --> 14/06/2023
*************************************/

/// @file semaphore.h
/// @brief contains variables and functions definition for semaphore management

#ifndef ELABORATO_SO_SEMAPHORE_H
#define ELABORATO_SO_SEMAPHORE_H

// management for IPC semaphore operations
union semun {
    int value;
    struct semid_ds *buffer;
    unsigned short *semaphoreSet;
};

/// @brief semaphore creation
int createSemaphoreSet(void);
/// @brief semaphore settings
void setSemaphore(int, unsigned short , short );
/// @brief semaphore removal
void removeSemaphoreSet(int);

#endif //ELABORATO_SO_SEMAPHORE_H