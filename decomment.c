#include <stdio.h>
#include <stdlib.h>

enum Statetype {START,MAYBECOMMENT,COMMENT,MAYBEEND,DOUBLEQUOTE,
DOUBLEESCAPE,SINGLEQUOTE,SINGLEESCAPE};

enum Statetype start(int c)
{
    if (c == '/') return MAYBECOMMENT;
    putchar(c);
    if (c == '\'') return SINGLEQUOTE;
    if (c == '"') return DOUBLEQUOTE;
    return START;
}

enum Statetype maybecomment(int c)
{
    if (c == '*') return COMMENT;
    putchar('/');
    putchar(c);
    if (c == '\'') return SINGLEQUOTE;
    if (c == '"') return DOUBLEQUOTE;
    return START;
}

enum Statetype comment(int c)
{
    if (c == '*') return MAYBEEND;
    if (c == '\n') putchar(c);
    return COMMENT;
}

enum Statetype maybeend(int c)
{
    if (c == '/') return START;
    if (c == '\n') putchar(c);
    return COMMENT;
}

enum Statetype doublequote(int c)
{
    putchar(c);
    if (c == '"') return START;
    if (c == '\\') return DOUBLEESCAPE;
    return DOUBLEQUOTE;
}

enum Statetype doubleescape(int c)
{
    putchar(c);
    return DOUBLEQUOTE;
}

enum Statetype singlequote(int c)
{
    putchar(c);
    if (c == '\'') return START;
    if (c == '\\') return SINGLEESCAPE;
    return SINGLEQUOTE;
}

enum Statetype singleescape(int c)
{
    putchar(c);
    return SINGLEESCAPE;
}

int main(void){
    int errorline = 0;
    int currentline = 0;
    int c;
    enum Statetype state = START;
    while (c = getchar() != EOF) {
        if (c == '\\') currentline += 1;
        switch (state) {
            case START:
                state = start(c);
                break;
            case MAYBECOMMENT:
                state = maybecomment(c);
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
        fprintf(stderr, "Error: line %d: unterminated comment%n", errorline);
        return EXIT_FAILURE;
    }
    return 0;
}