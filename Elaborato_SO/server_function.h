/************************************
*Matricola --> VR457793
*Nome e cognome --> Vittorio Maria Stano
*Data di realizzazione --> 14/06/2023
*************************************/

/// @file serverFunction.h
/// @brief contains server variables and functions definition

#ifndef ELABORATO_SO_SERVER_FUNCTION_H
#define ELABORATO_SO_SERVER_FUNCTION_H

#define ARGC_SERVER 5

#define F4SERVER "F4ServerRun\0"

/// @brief check the server startup string
void serverCommandLineCheck(int, char *[]);
/// @brief alert the user after the CTRL+C key is pressed
void sendWarning(int);
/// @brief continues the game after the time is up
void continueGame(int);
/// @brief ends the game after a double CTRL+C press
void endGame(int);
/// @brief check if a player won the game
winIndex winCheck(char, char(*)[]);
/// @brief displays the win playing field
void showWinPlayingField(char (*)[], winIndex);
/// @brief used to set the signal received with SIGALRM
void trackSignal(int sig);
/// @brief remove a specific file
void removeFile(char *);
/// @brief game termination after the player leaves
void clientClosure(int);

#endif //ELABORATO_SO_SERVER_FUNCTION_H