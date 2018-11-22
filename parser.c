#include "lex.h"
#include <stdlib.h>

void error(const char msg[]);
void program();
void block(void);
void statement(void);
void expression(void);
void condition(void);
void term(void);
void factor(void);

void printToken();

int main(int argc, char *argv[])
{
    if (argc > 1)
        f = fopen(argv[1], "rt");
    else
        f = fopen("test.pl0", "rt");
    Ch = ' ';

    // do
    // {
    //     Token = getToken();
    //     // if (Token == NUMBER)
    //     //     printf("NUMBER (%d)\n", Num);
    //     // if (Token == IDENT)
    //     // {
    //     //     if (strlen(Id) > MAX_IDENT_LEN)
    //     //     {
    //     //         printf("%s is longer than Maximum ident length = %d\n", Id, MAX_IDENT_LEN);
    //     //         printf("ln %d col %d\n", ln, col);
    //     //         return 0;
    //     //     }
    //     //     printf("IDENT (%s)\n", Id);
    //     // }
    //     // else
    //     printf("%s\n", TokenTab[Token]);
    // } while (Token != NONE);

    Token = getToken();
    program();
    fclose(f);
    return 0;
}

void error(const char msg[])
{
    printf("\n%s\n", msg);
    printf("ln %d col %d\n", ln, col);
    exit(0);
}

void printToken()
{
    printf("\n%s\n", TokenTab[Token]);
}

void program()
{
    if (Token == PROGRAM)
    {
        Token = getToken();
        if (Token == IDENT)
        {
            Token = getToken();
            if (Token == SEMICOLON)
            {
                Token = getToken();
                block();
                if (Token == PERIOD || Token == SEMICOLON)
                    printf("\nThanh cong\n");
                else
                    error("Thieu PERIOD");
            }
            else
                error("Thieu SEMICOLON");
        }
        else
            error("Thieu PROGRAM NAME");
    }
    else
        error("Thieu PROGRAM");
}

void block()
{
    if (Token == CONST)
    {
        Token = getToken();
        while (1)
        {
            if (Token == IDENT)
            {
                Token = getToken();
                if (Token == ASSIGN)
                {
                    Token = getToken();
                    if (Token == NUMBER)
                    {
                        Token = getToken();
                        if (Token == COMMA)
                            Token = getToken();
                        else
                        {
                            if (Token == SEMICOLON)
                            {
                                Token = getToken();
                                break;
                            }
                            else
                                error("Thieu SEMICOLON");
                        }
                    }
                    else
                        error("Thieu NUMBER");
                }
                else
                    error("Thieu ASSIGN");
            }
            else
                error("Thieu IDENT");
        }
        block();
    }

    if (Token == VAR)
    {
        Token = getToken();
        while (1)
        {
            if (Token == IDENT)
            {
                Token = getToken();
                if (Token == COMMA)
                    Token = getToken();
                else
                {
                    if (Token == SEMICOLON)
                    {
                        Token = getToken();
                        break;
                    }
                    else
                        error("Thieu SEMICOLON");
                }
            }
        }
        block();
    }

    if (Token == PROCEDURE)
        while (Token == PROCEDURE)
        {
            Token = getToken();
            if (Token == IDENT)
            {
                Token = getToken();
                if (Token == SEMICOLON)
                {
                    Token = getToken();
                    block();
                    if (Token == SEMICOLON)
                    {
                        Token = getToken();
                    }
                    else
                        error("Thieu SEMICOLON");
                }
                else
                    error("Thieu SEMICOLON");
            }
            else
                error("Thieu IDENT");
        }

    if (Token == BEGIN)
    {
        Token = getToken();
        statement();
        while (Token == SEMICOLON)
        {
            Token = getToken();
            statement();
        }
        if (Token == END)
            Token = getToken();
        else
            error("Thieu END");
    }
    // else
    //     error("Thieu BEGIN");
}

void statement()
{
    if (Token == IDENT)
    {
        Token = getToken();
        if (Token == ASSIGN)
        {
            Token = getToken();
            expression();
        }
        else
            error("Thieu toan tu gan");
    }
    else if (Token == CALL)
    {
        Token = getToken();
        if (Token == IDENT)
        {
            Token = getToken();
            if (Token == LPARENT)
            {
                Token = getToken();
                expression();
                while (Token == COMMA)
                {
                    Token = getToken();
                    expression();
                }
                if (Token == RPARENT)
                    Token = getToken();
                else
                    error("Thieu RPARENT");
            }
        }
        else
            error("Thieu PROCEDURE NAME");
    }
    else if (Token == BEGIN)
    {
        Token = getToken();
        statement();
        while (Token == SEMICOLON)
        {
            Token = getToken();
            statement();
        }
        if (Token == END)
            Token = getToken();
        else
            error("Thieu END");
    }
    else if (Token == IF)
    {
        Token = getToken();
        condition();
        printToken();
        if (Token == THEN)
        {
            Token = getToken();
            statement();
            if (Token == ELSE)
            {
                Token = getToken();
                statement();
            }
        }
        else
            error("Thieu THEN");
    }
    else if (Token == WHILE)
    {
        Token = getToken();
        condition();
        if (Token == DO)
        {
            Token = getToken();
            statement();
        }
        else
            error("Thieu DO");
    }
    else if (Token == FOR)
    {
        Token = getToken();
        if (Token == IDENT)
        {
            Token = getToken();
            if (Token == ASSIGN)
            {
                Token = getToken();
                expression();
                Token = getToken();
                if (Token == TO)
                {
                    Token = getToken();
                    expression();
                    if (Token == DO)
                    {
                        Token = getToken();
                        statement();
                    }
                    else
                        error("Thieu DO");
                }
                else
                    error("Thieu TO");
            }
            else
                error("Thieu ASSIGN");
        }
        else
            error("Thieu IDENT");
    }
}

void expression()
{
    if (Token == PLUS || Token == MINUS)
        Token = getToken();
    term();
    while (Token == PLUS || Token == MINUS)
    {
        Token = getToken();
        term();
    }
}

void condition()
{
    if (Token == ODD)
    {
        Token = getToken();
        expression();
    }
    else
    {
        expression();
        if (Token == EQU || Token == NEQ || Token == LSS || Token == LEQ || Token == GTR || Token == GEQ)
        {
            Token = getToken();
            expression();
        }
        else
            error("condition: syntax error");
    }
}

void term()
{
    factor();
    while (Token == TIMES || Token == SLASH)
    {
        Token = getToken();
        factor();
    }
}

void factor()
{
    if (Token == IDENT || Token == NUMBER)
        Token = getToken();
    else if (Token == LPARENT)
    {
        Token = getToken();
        expression();
        if (Token == RPARENT)
            Token = getToken();
        else
            error("Thieu )");
    }
}
