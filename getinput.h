#ifndef GETINPUTC
#define GETINPUTC

// Luke Brinkerhoff 02/16/2018
// getinput.h

// Interface for getinput.c
void run(char **);
int getinput(char **);
int parse(char *, char ***);
int add_history(char *, char ***, int);
void print_history(char ***, int);
int exec_last(char **, char ***, int, int *);

#endif
