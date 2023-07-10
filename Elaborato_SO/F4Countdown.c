/************************************
*Matricola --> VR457793
*Nome e cognome --> Vittorio Maria Stano
*Data di realizzazione --> 14/06/2023
*************************************/

/// @file F4Countdown.c
/// @brief functions for countdown management

#include "err_exit.h"
#include "function.h"

#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int errnoValue;

    // signalArray   ->   [0]SIGINT, [1]SIGUSR1, [2]SIGUSR2, [3]SIGALRM, [4]SIGTERM, [5]SIGABRT, [6]SIGHUP} //
    bool signalArray[] = {false, false, false, true, false, false, false};
    setSignalSet(signalArray);

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // BEGIN // ----------------------------------- /* SIGNAL MANAGEMENT */ -------------------------------- // BEGIN //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //

    if(signal(SIGALRM, nothing) == SIG_ERR){
        errnoValue = errno;
        printf(BOLDRED "SIGALRM failed" RESET);
        errnoCheck(errnoValue);
    }

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////// //
    // END // ------------------------------------- /* SIGNAL MANAGEMENT */ ---------------------------------- // END //
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////  //

    // defined in function.h --> time to enter the token
    int time = TIME_INSERT_TOKEN;

    writePid(COUNTDOWN, getpid());

    // countdown cycle --> display on xterm
    while(time > 0) {

        alarm(1);
        system("clear");

        if (time < 10){
            printf(RED "time left to insert token\n" RESET);
            printf(BOLDRED "           %d\n" RESET, time);
        } else if(time < 45) {
            printf(YELLOW "time left to insert token\n" RESET);
            printf(BOLDYELLOW "           %d\n" RESET, time);
        }
        else if(time < 90){
            printf(MAGENTA "time left to insert token\n" RESET);
            printf(BOLDMAGENTA "           %d\n" RESET, time);
        }
        else if(time < 135){
            printf(BLUE "time left to insert token\n" RESET);
            printf(BOLDBLUE "           %d\n" RESET, time);
        }else{
            printf("time left to insert token\n");
            printf("           %d\n", time);
        }

		if(pause() == -1){
            time --;
        }
	}

    exit(0);
}