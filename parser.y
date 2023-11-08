%token KW_IF
%token KW_ELSE
%token KW_INT
%token KW_BEGIN

%token IDENTIFIER
%token LIT_INT

%%
    
program: ldec
;

ldec: vardec ldec

