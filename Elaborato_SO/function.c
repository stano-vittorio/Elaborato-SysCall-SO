/************************************
*Matricola --> VR457793
*Nome e cognome --> Vittorio Maria Stano
*Data di realizzazione --> 14/06/2023
*************************************/

/// @file function.c
/// @brief other client and server shared functions

#include "err_exit.h"
#include "function.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

volatile sig_atomic_t receivedSignal;

int pidServer;

int pidClient1;
int pidClient2;

int childPid;

info parameter;

// take winning position for green playingField
winIndex cell;

sigset_t signalSet, oldSet;

/// @brief full signal set creation
void setSignalSet(bool signalArray[]) {
    int errnoValue;

    if (sigfillset(&signalSet) == -1) {
        errnoValue = errno;
        errnoCheck(errnoValue);
    }

    if (signalArray[0] == true) {
        if (sigdelset(&signalSet, SIGINT) == -1) {
            errnoValue = errno;
            errnoCheck(errnoValue);
        }
    }

    if (signalArray[1] == true) {
        if (sigdelset(&signalSet, SIGUSR1) == -1) {
            errnoValue = errno;
            errnoCheck(errnoValue);
        }
    }

    if (signalArray[2] == true) {
        if (sigdelset(&signalSet, SIGUSR2) == -1) {
            errnoValue = errno;
            errnoCheck(errnoValue);
        }
    }

    if (signalArray[3] == true) {
        if (sigdelset(&signalSet, SIGALRM) == -1) {
            errnoValue = errno;
            errnoCheck(errnoValue);
        }
    }

    if (signalArray[4] == true) {
        if (sigdelset(&signalSet, SIGTERM) == -1) {
            errnoValue = errno;
            errnoCheck(errnoValue);
        }
    }

    if (signalArray[5] == true) {
        if (sigdelset(&signalSet, SIGABRT) == -1) {
            errnoValue = errno;
            errnoCheck(errnoValue);
        }
    }

    if (signalArray[6] == true) {
        if (sigdelset(&signalSet, SIGHUP) == -1) {
            errnoValue = errno;
            errnoCheck(errnoValue);
        }
    }

    // blocks all signals except specified in signal[] set
    if (sigprocmask(SIG_SETMASK, &signalSet, &oldSet) == -1) {
        if (errnoValue == EINVAL) {
            errnoValue = errno;
            errnoCheck(errnoValue);
        }
    }
}

/// @brief integer to string conversion
void itoc(int intero, char *childPid) {
    int cpyIntero = intero;
    int resto;
    int counter = 0;

    for (int index = 0; index < intero; index++) {

        if (cpyIntero == 0) {
            break;
        }

        cpyIntero = cpyIntero / 10;
        counter++;
    }

    cpyIntero = intero;
    for (int index = 0; index < intero; index++) {

        if (cpyIntero == 0) {
            break;
        }

        resto = cpyIntero % 10;
        cpyIntero = cpyIntero / 10;
        childPid[counter - index - 1] = resto + '0';
    }
}

/// @brief reads the process ID from a file and returns it as an integer
int readPid(char *fileName) {
    int errnoValue;

    int fileDescriptor;
    char pid[11] = {'\0'};

    fileDescriptor = open(fileName, O_CREAT | O_RDONLY, S_IRWXU | S_IRWXG);

    // get file descriptor
    if (fileDescriptor == -1) {
        errnoValue = errno;
        printf(BOLDRED "> open failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    // read file
    if (read(fileDescriptor, pid, sizeof(pid)) == -1) {
        errnoValue = errno;
        printf(BOLDRED "> read failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    // close file
    if (close(fileDescriptor) == -1) {
        errnoValue = errno;
        printf(BOLDRED "> close failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    // return conversion
    return atoi(pid);
}

/// @brief converts the process ID to a string and writes it to file
void writePid(char *fileName, int pid) {
    int errnoValue;

    int fileDescriptor;
    // max pid id is 0xFFFFFFFE -> 4294967294
    char pidString[11] = {'\0'};

    itoc(pid, pidString);

    fileDescriptor = open(fileName, O_CREAT | O_WRONLY, S_IRWXU | S_IRWXG);

    // get file descriptor
    if (fileDescriptor == -1) {
        errnoValue = errno;
        printf(BOLDRED "> open failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    // read file
    if (write(fileDescriptor, pidString, strlen(pidString)) == -1) {
        errnoValue = errno;
        printf(BOLDRED "> write failed <\n" RESET);
        errnoCheck(errnoValue);
    }

    // close file
    if (close(fileDescriptor) == -1) {
        errnoValue = errno;
        printf(BOLDRED "> close failed <\n" RESET);
        errnoCheck(errnoValue);
    }
}

/// @brief search for a specific file
int searchFile(char *name) {
    int errnoValue;

    DIR *directory;
    struct dirent *directoryPath;

    // buffer
    char cwd[1024];

    getcwd(cwd, sizeof(cwd));

    // check existing directory
    if ((directory = opendir(cwd)) == NULL) {
        errnoValue = errno;
        printf(BOLDRED "open failed" RESET);
        errnoCheck(errnoValue);
    }

    // sentinel variable
    int searchFile = -1;

    while ((directoryPath = readdir(directory)) != NULL) {
        if ((searchFile = strcmp(name, directoryPath->d_name)) == 0) {
            break;
        }
    }

    // close directory
    if (closedir(directory) == -1) {
        errnoValue = errno;
        printf(BOLDRED "open failed" RESET);
        errnoCheck(errnoValue);
    }

    // return of research feedback
    return searchFile;
}

/// @brief confirm reception of a signal in client_function
void nothing(int sig){
}