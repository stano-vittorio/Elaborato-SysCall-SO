/************************************
*Matricola --> VR457793
*Nome e cognome --> Vittorio Maria Stano
*Data di realizzazione --> 14/06/2023
*************************************/

/// @file shared_memory.h
/// @brief contains variables and functions definition for shared memory management

#include <stddef.h>

#ifndef ELABORATO_SO_SHARED_MEMORY_H
#define ELABORATO_SO_SHARED_MEMORY_H

/// @brief shared memory creation
int createSharedMemory(size_t);
///@brief shared memory attach
void *attachSharedMemory(int, int);
/// @brief shared memory detach
void detachSharedMemory(int, char (*)[]);
/// @brief shared memory removal
void removeSharedMemory(int);

#endif //ELABORATO_SO_SHARED_MEMORY_H