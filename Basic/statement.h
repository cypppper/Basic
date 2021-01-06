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

#include "evalstate.h"
#include "exp.h"
#include "../StanfordCPPLib/tokenscanner.h"


enum StatementType {LET, PRINT, INPUT, GOTO,
                    CONDITIONAL, END, REM, RUN, ERROR,
                    QUIT,CLEAR, LIST, DELETE};

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

    Statement() = default;


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
   Statement(int _lineNumber);

    Statement(TokenScanner *_scanner, int _lineNumber);

   virtual void execute(EvalState & state) = 0;


   virtual void print() const = 0;
public:
    string line;
    int lineNumber;
};

class Letstmt: public Statement {
public:
    Letstmt(TokenScanner *_scanner, int _lineNumber);

     void execute(EvalState &state) override;

     void print() const override;
};

class Printstmt : public Statement {
public:
    Printstmt(TokenScanner *_scanner, int _lineNumber);

    void execute(EvalState &state) override;

    void print() const override;
};

class Inputstmt : public Statement {
public:
    Inputstmt(TokenScanner *_scanner, int _lineNumber);

   void execute(EvalState &state) override;

    void print() const override    ;
};

class Gotostmt : public Statement {
public:
    Gotostmt(TokenScanner *_scanner, int _lineNumber);

    void execute(EvalState &state) override;
    void print() const override;
};

class Ifstmt : public Statement {
public:
    Ifstmt (TokenScanner *_scanner, int _lineNumber);

   void execute(EvalState &state) override;

  void print() const override;
};

class Endstmt : public Statement {
public:
    Endstmt(int _lineNumber);

   void execute(EvalState &state)override;

    void print() const override;
};

class Remstmt : public Statement {
public:
    Remstmt(TokenScanner *_scanner, int _lineNumber);

     void execute(EvalState &state)override;

    void print() const override;
};
TokenScanner * init_scanner(const string&);

StatementType stmt_type(TokenScanner *scanner);
#endif
