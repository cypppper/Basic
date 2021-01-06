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
#include "evalstate.h"
using namespace std;

Program::Program() =default;

Program::~Program() {
   // Replace this stub with your own code
   clear();
}

void Program::clear() {
   for (auto i:pro_map){
        delete i.second;
   }
   return;
}

void Program::addSourceLine(int lineNumber,TokenScanner* scanner) {
   // Replace this stub with your own code
    Statement * tmp;
    stmttype cur_type;
    tmp=convertToStatement(scanner,cur_type,lineNumber);
    if(cur_type==DELETE){removeSourceLine(lineNumber);}//走到类型项
    if(tmp!=NULL){
        if(pro_map.count(lineNumber)) delete pro_map[lineNumber];
        pro_map[lineNumber]=tmp;
    }

}

void Program::removeSourceLine(int lineNumber) {
   // Replace this stub with your own code
    delete pro_map[lineNumber];
    pro_map.erase(lineNumber);
}

string Program::getSourceLine(int lineNumber) {

   return "";    // Replace this stub with your own code
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
   // Replace this stub with your own code
}

Statement *Program::getParsedStatement(int lineNumber) {
   return pro_map[lineNumber];  // Replace this stub with your own code
}

int Program::getFirstLineNumber() {
   return pro_map.begin()->first;     // Replace this stub with your own code
}
int Program::getPreLineNumber(int index){
    auto i = pro_map.find(index);
    --i;
    return i->first;
}
int Program::getNextLineNumber(int lineNumber) {
   auto i = pro_map.find(lineNumber);
   if(++i!=pro_map.end())return i->first;
   else return -1;
}
Statement *convertToStatement(TokenScanner *scanner,  stmttype &type, int lineNumber) {
    //scanner走到类型项
    //任务：分配type
    Statement * tmp=NULL;
    if(!scanner->hasMoreTokens()){
        type=DELETE;
        return NULL;
    }
    string type_s=scanner->nextToken();
    type=convertTOtype(type_s);
    switch (type) {//跳过LET，DELETE，派生类的line录入后面的内容
        case LET:
            tmp=new Letstmt(scanner,lineNumber);
            break;
        case REM:
            tmp=new Remstmt(scanner,lineNumber);
            break;
        case PRINT:
            tmp=new Printstmt(scanner,lineNumber);
            break;
        case INPUT:
            tmp=new Inputstmt(scanner,lineNumber);
            break;
        case GOTO:
            tmp=new Gotostmt(scanner,lineNumber);
            break;
        case IF:
            tmp=new Ifstmt(scanner,lineNumber);
            break;
        case END:
            tmp=new Endstmt(scanner,lineNumber);
            break;
        case RUN :

            break;
        case QUIT :


            break;
        case CLEAR :

            break;
        case LIST :

            break;


    }
    return tmp;

}
void dealSpecial(TokenScanner *scanner, EvalState &state, Program &program){
    stmttype type;
    Statement *p = convertToStatement(scanner, type,0);
    switch (type){
        case RUN:
            program.run(state);
            break;
        case CLEAR:
            program.clear();
            state.clear();
            break;
        case LIST:
            program.list();
    }
}

void Program::run(EvalState &state) {
    state.cur_line = getFirstLineNumber();
    for(int &i = state.cur_line ; i != -1; i = getNextLineNumber(i)) {
        pro_map[i] -> execute(state);
        if(i == 0) break;//end?
        if(i < 0) {
            i = getPreLineNumber(-i);
        }
    }
}
void Program::list() {
    for(auto i:pro_map){
        i.second->print();
    }
}
stmttype convertTOtype(string typestr){
    if(typestr=="LET"){
        return LET;
    }
    if(typestr=="REM"){
        return REM;
    }
    if(typestr=="PRINT"){
        return PRINT;
    }
    if(typestr=="INPUT"){//enum stmttype {REM,LET,PRINT,INPUT,GOTO,IF,END,DELETE};
        return INPUT;
    }
    if(typestr=="GOTO"){
        return GOTO;
    }
    if(typestr=="IF"){
        return IF;
    }
    if(typestr=="END"){
        return END;
    }
    if(typestr == "RUN")
        return RUN;
    if(typestr == "QUIT")
        error("QUIT");
    if(typestr== "CLEAR")
        return CLEAR;
    if(typestr == "LIST")
        return LIST;
    else return ERROR;
        // cerr << "Classifying!!!" << endl;

}