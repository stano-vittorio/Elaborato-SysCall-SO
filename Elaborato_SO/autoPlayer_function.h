/************************************
*Matricola --> VR457793
*Nome e cognome --> Vittorio Maria Stano
*Data di realizzazione --> 14/06/2023
*************************************/

/// @file clientFunction.h
/// @brief contains auto-client functions definitions

#ifndef ELABORATO_SO_CLIENT_FUNCTION_H
#define ELABORATO_SO_CLIENT_FUNCTION_H

#define ARGC_CLIENT 3

#define MAX_DEPTH 9

typedef struct{
    _Bool readCheck;
    int playerColumn;
}pipeInfo;

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
/// @brief notifies the auto client win
void clientWin(int);
/// @brief notifies the auto client loss
void clientLose(int);
/// @brief check if a move is valid
int is_valid_move(char (*)[], int);
/// @brief make a move in playing field
int make_move(char (*)[], int, char);
/// @brief evaluate a window of boxes
int evaluate_window(char *);
/// @brief calculates the score of the chosen position
int score_position(char (*)[]);
/// @brief find the best possible move
int find_best_move(char (*)[], int);
/// @brief implements the minimax algorithm
int minimax(char (*)[], int, int, int, int );
/// @brief returns the maximum value
int max(int, int);
/// @brief returns the minimum value
int min(int, int);

#endif //ELABORATO_SO_CLIENT_FUNCTION_H