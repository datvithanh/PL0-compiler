#include "lex.h"
#include <stdlib.h>

void constDeclaration();
void varDeclaration();
void procDeclaration();
int checkIdent(char s[MAX_IDENT_LEN + 1]);
symtab *mktable(symtab *tbl);
void enterproc(symtab *tbl, char name[MAX_IDENT_LEN + 1], symtab *newtbl);
void addWidth(symtab *tbl, int width);
void enter(symtab *tbl, symbol sym);
void push(symtab *tbl);
void pop();
symtab *top();
void printTable(symtab *tbl);

void error(int error);
void program();
void block(void);
void statement(void);
void expression(void);
void condition(void);
void term(void);
void factor(void);
void printToken();
symtab *table;
int main(int argc, char *argv[])
{
    if (argc > 1)
        f = fopen(argv[1], "rt");
    else
        f = fopen("test.pl0", "rt");
    Ch = ' ';
    Token = getToken();

    push(mktable(NULL));

    program();
    printf("\nSuccess \n");
    fclose(f);
    return 0;
}

void printTable(symtab *tbl)
{
    int i = 0;
    printf("%d\n", tbl->len);
    for (i = 0; i < tbl->len; ++i)
        printf("%s %d  ", tbl->table[i].name, tbl->table[i].kind);
    printf("\n");
}

symtab *top()
{
    return tblptr[tblptr_index - 1];
}

void push(symtab *tbl)
{
    tblptr[tblptr_index] = tbl;
    ++tblptr_index;
}

void pop()
{
    --tblptr_index;
}

symtab *mktable(symtab *to)
{
    symtab *t;
    t = (struct symtab *)malloc(sizeof(struct symtab));
    t->previous = to;
    t->width = 0;
    t->len = 0;
    return t;
}

void enterproc(symtab *tbl, char name[MAX_IDENT_LEN + 1], symtab *newtbl)
{
    symbol sym;
    strcpy(sym.name, name);
    sym.kind = PROC;
    sym.tbl = newtbl;

    tbl->table[tbl->len] = sym;
    tbl->len++;
}

void addWidth(symtab *tbl, int width)
{
    tbl->width = width;
}

void enter(symtab *tbl, symbol sym)
// char name[MAX_IDENT_LEN + 1], int kind, int type, int offset)
{
    // symbol sym;
    // strcpy(sym.name, name);
    // sym.offset = offset;
    // sym.type = type;
    // sym.tbl = NULL;
    // sym.val = 0;

    tbl->table[tbl->len] = sym;
    tbl->len++;
}

int checkIdent(char s[MAX_IDENT_LEN + 1])
{
    symtab *tbl = top();
    do
    {
        int i;
        for (i = 0; i < tbl->len; ++i)
            if (strcmp(tbl->table[i].name, s) == 0)
                return 1;
        tbl = tbl->previous;
    } while (tbl != NULL);
    return 0;
}

void constDeclaration()
{
    symbol sym;
    sym.kind = CONSTANT;
    do
    {
        Token = getToken();
        if (Token != IDENT)
            error(2);
        if (checkIdent(Id))
            error(18);
        strcpy(sym.name, Id);

        Token = getToken();
        if (Token != EQU)
            error(1);

        Token = getToken();
        if (Token != NUMBER)
            error(8);
        sym.val = Num;

        Token = getToken();

        enter(top(), sym);
    } while (Token == COMMA);

    if (Token != SEMICOLON)
        error(9);
    Token = getToken();
}

void varDeclaration()
{
    symbol sym;
    sym.kind = VARIABLE;

    do
    {
        Token = getToken();
        if (Token != IDENT)
            error(2);
        if (checkIdent(Id))
            error(18);
        strcpy(sym.name, Id);

        Token = getToken();
        if (Token == LBRACK)
        {
            Token = getToken();
            if (Token != NUMBER)
                error(8);

            Token = getToken();
            if (Token != RBRACK)
                error(5);

            Token = getToken();
        }

        enter(top(), sym);
    } while (Token == COMMA);

    if (Token != SEMICOLON)
        error(9);
    Token = getToken();
}

void procDeclaration()
{
    symbol sym;
    sym.kind = PROC;
    do
    {

        Token = getToken();
        if (Token != IDENT)
            error(2);
        if (checkIdent(Id))
            error(18);
        strcpy(sym.name, Id);

        table = mktable(top());
        enterproc(top(), Id, table);
        push(table);
        Token = getToken();
        if (Token == LPARENT)
        {
            Token = getToken();
            if (Token != VAR && Token != IDENT)
                error(10);
            while (Token == VAR || Token == IDENT)
            {
                if (Token == VAR)
                {
                    Token = getToken();
                    if (Token != IDENT)
                        error(2);
                    if (checkIdent(Id) == 1)
                        error(18);
                    Token = getToken();
                    if (Token == SEMICOLON)
                        Token = getToken();
                    else if (Token == RPARENT)
                    {
                        Token = getToken();
                        break;
                    }
                }
                else
                {
                    if (Token != IDENT)
                        error(2);
                    if (checkIdent(Id) == 0)
                        error(19);
                    Token = getToken();
                    if (Token == SEMICOLON)
                        Token = getToken();
                    else if (Token == RPARENT)
                    {
                        Token = getToken();
                        break;
                    }
                }
            }
        }
        if (Token != SEMICOLON)
            error(9);
        Token = getToken();
        block();
        if (Token != SEMICOLON)
            error(9);
        pop();
        Token = getToken();

    } while (Token == PROCEDURE);
}

void printToken()
{
    printf("\n%s\n", TokenTab[Token]);
}

void program()
{
    if (Token != PROGRAM)
        error(11);

    Token = getToken();
    if (Token != IDENT)
        error(12);

    Token = getToken();
    if (Token != SEMICOLON)
        error(9);

    Token = getToken();
    block();
    if (Token != PERIOD)
        error(13);
}

//checked
void block()
{
    if (Token == CONST)
        constDeclaration();

    if (Token == VAR)
        varDeclaration();

    if (Token == PROCEDURE)
        procDeclaration();

    if (Token != BEGIN)
        error(14);
    Token = getToken();
    statement();
    while (Token == SEMICOLON)
    {
        Token = getToken();
        statement();
    }
    if (Token != END)
        error(7);

    Token = getToken();
}

//checked
void statement()
{
    if (Token == IDENT)
    {
        if (!checkIdent(Id))
            error(19);
        Token = getToken();
        if (Token == LBRACK)
        {
            Token = getToken();
            expression();
            if (Token != RBRACK)
                error(5);
            Token = getToken();
        }
        if (Token != ASSIGN)
            error(6);
        Token = getToken();
        expression();
    }
    else if (Token == CALL)
    {
        Token = getToken();
        if (Token != IDENT)
            error(2);
        if (!checkIdent(Id))
            error(19);
        Token = getToken();
        if (!checkIdent(Id))
            error(19);
        if (Token == LPARENT)
        {
            Token = getToken();
            expression();
            while (Token == COMMA)
            {
                Token = getToken();
                expression();
            }
            if (Token != RPARENT)
                error(3);
            Token = getToken();
        }
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
        if (Token != END)
            error(7);
        Token = getToken();
    }
    else if (Token == IF)
    {
        Token = getToken();
        condition();
        if (Token != THEN)
            error(16);

        Token = getToken();
        statement();
        if (Token == ELSE)
        {
            Token = getToken();
            statement();
        }
    }
    else if (Token == WHILE)
    {
        Token = getToken();
        condition();
        if (Token != DO)
            error(17);
        Token = getToken();
        statement();
    }
    else if (Token == FOR)
    {
        Token = getToken();
        if (Token != IDENT)
            error(2);
        if (!checkIdent(Id))
            error(19);
        Token = getToken();
        if (Token != ASSIGN)
            error(6);

        Token = getToken();
        expression();

        Token = getToken();
        if (Token != TO)
            error(4);

        Token = getToken();
        expression();
        if (Token != DO)
            error(17);

        Token = getToken();
        statement();
    }
}

//checked
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

//checked
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
            error(18);
    }
}

//checked
void term()
{
    factor();
    while (Token == TIMES || Token == SLASH)
    {
        Token = getToken();
        factor();
    }
}

//checked
void factor()
{
    if (Token == IDENT)
        if (!checkIdent(Id))
            error(19);
    if (Token == IDENT || Token == NUMBER)
        Token = getToken();
    else if (Token == LPARENT)
    {
        Token = getToken();
        expression();
        if (Token == RPARENT)
            Token = getToken();
        else
            error(3);
    }
}

void error(int error)
{
    printf("\nError line %d, row %d\n", ln, col);

    switch (error)
    {
    case 1:
        printf("Expecting =");
        break;
    case 2:
        printf("Expecting IDENT");
        break;
    case 3:
        printf("Expecting )");
        break;
    case 4:
        printf("Expecting TO");
        break;
    case 5:
        printf("Expecting ]");
        break;
    case 6:
        printf("Expecting :=");
        break;
    case 7:
        printf("Expecting END");
        break;
    case 8:
        printf("Expecting NUMBER");
        break;
    case 9:
        printf("Expecting ;");
        break;
    case 10:
        printf("Expecting VAR or IDENT");
        break;
    case 11:
        printf("Expecting PROGRAM");
        break;
    case 12:
        printf("Expecting PROGRAM NAME");
        break;
    case 13:
        printf("Expecting .");
        break;
    case 14:
        printf("Expecting BEGIN");
        break;
    case 15:
        printf("Const is immutable");
        break;
    case 16:
        printf("Expecting THEN");
        break;
    case 17:
        printf("Expecting DO");
        break;
    case 18:
        printf("Variable name duplicated");
        break;
    case 19:
        printf("Variable is not declared");
        break;
    }
    printf("\n");
    exit(0);
}