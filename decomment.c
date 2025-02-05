#include <stdio.h>
#include <stdlib.h>
/* This program performs one of the main functions of the 
preprocessor: removing comments in a C file */

/* enum data type with all 8 states in the DFA */
enum Statetype {START,MAYBECOMMENT,COMMENT,MAYBEEND,DOUBLEQUOTE,
DOUBLEESCAPE,SINGLEQUOTE,SINGLEESCAPE};

/* Implement the start state of the DFA, based on the current 
character c, return the next state and print c if appropriate */
enum Statetype start(int c)
{
    if (c == '/') return MAYBECOMMENT;
    putchar(c); 
    if (c == '\'') return SINGLEQUOTE;
    if (c == '"') return DOUBLEQUOTE;
    return START;
}

/* Implement the maybecomment state of the DFA, based on the current 
character c, return the next state and print c if appropriate */
enum Statetype maybecomment(int c)
{
    if (c == '*') {
        putchar(' '); /* replace with space */
        return COMMENT;
    }
    putchar('/'); /* if not comment, make up the omitted / */
    if (c == '/') return MAYBECOMMENT; /* consecutive /'s */
    putchar(c);
    if (c == '\'') return SINGLEQUOTE;
    if (c == '"') return DOUBLEQUOTE;
    return START;
}

/* Implement the comment state of the DFA, based on the current 
character c, return the next state and print c if appropriate */
enum Statetype comment(int c)
{
    if (c == '*') return MAYBEEND;
    if (c == '\n') putchar(c);
    return COMMENT;
}

/* Implement the maybeend state of the DFA, based on the current 
character c, return the next state and print c if appropriate */
enum Statetype maybeend(int c)
{
    if (c == '/') return START;
    if (c == '*') return MAYBEEND; /* consecutive *'s */
    if (c == '\n') putchar(c);
    return COMMENT;
}

/* Implement the doublequote state of the DFA, based on the current 
character c, return the next state and print c if appropriate */
enum Statetype doublequote(int c)
{
    putchar(c);
    if (c == '"') return START;
    if (c == '\\') return DOUBLEESCAPE;
    return DOUBLEQUOTE;
}

/* Implement the doubleescape state of the DFA, based on the current 
character c, return the next state and print c if appropriate */
enum Statetype doubleescape(int c)
{
    putchar(c);
    return DOUBLEQUOTE;
}

/* Implement the singlequote state of the DFA, based on the current 
character c, return the next state and print c if appropriate */
enum Statetype singlequote(int c)
{
    putchar(c);
    if (c == '\'') return START;
    if (c == '\\') return SINGLEESCAPE;
    return SINGLEQUOTE;
}

/* Implement the singleescape state of the DFA, based on the current 
character c, return the next state and print c if appropriate */
enum Statetype singleescape(int c)
{
    putchar(c);
    return SINGLEQUOTE;
}

/* Read from stdin until the end, switch state according to the DFA 
and print a file with all comments removed, return EXIT_FAILURE if 
there is an unterminated comment and prints the line where it started,
 otherwise return 0 */
int main(void){
    int errorline; /* keep track of potential error line number */
    int currentline = 1; /* keep track of the number of lines read */
    int c; 
    enum Statetype state = START; 
    while ((c = getchar()) != EOF) {
        if (c == '\n') currentline += 1; /* increment if a new line */
        switch (state) {
            case START:
                state = start(c);
                break;
            case MAYBECOMMENT:
                state = maybecomment(c);
                /* if a new comment, update the potential error line 
                number to the current line number */
                if (state == COMMENT) errorline = currentline;
                break;
            case COMMENT:
                state = comment(c);
                break;
            case MAYBEEND:
                state = maybeend(c);
                break;
            case SINGLEQUOTE:
                state = singlequote(c);
                break;
            case SINGLEESCAPE:
                state = singleescape(c);
                break;
            case DOUBLEQUOTE:
                state = doublequote(c);
                break;
            case DOUBLEESCAPE:
                state = doubleescape(c);
                break;
        }
    }
    if (state == COMMENT || state == MAYBEEND) {
        fprintf(stderr, "Error: line %d: unterminated comment\n", 
        errorline);
        return EXIT_FAILURE;
    }
    /* add missing / if ends at MAYBECOMMENT state */
    if (state == MAYBECOMMENT) putchar('/');
    return 0;
}