// Luke Brinkerhoff 02/16/2018
// getinput.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int max = 512; // max number of char inputs

//fuction to run
void run(char **argv)
{
    pid_t childpid;
    int status;
    if((childpid = fork()) == -1) {
        perror("error in the fork");
        exit(1);
    } else if (childpid == 0) { // child code
        if(execvp(argv[0], argv) < 0) {
            perror("Exec failed");
            exit(1);
        }
    } else if (childpid != wait(&status)) //parent code
        perror("signal occurred before the child exited");
}

int getinput(char **s)
{
    // Function for getting input from commandline
    // Returns 1 if 'exit' is entered
    // Returns 2 if 'history' is entered
    // Returns 3 if '!!' is entered
    // Returns 4 if '!(#)' is entered
    // Else returns 0
    char temp[max];
    int c;
    int count = 0;
    printf("?:");
    while((c = getchar()) != EOF ) // gets a line from users
    {
        if(c == '\n')
            break;
        if(count < max)
        {
            temp[count] = c;
            count = count + 1;
        }
    }
    if(c == EOF && count == 0)
        return 0;
    temp[count] = '\0';
    *s = temp;
    //check for return conditions
    if(strcmp(temp, "exit") ==0)            
        return 1;
    else if (strcmp(temp, "history") ==0)   
        return 2;
    else if (strcmp(temp, "!!") ==0)      
        return 3;
    else if ( temp[0] == '!')
        return 4;
    else 
        return 0;
}

int parse(char *s, char ***args) 
{
    // Function for parsing the input from the command line
    // returns standard array of pointers using state machine
    // function declerations
    char **pointers = malloc(1 * sizeof(*pointers));
    char temp[max];
    temp[0] = '\0';
    int temppos = 0;
    int token_count = 0;
    int pos = 0;

    int state = 1;
    int exit_flag = 0;

    //enters state machine
    while(exit_flag == 0)
    {
        //State Machine Logic
        char tempchar = s[pos];
        pos = pos + 1;
        if(tempchar == '\0' && (state == 1 || state == 3 || state == 5))
            state = 4;
        else if((tempchar == '\0'|| tempchar == ';') && state == 2 ){
            state = 3;
            pos = pos - 1;
        }
        else if(tempchar == ';' && (state == 3 || state == 1))
            state = 5;
        else if(tempchar == ' ' && (state == 1 || state == 3 || state == 5))
            state = 1;
        else if(tempchar == ' ' && state == 2 )
            state = 3;
        else
            state = 2;
        
        //Debugging
        //printf("tempchar: %c state: %d\n", tempchar, state);

        //State Machine Functionality
        switch(state)
        {
        case 1 : // entry state
            break;  
        case 2 : // get chars until whitespace
            temp[temppos] = tempchar;
            temp[temppos + 1] = '\0';
            temppos  = temppos + 1;
            break;
        case 3 : // a token was found token stored
            token_count = token_count + 1;
            pointers = realloc(pointers, (token_count + 1) * sizeof(*pointers));
            pointers[token_count - 1] = strdup(temp);
            pointers[token_count] = NULL;
            temp[0] = '\0';
            temppos = 0;
            break;
        case 4 : //exit conditions
            exit_flag = 1;
            break;
        case 5 : // a semicolon was so there are multiple commands
            run(pointers);
            pointers = malloc(1 * sizeof(*pointers));
            token_count = 0;
            break;
        }
    }
    *args = pointers; // return data structure
    return token_count;
}

int add_history(char *s, char ***history, int count)
{
    // Function for creating the history data structure used for storing previous commands
    char **temp = *history;
    char *string = s;
    count = count + 1;
    temp = realloc(temp, (count + 1) * sizeof(*temp));
    temp[count - 1] = strdup(string);
    temp[count] = NULL;
    *history = temp;
    return count;
}

void print_history(char ***s, int count)
{
    // Function for priting the history data structure
    char **temp = *s;
    int j, i = 0;
    if(count > 20) {
        i = count - 20;
        j = count;
    } 
    else
        j = count;
    for(i; i < j; i++){
        printf("%d %s\n", i + 1, temp[i]);
    }
}

int exec_last(char **buf, char ***hist, int count, int *shellflag)
{
    // Fuction for executing the last command
    char **history = *hist;
    if(count == 1) {                                    // if first command do nothing
        *shellflag = 1;
        return count;
    }
    if(strcmp(history[count -2 ], "!!") == 0) {         // if last command was '!!' do nothing 
        *shellflag = 1;
        return count;
    }
    if(strcmp(history[count -2 ], "history") == 0) {    // if last command was 'history' run it 
       count = add_history(history[count -2 ], &history, count); 
       print_history(&history, count);
       *shellflag =1;
    }
    else                                                // else set buffer for previous command
    {
        *buf = history[count - 2 ];    
    }
    return count;           
}

int exec_old(char **buf, char ***hist, int count, int *shellflag, int number)
{
    // Function for executing a specific command from the history data structure
    char **history = *hist;
    if(count < 1) {                                  // if command to run is the first or less do 
        *shellflag = 1;                                 // do nothing
        return count;
    }
    if(strcmp(history[number - 1], "!!") == 0) {             // if the command was '!!' do nothing 
        *shellflag = 1;
        return count;
    }
    if(strcmp(history[number- 1], "history") == 0) {        // if the command was 'history' run it 
       count = add_history(history[number - 1], &history, count); 
       print_history(&history, count);
       *shellflag =1;
    }
    else                                                // else set buffer for the command
    {
        *buf = history[number - 1];    
    }
    return count;           
}

int parse_cmdnum(char *s)
{
    int i = 1;
    int j = 0;
    char temp[25];
    while(s[i] != '\0'){
        temp[j] = s[i];    
        temp[j + 1] = '\0';
        j = j + 1;
        i = i + 1;
    }
    i = atoi(temp);
    return i;
}
