********************************
Luke Brinkerhoff 02/16/2018

To compile msh use
    gcc msh.c getinput.c -o msh

This program is the beginning of a custom shell called myshell. At the moment
it only takes simple input from the user and then parses that input to get the
command and options. This command is then ran and the user is again prompted for
input. The prompt for the user is to input is "?:". The shell will continue to 
run until the user types "exit".

Working commands include: ls, ps, top, nano, vim.

Non Working Commands include: cd.
