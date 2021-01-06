/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/strlib.h"
#include "../StanfordCPPLib/simpio.h"
using namespace std;

/* Implementation of the Statement class */
Statement::Statement(TokenScanner * scanner, int line_num) {
    linenum = line_num;
    string tmp = " ";
    while (scanner->hasMoreTokens()) {
        tmp += " ";
        tmp += scanner->nextToken();
    }
    line.clear();
    line = tmp;
}


Statement::~Statement() {
   /* Empty */
}
void Remstmt::execute(EvalState &state){}
void Remstmt::print() const {
    cout<<linenum<<" REM"<<line<<endl;
}
TokenScanner *scanner_Init(const string &line) {
    TokenScanner *scanner = new TokenScanner;
    scanner -> scanStrings();
    scanner -> ignoreWhitespace();
    scanner -> scanNumbers();
    scanner -> setInput(line);
    return scanner;
}
void Inputstmt::execute(EvalState &state){
    TokenScanner* scanner=scanner_Init(line);
    cout<<"?";
    string inputstr=getLine();
    state.setValue(scanner->nextToken(),stringToInteger(inputstr));
}
void Letstmt::print() const {
    cout<<linenum<<" LET"<<line<<endl;
}
void Printstmt::execute(EvalState &state){
    TokenScanner* scanner=scanner_Init(line);
    Expression *exp = parseExp(*scanner);
    int value=exp->eval(state);
    cout<<value<<endl;
    delete scanner;
};

void Printstmt::print() const {
    cout<<linenum<<" PRINT"<<line<<endl;
}
void Letstmt::execute(EvalState &state){///________________________________________________
    TokenScanner *scanner = scanner_Init(line);
    parseExp(*scanner) -> eval(state);
    delete scanner;
}

void Inputstmt::print() const {
    cout<<linenum<<" INPUT"<<line<<endl;
}
void Gotostmt::execute(EvalState &state){
    TokenScanner *scanner = scanner_Init(line);
    Expression *exp = parseExp(*scanner);
    int value = exp->eval(state);
    //state.cur_line = Program::getPreLineNumber(value);
    state.cur_line = -value;
    delete scanner;
}

void Gotostmt::print() const {
    cout<<linenum<<" GOTO"<<line<<endl;
}
void Ifstmt::execute(EvalState &state){
    TokenScanner *scanner = scanner_Init(line);
    TokenScanner tmpScanner;
    string cmp;
    int v1, v2;

    string tmp = "";
    for(;;) {
        string p = scanner -> nextToken();
        if(p == "<" || p == "=" || p == ">") {
            v1 = (tmpScanner.setInput(tmp), parseExp(tmpScanner) -> eval(state));
            cmp = p;
            break;
        }
        tmp += p;
    }
    while(tmpScanner.hasMoreTokens()) tmpScanner.nextToken();
    tmp = "";
    for(;;) {
        string p = scanner -> nextToken();
        if(p == "THEN") {
            v2 = parseExp((tmpScanner.setInput(tmp), tmpScanner)) -> eval(state);
            break;
        }
        tmp += p;
    }
    bool flag = false;
    if(cmp == "=") flag = (v1 == v2);
    if(cmp == "<") flag = (v1 < v2);
    if(cmp == ">") flag = (v1 > v2);
    if(flag) {
        Expression *exp = parseExp(*scanner);
        int value = exp->eval(state);
        state.cur_line = -value;
    }
    delete scanner;
};

void Ifstmt::print() const {
    cout<<linenum<<" IF"<<line<<endl;
}
void Endstmt::execute(EvalState &state){
    state.cur_line=0;
};

void Endstmt::print() const {
    cout<<linenum<<" END"<<line<<endl;
}