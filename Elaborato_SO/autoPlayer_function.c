/************************************
*Matricola --> VR457793
*Nome e cognome --> Vittorio Maria Stano
*Data di realizzazione --> 14/06/2023
*************************************/

/// @file autoPlayer_function.c
/// @brief auto-client functions implementation

#include "function.h"
#include "autoPlayer_function.h"

#include "err_exit.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/wait.h>
#include <stdbool.h>

/// @brief alert the user after the CTRL+C key is pressed
void sendWarning(int sig){
    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // BEGIN // ----------------------------------- /* SIGNAL MANAGEMENT */ -------------------------------- // BEGIN //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //

    int errnoValue;

    if(signal(SIGALRM, continueGame) == SIG_ERR){
        errnoValue = errno;
        errExit("> sigalrm failed \n<");
        errnoCheck(errnoValue);
    }

    if(signal(SIGINT, youAbandoned) == SIG_ERR){
        errnoValue = errno;
        errExit("> sigint failed <\n");
        errnoCheck(errnoValue);
    }

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // END // ------------------------------------- /* SIGNAL MANAGEMENT */ ---------------------------------- // END //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //

    printf(RED "\n> if you press ctrl-c again the game will end <\n" RESET);
    printf(YELLOW "> detecting ctr+C ... <\n" RESET);

    // -------------------------------------------------------------------------------------------------------------- //

    // signalArray   ->   [0]SIGINT, [1]SIGUSR1, [2]SIGUSR2, [3]SIGALRM, [4]SIGTERM, [5]SIGABRT, [6]SIGHUP} //
    bool signalArray[] = {true, true, true, true, true, true, true};

    setSignalSet(signalArray);

    // -------------------------------------------------------------------------------------------------------------- //

    alarm(TIME_CTRL_C);

    // waiting for signal
    if(pause() == -1){
        // ////////////////////////////////////////////////////////////////////////////////////////////////////////// //
        // BEGIN // ------------------------------ /* RESET SIGNAL MANAGEMENT */ --------------------------- // BEGIN //
        // /////////////////////////////////////////////////////////////////////////////////////////////////////////  //

        if (signal(SIGALRM, nothing) == SIG_ERR) {
            errnoValue = errno;
            printf("> SIGALRM failed <\n");
            errnoCheck(errnoValue);
        }

        if(signal(SIGINT, sendWarning) == SIG_ERR){
            errnoValue = errno;
            errExit("> sigint failed <\n");
            errnoCheck(errnoValue);
        }

        // ////////////////////////////////////////////////////////////////////////////////////////////////////////// //
        // END // -------------------------------- /* RESET SIGNAL MANAGEMENT */ ----------------------------- // END //
        // /////////////////////////////////////////////////////////////////////////////////////////////////////////  //
    }
}

/// @brief continues the game after the time is up
void continueGame(int sig) {
    printf(GREEN "> ctr+C not detected game will continue <\n" RESET);
    printf("> Insert column < ");
    fflush(stdout);
}

/// @brief inserts a token into playing field
int insertToken(char (*playingField)[parameter.column]){
    printf("\n> Generating column ... <\n");
    fflush(stdout);

    int errnoValue;

    pipeInfo infoPipe;


    // begin // ---------------------------------- // CREATE PIPE // --------------------------------------- // begin //

    ssize_t nBytes;
    int fd[2];

    if(pipe(fd) == -1){
        errnoValue = errno;
        printf("> pipe creation failed <\n");
        errnoCheck(errnoValue);
    }

    // end // ------------------------------------ // CREATE PIPE // ----------------------------------------- // end //



    // ----------------------------------------------- / READ TOKEN / ------------------------------------------------//
    pid_t  readToken = fork();

    if(readToken == -1){
        errnoValue = errno;
        printf(RED "> fork failed <\n" RESET);
    }

    if(readToken == 0){

        // ---------------------------------------------------------------------------------------------------------- //

        // signalArray   ->   [0]SIGINT, [1]SIGUSR1, [2]SIGUSR2, [3]SIGALRM, [4]SIGTERM, [5]SIGABRT, [6]SIGHUP} //
        bool signalArray[] = {false, false, false, false, false, false, false};

        setSignalSet(signalArray);

        // ---------------------------------------------------------------------------------------------------------- //


        writePid(READ_TOKEN, getpid());


        // begin // ---------------------------- // CREATE AUTOPLAYER MATRIX // ---------------------------- // begin //

        char board[parameter.row][parameter.column];

        for(int r = 0; r < parameter.row; r++){
            for(int c = 0; c < parameter.column; c++){
                if(playingField[r][c] == parameter.playerToken) {
                    board[r][c] = 'X';
                }else if (playingField[r][c] == ' ') {
                    board[r][c] = ' ';
                }
                else {
                    board[r][c] = 'O';
                }
            }
        }

        // end // ------------------------------ // CREATE AUTOPLAYER MATRIX // ------------------------------ // end //



        // ------------------------------------------- // READ COLUMN // ---------------------------------------------//

        infoPipe.playerColumn = find_best_move(board, MAX_DEPTH);

        // ------------------------------------------- // READ COLUMN // ---------------------------------------------//


        infoPipe.readCheck = true;


        // begin // -------------------------------- // WRITE PIPE // -------------------------------------- // begin //

        if(close(fd[0]) == -1){
            errnoValue = errno;
            printf("> pipe close failed <\n");
            errnoCheck(errnoValue);
        }

        nBytes = write(fd[1], &infoPipe, sizeof(infoPipe));

        if(nBytes != sizeof(infoPipe)){
            errnoValue = errno;
            printf("> pipe close failed <\n");
            errnoCheck(errnoValue);
        }

        if(close(fd[1]) == -1){
            errnoValue = errno;
            printf("> pipe close failed <\n");
            errnoCheck(errnoValue);
        }

        // end // ---------------------------------- // WRITE PIPE // ---------------------------------------- // end //

        exit(0);
        // --------------------------------------------- / READ TOKEN / ----------------------------------------------//
    }else{
        waitpid(readToken, NULL, WUNTRACED | WCONTINUED);


        // begin // -------------------------------- // READ PIPE // --------------------------------------- // begin //

        if(close(fd[1]) == -1){
            errnoValue = errno;
            printf("> pipe close failed <\n");
            errnoCheck(errnoValue);
        }

        nBytes = read(fd[0], &infoPipe, sizeof(infoPipe));

        if(nBytes > 0){

        }else{
            infoPipe.readCheck = false;
        }

        if(close(fd[0]) == -1){
            errnoValue = errno;
            printf("> pipe close failed <\n");
            errnoCheck(errnoValue);
        }

        // end // ----------------------------------- // READ PIPE // ---------------------------------------- // end //
    }



    if(infoPipe.readCheck == false){
        return -1;
    }

    for (int r = 0; r < parameter.row; r++) {
        if (playingField[parameter.row - r - 1][infoPipe.playerColumn] == ' ') {
            playingField[parameter.row - r - 1][infoPipe.playerColumn] = parameter.playerToken;
            printf("\n> token entered in column %d <\n", infoPipe.playerColumn+1);
            return 1;
        }
    }
}

/// @brief displays playing field
void showPlayingField(char (*playingField)[parameter.column]){
    for(int r = 0; r < parameter.row; r++){
        printf("\n");
        for(int c = 0; c < parameter.column; c++){
            printf("| %c |", playingField[r][c]);
        }
        printf("\n");
        for(int s = 0; s < parameter.column * 5; s++){
            printf("-");
        }
    }

    printf("\n");

    for(int c = 0; c < parameter.column; c++){
        if(c < 10) {
            printf("  %d  ", c + 1);
        }
        else{
            printf(" %d  ", c + 1);
        }
    }

    printf("\n\n");
}


/// @brief alerts the opponent's abandonment
void opponentAbandoned(int sig){ //clientWin
    printf(BOLDGREEN "\n> opponent abandoned >>> you win ! <\n" RESET);
    printf(BOLDYELLOW "\n> client closure ... <\n" RESET);
    printf(BOLDGREEN "\n> client closed successfully <\n" RESET);

    exit(0);
}

/// @brief warns loss by abandonment
void youAbandoned(int sig){
    int errnoValue;

    printf(BOLDRED "\n> game abandoned >>> you lose ! <\n" RESET);

    printf(BOLDYELLOW "\n> client closure ... <\n" RESET);

    if(kill(parameter.pid, 0) == 0) {
        if (kill(parameter.pid, SIGABRT) == -1) {
            errnoValue = errno;
            printf(BOLDRED "> kill failed <\n" RESET);
            errnoCheck(errnoValue);
        }
    }

    printf(BOLDGREEN "\n> client closed successfully <\n" RESET);

    exit(0);
}


/// @brief terminates the client when the server sends a close signal
void serverClosure(int sig){
    printf(BOLDYELLOW "\n> external closure from server <\n" RESET);
    printf(BOLDYELLOW "\n> nobody won <\n" RESET);

    printf(BOLDYELLOW "\n> client closure ... <\n" RESET);
    printf(BOLDGREEN "\n> client closed successfully <\n" RESET);

    exit(0);
}

/// @brief used to set the signal received with SIGUSR2
void trackSignal(int sig){
    receivedSignal = SIGUSR2;
}

/// @brief notifies the auto client win
void clientWin(int sig){
    printf(BOLDGREEN "> you win ! <\n" RESET);

    exit(0);
}

/// @brief notifies the auto client loss
void clientLose(int sig){
    printf(BOLDRED "> you lose ! <\n" RESET);

    exit(0);
}

/// @brief check if a move is valid
int is_valid_move(char (*board)[parameter.column], int col) {
    return board[0][col] == ' ';
}

/// @brief make a move in playing field
int make_move(char (*board)[parameter.column], int col, char player) {
    int row;
    for (row = parameter.row - 1; row >= 0; row--) {
        if (board[row][col] == ' ') {
            board[row][col] = player;
            return row;
        }
    }
    return -1; // Invalid move
}

/// @brief evaluate a window of boxes
int evaluate_window(char *window) {
    char opponent = (parameter.playerToken == 'X') ? 'O' : 'X';
    int player_count = 0;
    int opponent_count = 0;
    int empty_count = 0;

    for (int i = 0; i < 4; i++) {
        if (window[i] == parameter.playerToken)
            player_count++;
        else if (window[i] == opponent)
            opponent_count++;
        else
            empty_count++;
    }

    if (player_count == 4)
        return 100; // Player wins
    else if (opponent_count == 4)
        return -100; // Opponent wins
    else if (player_count == 3 && empty_count == 1)
        return 5; // Player has a potential winning move
    else if (opponent_count == 3 && empty_count == 1)
        return -5; // Opponent has a potential winning move
    else if (player_count == 2 && empty_count == 2)
        return 2; // Player has a potential double move
    else if (opponent_count == 2 && empty_count == 2)
        return -2; // Opponent has a potential double move
    else
        return 0; // Neutral
}

/// @brief calculates the score of the chosen position
int score_position(char (*board)[parameter.column]) {
    int score = 0;

    // Horizontal
    for (int row = 0; row < parameter.row; row++) {
        for (int col = 0; col < parameter.column - 3; col++) {
            char window[4] = {board[row][col], board[row][col + 1], board[row][col + 2], board[row][col + 3]};
            score += evaluate_window(window);
        }
    }

    // Vertical
    for (int col = 0; col < parameter.column; col++) {
        for (int row = 0; row < parameter.row - 3; row++) {
            char window[4] = {board[row][col], board[row + 1][col], board[row + 2][col], board[row + 3][col]};
            score += evaluate_window(window);
        }
    }

    // Diagonal
    for (int row = 0; row < parameter.row - 3; row++) {
        for (int col = 0; col < parameter.column - 3; col++) {
            char window[4] = {board[row][col], board[row + 1][col + 1], board[row + 2][col + 2], board[row + 3][col + 3]};
            score += evaluate_window(window);
        }
    }

    // Diagonal
    for (int row = 0; row < parameter.row - 3; row++) {
    for (int col = parameter.column - 1; col >= 3; col--) {
        char window[4] = {board[row][col], board[row + 1][col - 1], board[row + 2][col - 2], board[row + 3][col - 3]};
        score += evaluate_window(window);
    }
}

return score;
}

/// @brief find the best possible move
int find_best_move(char (*board)[parameter.column], int depth) {
    int best_score = -1000;
    int best_move = -1;

    for (int col = 0; col < parameter.column; col++) {
        if (is_valid_move(board, col)) {
            int row = make_move(board, col, parameter.playerToken);
            int score = minimax(board, depth, -100000, 100000, 1);
            board[row][col] = ' '; // Undo the move

            if (score > best_score) {
                best_score = score;
                best_move = col;
            }
        }
    }

    return best_move;
}

/// @brief implements the minimax algorithm
int minimax(char (*board)[parameter.column], int depth, int alpha, int beta, int maximizingPlayer) {
    if (depth == 0) {
        return score_position(board);
    }

    if (maximizingPlayer) {
        int value = -100000;
        for (int col = 0; col < parameter.column; col++) {
            if (is_valid_move(board, col)) {
                int row = make_move(board, col, parameter.playerToken);
                value = max(value, minimax(board, depth - 1, alpha, beta, 0));
                board[row][col] = ' '; // Undo the move
                alpha = max(alpha, value);
                if (alpha >= beta) {
                    break;
                }
            }
        }
        return value;
    } else {
        char opponent = (parameter.playerToken == 'X') ? 'O' : 'X';
        int value = 100000;
        for (int col = 0; col < parameter.column; col++) {
            if (is_valid_move(board, col)) {
                int row = make_move(board, col, opponent);
                value = min(value, minimax(board, depth - 1, alpha, beta, 1));
                board[row][col] = ' '; // Undo the move
                beta = min(beta, value);
                if (alpha >= beta) {
                    break;
                }
            }
        }
        return value;
    }
}

/// @brief returns the maximum value
int max(int a, int b) {
    return a > b ? a : b;
}

/// @brief returns the minimum value
int min(int a, int b) {
    return a < b ? a : b;
}