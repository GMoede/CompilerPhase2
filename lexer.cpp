/*
 * File:	lexer.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the lexical analyzer for Simple C.
 */

# include <cstdio>
# include <cctype>
# include <string>
# include <cstdlib>
# include <iostream>
# include "lexer.h"
# include <cerrno>

using namespace std;
int numerrors, lineno = 1;


/* Later, we will associate token values with each keyword */
enum {
        AUTO = 1,
        BREAK,
        CASE,
        CHAR,
        CONST,
        CONTINUE,
        DEFAULT,
        DO,
        DOUBLE,
        ELSE,
        ENUM,
        EXTERN,
        FLOAT,
        FOR,
        GOTO,
        IF,
        INT,
        LONG,
        REGISTER,
        RETURN,
        SHORT,
        SIGNED,
        SIZEOF,
        STATIC,
        STRUCT,
        SWITCH,
        TYPEDEF,
        UNION,
        UNSIGNED,
        VOID,
        VOLATILE,
        WHILE,
        LEFTBRACKET,
        RIGHTBRACKET,
        ADDR,
        DEREF,
        NOT,
        NEG,
        ASSIGN,
        LEFTPAREN,
        RIGHTPAREN,
        MUL,
        DIV,
        REM,
        ADD,
        SUB,
        LTN,
        GTN,
        LEQ,
        GEQ,
        EQL,
        NEQ,
        AND,
        OR,
        ID,
        NUM,
        STRING,
        ERROR,
        DONE,
};

//#define ENDOFFILE -1;

static string keywords[] = {
    "auto",
    "break",
    "case",
    "char",
    "const",
    "continue",
    "default",
    "do",
    "double",
    "else",
    "enum",
    "extern",
    "float",
    "for",
    "goto",
    "if",
    "int",
    "long",
    "register",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "struct",
    "switch",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "while",
};

# define numKeywords (sizeof(keywords) / sizeof(keywords[0]))


/*
 * Function:	report
 *
 * Description:	Report an error to the standard error prefixed with the
 *		line number.  We'll be using this a lot later with an
 *		optional string argument, but C++'s stupid streams don't do
 *		positional arguments, so we actually resort to snprintf.
 *		You just can't beat C for doing things down and dirty.
 */

void report(const string &str, const string &arg)
{
    char buf[1000];

    snprintf(buf, sizeof(buf), str.c_str(), arg.c_str());
    cerr << "line " << lineno << ": " << buf << endl;
    numerrors ++;
}


/*
 * Function:	lexan
 *
 * Description:	Read and tokenize the standard input stream.  The lexeme is
 *		stored in a buffer.
 */

int lexan(string &lexbuf)
{
    int p;
    unsigned i;
    static int c = cin.get();


    /* The invariant here is that the next character has already been read
       and is ready to be classified.  In this way, we eliminate having to
       push back characters onto the stream, merely to read them again. */

    while (!cin.eof()) {
	lexbuf.clear();


	/* Ignore white space */

	while (isspace(c)) {
	    if (c == '\n')
		lineno ++;

	    c = cin.get();
	}


	/* Check for an identifier or a keyword */

	if (isalpha(c) || c == '_') {
	    do {
		lexbuf += c;
		c = cin.get();
	    } while (isalnum(c) || c == '_');

	    for (i = 0; i < numKeywords; i ++)
		if (keywords[i] == lexbuf)
		    break;

	    if (i < numKeywords){
            return (i + 1); 
        }
	    else{
            return ID;
        }
        return ERROR;
	    //return true;


	/* Check for a number */

	} else if (isdigit(c)) {
	    do {
		lexbuf += c;
		c = cin.get();
	    } while (isdigit(c));

	    errno = 0;
	    strtol(lexbuf.c_str(), NULL, 0);

	    if (errno != 0)
		report("integer constant too large");

	    if (c == 'l' || c == 'L') {
		lexbuf += c;
		c = cin.get();
	    } else

	    return NUM;


	/* There must be an easier way to do this.  It might seem stupid at
	   this point to recognize each token separately, but eventually
	   we'll be returning separate token values to the parser, so we
	   might as well do it now. */

	} else {
	    lexbuf += c;

	    switch(c) {


	    /* Check for '||' */

	    case '|':
		c = cin.get();

		if (c == '|') {
		    lexbuf += c;
		    c = cin.get();
            return OR;
		}

		return ERROR;


	    /* Check for '=' and '==' */

	    case '=':
		c = cin.get();

		if (c == '=') {
		    lexbuf += c;
		    c = cin.get();
            return EQL;
		}

		return ASSIGN;


	    /* Check for '&' and '&&' */

	    case '&':
		c = cin.get();

		if (c == '&') {
		    lexbuf += c;
		    c = cin.get();
            return AND;
		}

		return ADDR;


	    /* Check for '!' and '!=' */

	    case '!':
		c = cin.get();

		if (c == '=') {
		    lexbuf += c;
		    c = cin.get();
            return NEQ;
		}

		return NOT;


	    /* Check for '<' and '<=' */

	    case '<':
		c = cin.get();

		if (c == '=') {
		    lexbuf += c;
		    c = cin.get();
            return LEQ;
		}

		return LTN;


	    /* Check for '>' and '>=' */

	    case '>':
		c = cin.get();

		if (c == '=') {
		    lexbuf += c;
		    c = cin.get();
            return GEQ;
		}

		return GTN;


	    /* Check for '-', '--', and '->' */

	    case '-':
		c = cin.get();

		if (c == '-') {
		    lexbuf += c;
		    c = cin.get();
            return ERROR;
		} else if (c == '>') {
		    lexbuf += c;
		    c = cin.get();
            return ERROR;
		}

		return SUB;


	    /* Check for '+' and '++' */

	    case '+':
		c = cin.get();

		if (c == '+') {
		    lexbuf += c;
		    c = cin.get();
            return ERROR;
		}

		return ADD;


	    /* Check for simple, single character tokens */

	    case '*':
            c = cin.get(); 
            return MUL;
        case '%': 
            c = cin.get();
            return REM;
        case ':': 
            c = cin.get();
            return ERROR;
        case ';':
	        c = cin.get();
            return ERROR;
        case '(': 
            c = cin.get();
            return LEFTPAREN;
        case ')': 
            c = cin.get();
            return RIGHTPAREN;
        case '[': 
            c = cin.get();
            return LEFTBRACKET;
        case ']':
	        c = cin.get();
            return RIGHTBRACKET;
        case '{': 
            c = cin.get();
            return ERROR;
        case '}': 
            c = cin.get();
            return ERROR;
        case '.': 
            c = cin.get();
            return ERROR;
        case ',':
		    c = cin.get();
            return ERROR;

	    /* Check for '/' or a comment */

	    case '/':
		c = cin.get();

		if (c == '*') {
		    do {
			while (c != '*' && !cin.eof()) {
			    if (c == '\n')
				lineno ++;

			    c = cin.get();
			}

			c = cin.get();
		    } while (c != '/' && !cin.eof());

		    c = cin.get();
		    break;

		} else {
		    return DIV;
		}


	    /* Check for a string literal */

	    case '"':
		do {
		    p = c;
		    c = cin.get();
		    lexbuf += c;
		} while ((c != '"' || p == '\\') && c != '\n' && !cin.eof());

		if (c == '\n' || cin.eof())
		    report("malformed string literal");

		c = cin.get();
		return STRING;


	    /* Handle EOF here as well */

	    case EOF:
		return DONE;


	    /* Everything else is illegal */

	    default:
		c = cin.get();
		break;
	    }
	}
    }

    return ERROR;
}


/*
 * Function:	main
 *
 * Description:	Read and tokenize and standard input stream.
 */
/*
int main()
{
    string lexbuf;

    while (lexan(lexbuf))
	continue;

    return 0;
}*/
