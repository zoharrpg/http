/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 13 "backend/parser.y"

#include "parse_http.h"

/* Define YACCDEBUG to enable debug messages for this lex file */
//#define YACCDEBUG
#define YYERROR_VERBOSE
#ifdef YACCDEBUG
#include <stdio.h>
#define YPRINTF(...) printf(__VA_ARGS__)
#else
#define YPRINTF(...)
#endif

/* yyparse() calls yyerror() on error */
void yyerror (const char *s);

void set_parsing_options(char *buf, size_t siz, Request *parsing_request);

/* yyparse() calls yylex() to get tokens */
extern int yylex();


/*
** Global variables required for parsing from buffer
** instead of stdin:
*/

/* Pointer to the buffer that contains input */
char *parsing_buf;

/* Current position in the buffer */
int parsing_offset;

/* Buffer size */
size_t parsing_buf_siz;

/* Current parsing_request Header Struct */
Request *parsing_request;


#line 112 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    t_uphex = 258,                 /* t_uphex  */
    t_lohex = 259,                 /* t_lohex  */
    t_upalpha = 260,               /* t_upalpha  */
    t_loalpha = 261,               /* t_loalpha  */
    t_digit = 262,                 /* t_digit  */
    t_cr = 263,                    /* t_cr  */
    t_lf = 264,                    /* t_lf  */
    t_sp = 265,                    /* t_sp  */
    t_ht = 266,                    /* t_ht  */
    t_ctl = 267,                   /* t_ctl  */
    t_slash = 268,                 /* t_slash  */
    t_dot = 269,                   /* t_dot  */
    t_colon = 270,                 /* t_colon  */
    t_question = 271,              /* t_question  */
    t_asterisk = 272,              /* t_asterisk  */
    t_percent = 273,               /* t_percent  */
    t_plus = 274,                  /* t_plus  */
    t_minus = 275,                 /* t_minus  */
    t_at = 276,                    /* t_at  */
    t_semicolon = 277,             /* t_semicolon  */
    t_ampersand = 278,             /* t_ampersand  */
    t_equal = 279,                 /* t_equal  */
    t_dollar = 280,                /* t_dollar  */
    t_comma = 281,                 /* t_comma  */
    t_reserved_sep_no_slash = 282, /* t_reserved_sep_no_slash  */
    t_reserved_sep_slash = 283,    /* t_reserved_sep_slash  */
    t_mark_sep = 284,              /* t_mark_sep  */
    t_separators = 285,            /* t_separators  */
    t_mark_token = 286,            /* t_mark_token  */
    t_token = 287,                 /* t_token  */
    t_text = 288,                  /* t_text  */
    t_lws = 289                    /* t_lws  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define t_uphex 258
#define t_lohex 259
#define t_upalpha 260
#define t_loalpha 261
#define t_digit 262
#define t_cr 263
#define t_lf 264
#define t_sp 265
#define t_ht 266
#define t_ctl 267
#define t_slash 268
#define t_dot 269
#define t_colon 270
#define t_question 271
#define t_asterisk 272
#define t_percent 273
#define t_plus 274
#define t_minus 275
#define t_at 276
#define t_semicolon 277
#define t_ampersand 278
#define t_equal 279
#define t_dollar 280
#define t_comma 281
#define t_reserved_sep_no_slash 282
#define t_reserved_sep_slash 283
#define t_mark_sep 284
#define t_separators 285
#define t_mark_token 286
#define t_token 287
#define t_text 288
#define t_lws 289

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 56 "backend/parser.y"

	char str[8192];
	int i;

#line 238 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_t_uphex = 3,                    /* t_uphex  */
  YYSYMBOL_t_lohex = 4,                    /* t_lohex  */
  YYSYMBOL_t_upalpha = 5,                  /* t_upalpha  */
  YYSYMBOL_t_loalpha = 6,                  /* t_loalpha  */
  YYSYMBOL_t_digit = 7,                    /* t_digit  */
  YYSYMBOL_t_cr = 8,                       /* t_cr  */
  YYSYMBOL_t_lf = 9,                       /* t_lf  */
  YYSYMBOL_t_sp = 10,                      /* t_sp  */
  YYSYMBOL_t_ht = 11,                      /* t_ht  */
  YYSYMBOL_t_ctl = 12,                     /* t_ctl  */
  YYSYMBOL_t_slash = 13,                   /* t_slash  */
  YYSYMBOL_t_dot = 14,                     /* t_dot  */
  YYSYMBOL_t_colon = 15,                   /* t_colon  */
  YYSYMBOL_t_question = 16,                /* t_question  */
  YYSYMBOL_t_asterisk = 17,                /* t_asterisk  */
  YYSYMBOL_t_percent = 18,                 /* t_percent  */
  YYSYMBOL_t_plus = 19,                    /* t_plus  */
  YYSYMBOL_t_minus = 20,                   /* t_minus  */
  YYSYMBOL_t_at = 21,                      /* t_at  */
  YYSYMBOL_t_semicolon = 22,               /* t_semicolon  */
  YYSYMBOL_t_ampersand = 23,               /* t_ampersand  */
  YYSYMBOL_t_equal = 24,                   /* t_equal  */
  YYSYMBOL_t_dollar = 25,                  /* t_dollar  */
  YYSYMBOL_t_comma = 26,                   /* t_comma  */
  YYSYMBOL_t_reserved_sep_no_slash = 27,   /* t_reserved_sep_no_slash  */
  YYSYMBOL_t_reserved_sep_slash = 28,      /* t_reserved_sep_slash  */
  YYSYMBOL_t_mark_sep = 29,                /* t_mark_sep  */
  YYSYMBOL_t_separators = 30,              /* t_separators  */
  YYSYMBOL_t_mark_token = 31,              /* t_mark_token  */
  YYSYMBOL_t_token = 32,                   /* t_token  */
  YYSYMBOL_t_text = 33,                    /* t_text  */
  YYSYMBOL_t_lws = 34,                     /* t_lws  */
  YYSYMBOL_YYACCEPT = 35,                  /* $accept  */
  YYSYMBOL_upalpha = 36,                   /* upalpha  */
  YYSYMBOL_loalpha = 37,                   /* loalpha  */
  YYSYMBOL_alpha = 38,                     /* alpha  */
  YYSYMBOL_alphanum = 39,                  /* alphanum  */
  YYSYMBOL_ctl = 40,                       /* ctl  */
  YYSYMBOL_spht = 41,                      /* spht  */
  YYSYMBOL_reserved_sep_no_slash = 42,     /* reserved_sep_no_slash  */
  YYSYMBOL_reserved_sep = 43,              /* reserved_sep  */
  YYSYMBOL_separators = 44,                /* separators  */
  YYSYMBOL_reserved_token = 45,            /* reserved_token  */
  YYSYMBOL_mark_token = 46,                /* mark_token  */
  YYSYMBOL_token_char = 47,                /* token_char  */
  YYSYMBOL_text_char = 48,                 /* text_char  */
  YYSYMBOL_octet = 49,                     /* octet  */
  YYSYMBOL_crlf = 50,                      /* crlf  */
  YYSYMBOL_token = 51,                     /* token  */
  YYSYMBOL_text = 52,                      /* text  */
  YYSYMBOL_field_value_part = 53,          /* field_value_part  */
  YYSYMBOL_field_value = 54,               /* field_value  */
  YYSYMBOL_request_header_part = 55,       /* request_header_part  */
  YYSYMBOL_request_header = 56,            /* request_header  */
  YYSYMBOL_http_version = 57,              /* http_version  */
  YYSYMBOL_request_line = 58,              /* request_line  */
  YYSYMBOL_request = 59                    /* request  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  32
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   281

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  35
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  63
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  80

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   289


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   160,   160,   160,   164,   164,   168,   168,   172,   174,
     179,   179,   179,   179,   183,   183,   187,   187,   187,   187,
     187,   187,   191,   191,   195,   195,   195,   195,   199,   199,
     199,   203,   203,   203,   203,   207,   207,   207,   207,   207,
     211,   211,   211,   211,   211,   215,   215,   219,   227,   232,
     243,   248,   258,   262,   266,   268,   275,   300,   328,   330,
     334,   351,   359,   363
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "t_uphex", "t_lohex",
  "t_upalpha", "t_loalpha", "t_digit", "t_cr", "t_lf", "t_sp", "t_ht",
  "t_ctl", "t_slash", "t_dot", "t_colon", "t_question", "t_asterisk",
  "t_percent", "t_plus", "t_minus", "t_at", "t_semicolon", "t_ampersand",
  "t_equal", "t_dollar", "t_comma", "t_reserved_sep_no_slash",
  "t_reserved_sep_slash", "t_mark_sep", "t_separators", "t_mark_token",
  "t_token", "t_text", "t_lws", "$accept", "upalpha", "loalpha", "alpha",
  "alphanum", "ctl", "spht", "reserved_sep_no_slash", "reserved_sep",
  "separators", "reserved_token", "mark_token", "token_char", "text_char",
  "octet", "crlf", "token", "text", "field_value_part", "field_value",
  "request_header_part", "request_header", "http_version", "request_line",
  "request", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-59)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     249,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,
     -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,
     -59,   -59,   150,   180,     5,    88,   -59,    -3,   -59,   203,
     -59,   180,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,
     -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,
     -59,   -59,   -59,   119,   -59,    56,   -59,   -59,   249,   -59,
      -3,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,
      56,   226,    -1,   -59,   -59,     2,   -59,    -4,     4,   -59
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     3,     5,     2,     4,     9,    32,    33,    39,    28,
      34,    29,    30,    31,    35,     6,     7,     8,    36,    37,
      38,    48,     0,     0,     0,     0,    49,     0,    63,     0,
      58,     0,     1,    42,    43,    14,    15,    23,    16,    17,
      18,    19,    20,    21,    27,    24,    40,    25,    22,    26,
      44,    41,    50,     0,    47,     0,    62,    59,    14,    51,
      11,    12,    13,    10,    53,    46,    45,    52,    57,    54,
       0,     0,     0,    56,    55,     0,    61,     0,     0,    60
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,   -59,
     -59,   -59,   -21,   -22,   -59,   -29,     0,   -59,   -58,   -59,
     -18,   -59,   -59,   -59,   -59
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    15,    16,    17,    18,    65,    47,    48,    49,    50,
      19,    20,    21,    66,    67,    28,    29,    53,    69,    70,
      30,    31,    72,    23,    24
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      22,    26,    56,    52,    51,    32,    54,    27,    26,    77,
      78,    79,    74,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    68,     0,     0,     0,
       0,    59,    51,     0,    51,     0,     0,     0,     0,     0,
       0,    73,     0,    76,     0,     0,     0,     0,     0,    51,
      26,     0,     0,     0,     0,     0,     0,     0,    71,     1,
       2,     3,     4,     5,    60,    61,    35,    62,    63,    37,
       6,    38,    39,     7,     8,     9,    10,    40,    41,    11,
      42,    12,    43,     0,     0,    44,    45,    13,    14,    46,
      64,     1,     2,     3,     4,     5,    33,    34,    35,    36,
       0,    37,     6,    38,    39,     7,     8,     9,    10,    40,
      41,    11,    42,    12,    43,     0,     0,    44,    45,    13,
      14,    46,     1,     2,     3,     4,     5,    33,    34,    58,
      36,     0,    37,     6,    38,    39,     7,     8,     9,    10,
      40,    41,    11,    42,    12,    43,     0,     0,    44,    45,
      13,    14,    46,     1,     2,     3,     4,     5,     0,     0,
      25,     0,     0,     0,     6,     0,     0,     7,     8,     9,
      10,     0,     0,    11,     0,    12,     0,     0,     0,     0,
       0,    13,    14,     1,     2,     3,     4,     5,    27,     0,
       0,     0,     0,     0,     6,     0,     0,     7,     8,     9,
      10,     0,     0,    11,     0,    12,     1,     2,     3,     4,
       5,    13,    14,     0,     0,     0,     0,     6,    55,     0,
       7,     8,     9,    10,     0,     0,    11,     0,    12,     1,
       2,     3,     4,     5,    13,    14,     0,     0,     0,    75,
       6,     0,     0,     7,     8,     9,    10,     0,     0,    11,
       0,    12,     1,     2,     3,     4,     5,    13,    14,     0,
       0,     0,     0,     6,     0,     0,     7,     8,     9,    10,
       0,     0,    11,     0,    12,     0,     0,     0,     0,     0,
      13,    14
};

static const yytype_int8 yycheck[] =
{
       0,    22,    31,    25,    25,     0,     9,     8,    29,     7,
      14,     7,    70,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    55,    -1,    -1,    -1,
      -1,    53,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    -1,    72,    -1,    -1,    -1,    -1,    -1,    70,
      71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    -1,    -1,    29,    30,    31,    32,    33,
      34,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      -1,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    -1,    -1,    29,    30,    31,
      32,    33,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    -1,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    -1,    -1,    29,    30,
      31,    32,    33,     3,     4,     5,     6,     7,    -1,    -1,
      10,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    19,
      20,    -1,    -1,    23,    -1,    25,    -1,    -1,    -1,    -1,
      -1,    31,    32,     3,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    14,    -1,    -1,    17,    18,    19,
      20,    -1,    -1,    23,    -1,    25,     3,     4,     5,     6,
       7,    31,    32,    -1,    -1,    -1,    -1,    14,    15,    -1,
      17,    18,    19,    20,    -1,    -1,    23,    -1,    25,     3,
       4,     5,     6,     7,    31,    32,    -1,    -1,    -1,    13,
      14,    -1,    -1,    17,    18,    19,    20,    -1,    -1,    23,
      -1,    25,     3,     4,     5,     6,     7,    31,    32,    -1,
      -1,    -1,    -1,    14,    -1,    -1,    17,    18,    19,    20,
      -1,    -1,    23,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      31,    32
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,    14,    17,    18,    19,
      20,    23,    25,    31,    32,    36,    37,    38,    39,    45,
      46,    47,    51,    58,    59,    10,    47,     8,    50,    51,
      55,    56,     0,     8,     9,    10,    11,    13,    15,    16,
      21,    22,    24,    26,    29,    30,    33,    41,    42,    43,
      44,    47,    48,    52,     9,    15,    50,    55,    10,    48,
       8,     9,    11,    12,    34,    40,    48,    49,    50,    53,
      54,    51,    57,    50,    53,    13,    50,     7,    14,     7
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    35,    36,    36,    37,    37,    38,    38,    39,    39,
      40,    40,    40,    40,    41,    41,    42,    42,    42,    42,
      42,    42,    43,    43,    44,    44,    44,    44,    45,    45,
      45,    46,    46,    46,    46,    47,    47,    47,    47,    47,
      48,    48,    48,    48,    48,    49,    49,    50,    51,    51,
      52,    52,    53,    53,    54,    54,    55,    55,    56,    56,
      57,    58,    59,    59
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     2,
       1,     2,     1,     1,     1,     2,     4,     3,     1,     2,
       5,     6,     3,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 3: /* upalpha: t_uphex  */
#line 160 "backend/parser.y"
                             {
//    printf("upalpha: %c\n", $$);
}
#line 1376 "y.tab.c"
    break;

  case 5: /* loalpha: t_lohex  */
#line 164 "backend/parser.y"
                             {
//    printf("loalpha: %c\n", $$);
}
#line 1384 "y.tab.c"
    break;

  case 7: /* alpha: loalpha  */
#line 168 "backend/parser.y"
                         {
//    printf("alpha: %c\n", $$);
}
#line 1392 "y.tab.c"
    break;

  case 8: /* alphanum: alpha  */
#line 172 "backend/parser.y"
                {
//    printf("alphanum: %c\n", $$);
}
#line 1400 "y.tab.c"
    break;

  case 9: /* alphanum: t_digit  */
#line 174 "backend/parser.y"
             {
    (yyval.i) = (yyvsp[0].i) + '0';
//    printf("alphanum: %c\n", $$);
}
#line 1409 "y.tab.c"
    break;

  case 13: /* ctl: t_ht  */
#line 179 "backend/parser.y"
                                {
//    printf("ctl\n");
}
#line 1417 "y.tab.c"
    break;

  case 15: /* spht: t_ht  */
#line 183 "backend/parser.y"
                  {
//    printf("spht: %c\n", $$);
}
#line 1425 "y.tab.c"
    break;

  case 21: /* reserved_sep_no_slash: t_comma  */
#line 187 "backend/parser.y"
                                                                                     {
//    printf("reserved_sep_no_slash: %c\n", $$);
}
#line 1433 "y.tab.c"
    break;

  case 23: /* reserved_sep: t_slash  */
#line 191 "backend/parser.y"
                                              {
//    printf("reserved_sep: %c\n", $$);
}
#line 1441 "y.tab.c"
    break;

  case 27: /* separators: t_mark_sep  */
#line 195 "backend/parser.y"
                                                            {
//    printf("separators: %c\n", $$);
}
#line 1449 "y.tab.c"
    break;

  case 30: /* reserved_token: t_dollar  */
#line 199 "backend/parser.y"
                                                {
//    printf("reserved_token: %c\n", $$);
}
#line 1457 "y.tab.c"
    break;

  case 34: /* mark_token: t_minus  */
#line 203 "backend/parser.y"
                                                        {
//    printf("mark_token: %c\n", $$);
}
#line 1465 "y.tab.c"
    break;

  case 39: /* token_char: t_percent  */
#line 207 "backend/parser.y"
                                                                         {
//    printf("token_char: %c\n", $$);
}
#line 1473 "y.tab.c"
    break;

  case 44: /* text_char: separators  */
#line 211 "backend/parser.y"
                                                          {
//    printf("text_char: %c\n", $$);
}
#line 1481 "y.tab.c"
    break;

  case 46: /* octet: ctl  */
#line 215 "backend/parser.y"
                       {
//    printf("octet: %c\n", $$);
}
#line 1489 "y.tab.c"
    break;

  case 47: /* crlf: t_cr t_lf  */
#line 219 "backend/parser.y"
                {
    (yyval.str)[0] = '\r';
    (yyval.str)[1] = '\n';
    (yyval.str)[3] = 0;
//    printf("crlf\n");
}
#line 1500 "y.tab.c"
    break;

  case 48: /* token: token_char  */
#line 227 "backend/parser.y"
           {
	YPRINTF("token: Matched rule 1.\n");
	snprintf((yyval.str), 8192, "%c", (yyvsp[0].i));
//	printf("token: %s\n", $$);
}
#line 1510 "y.tab.c"
    break;

  case 49: /* token: token token_char  */
#line 232 "backend/parser.y"
                 {
	YPRINTF("token: Matched rule 2.\n");
	size_t len = strlen((yyvsp[-1].str));
	memcpy((yyval.str), (yyvsp[-1].str), len);
	(yyval.str)[len] = (yyvsp[0].i);
	(yyval.str)[len + 1] = 0;
    // snprintf($$, 8192, "%s%c", $1, $2);
//    printf("token: %s\n", $$);
}
#line 1524 "y.tab.c"
    break;

  case 50: /* text: text_char  */
#line 243 "backend/parser.y"
          {
	YPRINTF("text: Matched rule 1.\n");
	snprintf((yyval.str), 8192, "%c", (yyvsp[0].i));
//	printf("text: %s\n", $$);
}
#line 1534 "y.tab.c"
    break;

  case 51: /* text: text text_char  */
#line 248 "backend/parser.y"
               {
	YPRINTF("text: Matched rule 2.\n");
	size_t len = strlen((yyvsp[-1].str));
	memcpy((yyval.str), (yyvsp[-1].str), len);
	(yyval.str)[len] = (yyvsp[0].i);
	(yyval.str)[len + 1] = 0;
    // snprintf($$, 8192, "%s%c", $1, $2);
//    printf("text: %s\n", $$);
}
#line 1548 "y.tab.c"
    break;

  case 52: /* field_value_part: octet  */
#line 258 "backend/parser.y"
                        {
    (yyval.str)[0] = (yyvsp[0].i);
    (yyval.str)[1] = 0;
//    printf("octet field_value_part: %s\n", $$);
}
#line 1558 "y.tab.c"
    break;

  case 53: /* field_value_part: t_lws  */
#line 262 "backend/parser.y"
          {
//    printf("lws field_value_part: %s\n", $1);
}
#line 1566 "y.tab.c"
    break;

  case 54: /* field_value: field_value_part  */
#line 266 "backend/parser.y"
                              {
//    printf("Single field_value: %s\n", $1);
}
#line 1574 "y.tab.c"
    break;

  case 55: /* field_value: field_value field_value_part  */
#line 268 "backend/parser.y"
                                  {
    size_t len = strlen((yyvsp[-1].str));
    memcpy((yyval.str), (yyvsp[-1].str), len);
    strcpy((yyval.str) + len, (yyvsp[0].str));
//    printf("Multiple field_value: %s\n", $$);
}
#line 1585 "y.tab.c"
    break;

  case 56: /* request_header_part: token t_colon field_value crlf  */
#line 275 "backend/parser.y"
                                                    {
//    printf("request_header_part: %s:%s\n", $1, $3);

	parsing_request->header_count++;

	// Reallocate header size if necessary
	if (parsing_request->header_count > parsing_request->allocated_headers) {
	    parsing_request->allocated_headers *= 2;
	    Request_header *headers = (Request_header *) malloc(sizeof(Request_header)*parsing_request->allocated_headers);
    	for(int i = 0; i < parsing_request->header_count - 1; i++) {
    	    strcpy(headers[i].header_name, parsing_request->headers[i].header_name);
    	    strcpy(headers[i].header_value, parsing_request->headers[i].header_value);
    	}

        strcpy(headers[parsing_request->header_count - 1].header_name, (yyvsp[-3].str));
    	strcpy(headers[parsing_request->header_count - 1].header_value, (yyvsp[-1].str));

    	free(parsing_request->headers);
    	parsing_request->headers = headers;
	} else {
	    strcpy(parsing_request->headers[parsing_request->header_count - 1].header_name, (yyvsp[-3].str));
        strcpy(parsing_request->headers[parsing_request->header_count - 1].header_value, (yyvsp[-1].str));
	}

	parsing_request->status_header_size += strlen((yyvsp[-3].str)) + 1 + strlen((yyvsp[-1].str)) + 2;
}
#line 1616 "y.tab.c"
    break;

  case 57: /* request_header_part: token t_colon crlf  */
#line 300 "backend/parser.y"
                        {
//    printf("request_header_part: %s:\n", $1);

    // Reallocate header size
	parsing_request->header_count++;

	// Reallocate header size if necessary
    if (parsing_request->header_count > parsing_request->allocated_headers) {
        parsing_request->allocated_headers *= 2;
        Request_header *headers = (Request_header *) malloc(sizeof(Request_header)*parsing_request->allocated_headers);
       	for(int i = 0; i < parsing_request->header_count - 1; i++) {
       	    strcpy(headers[i].header_name, parsing_request->headers[i].header_name);
       	    strcpy(headers[i].header_value, parsing_request->headers[i].header_value);
       	}

        strcpy(headers[parsing_request->header_count - 1].header_name, (yyvsp[-2].str));
       	strcpy(headers[parsing_request->header_count - 1].header_value, "");

       	free(parsing_request->headers);
       	parsing_request->headers = headers;
    } else {
        strcpy(parsing_request->headers[parsing_request->header_count - 1].header_name, (yyvsp[-2].str));
        strcpy(parsing_request->headers[parsing_request->header_count - 1].header_value, "");
    }

	parsing_request->status_header_size += strlen((yyvsp[-2].str)) + 1 + 2;
}
#line 1648 "y.tab.c"
    break;

  case 58: /* request_header: request_header_part  */
#line 328 "backend/parser.y"
                                    {
//    printf("Single request_header\n");
}
#line 1656 "y.tab.c"
    break;

  case 59: /* request_header: request_header request_header_part  */
#line 330 "backend/parser.y"
                                        {
//    printf("Multiple request_header\n");
}
#line 1664 "y.tab.c"
    break;

  case 60: /* http_version: token t_slash t_digit t_dot t_digit  */
#line 334 "backend/parser.y"
                                                  {
    if (strcmp("HTTP", (yyvsp[-4].str)) == 0)
    {
        memcpy((yyval.str), (yyvsp[-4].str), 4);
        (yyval.str)[4] = (yyvsp[-3].i);
        (yyval.str)[5] = (yyvsp[-2].i) + '0';
        (yyval.str)[6] = (yyvsp[-1].i);
        (yyval.str)[7] = (yyvsp[0].i) + '0';
        (yyval.str)[8] = 0;
    }
    else
    {
        (yyval.str)[0] = 0;
    }
//    printf("Version: %s\n", $$);
}
#line 1685 "y.tab.c"
    break;

  case 61: /* request_line: token t_sp text t_sp http_version crlf  */
#line 351 "backend/parser.y"
                                                     {
//	printf("request_Line:\n%s %s %s\n",$1, $3,$5);
	strcpy(parsing_request->http_method, (yyvsp[-5].str));
	strcpy(parsing_request->http_uri, (yyvsp[-3].str));
	strcpy(parsing_request->http_version, (yyvsp[-1].str));
	parsing_request->status_header_size += strlen((yyvsp[-5].str)) + 1 + strlen((yyvsp[-3].str)) + 1 + strlen((yyvsp[-1].str)) + 2;
}
#line 1697 "y.tab.c"
    break;

  case 62: /* request: request_line request_header crlf  */
#line 359 "backend/parser.y"
                                          {
    YPRINTF("parsing_request: Matched Success.\n");
    parsing_request->status_header_size += 2;
    return SUCCESS;
}
#line 1707 "y.tab.c"
    break;

  case 63: /* request: request_line crlf  */
#line 363 "backend/parser.y"
                       {
    YPRINTF("parsing_request: Matched Success.\n");
    parsing_request->status_header_size += 2;
    return SUCCESS;
}
#line 1717 "y.tab.c"
    break;


#line 1721 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 369 "backend/parser.y"


/* C code */

void set_parsing_options(char *buf, size_t siz, Request *request)
{
    parsing_buf = buf;
	parsing_offset = 0;
	parsing_buf_siz = siz;
    parsing_request = request;
}

void yyerror (const char *s) {fprintf (stderr, "%s\n", s);}
