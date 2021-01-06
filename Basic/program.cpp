/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
#include "../StanfordCPPLib/tokenscanner.h"

using namespace std;

Statement *convertToStatement(TokenScanner *scanner, bool direct, Program &program, StatementType &type, int lineNumber) {
    if(!scanner -> hasMoreTokens()) {
        type = DELETE;
        return NULL;
    }
    type = stmt_type(scanner);
    Statement *p = NULL;
    switch (type) {
        case LET :
            p = new Letstmt(scanner, lineNumber);
            break;
        case PRINT :
            p = new Printstmt(scanner, lineNumber);
            break;
        case INPUT :
            p = new Inputstmt(scanner, lineNumber);
            break;
        case GOTO :
            p = new Gotostmt(scanner, lineNumber);
            break;
        case CONDITIONAL :
            p = new Ifstmt(scanner, lineNumber);
            break;
        case END :
            p = new Endstmt(lineNumber);
            break;
        case REM :
            p = new Remstmt(scanner, lineNumber);
            break;
        case RUN :
            break;
        case QUIT :
            error("QUIT");
            break;
        case CLEAR :
            break;
        case LIST :
            break;
        default :
            error("SYNTAX ERROR");
    }
    return p;
}

Program::~Program() {
    clear();
}

void Program::clear() {
    for(auto i : pro_map) {
        delete i.second;
    }
    pro_map.clear();
}

void Program::addSourceLine(int lineNumber, TokenScanner *scanner) {
    StatementType type;
    Statement *p = convertToStatement(scanner, false, *this, type, lineNumber);
    if(type == DELETE) {
        removeSourceLine(lineNumber);
    }
    if(p != NULL){
        if(pro_map.count(lineNumber)) delete pro_map[lineNumber];
        pro_map[lineNumber] = p;
    }
}

void Program::removeSourceLine(int lineNumber) {
    delete pro_map[lineNumber];
    pro_map.erase(lineNumber);
}


Statement *Program::getParsedStatement(int lineNumber) {
    return pro_map[lineNumber];
}

int Program::getFirstLineNumber() {
    return pro_map.begin() -> first;
    return 0;
}

int Program::getPreviousLineNumber(int lineNumber) {
    auto i = pro_map.find(lineNumber);
    if(i != pro_map.begin()) return (--i) -> first;
    return -1;
}

int Program::getNextLineNumber(int lineNumber) {
    if(lineNumber == -1) return getFirstLineNumber();
    auto i = pro_map.find(lineNumber);
    if(++i != pro_map.end()) return i -> first;
    return -1;
}

void Program::run(EvalState &state) {
    state.currentLine = getFirstLineNumber();
    for(int &i = state.currentLine; i != -1; i = getNextLineNumber(i)) {
        pro_map[i] -> execute(state);
        if(i == 0) break;
        if(i < 0) {
            if(!pro_map.count(-i)) error("LINE NUMBER ERROR");
            i = getPreviousLineNumber(-i);
        }
    }
}

void Program::list() {
    for(auto i : pro_map) {
        i.second -> print();
    }
}

void dealspecial(TokenScanner *scanner, EvalState &state, Program &program) {
    StatementType type;
    Statement *p = convertToStatement(scanner, true, program, type);
    if(p != NULL) {
        p -> execute(state);
        delete p;
    }
    else {
        if(type == RUN) program.run(state);
        else if(type == CLEAR) {
            program.clear();
            state.clear();
        }
        else if(type == LIST) {
            program.list();
        }
    }
}


