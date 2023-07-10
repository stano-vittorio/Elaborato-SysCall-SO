/************************************
*Matricola --> VR457793
*Nome e cognome --> Vittorio Maria Stano
*Data di realizzazione --> 14/06/2023
*************************************/

/// @file clientFunction.h
/// @brief contains client functions definitions

#ifndef ELABORATO_SO_CLIENT_FUNCTION_H
#define ELABORATO_SO_CLIENT_FUNCTION_H

#define ARGC_CLIENT 3

// structure sent with pipe
typedef struct{
    _Bool readCheck;
    char cPlayerColumn[100];
}pipeInfo;

/// @brief check the client startup string
int clientCommandLineCheck(int, char *[]);
/// @brief alert the user after the CTRL+C key is pressed
void sendWarning(int);
/// @brief continues the game after the time is up
void continueGame(int);
/// @brief inserts a token into playing field
int insertToken(char (*)[]);
/// @brief displays playing field
void showPlayingField(char (*)[]);
/// @brief alerts the opponent's abandonment
void opponentAbandoned(int);
/// @brief warns loss by abandonment
void youAbandoned(int);
/// @brief terminates the client when the server sends a close signal
void serverClosure(int);
/// @brief used to set the signal received with SIGUSR2
void trackSignal(int);
/// @brief notifies the client win
void clientWin(int);
/// @brief notifies the client loss
void clientLose(int);

#endif //ELABORATO_SO_CLIENT_FUNCTION_H