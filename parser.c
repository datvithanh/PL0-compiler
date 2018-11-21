#include "lex.h"

void error(const char msg[]);
void program();
void block(void);
void statement(void);
void expression(void);
void condition(void);
void term(void);
void factor(void);

int main(int argc, char *argv[])
{
    printf("sdajdkaksdkk ear \n");
    if (argc > 1)
        f = fopen(argv[1], "rt");
    else
        f = fopen("test.pl0", "rt");
    Ch = ' ';
    do
    {
        Token = getToken();
        // if (Token == NUMBER)
        //     printf("NUMBER (%d)\n", Num);
        // if (Token == IDENT)
        // {
        //     if (strlen(Id) > MAX_IDENT_LEN)
        //     {
        //         printf("%s is longer than Maximum ident length = %d\n", Id, MAX_IDENT_LEN);
        //         printf("ln %d col %d\n", ln, col);
        //         return 0;
        //     }
        //     printf("IDENT (%s)\n", Id);
        // }
        // else
        printf("%s\n", TokenTab[Token]);
    } while (Token != NONE);
    printf("ln %d col %d\n", ln, col);
    fclose(f);
    return 0;
}

void error(const char msg[])
{
    printf("%s\n", msg);
}

void Program()
{
    if (Token == PROGRAM)
    {
        Token = getToken();
        if (Token == IDENT)
        {
            getToken();
            if (Token == SEMICOLON)
            {
                getToken();
                block();
                if (Token == PERIOD)
                    printf("Thành công");
                else
                    error("Thiếu dấu.");
            }
            else
                error("Thiếu dấu chấm phẩy");
        }
        else
            error("Thiếu tên chương trình");
    }
    else
        error("Thiếu từ khóa Program");
}

void block()
{
    if (Token == CONST)
    {
        Token = getToken();
        while (Token == IDENT)
        {   
            
        }
    }
}

void statement()
{
    if (Token == IDENT)
    {
        Token = getToken();
        if (Token == ASSIGN)
        {
            getToken();
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
                    getToken();
                else
                    error("Thiếu dấu đóng ngoặc");
            }
        }
        else
            error("Thiếu tên thủ tục / hàm");
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
            error("Thiếu từ khóa End");
    }
    else if (Token == IF)
    {
        Token = getToken();
        condition();
        if (Token == THEN)
            statement();
        else
            error("Thiếu THEN");
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
            error("Thiếu DO");
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
    expression();
    if (Token == EQU || Token == NEQ || Token == LSS || Token == LEQ || Token == GTR || Token == GEQ)
    {
        Token = getToken();
        expression();
    }
    else
        error("condition: syntax error");
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
