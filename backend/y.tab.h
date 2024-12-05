/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

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

#line 140 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
