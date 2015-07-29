#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#include <stdlib.h>
#include <string.h>

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20090221

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
#ifdef YYPARSE_PARAM_TYPE
#define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
#else
#define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
#endif
#else
#define YYPARSE_DECL() yyparse(void)
#endif /* YYPARSE_PARAM */

extern int YYPARSE_DECL();

static int yygrowstack(void);
#define YYPREFIX "yy"
#line 22 "MdlParser.yy"

#include <stdio.h>
#include <stdlib.h>
#include <wx/string.h>

#include "Expression.h"

#define YYDEBUG 1

#define YYERROR_VERBOSE

int yyerror ( char* s);
extern int yylex( void );
extern int yylineno;
extern char *yytext;

Rule *rule = 0;
wxString parseErrorMessage;

#line 43 "MdlParser.yy"
typedef union {
  int ival;
	char *sval;
	double dval;
	Expression *expval;
} YYSTYPE;
#line 61 "y.tab.c"
#define LPAR 257
#define RPAR 258
#define SEMICOLON 259
#define COMMA 260
#define PERCENT 261
#define DIGIT 262
#define OR 263
#define AND 264
#define NOT 265
#define MUTUAL_KENDRA 266
#define DESCR 267
#define LORDOF 268
#define ISPLANETINHOUSE 269
#define ISPLANETINRASI 270
#define ISEQUAL 271
#define ISNOTEQUAL 272
#define RETURN 273
#define PLANET 274
#define GETRASI 275
#define GETBHAVA 276
#define PLUS 277
#define MINUS 278
#define MULT 279
#define DIV 280
#define MOD 281
#define LT 282
#define LE 283
#define GT 284
#define GE 285
#define PLANETS_INSIGN 286
#define PLANETS_INHOUSE 287
#define NUMBER_OF_OCCUPIED_SIGNS 288
#define GRAHA_DRISHTI 289
#define GETLORD 290
#define ISBENEFIC 291
#define ISMALEFIC 292
#define ISPLANETINKENDRA 293
#define ISPLANETINAPOKLIMA 294
#define ISPLANETINPANAPHARA 295
#define ISPLANETINDUALRASI 296
#define ISPLANETINFIXEDRASI 297
#define ISPLANETINMOVABLERASI 298
#define RED12 299
#define GETNAKSHATRA 300
#define RED27 301
#define RASIONLY 302
#define HOUSE 303
#define SIGN 304
#define NAKSHATRA 305
#define BOOLEAN 306
#define TYPE_INT 307
#define TYPE_DOUBLE 308
#define TYPE_PLANET 309
#define TYPE_SIGN 310
#define TYPE_NAKSHATRA 311
#define TYPE_HOUSE 312
#define TYPE_BOOLEAN 313
#define IDENT 314
#define STRING 315
#define YYERRCODE 256
static const short yylhs[] = {                           -1,
   46,    0,   45,   45,   47,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    2,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,   35,   36,    5,    9,   22,   10,   19,   20,   37,
   23,   24,   25,   26,   27,   32,   33,   34,   38,   39,
   40,   41,   42,   43,   44,    4,    4,    4,    4,    4,
    4,    4,    4,    4,    4,    4,    4,    4,    4,    4,
    4,    6,    7,   11,   12,   13,   14,   16,   15,   17,
   28,   29,   30,   31,   18,    8,   21,
};
static const short yylen[] = {                            2,
    0,    2,    1,    2,    2,    1,    1,    1,    3,    3,
    1,    1,    1,    1,    1,    1,    2,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    2,    4,    4,    4,    4,    4,    4,    4,    4,
    4,    4,    4,    4,    4,    4,    4,    4,    4,    4,
    4,    4,    4,    4,    4,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    3,    3,    3,    3,    3,    3,    3,    3,    3,
    3,    3,    3,    3,    6,    6,    6,
};
static const short yydefred[] = {                         1,
    0,    0,    0,   11,    0,    0,    0,    0,    0,   13,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   12,   14,   15,
   16,    0,    8,    6,    7,   19,   67,   68,   66,   21,
   20,   69,   70,   71,   72,   73,   74,   75,   76,   23,
   24,   77,   22,   26,   25,   27,   28,   29,   78,   79,
   80,   81,   30,   31,   32,   18,   33,   34,   35,   36,
   37,   38,   39,   40,   41,    0,    3,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    5,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    4,    0,    0,    0,    0,    0,    0,    0,    9,
    0,    0,    0,    0,    0,    0,    0,   10,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   88,   89,   90,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   44,    0,   45,   47,   48,   49,    0,   46,   51,
   52,   53,   54,   55,   56,   57,   58,   43,   50,    0,
    0,    0,   95,   96,   97,
};
static const short yydgoto[] = {                          1,
   32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
   42,   43,   44,   45,   46,   47,   48,   49,   50,   51,
   52,   53,   54,   55,   56,   57,   58,   59,   60,   61,
   62,   63,   64,   65,   66,   67,   68,   69,   70,   71,
   72,   73,   74,   75,   76,    2,   77,
};
static const short yysindex[] = {                         0,
    0, -110, -254,    0, -110, -256, -253, -252, -110,    0,
 -250, -248, -247, -244, -231, -230, -227, -226, -210, -209,
 -185, -184, -174, -170, -165, -152, -138,    0,    0,    0,
    0,  744,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -110,    0, -129, -128, -127,
 -126, -109, -108, -107,  -66, -263, -110, -110, -110, -203,
 -110, -110, -110, -110, -105, -110, -110, -110, -110, -110,
 -110, -110, -110, -110, -110, -110, -110,    0, -110, -110,
 -110, -110, -110, -110, -110, -110, -110, -110, -110, -110,
 -110,    0, -110, -110, -110, -110, -110, -110, -110,    0,
  770,  -38,  796,  -10,   18,   46,   74,    0,  822,  102,
  130,  158,  186,  214,  242,  270,  298,  326,  354,  382,
 -144, -144, -135, -135, -263, -263,    0,    0,    0, -135,
 -135, -135, -135, -263, -263, -263, -263, -263, -263, -263,
 -110,    0, -110,    0,    0,    0,    0, -110,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  410,
  438,  466,    0,    0,    0,
};
static const short yyrindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  148,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  494,    0,    0,    0, -196,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -235, -193,  654,  669,  510,  526,    0,    0,    0,  684,
  699,  714,  729,  542,  558,  574,  590,  606,  622,  638,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,
};
static const short yygindex[] = {                         0,
   -3,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   78,
};
#define YYTABLESIZE 1107
static const short yytable[] = {                         85,
   87,   86,    3,   88,   89,   90,   91,    4,   92,   93,
    5,    6,   94,    7,    8,  115,  116,  117,    9,   10,
   11,   12,   83,   83,   83,   95,   96,   83,   83,   97,
   98,   13,   14,   15,   16,   17,   18,   19,   20,   21,
   22,   23,   24,   25,   26,   27,   99,  100,   28,   29,
   30,   31,   78,   79,   80,   81,   82,   83,   84,  109,
  110,   17,   17,   17,   82,   82,   82,  111,  112,   82,
   82,  101,  102,  113,  114,  115,  116,  117,  118,  119,
  120,  121,  103,  131,  132,  133,  104,  134,  135,  136,
  137,  105,  139,  140,  141,  142,  143,  144,  145,  146,
  147,  148,  149,  150,  106,  151,  152,  153,  154,  155,
  156,  157,  158,  159,  160,  161,  162,  163,  107,  164,
  165,  166,  167,  168,  169,  170,  111,  112,  123,  124,
  125,  126,  113,  114,  115,  116,  117,  118,  119,  120,
  121,  113,  114,  115,  116,  117,    3,    2,  127,  128,
  129,    4,  138,  122,    5,    6,    0,    7,    8,    0,
    0,    0,    9,   10,   11,   12,    0,  190,    0,  191,
    0,    0,    0,    0,  192,   13,   14,   15,   16,   17,
   18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
    0,  130,   28,   29,   30,   31,  109,  110,    0,    0,
    0,    0,    0,    0,  111,  112,    0,    0,    0,    0,
  113,  114,  115,  116,  117,  118,  119,  120,  121,  172,
    0,    0,    0,    0,  109,  110,    0,    0,    0,    0,
    0,    0,  111,  112,    0,    0,    0,    0,  113,  114,
  115,  116,  117,  118,  119,  120,  121,  174,    0,    0,
    0,    0,  109,  110,    0,    0,    0,    0,    0,    0,
  111,  112,    0,    0,    0,    0,  113,  114,  115,  116,
  117,  118,  119,  120,  121,  175,    0,    0,    0,    0,
  109,  110,    0,    0,    0,    0,    0,    0,  111,  112,
    0,    0,    0,    0,  113,  114,  115,  116,  117,  118,
  119,  120,  121,  176,    0,    0,    0,    0,  109,  110,
    0,    0,    0,    0,    0,    0,  111,  112,    0,    0,
    0,    0,  113,  114,  115,  116,  117,  118,  119,  120,
  121,  177,    0,    0,    0,    0,  109,  110,    0,    0,
    0,    0,    0,    0,  111,  112,    0,    0,    0,    0,
  113,  114,  115,  116,  117,  118,  119,  120,  121,  179,
    0,    0,    0,    0,  109,  110,    0,    0,    0,    0,
    0,    0,  111,  112,    0,    0,    0,    0,  113,  114,
  115,  116,  117,  118,  119,  120,  121,  180,    0,    0,
    0,    0,  109,  110,    0,    0,    0,    0,    0,    0,
  111,  112,    0,    0,    0,    0,  113,  114,  115,  116,
  117,  118,  119,  120,  121,  181,    0,    0,    0,    0,
  109,  110,    0,    0,    0,    0,    0,    0,  111,  112,
    0,    0,    0,    0,  113,  114,  115,  116,  117,  118,
  119,  120,  121,  182,    0,    0,    0,    0,  109,  110,
    0,    0,    0,    0,    0,    0,  111,  112,    0,    0,
    0,    0,  113,  114,  115,  116,  117,  118,  119,  120,
  121,  183,    0,    0,    0,    0,  109,  110,    0,    0,
    0,    0,    0,    0,  111,  112,    0,    0,    0,    0,
  113,  114,  115,  116,  117,  118,  119,  120,  121,  184,
    0,    0,    0,    0,  109,  110,    0,    0,    0,    0,
    0,    0,  111,  112,    0,    0,    0,    0,  113,  114,
  115,  116,  117,  118,  119,  120,  121,  185,    0,    0,
    0,    0,  109,  110,    0,    0,    0,    0,    0,    0,
  111,  112,    0,    0,    0,    0,  113,  114,  115,  116,
  117,  118,  119,  120,  121,  186,    0,    0,    0,    0,
  109,  110,    0,    0,    0,    0,    0,    0,  111,  112,
    0,    0,    0,    0,  113,  114,  115,  116,  117,  118,
  119,  120,  121,  187,    0,    0,    0,    0,  109,  110,
    0,    0,    0,    0,    0,    0,  111,  112,    0,    0,
    0,    0,  113,  114,  115,  116,  117,  118,  119,  120,
  121,  188,    0,    0,    0,    0,  109,  110,    0,    0,
    0,    0,    0,    0,  111,  112,    0,    0,    0,    0,
  113,  114,  115,  116,  117,  118,  119,  120,  121,  189,
    0,    0,    0,    0,  109,  110,    0,    0,    0,    0,
    0,    0,  111,  112,    0,    0,    0,    0,  113,  114,
  115,  116,  117,  118,  119,  120,  121,  193,    0,    0,
    0,    0,  109,  110,    0,    0,    0,    0,    0,    0,
  111,  112,    0,    0,    0,    0,  113,  114,  115,  116,
  117,  118,  119,  120,  121,  194,    0,    0,    0,    0,
  109,  110,    0,    0,    0,    0,    0,    0,  111,  112,
    0,    0,    0,    0,  113,  114,  115,  116,  117,  118,
  119,  120,  121,  195,    0,    0,    0,    0,  109,  110,
    0,    0,    0,    0,    0,    0,  111,  112,    0,    0,
    0,    0,  113,  114,  115,  116,  117,  118,  119,  120,
  121,   42,   42,   42,    0,    0,   42,   42,    0,    0,
    0,    0,    0,    0,   42,   42,    0,   86,   86,   86,
   42,   42,   86,   86,    0,   42,   42,   42,   42,    0,
   86,   86,    0,   87,   87,   87,   86,   86,   87,   87,
    0,   86,   86,   86,   86,    0,   87,   87,    0,   59,
   59,   59,   87,   87,   59,   59,    0,   87,   87,   87,
   87,    0,   59,   59,    0,   60,   60,   60,   59,   59,
   60,   60,    0,   59,   59,   59,   59,    0,   60,   60,
    0,   61,   61,   61,   60,   60,   61,   61,    0,   60,
   60,   60,   60,    0,   61,   61,    0,   65,   65,   65,
   61,   61,   65,   65,    0,   61,   61,   61,   61,    0,
   65,   65,    0,   63,   63,   63,   65,   65,   63,   63,
    0,   65,   65,   65,   65,    0,   63,   63,    0,   62,
   62,   62,   63,   63,   62,   62,    0,   63,   63,   63,
   63,    0,   62,   62,    0,   64,   64,   64,   62,   62,
   64,   64,    0,   62,   62,   62,   62,    0,   64,   64,
    0,   84,   84,   84,   64,   64,   84,   84,    0,   64,
   64,   64,   64,    0,   84,   84,   85,   85,   85,    0,
    0,   85,   85,    0,    0,   84,   84,   84,   84,   85,
   85,   91,   91,   91,    0,    0,   91,   91,    0,    0,
   85,   85,   85,   85,   91,   91,   92,   92,   92,    0,
    0,   92,   92,    0,    0,   91,   91,   91,   91,   92,
   92,   93,   93,   93,    0,    0,   93,   93,    0,    0,
   92,   92,   92,   92,   93,   93,   94,   94,   94,    0,
    0,   94,   94,    0,    0,   93,   93,   93,   93,   94,
   94,    0,  108,    0,    0,    0,  109,  110,    0,    0,
   94,   94,   94,   94,  111,  112,    0,    0,    0,    0,
  113,  114,  115,  116,  117,  118,  119,  120,  121,  171,
    0,    0,  109,  110,    0,    0,    0,    0,    0,    0,
  111,  112,    0,    0,    0,    0,  113,  114,  115,  116,
  117,  118,  119,  120,  121,  173,    0,    0,  109,  110,
    0,    0,    0,    0,    0,    0,  111,  112,    0,    0,
    0,    0,  113,  114,  115,  116,  117,  118,  119,  120,
  121,  178,    0,    0,  109,  110,    0,    0,    0,    0,
    0,    0,  111,  112,    0,    0,    0,    0,  113,  114,
  115,  116,  117,  118,  119,  120,  121,
};
static const short yycheck[] = {                          3,
  257,    5,  257,  257,  257,    9,  257,  262,  257,  257,
  265,  266,  257,  268,  269,  279,  280,  281,  273,  274,
  275,  276,  258,  259,  260,  257,  257,  263,  264,  257,
  257,  286,  287,  288,  289,  290,  291,  292,  293,  294,
  295,  296,  297,  298,  299,  300,  257,  257,  303,  304,
  305,  306,  307,  308,  309,  310,  311,  312,  313,  263,
  264,  258,  259,  260,  258,  259,  260,  271,  272,  263,
  264,  257,  257,  277,  278,  279,  280,  281,  282,  283,
  284,  285,  257,   87,   88,   89,  257,   91,   92,   93,
   94,  257,   96,   97,   98,   99,  100,  101,  102,  103,
  104,  105,  106,  107,  257,  109,  110,  111,  112,  113,
  114,  115,  116,  117,  118,  119,  120,  121,  257,  123,
  124,  125,  126,  127,  128,  129,  271,  272,  258,  258,
  258,  258,  277,  278,  279,  280,  281,  282,  283,  284,
  285,  277,  278,  279,  280,  281,  257,    0,  258,  258,
  258,  262,  258,   76,  265,  266,   -1,  268,  269,   -1,
   -1,   -1,  273,  274,  275,  276,   -1,  171,   -1,  173,
   -1,   -1,   -1,   -1,  178,  286,  287,  288,  289,  290,
  291,  292,  293,  294,  295,  296,  297,  298,  299,  300,
   -1,  258,  303,  304,  305,  306,  263,  264,   -1,   -1,
   -1,   -1,   -1,   -1,  271,  272,   -1,   -1,   -1,   -1,
  277,  278,  279,  280,  281,  282,  283,  284,  285,  258,
   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,
   -1,   -1,  271,  272,   -1,   -1,   -1,   -1,  277,  278,
  279,  280,  281,  282,  283,  284,  285,  258,   -1,   -1,
   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,
  271,  272,   -1,   -1,   -1,   -1,  277,  278,  279,  280,
  281,  282,  283,  284,  285,  258,   -1,   -1,   -1,   -1,
  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,  271,  272,
   -1,   -1,   -1,   -1,  277,  278,  279,  280,  281,  282,
  283,  284,  285,  258,   -1,   -1,   -1,   -1,  263,  264,
   -1,   -1,   -1,   -1,   -1,   -1,  271,  272,   -1,   -1,
   -1,   -1,  277,  278,  279,  280,  281,  282,  283,  284,
  285,  258,   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,
   -1,   -1,   -1,   -1,  271,  272,   -1,   -1,   -1,   -1,
  277,  278,  279,  280,  281,  282,  283,  284,  285,  258,
   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,
   -1,   -1,  271,  272,   -1,   -1,   -1,   -1,  277,  278,
  279,  280,  281,  282,  283,  284,  285,  258,   -1,   -1,
   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,
  271,  272,   -1,   -1,   -1,   -1,  277,  278,  279,  280,
  281,  282,  283,  284,  285,  258,   -1,   -1,   -1,   -1,
  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,  271,  272,
   -1,   -1,   -1,   -1,  277,  278,  279,  280,  281,  282,
  283,  284,  285,  258,   -1,   -1,   -1,   -1,  263,  264,
   -1,   -1,   -1,   -1,   -1,   -1,  271,  272,   -1,   -1,
   -1,   -1,  277,  278,  279,  280,  281,  282,  283,  284,
  285,  258,   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,
   -1,   -1,   -1,   -1,  271,  272,   -1,   -1,   -1,   -1,
  277,  278,  279,  280,  281,  282,  283,  284,  285,  258,
   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,
   -1,   -1,  271,  272,   -1,   -1,   -1,   -1,  277,  278,
  279,  280,  281,  282,  283,  284,  285,  258,   -1,   -1,
   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,
  271,  272,   -1,   -1,   -1,   -1,  277,  278,  279,  280,
  281,  282,  283,  284,  285,  258,   -1,   -1,   -1,   -1,
  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,  271,  272,
   -1,   -1,   -1,   -1,  277,  278,  279,  280,  281,  282,
  283,  284,  285,  258,   -1,   -1,   -1,   -1,  263,  264,
   -1,   -1,   -1,   -1,   -1,   -1,  271,  272,   -1,   -1,
   -1,   -1,  277,  278,  279,  280,  281,  282,  283,  284,
  285,  258,   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,
   -1,   -1,   -1,   -1,  271,  272,   -1,   -1,   -1,   -1,
  277,  278,  279,  280,  281,  282,  283,  284,  285,  258,
   -1,   -1,   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,
   -1,   -1,  271,  272,   -1,   -1,   -1,   -1,  277,  278,
  279,  280,  281,  282,  283,  284,  285,  258,   -1,   -1,
   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,
  271,  272,   -1,   -1,   -1,   -1,  277,  278,  279,  280,
  281,  282,  283,  284,  285,  258,   -1,   -1,   -1,   -1,
  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,  271,  272,
   -1,   -1,   -1,   -1,  277,  278,  279,  280,  281,  282,
  283,  284,  285,  258,   -1,   -1,   -1,   -1,  263,  264,
   -1,   -1,   -1,   -1,   -1,   -1,  271,  272,   -1,   -1,
   -1,   -1,  277,  278,  279,  280,  281,  282,  283,  284,
  285,  258,  259,  260,   -1,   -1,  263,  264,   -1,   -1,
   -1,   -1,   -1,   -1,  271,  272,   -1,  258,  259,  260,
  277,  278,  263,  264,   -1,  282,  283,  284,  285,   -1,
  271,  272,   -1,  258,  259,  260,  277,  278,  263,  264,
   -1,  282,  283,  284,  285,   -1,  271,  272,   -1,  258,
  259,  260,  277,  278,  263,  264,   -1,  282,  283,  284,
  285,   -1,  271,  272,   -1,  258,  259,  260,  277,  278,
  263,  264,   -1,  282,  283,  284,  285,   -1,  271,  272,
   -1,  258,  259,  260,  277,  278,  263,  264,   -1,  282,
  283,  284,  285,   -1,  271,  272,   -1,  258,  259,  260,
  277,  278,  263,  264,   -1,  282,  283,  284,  285,   -1,
  271,  272,   -1,  258,  259,  260,  277,  278,  263,  264,
   -1,  282,  283,  284,  285,   -1,  271,  272,   -1,  258,
  259,  260,  277,  278,  263,  264,   -1,  282,  283,  284,
  285,   -1,  271,  272,   -1,  258,  259,  260,  277,  278,
  263,  264,   -1,  282,  283,  284,  285,   -1,  271,  272,
   -1,  258,  259,  260,  277,  278,  263,  264,   -1,  282,
  283,  284,  285,   -1,  271,  272,  258,  259,  260,   -1,
   -1,  263,  264,   -1,   -1,  282,  283,  284,  285,  271,
  272,  258,  259,  260,   -1,   -1,  263,  264,   -1,   -1,
  282,  283,  284,  285,  271,  272,  258,  259,  260,   -1,
   -1,  263,  264,   -1,   -1,  282,  283,  284,  285,  271,
  272,  258,  259,  260,   -1,   -1,  263,  264,   -1,   -1,
  282,  283,  284,  285,  271,  272,  258,  259,  260,   -1,
   -1,  263,  264,   -1,   -1,  282,  283,  284,  285,  271,
  272,   -1,  259,   -1,   -1,   -1,  263,  264,   -1,   -1,
  282,  283,  284,  285,  271,  272,   -1,   -1,   -1,   -1,
  277,  278,  279,  280,  281,  282,  283,  284,  285,  260,
   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,   -1,   -1,
  271,  272,   -1,   -1,   -1,   -1,  277,  278,  279,  280,
  281,  282,  283,  284,  285,  260,   -1,   -1,  263,  264,
   -1,   -1,   -1,   -1,   -1,   -1,  271,  272,   -1,   -1,
   -1,   -1,  277,  278,  279,  280,  281,  282,  283,  284,
  285,  260,   -1,   -1,  263,  264,   -1,   -1,   -1,   -1,
   -1,   -1,  271,  272,   -1,   -1,   -1,   -1,  277,  278,
  279,  280,  281,  282,  283,  284,  285,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 315
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"LPAR","RPAR","SEMICOLON","COMMA",
"PERCENT","DIGIT","OR","AND","NOT","MUTUAL_KENDRA","DESCR","LORDOF",
"ISPLANETINHOUSE","ISPLANETINRASI","ISEQUAL","ISNOTEQUAL","RETURN","PLANET",
"GETRASI","GETBHAVA","PLUS","MINUS","MULT","DIV","MOD","LT","LE","GT","GE",
"PLANETS_INSIGN","PLANETS_INHOUSE","NUMBER_OF_OCCUPIED_SIGNS","GRAHA_DRISHTI",
"GETLORD","ISBENEFIC","ISMALEFIC","ISPLANETINKENDRA","ISPLANETINAPOKLIMA",
"ISPLANETINPANAPHARA","ISPLANETINDUALRASI","ISPLANETINFIXEDRASI",
"ISPLANETINMOVABLERASI","RED12","GETNAKSHATRA","RED27","RASIONLY","HOUSE",
"SIGN","NAKSHATRA","BOOLEAN","TYPE_INT","TYPE_DOUBLE","TYPE_PLANET","TYPE_SIGN",
"TYPE_NAKSHATRA","TYPE_HOUSE","TYPE_BOOLEAN","IDENT","STRING",
};
static const char *yyrule[] = {
"$accept : rule_decl",
"$$1 :",
"rule_decl : $$1 exp_list",
"exp_list : stmt",
"exp_list : exp_list stmt",
"stmt : exp SEMICOLON",
"exp : unary_exp",
"exp : dual_exp",
"exp : return_exp",
"exp : LPAR exp RPAR",
"exp : NUMBER_OF_OCCUPIED_SIGNS LPAR RPAR",
"exp : DIGIT",
"exp : HOUSE",
"exp : PLANET",
"exp : SIGN",
"exp : NAKSHATRA",
"exp : BOOLEAN",
"return_exp : RETURN exp",
"unary_exp : not_exp",
"unary_exp : lordof_exp",
"unary_exp : getbhava_exp",
"unary_exp : getrasi_exp",
"unary_exp : getlord_exp",
"unary_exp : planetsinsign_exp",
"unary_exp : planetsinhouse_exp",
"unary_exp : ismalefic_exp",
"unary_exp : isbenefic_exp",
"unary_exp : isplanetinkendra_exp",
"unary_exp : isplanetinapoklima_exp",
"unary_exp : isplanetinpanaphara_exp",
"unary_exp : isplanetindualrasi_exp",
"unary_exp : isplanetinfixedrasi_exp",
"unary_exp : isplanetinmovablerasi_exp",
"unary_exp : red12_exp",
"unary_exp : getnakshatra_exp",
"unary_exp : typecast_int_exp",
"unary_exp : typecast_double_exp",
"unary_exp : typecast_planet_exp",
"unary_exp : typecast_house_exp",
"unary_exp : typecast_nakshatra_exp",
"unary_exp : typecast_boolean_exp",
"unary_exp : typecast_sign_exp",
"not_exp : NOT exp",
"red12_exp : RED12 LPAR exp RPAR",
"lordof_exp : LORDOF LPAR exp RPAR",
"getrasi_exp : GETRASI LPAR exp RPAR",
"getlord_exp : GETLORD LPAR exp RPAR",
"getbhava_exp : GETBHAVA LPAR exp RPAR",
"planetsinsign_exp : PLANETS_INSIGN LPAR exp RPAR",
"planetsinhouse_exp : PLANETS_INHOUSE LPAR exp RPAR",
"getnakshatra_exp : GETNAKSHATRA LPAR exp RPAR",
"isbenefic_exp : ISBENEFIC LPAR exp RPAR",
"ismalefic_exp : ISMALEFIC LPAR exp RPAR",
"isplanetinkendra_exp : ISPLANETINKENDRA LPAR exp RPAR",
"isplanetinapoklima_exp : ISPLANETINAPOKLIMA LPAR exp RPAR",
"isplanetinpanaphara_exp : ISPLANETINPANAPHARA LPAR exp RPAR",
"isplanetindualrasi_exp : ISPLANETINDUALRASI LPAR exp RPAR",
"isplanetinfixedrasi_exp : ISPLANETINFIXEDRASI LPAR exp RPAR",
"isplanetinmovablerasi_exp : ISPLANETINMOVABLERASI LPAR exp RPAR",
"typecast_int_exp : LPAR TYPE_INT RPAR exp",
"typecast_double_exp : LPAR TYPE_DOUBLE RPAR exp",
"typecast_planet_exp : LPAR TYPE_PLANET RPAR exp",
"typecast_house_exp : LPAR TYPE_HOUSE RPAR exp",
"typecast_nakshatra_exp : LPAR TYPE_NAKSHATRA RPAR exp",
"typecast_boolean_exp : LPAR TYPE_BOOLEAN RPAR exp",
"typecast_sign_exp : LPAR TYPE_SIGN RPAR exp",
"dual_exp : housematch_exp",
"dual_exp : and_exp",
"dual_exp : or_exp",
"dual_exp : isequal_exp",
"dual_exp : isnotequal_exp",
"dual_exp : plus_exp",
"dual_exp : minus_exp",
"dual_exp : div_exp",
"dual_exp : mult_exp",
"dual_exp : mod_exp",
"dual_exp : mutual_kendra_exp",
"dual_exp : grahadrishti_exp",
"dual_exp : lt_exp",
"dual_exp : le_exp",
"dual_exp : gt_exp",
"dual_exp : ge_exp",
"and_exp : exp AND exp",
"or_exp : exp OR exp",
"isequal_exp : exp ISEQUAL exp",
"isnotequal_exp : exp ISNOTEQUAL exp",
"plus_exp : exp PLUS exp",
"minus_exp : exp MINUS exp",
"mult_exp : exp MULT exp",
"div_exp : exp DIV exp",
"mod_exp : exp MOD exp",
"lt_exp : exp LT exp",
"le_exp : exp LE exp",
"gt_exp : exp GT exp",
"ge_exp : exp GE exp",
"mutual_kendra_exp : MUTUAL_KENDRA LPAR exp COMMA exp RPAR",
"housematch_exp : ISPLANETINHOUSE LPAR exp COMMA exp RPAR",
"grahadrishti_exp : GRAHA_DRISHTI LPAR exp COMMA exp RPAR",

};
#endif
#if YYDEBUG
#include <stdio.h>
#endif

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH  500
#endif
#endif

#define YYINITSTACKSIZE 500

int      yydebug;
int      yynerrs;
int      yyerrflag;
int      yychar;
short   *yyssp;
YYSTYPE *yyvsp;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* variables for the parser stack */
static short   *yyss;
static short   *yysslim;
static YYSTYPE *yyvs;
static unsigned yystacksize;
#line 153 "MdlParser.yy"
/*****************************************************
**
**   yyerror
**
******************************************************/
int yyerror ( char* s)
{
	char msg[1024];
	sprintf( msg, "Parse error at line %d near token \"%s\" (message: %s)", yylineno, yytext, s );

	parseErrorMessage = wxConvCurrent->cMB2WX( msg );
	fprintf( stderr, "%s\n", msg );
	yylineno = 1;
	return 1;
}

#line 630 "y.tab.c"
/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(void)
{
    int i;
    unsigned newsize;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = yystacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = yyssp - yyss;
    newss = (yyss != 0)
          ? (short *)realloc(yyss, newsize * sizeof(*newss))
          : (short *)malloc(newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    yyss  = newss;
    yyssp = newss + i;
    newvs = (yyvs != 0)
          ? (YYSTYPE *)realloc(yyvs, newsize * sizeof(*newvs))
          : (YYSTYPE *)malloc(newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    yyvs = newvs;
    yyvsp = newvs + i;
    yystacksize = newsize;
    yysslim = yyss + newsize - 1;
    return 0;
}

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

    if (yyss == NULL && yygrowstack()) goto yyoverflow;
    yyssp = yyss;
    yyvsp = yyvs;
    yystate = 0;
    *yyssp = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yysslim && yygrowstack())
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yyssp = yytable[yyn];
        *++yyvsp = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yysslim && yygrowstack())
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yyssp = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yyvsp[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
#line 78 "MdlParser.yy"
	{ rule = new Rule; }
break;
case 5:
#line 82 "MdlParser.yy"
	{ rule->addExpression( yyvsp[-1].expval ); }
break;
case 9:
#line 85 "MdlParser.yy"
	{ yyval.expval = yyvsp[-1].expval; }
break;
case 10:
#line 86 "MdlParser.yy"
	{ yyval.expval = new NumericExpression( ED_NUMBER_OF_OCCUPIED_SIGNS ); }
break;
case 11:
#line 87 "MdlParser.yy"
	{ yyval.expval = new NumericExpression( ED_CONST, yyvsp[0].ival ); }
break;
case 12:
#line 88 "MdlParser.yy"
	{ yyval.expval = new NumericExpression( ED_HOUSE, yyvsp[0].ival ); }
break;
case 13:
#line 89 "MdlParser.yy"
	{ yyval.expval = new NumericExpression( ED_PLANET, yyvsp[0].ival ); }
break;
case 14:
#line 90 "MdlParser.yy"
	{ yyval.expval = new NumericExpression( ED_SIGN, yyvsp[0].ival ); }
break;
case 15:
#line 91 "MdlParser.yy"
	{ yyval.expval = new NumericExpression( ED_NAKSHATRA, yyvsp[0].ival ); }
break;
case 16:
#line 92 "MdlParser.yy"
	{ yyval.expval = new NumericExpression( ED_BOOLEAN, yyvsp[0].ival ); }
break;
case 17:
#line 95 "MdlParser.yy"
	{ yyval.expval = new ReturnExpression( yyvsp[0].expval ); }
break;
case 42:
#line 103 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[0].expval, E1_NOT ); }
break;
case 43:
#line 104 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[-1].expval, E1_RED12 ); }
break;
case 44:
#line 105 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[-1].expval, E1_LORDOFHOUSE ); }
break;
case 45:
#line 106 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[-1].expval, E1_GETRASI ); }
break;
case 46:
#line 107 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[-1].expval, E1_GETLORD ); }
break;
case 47:
#line 108 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[-1].expval, E1_GETBHAVA ); }
break;
case 48:
#line 109 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[-1].expval, E1_PLANETS_INSIGN ); }
break;
case 49:
#line 110 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[-1].expval, E1_PLANETS_INHOUSE ); }
break;
case 50:
#line 111 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[-1].expval, E1_GETNAKSHATRA ); }
break;
case 51:
#line 112 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[-1].expval, E1_ISBENEFIC ); }
break;
case 52:
#line 113 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[-1].expval, E1_ISMALEFIC ); }
break;
case 53:
#line 115 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[-1].expval, E1_ISPLANETINKENDRA ); }
break;
case 54:
#line 116 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[-1].expval, E1_ISPLANETINAPOKLIMA ); }
break;
case 55:
#line 117 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[-1].expval, E1_ISPLANETINPANAPHARA ); }
break;
case 56:
#line 118 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[-1].expval, E1_ISPLANETINDUALRASI ); }
break;
case 57:
#line 119 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[-1].expval, E1_ISPLANETINFIXEDRASI ); }
break;
case 58:
#line 120 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[-1].expval, E1_ISPLANETINMOVABLERASI ); }
break;
case 59:
#line 122 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[0].expval, E1_TYPECAST_INT ); }
break;
case 60:
#line 123 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[0].expval, E1_TYPECAST_DOUBLE ); }
break;
case 61:
#line 124 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[0].expval, E1_TYPECAST_PLANET ); }
break;
case 62:
#line 125 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[0].expval, E1_TYPECAST_HOUSE ); }
break;
case 63:
#line 126 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[0].expval, E1_TYPECAST_NAKSHATRA ); }
break;
case 64:
#line 127 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[0].expval, E1_TYPECAST_BOOLEAN ); }
break;
case 65:
#line 128 "MdlParser.yy"
	{ yyval.expval = new UnaryExpression( yyvsp[0].expval, E1_TYPECAST_SIGN ); }
break;
case 82:
#line 134 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yyvsp[-2].expval, yyvsp[0].expval, E2_LOGICAL_AND ); }
break;
case 83:
#line 135 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yyvsp[-2].expval, yyvsp[0].expval, E2_LOGICAL_OR ); }
break;
case 84:
#line 136 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yyvsp[-2].expval, yyvsp[0].expval, E2_ISEQUAL ); }
break;
case 85:
#line 137 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yyvsp[-2].expval, yyvsp[0].expval, E2_ISNOTEQUAL ); }
break;
case 86:
#line 138 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yyvsp[-2].expval, yyvsp[0].expval, E2_PLUS ); }
break;
case 87:
#line 139 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yyvsp[-2].expval, yyvsp[0].expval, E2_MINUS ); }
break;
case 88:
#line 140 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yyvsp[-2].expval, yyvsp[0].expval, E2_MULT ); }
break;
case 89:
#line 141 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yyvsp[-2].expval, yyvsp[0].expval, E2_DIV ); }
break;
case 90:
#line 142 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yyvsp[-2].expval, yyvsp[0].expval, E2_MOD ); }
break;
case 91:
#line 143 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yyvsp[-2].expval, yyvsp[0].expval, E2_LT ); }
break;
case 92:
#line 144 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yyvsp[-2].expval, yyvsp[0].expval, E2_LE ); }
break;
case 93:
#line 145 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yyvsp[-2].expval, yyvsp[0].expval, E2_GT ); }
break;
case 94:
#line 146 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yyvsp[-2].expval, yyvsp[0].expval, E2_GE ); }
break;
case 95:
#line 147 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yyvsp[-3].expval, yyvsp[-1].expval, E2_MUTUAL_KENDRA ); }
break;
case 96:
#line 148 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yyvsp[-3].expval, yyvsp[-1].expval, E2_PLANETINHOUSE ); }
break;
case 97:
#line 149 "MdlParser.yy"
	{ yyval.expval = new DualExpression( yyvsp[-3].expval, yyvsp[-1].expval, E2_GRAHA_DRISHTI ); }
break;
#line 1019 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yysslim && yygrowstack())
    {
        goto yyoverflow;
    }
    *++yyssp = (short) yystate;
    *++yyvsp = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    return (1);

yyaccept:
    return (0);
}
