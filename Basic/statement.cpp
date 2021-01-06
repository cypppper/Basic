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
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/strlib.h"
#include "../StanfordCPPLib/simpio.h"

using namespace std;

/* Implementation of the Statement class */
StatementType stmt_type(TokenScanner *scanner) {
    string token = scanner -> nextToken();
    if(token == "END") return END;
    if(token == "REM") return REM;
    if(token == "LIST") return LIST;
    if(token == "LET") return LET;
    if(token == "PRINT") return PRINT;
    if(token == "INPUT") return INPUT;
    if(token == "GOTO") return GOTO;
    if(token == "IF") return CONDITIONAL;
    if(token == "RUN") return RUN;
    if(token == "QUIT") return QUIT;
    if(token == "CLEAR") return CLEAR;
    return ERROR;
}
Statement::Statement(TokenScanner *_scanner, int _lineNumber) : lineNumber(_lineNumber){
    string tmp;
    tmp="";
    while(_scanner -> hasMoreTokens()){
        tmp+=" ";
        tmp += _scanner -> nextToken();
    }
    line=tmp;
}

Statement::Statement(int _lineNumber) : lineNumber(_lineNumber) {}

Statement::~Statement() {
}
Letstmt::Letstmt(TokenScanner *_scanner, int _lineNumber) : Statement(_scanner, _lineNumber) {}
void Letstmt::print() const {
    cout << lineNumber << " LET" << line << endl;
}
void Letstmt::execute(EvalState &state) {
    TokenScanner *scanner = init_scanner(line);
    parseExp(*scanner) -> eval(state);
    delete scanner;
}

Printstmt::Printstmt(TokenScanner *_scanner, int _lineNumber) : Statement(_scanner, _lineNumber) {}

void Printstmt::execute(EvalState &state) {
    TokenScanner *scanner = init_scanner(line);
    Expression *exp = parseExp(*scanner);
    int value = exp->eval(state);
    cout << value << endl;
    delete scanner;
}
void Printstmt::print() const {
    cout << lineNumber << " PRINT" << line << endl;
}
Inputstmt::Inputstmt(TokenScanner *_scanner, int _lineNumber) : Statement(_scanner, _lineNumber) {}

void Inputstmt::execute(EvalState &state) {
    TokenScanner *scanner = init_scanner(line);
    bool flag = true;
    while (flag) {
        flag = false;
        try {
            cout << " ? ";
            string inputString = getLine();
            state.setValue(scanner -> nextToken(), stringToInteger(inputString));
        } catch (...) {
            flag = true;
            cout << "INVALID NUMBER" << endl;
        }
    }
    delete scanner;
}
void Inputstmt::print() const {
    cout << lineNumber << " INPUT" << line << endl;
}

Gotostmt::Gotostmt(TokenScanner *_scanner, int _lineNumber) : Statement(_scanner, _lineNumber) {}

void Gotostmt::execute(EvalState &state) {
    TokenScanner *scanner = init_scanner(line);
    Expression *exp = parseExp(*scanner);
    int value = exp->eval(state);
    state.currentLine = -value;
    delete scanner;
}
void Gotostmt::print() const {
    cout << lineNumber << " GOT" << line << endl;
}
Ifstmt :: Ifstmt (TokenScanner *_scanner, int _lineNumber) : Statement(_scanner, _lineNumber) {}

void  Ifstmt ::execute(EvalState &state) {
    TokenScanner *scanner = init_scanner(line);
    TokenScanner tmp;
    string operater_str;
    int lft,rft;
    string str = "";
    for(;;)
    {
        string p = scanner -> nextToken();
        if(p == "<" || p == "=" || p == ">") {
            lft = (tmp.setInput(str), parseExp(tmp) -> eval(state));
            operater_str = p;
            break;
        }
        str += p;
    }
    while(tmp.hasMoreTokens()) tmp.nextToken();
    str = "";
    for(;;)
    {
        string p = scanner -> nextToken();
        if(p == "THEN") {
            rft = parseExp((tmp.setInput(str), tmp)) -> eval(state);
            break;
        }
        str += p;
    }
    bool flag = 0;
    if(operater_str == "=") flag = (lft == rft);
    if(operater_str == "<") flag = (lft < rft);
    if(operater_str == ">") flag = (lft > rft);
    if(flag) {
        Expression *exp = parseExp(*scanner);
        int value = exp->eval(state);
        state.currentLine = -value;
    }
    delete scanner;
}
void  Ifstmt ::print() const {
    cout << lineNumber << " IF" << line << endl;
}
Endstmt::Endstmt(int _lineNumber) : Statement(_lineNumber) {}

void Endstmt::execute(EvalState &state) {
    state.currentLine = 0;
}
void Endstmt::print() const {
    cout << lineNumber << " END" << line << endl;
}
Remstmt::Remstmt(TokenScanner *_scanner, int _lineNumber) : Statement(_scanner, _lineNumber) {}
void Remstmt::execute(EvalState &state) {}
void Remstmt::print() const{
    cout << lineNumber << " REM" << line << endl;
}
TokenScanner *init_scanner(const string &line) {
    TokenScanner *scanner = new TokenScanner;
    scanner -> scanStrings();
    scanner -> ignoreWhitespace();
    scanner -> scanNumbers();
    scanner -> setInput(line);
    return scanner;
}


