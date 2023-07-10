/************************************
*Matricola --> VR457793
*Nome e cognome --> Vittorio Maria Stano
*Data di realizzazione --> 14/06/2023
*************************************/

/// @file function.h
/// @brief contains variables and functions definition for client and server

#ifndef ELABORATO_SO_FUNCTION_H
#define ELABORATO_SO_FUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define TIME_CTRL_C 3

#define TIME_INSERT_TOKEN 134

#define BLOCK_SHARED_MEMORY 0
#define UNBLOCK_SHARED_MEMORY 0

#define F4CLIENT1 "F4Client1Run\0"
#define F4CLIENT2 "F4Client2Run\0"

extern volatile sig_atomic_t receivedSignal;

#define FIFO "FIFO\0"

#define COUNTDOWN "pid_countDown_.txt\0"
#define READ_TOKEN "pid_readToken_.txt\0"


extern int pidServer;

extern int pidClient1;
extern int pidClient2;

extern int childPid;

extern sigset_t signalSet, oldSet;

// structure sent with fifo
typedef struct{
    int pid;
    int shmid;
    int semid;
    char playerToken;
    int row;
    int column;
} info;

// store the player's winning coordinates
typedef struct {
    int row[4];
    int column[4];
    int returnValue;
}winIndex;

extern info parameter;

// take winning position for green token playingField
extern winIndex cell;

/// @brief full signal set creation
void setSignalSet(_Bool []);
/// @brief integer to string conversion
void itoc(int, char *);
/// @brief reads the process ID from a file and returns it as an integer
int readPid(char *);
/// @brief converts the process ID to a string and writes it to file
void writePid(char *, int);
/// @brief search for a specific file
int searchFile(char *);
/// @brief confirm reception of a signal in client_function
void nothing(int);

#endif //ELABORATO_SO_FUNCTION_H