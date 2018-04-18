# include <cstdio>
# include <cctype>
# include <string>
# include <cstdlib>
# include <iostream>
# include "lexer.h"
# include <cerrno>
# include "tokens.h"


using namespace std;
string lexbuf, nextbuf;
int lookahead = lexan(lexbuf);
int nexttoken;
void error(){
        cout << "error" << endl;
}

int peek(){
        if(!nexttoken)
                nexttoken = lexan(nextbuf);
        return nexttoken;
}

void match(int t){
        if(lookahead != t){
                error();
        }
        if(nexttoken){
                        lookahead = nexttoken;
                        lexbuf = nextbuf;
                        nexttoken = 0;
        }
                else{
                        lookahead = lexan(lexbuf);
        }
}

void expression();

void 

void expressionPOINTER(){
    if(lookahead == MUL){
//LOOK: DO I MATCH PTR?
        match(MUL);
        expressionPOINTER();
        cerr << "ptr" << endl;
    }
    //cout << "pointer" << endl;
}

void expressionSPECIFIER(){
    if(lookahead == INT){
            match(INT);
    }
    else if(lookahead == LONG){
            match(LONG);
    }
    else if(lookahead == CHAR){
            match(CHAR);
    }
    else{
            error();
    }
    //cout << "specifier" << endl;
}
/*
void expressionPREFIX(){
        expressionPREFIX();
        while(1){
                if (lookahead == ADDR){
                        match(ADDR);
                        expressionPREFIX();
                        cout << "ADDR" << endl;
                }
                else if (lookahead == DEREF){
                        match(DEREF);
                        expressionPREFIX();
                        cout << "DEREF" << endl;
                }
                else if (lookahead == NOT){
                        match(NOT);
                        expressionPREFIX();
                        cout << "NOT" << endl;
                }
                else if (lookahead == NEG){
                        match(NEG);
                        expressionPREFIX();
                        cout << "NEG" << endl;
                }
                else if (lookahead == SIZEOF){
                        match(SIZEOF);
                        if (lookahead == LEFTPAREN){
                            
                        }
                }
        }
}
*/
void expression();

void expressionIDENTIFIER(){
        while(1){
            if(lookahead == ID){
                match(ID);
                cerr << "id" << endl;
                if (lookahead == LEFTPAREN && (peek() != RIGHTPAREN)){
                        match(LEFTPAREN);
                        expressionLIST();
                        match(RIGHTPAREN);
                }
                else if (lookahead == LEFTPAREN && (peek() == RIGHTPAREN)){
                        match(LEFTPAREN);
                        match(RIGHTPAREN);
                }
                break;
            }
            else if (lookahead == NUM){
                    match(NUM);
                    cerr << "num" << endl;
            }
            else if (lookahead == STRING){
                    match(STRING);
                    cerr << "string" << endl;
            }
            else if (lookahead == LEFTPAREN){
                    expression();
            }
            else{
                    break;
            }
        }



//        expression();
}

void expressionBRACKETS(){
        while (1){
                if (lookahead == LEFTBRACKET){
                        match(LEFTBRACKET);
                        expressionBRACKETS();
                        //LOOK: DO I do cerr or cout here??
                        cerr << "left bracket" << endl;
                }
                else {
                        expressionIDENTIFIER();
                        break;
                }
        }
    //expressionIDENTIFIER();
}



void expressionPREFIX(){
    //match(ID);
    while (1){
            if (lookahead == ADDR){
                    match(ADDR);
                    expressionPREFIX();
                    cout << "ADDR" << endl;
            }
            else if (lookahead == DEREF){
                    match(DEREF);
                    expressionPREFIX();
                    cout << "DEREF" << endl;
            }
            else if (lookahead == NOT){
                    match(NOT);
                    expressionPREFIX();
                    cout << "NOT" << endl;
            }
            else if (lookahead == NEG){
                    match(NEG);
                    expressionPREFIX();
                    cout << "NEG" << endl;
            }
            else if (lookahead == SIZEOF){
                    match(SIZEOF);
                    if(lookahead == LEFTPAREN && (peek() == INT || peek() == LONG || peek() == CHAR)){
                            match(LEFTPAREN);
                            expressionSPECIFIER();
                            cerr << "specifier" << endl;
                            expressionPOINTER();
                            cerr << "pointer" << endl;
                            match(RIGHTPAREN);
                    }
                    else {
                            //expressionPREFIX();
                            cerr << "sizeof expression reached. calling expression" << endl;
                            expressionPREFIX();
                    }
            }
            expressionBRACKETS();
            break;
    }
}

void expressionTYPECAST(){
        if(lookahead == LEFTPAREN && (peek() == INT || peek() == LONG || peek() == CHAR)){
                match(LEFTPAREN);
                while(lookahead != RIGHTPAREN){
                        expressionSPECIFIER();
                        cerr << "specifier" << endl;
                        expressionPOINTER();
                        cerr << "pointer" << endl;
                }
                match(RIGHTPAREN);
                cout << "cast" << endl;
                expressionTYPECAST();
        }
        expressionPREFIX();
}
/*
expressionTYPECAST(){
        while (1){
                if (lookahead == LEFTPAREN){
                        match(LEFTPAREN);
                        if (lookahead == INT || lookahead == LONG || lookahead == CHAR){
                                match(lookahead);
                                if(lookahead == 
*/


void expressionMULDIVREM(){
        expressionTYPECAST();
        while (1){
                if (lookahead == MUL){
                        match(MUL);
                        expressionTYPECAST();
                        cout << "MUL" << endl;
                }
                else if (lookahead == DIV){
                        match(DIV);
                        expressionTYPECAST();
                        cout << "DIV" << endl;
                }
                else if (lookahead == REM){
                        match(REM);
                        expressionTYPECAST();
                        cout << "REM" << endl;
                }
                else {
                        break;
                }
        }
}



void expressionADDSUB(){
        expressionMULDIVREM();
        while (1){
                if (lookahead == ADD){
                        match(ADD);
                        expressionMULDIVREM();
                        cout << "ADD" << endl;
                }
                else if (lookahead == SUB){
                        match(SUB);
                        expressionMULDIVREM();
                        cout << "SUB" << endl;
                }
                else {
                        break;
                }
        }
}

void expressionCOMPARE(){
        expressionADDSUB();
        while (1){
                if (lookahead == LTN){
                        match(LTN);
                        expressionADDSUB();
                        cout << "LTN" << endl;
                }
                else if (lookahead == GTN){
                        match(GTN);
                        expressionADDSUB();
                        cout << "GTN" << endl;
                }
                else if (lookahead == LEQ){
                        match(LEQ);
                        expressionADDSUB();
                        cout << "LEQ" << endl;
                }
                else if (lookahead == GEQ){
                        match(GEQ);
                        expressionADDSUB();
                        cout << "GEQ" << endl;
                }
                else {
                        break;
                }
        }
}

void expressionEQUALITY(){
        expressionCOMPARE();
        while (1){
                if (lookahead == EQL){
                        match(EQL);
                        expressionCOMPARE();
                        cout << "EQL" << endl;
                } else if (lookahead == NEQ){
                        match(NEQ);
                        expressionCOMPARE();
                        cout << "NEQ" << endl;
                } else {
                        break;
                }
        }
}

void expressionAND(){
        expressionEQUALITY();

        while (lookahead == AND){
                match(AND);
                expressionEQUALITY();
                cout << "AND" << endl;
        }
}

void expression(){
        expressionAND();

        while (lookahead == OR){
                match(OR);
                expressionAND();
                cout << "OR" << endl;
        }
}


/*
void F() {
        if (lookahead == '('){
                match('(');
                E();
                match(')');
        } else
                match(ID);
}

void T() {
        F();

        while (lookahead == '*' || lookahead == '/') {
                match(lookahead);
                F();
        }
}

void E() {
        T();

        while (lookahead == '+' || lookahead == '-') {
                match(lookahead);
                T();
        }
}



*/
/*
int main(void){

 return 0;
} 
*/

int main(void){
        lookahead = lexan(lexbuf);
        while (lookahead != DONE){
            expression();
        }
        return 0;
}
