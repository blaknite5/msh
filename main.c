// Luke Brinkerhoff 02/16/2018
// msh.c

#include <stdlib.h>
#include <stdio.h>
#include "getinput.h"

int main()
{
    // Variable Declerations
    int argc;
    int history_count = 0;          // number of commands entered
    int inputflag = 0;              // condition flag      
    char **argv;
    char **history = malloc(1 * sizeof(*history));
    char *buf = NULL;

    // While loop makes shell run until exit is typed
    while(1)
    {
        inputflag = getinput(&buf); // inputflag set here 
        argc = parse(buf, &argv);
        history_count = add_history(buf, &history, history_count);

        if(inputflag == 1) {          // exit condition
            free(argv);
            free(history);
            exit(0);
        } else if(inputflag == 2)          // history command
            print_history(&history, history_count);
        else if(inputflag == 3) {
            int exitflag = 0;
            history_count = exec_last(&buf, &history, history_count, &exitflag);
            if(exitflag == 0) {
                argc = parse(buf, &argv);
                history_count = add_history(buf, &history, history_count);
                run(argv);
            }
        } else {
            run(argv);
        }
	}
}

