#define MAX_IDENT_LEN 10
#define MAX_SYMBOL_TABLE_SIZE 100

typedef enum
{
    NONE = 0,
    IDENT,
    NUMBER,
    BEGIN,
    CALL,
    CONST,
    DO,
    ELSE,
    END,
    FOR,
    IF,
    ODD,
    PROCEDURE,
    PROGRAM,
    THEN,
    TO,
    VAR,
    WHILE,
    PLUS,
    MINUS,
    TIMES,
    SLASH,
    EQU,
    NEQ,
    LSS,
    LEQ,
    GTR,
    GEQ,
    LPARENT,
    RPARENT,
    LBRACK,
    RBRACK,
    PERIOD,
    COMMA,
    SEMICOLON,
    ASSIGN,
    PERCENT
} TokenType;

char TokenTab[][15] = {
    "NONE", "IDENT", "NUMBER",
    "BEGIN", "CALL", "CONST", "DO", "ELSE", "END", "FOR", "IF", "ODD",
    "PROCEDURE", "PROGRAM", "THEN", "TO", "VAR", "WHILE",
    "PLUS", "MINUS", "TIMES", "SLASH", "EQU", "NEQ", "LSS", "LEQ", "GTR", "GEQ", "LPARENT", "RPARENT", "LBRACK", "RBRACK", "PERIOD", "COMMA", "SEMICOLON", "ASSIGN", "PERCENT"};

char Keywords[][10] = {
    "BEGIN", "CALL", "CONST", "DO", "ELSE", "END", "FOR", "IF", "ODD", "PROCEDURE", "PROGRAM", "THEN", "TO", "VAR", "WHILE"};

int KeywordsIdx[] = {
    3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};

typedef enum
{
    CONSTANT = 1,
    VARIABLE,
    PROC
} kind;

typedef enum {
    INTEGER = 1,
    ARRAY,
    SUBPROC 
} type;

typedef struct symbol
{
    char name[MAX_IDENT_LEN+1]; 
    int kind;
    int val;
    int type;
    int offset;
    struct symtab *tbl;
} symbol;

typedef struct symtab 
{
    int width;
    int len;
    symbol table[MAX_SYMBOL_TABLE_SIZE];
    struct symtab *previous;
} symtab;

symtab* tblptr[MAX_SYMBOL_TABLE_SIZE];
int tblptr_index = 0;
