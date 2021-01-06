/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface specified in Chapter 17, which is an
 * excellent model for the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h
#include "../StanfordCPPLib/tokenscanner.h"
#include "evalstate.h"
#include "exp.h"


enum stmttype {REM,LET,PRINT,INPUT,GOTO,IF,END,DELETE,ERROR,RUN,QUIT,CLEAR,LIST};
/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

   Statement(TokenScanner * scanner,int line_num);

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

   virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

   virtual void execute(EvalState & state) = 0;
   virtual void print() const=0;

protected:
    int linenum;
    string line;
};
class Remstmt: public Statement{
public:
    Remstmt(TokenScanner* scanner,int  _lineNumber):Statement(scanner,_lineNumber) {};

     void execute(EvalState& state)override;

     void print() const override;

};
class Letstmt: public Statement{
public:
    Letstmt(TokenScanner* scanner,int  _lineNumber):Statement(scanner,_lineNumber) {};
    void execute(EvalState &state)override;

    void print() const override;
};
class Printstmt: public Statement{
public:
    Printstmt(TokenScanner* scanner,int  _lineNumber):Statement(scanner,_lineNumber) {};
    void execute(EvalState &state)override;

    void print() const override;
};
class Inputstmt: public Statement{
public:
    Inputstmt(TokenScanner* scanner,int  _lineNumber):Statement(scanner,_lineNumber) {};
    void execute(EvalState &state)override;

    void print() const override;
};
class Gotostmt: public Statement{
public:
    Gotostmt(TokenScanner* scanner,int  _lineNumber):Statement(scanner,_lineNumber) {};
    void execute(EvalState &state)override;

    void print() const override;
};
class Ifstmt: public Statement{
public:
    Ifstmt(TokenScanner* scanner,int  _lineNumber):Statement(scanner,_lineNumber) {};
    void execute(EvalState &state)override;

    void print() const override;
};
class Endstmt: public Statement{
public:
    Endstmt(TokenScanner* scanner,int  _lineNumber):Statement(scanner,_lineNumber) {};
    void execute(EvalState &state)override;

    void print() const override;
};
TokenScanner *scanner_Init(const string &line);
/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

#endif
