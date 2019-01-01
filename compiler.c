#include "lex.h"
#include <stdlib.h>

void constDeclaration();
void varDeclaration();
void procDeclaration();
int checkIdent(char s[MAX_IDENT_LEN + 1]);
int getParamCount(char s[MAX_IDENT_LEN + 1]);
void setKind(char s[MAX_IDENT_LEN + 1], int kind);
void addParamCount(char s[MAX_IDENT_LEN + 1], int paramCount);
symtab *mktable(symtab *tbl);
void enterproc(symtab *tbl, char name[MAX_IDENT_LEN + 1], symtab *newtbl);
void addWidth(symtab *tbl, int width);
void enter(symtab *tbl, symbol sym);
void push(symtab *tbl);
void pop();
address getAddress(char s[MAX_IDENT_LEN + 1]);
symtab *top();
symtab *findProcTable(char s[MAX_IDENT_LEN + 1], int *depth, int *pc);
void printTable(symtab *tbl);
void printToken();
int checkTableIdent(char s[MAX_IDENT_LEN + 1]);
void printCode(void);
void emit(OpCode op, int p, int q);

void error(int error);
void program();
void block(void);
void statement(void);
void expression(void);
void condition(void);
void term(void);
void factor(void);
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
    printf("\nSuccess \n\n\n");
    // printTable(top());
    printCode();
    fclose(f);
    return 0;
}

void printCode()
{
    int i;
    for (i = 0; i < code_length; ++i)
    {
        if (code[i].op == OP_HL || code[i].op == OP_ST || code[i].op == OP_EP ||
            code[i].op == OP_ADD || code[i].op == OP_MUL || code[i].op == OP_SUB || code[i].op == OP_DIV ||
            code[i].op == OP_EQ || code[i].op == OP_NE || code[i].op == OP_LT || code[i].op == OP_GT || code[i].op == OP_GE || code[i].op == OP_LE)
            printf("%d %s\n", i, op_name[code[i].op]);
        else if (code[i].op == OP_J || code[i].op == OP_INT || code[i].op == OP_DCT || code[i].op == OP_LC || code[i].op == OP_FJ)
            printf("%d %s %d\n", i, op_name[code[i].op], code[i].p);
        else
            printf("%d %s %d %d\n", i, op_name[code[i].op], code[i].p, code[i].q);
    }
}

void emit(OpCode op, int p, int q)
{
    instruction inst;
    inst.op = op;
    inst.p = p;
    inst.q = q;
    code[code_length++] = inst;
}

void printToken()
{
    printf("\n%s\n", TokenTab[Token]);
}

void printTable(symtab *tbl)
{
    int i = 0;
    printf("------------------------------------\n");
    printf("%d %d\n", tbl->len, tbl->width);
    for (i = 0; i < tbl->len; ++i)
    {
        printf("%s %d %d %d\n", tbl->table[i].name, tbl->table[i].kind, tbl->table[i].offset, tbl->table[i].val);
        if (tbl->table[i].kind == PROC)
            printTable(tbl->table[i].tbl);
    }
    printf("------------------------------------\n");
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
    t->width = 4;
    t->len = 0;
    return t;
}

symtab *findProcTable(char s[MAX_IDENT_LEN + 1], int *depth, int *pc)
{
    symtab *tbl = top();
    do
    {
        int i;
        for (i = 0; i < tbl->len; ++i)
            if (tbl->table[i].kind == PROC && strcmp(tbl->table[i].name, s) == 0)
            {
                *pc = tbl->table[i].val;
                return tbl->table[i].tbl;
            }
        tbl = tbl->previous;
        *depth += 1;
    } while (tbl != NULL);
    return top();
}

void enterproc(symtab *tbl, char name[MAX_IDENT_LEN + 1], symtab *newtbl)
{
    symbol sym;
    strcpy(sym.name, name);
    sym.kind = PROC;
    sym.tbl = newtbl;
    sym.offset = top()->width;
    sym.val = code_length;

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

address getAddress(char s[MAX_IDENT_LEN + 1])
{
    symtab *tbl = top();
    address addr;

    int lev = 0;
    do
    {
        int i;
        for (i = 0; i < tbl->len; ++i)
            if (strcmp(tbl->table[i].name, s) == 0)
            {
                addr.level = lev;
                addr.offset = tbl->table[i].offset;
                return addr;
            }
        tbl = tbl->previous;
        ++lev;
    } while (tbl != NULL);
    return addr;
}

int checkTableIdent(char s[MAX_IDENT_LEN + 1])
{
    symtab *tbl = top();
    int i;
    for (i = 0; i < tbl->len; ++i)
        if (strcmp(tbl->table[i].name, s) == 0)
            return 1;
    return 0;
}

void addParamCount(char s[MAX_IDENT_LEN + 1], int paramCount)
{
    symtab *tbl = top()->previous;
    int i;
    for (i = 0; i < tbl->len; ++i)
        if (strcmp(tbl->table[i].name, s) == 0)
            tbl->table[i].type = paramCount;
}

int getParamCount(char s[MAX_IDENT_LEN + 1])
{
    symtab *tbl = top();
    int i;
    for (i = 0; i < tbl->len; ++i)
        if (strcmp(tbl->table[i].name, s) == 0)
            return tbl->table[i].type;
    return 0;
}

void setKind(char s[MAX_IDENT_LEN + 1], int kind)
{
    symtab *tbl = top();
    int i;
    for (i = 0; i < tbl->len; ++i)
        if (strcmp(tbl->table[i].name, s) == 0)
            tbl->table[i].kind = kind;
}

int getKind(char s[MAX_IDENT_LEN + 1])
{
    symtab *tbl = top();
    do
    {
        int i;
        for (i = 0; i < tbl->len; ++i)
            if (strcmp(tbl->table[i].name, s) == 0)
                return tbl->table[i].kind;
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
        sym.offset = top()->width;
        enter(top(), sym);
        top()->width += 2;

        Token = getToken();
    } while (Token == COMMA);

    if (Token != SEMICOLON)
        error(9);
    Token = getToken();
}

void varDeclaration()
{
    symbol sym;
    sym.kind = VARIABLE;
    sym.type = 0;
    char Ident[MAX_IDENT_LEN + 1];

    do
    {
        Token = getToken();
        if (Token != IDENT)
            error(2);
        if (checkTableIdent(Id))
            error(18);

        strcpy(sym.name, Id);
        strcpy(Ident, Id);
        sym.offset = top()->width;
        enter(top(), sym);
        top()->width += 2;

        Token = getToken();
        if (Token == LBRACK)
        {
            Token = getToken();
            if (Token != NUMBER)
                error(8);

            setKind(Ident, ARR);
            top()->width += Num * 2 - 2;

            Token = getToken();
            if (Token != RBRACK)
                error(5);

            Token = getToken();
        }

    } while (Token == COMMA);

    if (Token != SEMICOLON)
        error(9);
    Token = getToken();
}

void procDeclaration()
{
    symbol sym;
    sym.kind = PROC;
    char Ident[MAX_IDENT_LEN + 1];
    sym.val = code_length;
    do
    {
        Token = getToken();
        if (Token != IDENT)
            error(2);
        if (checkIdent(Id))
            error(18);

        strcpy(Ident, Id);
        table = mktable(top());

        enterproc(top(), Id, table);
        push(table);
        Token = getToken();

        int paramCount = 0;

        if (Token == LPARENT)
        {
            Token = getToken();
            if (Token != VAR && Token != IDENT)
                error(10);
            while (Token == VAR || Token == IDENT)
            {
                ++paramCount;
                if (Token == VAR)
                {
                    Token = getToken();
                    if (Token != IDENT)
                        error(2);

                    // if (checkIdent(Id) == 1)
                    //     error(18);
                    sym.kind = VARIABLE;
                    sym.type = 0;
                    strcpy(sym.name, Id);
                    sym.offset = top()->width;
                    enter(top(), sym);
                    top()->width += 2;

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

                    sym.kind = VARIABLE;
                    sym.type = 1;
                    strcpy(sym.name, Id);
                    sym.offset = top()->width;
                    enter(top(), sym);
                    top()->width += 2;

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
        addParamCount(Ident, paramCount);
        if (Token != SEMICOLON)
            error(9);
        Token = getToken();
        block();
        if (Token != SEMICOLON)
            error(9);
        int width = top()->width;
        pop();
        // top()->width += width;
        Token = getToken();
        emit(OP_EP, 0, 0);
    } while (Token == PROCEDURE);
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
    emit(OP_HL, 0, 0);
}

//checked
void block()
{
    int idx = code_length;
    emit(OP_J, 0, 0);
    if (Token == CONST)
        constDeclaration();

    if (Token == VAR)
        varDeclaration();

    if (Token == PROCEDURE)
        procDeclaration();

    if (Token != BEGIN)
        error(14);
    code[idx].p = code_length;
    emit(OP_INT, 4 + (top()->width - 4)/2, 0);
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
    char Ident[MAX_IDENT_LEN + 1];

    if (Token == IDENT)
    {
        if (!checkIdent(Id))
            error(19);
        int Type = getKind(Id);

        address addr = getAddress(Id);

        emit(OP_LA, addr.level, addr.offset);

        Token = getToken();
        if (Token == LBRACK)
        {
            if (Type == CONSTANT || Type == VARIABLE)
                error(22);
            Token = getToken();
            expression();
            if (Token != RBRACK)
                error(5);
            Token = getToken();
            Type = VARIABLE;
        }
        else if (Type == ARR)
            error(22);

        if (Token != ASSIGN)
            error(6);
        if (Type != VARIABLE)
            error(15);
        Token = getToken();
        expression();
        emit(OP_ST, 0, 0);
    }
    else if (Token == CALL)
    {
        emit(OP_INT, 4, 0);
        Token = getToken();
        if (Token != IDENT)
            error(2);
        if (!checkIdent(Id))
            error(24);
        symtab *tbl;
        int p = 0;
        int q = 0;
        tbl = findProcTable(Id, &p, &q);
        Token = getToken();
        if (!checkIdent(Id))
            error(19);
        if (getKind(Id) != 3)
            error(20);
        int paramCount = 0;

        strcpy(Ident, Id);
        if (Token == LPARENT)
        {
            Token = getToken();
            if (tbl->table[paramCount].type == 0)
            {
                if (Token != IDENT)
                    error(2);
                address addr = getAddress(Id);
                int idx = code_length;
                int Type = getKind(Id);
                emit(OP_LA, addr.level, addr.offset);
                Token = getToken();
                if (Token == LBRACK)
                {
                    if (Type == VARIABLE || Type == CONSTANT)
                        error(22);
                    Token = getToken();
                    expression();
                    if (Token != RBRACK)
                        error(5);
                    Token = getToken();
                }
            }
            else
                expression();
            ++paramCount;
            while (Token == COMMA)
            {
                Token = getToken();
                if (tbl->table[paramCount].type == 0)
                {
                    if (Token != IDENT)
                        error(2);
                    address addr = getAddress(Id);
                    int idx = code_length;
                    int Type = getKind(Id);
                    emit(OP_LA, addr.level, addr.offset);
                    Token = getToken();
                    if (Token == LBRACK)
                    {
                        if (Type == VARIABLE || Type == CONSTANT)
                            error(22);
                        Token = getToken();
                        expression();
                        if (Token != RBRACK)
                            error(5);
                        Token = getToken();
                    }
                }
                else
                    expression();
                ++paramCount;
            }
            if (Token != RPARENT)
                error(3);
            Token = getToken();
        }

        if (getParamCount(Ident) != paramCount)
            error(21);
        emit(OP_DCT, 4 + paramCount, 0);
        emit(OP_CALL, p, q);
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
        int idx = code_length;
        emit(OP_FJ, 0, 0);
        if (Token != THEN)
            error(16);
        Token = getToken();
        statement();

        code[idx].p = code_length;
        if (Token == ELSE)
        {
            idx = code_length;
            emit(OP_J, 0, 0);
            Token = getToken();
            statement();
            code[idx].p = code_length;
        }
    }
    else if (Token == WHILE)
    {
        int pos = code_length;
        Token = getToken();
        condition();
        int idx = code_length;
        emit(OP_FJ, 0, 0);
        if (Token != DO)
            error(17);
        Token = getToken();
        statement();
        emit(OP_J, pos, 0);
        code[idx].p = code_length;
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
    int Op = -1;
    if (Token == PLUS || Token == MINUS)
    {
        Op = Token;
        Token = getToken();
    }
    term();
    if (Op != -1)
    {
        if (Op == PLUS)
            emit(OP_ADD, 0, 0);
        else
            emit(OP_SUB, 0, 0);
    }
    while (Token == PLUS || Token == MINUS)
    {
        Op = Token;
        Token = getToken();
        term();
        if (Op == PLUS)
            emit(OP_ADD, 0, 0);
        else
            emit(OP_SUB, 0, 0);
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
            int Op = Token;
            Token = getToken();
            expression();
            switch (Op)
            {
            case EQU:
                emit(OP_EQ, 0, 0);
                break;
            case NEQ:
                emit(OP_NE, 0, 0);
                break;
            case LSS:
                emit(OP_LT, 0, 0);
                break;
            case LEQ:
                emit(OP_LE, 0, 0);
                break;
            case GTR:
                emit(OP_GT, 0, 0);
                break;
            case GEQ:
                emit(OP_GE, 0, 0);
                break;
            }
        }
        else
            error(25);
    }
}

//checked
void term()
{
    factor();
    while (Token == TIMES || Token == SLASH)
    {
        int Op = Token;
        Token = getToken();
        factor();
        if (Op == TIMES)
            emit(OP_MUL, 0, 0);
        else
            emit(OP_DIV, 0, 0);
    }
}

//checked
void factor()
{
    if (Token == IDENT)
    {
        if (!checkIdent(Id))
            error(19);
        int Type = getKind(Id);
        address addr = getAddress(Id);
        emit(OP_LV, addr.level, addr.offset);
        Token = getToken();
        if (Token == LBRACK)
        {
            if (Type == VARIABLE || Type == CONSTANT)
                error(22);
            Token = getToken();
            expression();
            if (Token != RBRACK)
                error(5);
            Token = getToken();
        }
        else if (Type == ARR)
            error(22);
    }
    else if (Token == NUMBER)
    {
        emit(OP_LC, Num, 0);
        Token = getToken();
    }
    else if (Token == LPARENT)
    {
        Token = getToken();
        expression();
        if (Token == RPARENT)
            Token = getToken();
        else
            error(3);
    }
    else
        error(23);
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
        printf("Can only assign VARIABLE");
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
    case 20:
        printf("Can only call PROCEDURE");
        break;
    case 21:
        printf("Wrong number of params");
        break;
    case 22:
        printf("Type not compatible");
        break;
    case 23:
        printf("Expecting an expression");
        break;
    case 24:
        printf("Proc is not declared");
        break;
    case 25:
        printf("Dont understand notation");
        break;
    }
    printf("\n");
    exit(0);
}