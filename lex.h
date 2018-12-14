#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "token.h"

TokenType Token;
int Num;
char Id[MAX_IDENT_LEN + 1];
FILE *f;
char Ch;
int ln = 1, col = 0;

TokenType checkKeyword(char *Id)
{
    int i;
    for (i = 0; i < 15; ++i)
        if (strcmp(Id, Keywords[i]) == 0)
            return KeywordsIdx[i];
    return IDENT;
}

char getCh()
{
    char x = fgetc(f);
    if (x == -1)
        printf("\n");
    else
    {
        if (x == '\t')
            printf("    ");
        else
            printf("%c", x);
    }
    if (x == '\t')
        col += 4;
    else
        ++col;
    if (x == '\n')
    {
        ++ln;
        col = 0;
    }
    return toupper(x);
}

TokenType getToken()
{
    while (Ch == ' ' || Ch == '\t' || Ch == '\n')
        Ch = getCh();
    if (isalpha(Ch))
    {
        int l = 0;
        do
        {
            Id[l++] = Ch;
            Ch = getCh();
        } while (isalpha(Ch) || isdigit(Ch));
        Id[l] = '\0';
        return checkKeyword(Id);
    }
    else if (isdigit(Ch))
    {
        Num = 0;
        do
        {
            Num = 10 * Num + Ch - 48;
            Ch = getCh();
        } while (isdigit(Ch));
        return NUMBER;
    }
    else if (Ch == '+')
    {
        Ch = getCh();
        return PLUS;
    }
    else if (Ch == '-')
    {
        Ch = getCh();
        return MINUS;
    }
    else if (Ch == '*')
    {
        Ch = getCh();
        return TIMES;
    }
    else if (Ch == '/')
    {
        Ch = getCh();
        return SLASH;
    }
    else if (Ch == '/')
    {
        Ch = getCh();
        return SLASH;
    }
    else if (Ch == '%')
    {
        Ch = getCh();
        return PERCENT;
    }
    else if (Ch == '=')
    {
        Ch = getCh();
        return EQU;
    }
    else if (Ch == '<')
    {
        Ch = getCh();
        if (Ch == '>')
        {
            Ch = getCh();
            return NEQ;
        }
        else if (Ch == '=')
        {
            Ch = getCh();
            return LEQ;
        }
        else
            return LSS;
    }
    else if (Ch == '>')
    {
        Ch = getCh();
        if (Ch == '=')
        {
            Ch = getCh();
            return GEQ;
        }
        else
            return GTR;
    }
    else if (Ch == '(')
    {
        Ch = getCh();
        return LPARENT;
    }
    else if (Ch == ')')
    {
        Ch = getCh();
        return RPARENT;
    }
    else if (Ch == '[')
    {
        Ch = getCh();
        return LBRACK;
    }
    else if (Ch == ']')
    {
        Ch = getCh();
        return RBRACK;
    }
    else if (Ch == '.')
    {
        Ch = getCh();
        return PERIOD;
    }
    else if (Ch == ',')
    {
        Ch = getCh();
        return COMMA;
    }
    else if (Ch == ';')
    {
        Ch = getCh();
        return SEMICOLON;
    }
    else if (Ch == ':')
    {
        Ch = getCh();
        if (Ch == '=')
        {
            Ch = getCh();
            return ASSIGN;
        }
        else
            return NONE;
    }
    else
        return NONE;
}

// int main(int argc, char *argv[])
// {
//     if (argc > 1)
//         f = fopen(argv[1], "rt");
//     else
//         f = fopen("test.pl0", "rt");
//     Ch = ' ';
//     do
//     {
//         Token = getToken();
//         if (Token == NUMBER)
//             printf("NUMBER (%d)\n", Num);
//         if (Token == IDENT)
//         {
//             if (strlen(Id) > MAX_IDENT_LEN)
//             {
//                 printf("%s is longer than Maximum ident length = %d\n", Id, MAX_IDENT_LEN);
//                 printf("ln %d col %d\n", ln, col);
//                 return 0;
//             }
//             printf("IDENT (%s)\n", Id);
//         }
//         else
//             printf("%s\n", TokenTab[Token]);
//     } while (Token != NONE);
//     printf("ln %d col %d\n", ln, col);
//     fclose(f);
//     return 0;
// }