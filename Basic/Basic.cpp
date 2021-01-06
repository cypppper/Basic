#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "statement.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/strlib.h"
#include "../StanfordCPPLib/simpio.h"

using namespace std;

void processLine(string line, Program & program, EvalState & state);

int main() {
   EvalState state;
   Program program;
   while (true) {
      try {
         processLine(getLine(), program, state);
      } catch (ErrorException & ex) {
          if(ex.getMessage() == "QUIT") break;
         cout << ex.getMessage() << endl;
      }
   }
   program.clear();
   return 0;
}

void processLine(string line, Program & program, EvalState & state) {
    TokenScanner *scanner =init_scanner(line);
    if(line[0] >= '0' && line[0] <= '9') {
        int tmp = stringToInteger(scanner -> nextToken());
        program.addSourceLine(tmp, scanner);
    }
    else {
        dealspecial(scanner, state, program);
    }
    delete scanner;

}
