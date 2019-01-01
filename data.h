#define MAX_IDENT_LEN 10
#define MAX_SYMBOL_TABLE_SIZE 1000
#define CODE_SIZE 1000

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

char op_name[][6] = {"None", "LA", "LV", "LC", "LI", "INT", "DCT", "J", "FJ", "HL", "ST", "CALL", "EP", "EF", "RC", "RI", "WRC", "WRI", "WLN", "ADD", "SUB", "MUL", "DIV", "NEG", "CV", "EQ", "NE", "GT", "LT", "GE", "LE", "BP"};
typedef enum
{
    OP_LA = 1, // Load Address:
    OP_LV,     // Load Value:
    OP_LC,     // load Constant
    OP_LI,     // Load Indirect
    OP_INT,    // Increment t
    OP_DCT,    // Decrement t
    OP_J,      // Jump
    OP_FJ,     // False Jump
    OP_HL,     // Halt
    OP_ST,     // Store
    OP_CALL,   // Call
    OP_EP,     // Exit Procedure
    OP_EF,     // Exit Function
    OP_RC,     // Read Char
    OP_RI,     // Read Integer
    OP_WRC,    // Write Char
    OP_WRI,    // Write Int
    OP_WLN,    // WriteLN
    OP_ADD,    // Add
    OP_SUB,    // Substract
    OP_MUL,    // Multiple
    OP_DIV,    // Divide
    OP_NEG,    // Negative
    OP_CV,     // Copy Top
    OP_EQ,     // Equal
    OP_NE,     // Not Equal
    OP_GT,     // Greater
    OP_LT,     // Less
    OP_GE,     // Greater or Equal
    OP_LE,     // Less or Equal
    OP_BP      // Break point.
} OpCode;

typedef struct
{
    int op;
    int p;
    int q;
} instruction;

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
    PROC,
    ARR
} kind;

typedef enum
{
    INTEGER = 1,
    ARRAY,
    SUBPROC
} type;

typedef struct symbol
{
    char name[MAX_IDENT_LEN + 1];
    int kind;
    int val;
    int type;
    int offset;
    // int isVar[5];
    struct symtab *tbl;
} symbol;

typedef struct symtab
{
    int width;
    int len;
    symbol table[MAX_SYMBOL_TABLE_SIZE];
    struct symtab *previous;
} symtab;

symtab *tblptr[MAX_SYMBOL_TABLE_SIZE];
int tblptr_index = 0;
instruction code[CODE_SIZE];
int code_length = 0;
// TokenType Op;

typedef struct {
    int level;
    int offset;
} address;