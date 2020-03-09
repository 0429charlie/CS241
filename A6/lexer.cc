#include "lexer.h"
#include "kind.h"
using std::string;
using std::vector;

// Use the annonymous namespace to prevent external linking
namespace {
  // An array represents the Token kind that each state represents
  // The ERR Kind represents an invalid token
  ASM::Kind stateKinds[] = {
    ASM::NUM,            // ST_NUM
    ASM::ERR,            // ST_START
    ASM::BECOMES,        // ST_COMP
    ASM::ERR,       // ST_NOT
    ASM::NE,            // ST_NE
    ASM::ID,          // ST_ID
    ASM::NUM,          // ST_ZERO
    ASM::WHITESPACE,      // ST_WHITESPACE
	ASM::WHITESPACE,	// ST_COMMENT
	ASM::SLASH,	// ST_SLASH
	ASM::LT,	//ST_LT
	ASM::GT,	//ST_GTi
	ASM::EQ,	//ST_EQ
	ASM::LE,	//ST_LE
	ASM::GE,	//ST_GE
	ASM::LPAREN,
	ASM::RPAREN,
	ASM::LBRACE,
	ASM::RBRACE,
	ASM::PLUS,
	ASM::MINUS,
	ASM::STAR,
	ASM::PCT,
	ASM::COMMA,
	ASM::SEMI,
	ASM::LBRACK,
	ASM::RBRACK,
	ASM::AMP,
	ASM::ID,	//ST_R
	ASM::ID,	//ST_RE
	ASM::ID,	//ST_RET
	ASM::ID,	//ST_RETU
	ASM::ID,	//ST_RETUR
	ASM::RETURN,	//ST_RETURNN
	ASM::ID,	//ST_I
	ASM::IF,	//ST_IF
	ASM::ID,	//ST_IN
	ASM::INT,	//ST_INT
	ASM::ID,	//ST_E
	ASM::ID,       //ST_EL
        ASM::ID,       //ST_ELS
        ASM::ELSE,	//ST_ELSE
	ASM::ID,       //ST_W
        ASM::ID,       //ST_WH
        ASM::ID,       //ST_WHI
        ASM::ID,       //ST_WHIL
        ASM::WHILE,     //ST_WHILE
        ASM::ID,       //ST_P
        ASM::ID,       //ST_PR
        ASM::ID,       //ST_PRI
        ASM::ID,       //ST_PRIN
        ASM::ID,       //ST_PRINT
        ASM::ID,       //ST_PRINTL
        ASM::PRINTLN,   //ST_PRINTLN
        ASM::ID,       //ST_WA
        ASM::ID,       //ST_WAI
        ASM::WAIN,      //ST_WAIN
     	ASM::ID,       //ST_N
        ASM::ID,       //ST_Ne
        ASM::NEW,       //ST_NEW
        ASM::ID,       //ST_D
        ASM::ID,  	//ST_DE
        ASM::ID,       //ST_DEL
        ASM::ID,       //ST_DELE
        ASM::ID,       //ST_DELET
        ASM::DELETE,    //ST_DELETE
	ASM::ID,       //ST_n
        ASM::ID,       //ST_nu
        ASM::ID,       //ST_nul
        ASM::NUL,       //ST_NUL
	ASM::ERR,	//ST_CUR
	ASM::ERR	//ST_ERR
  };
  const string whitespace = "\t\n\r ";
  const string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  const string letterwoinstr = "ABCDEFGHIJKLMOPQRSTUVWXYZabcfghjklmoqstuvxyz";
  const string digits = "0123456789";
//  const string hexDigits = "0123456789ABCDEFabcdef";
  const string oneToNine =  "123456789";
  const string other = "(){}+-*/%,;[]&";

}

ASM::Lexer::Lexer(){
  // Set default transitions to the Error state
  for(int i=0; i < maxStates; ++i){
    for(int j=0; j < maxTrans; ++j){
      delta[i][j] = ST_ERR;
    }
  }
  // Change transitions as appropriate for the MIPS recognizer
  setTrans( ST_START,      whitespace,     ST_WHITESPACE );
  setTrans( ST_WHITESPACE, whitespace,     ST_WHITESPACE );
  setTrans( ST_START,      "N",            ST_n          );
  setTrans( ST_n,         "ABCDEFGHIJKLMNOPQRSTVWXYZabcdefghijklmnopqrstuvwxyz",  ST_ID);
  setTrans( ST_n,          "U",            ST_nu         );
  setTrans( ST_nu,         "ABCDEFGHIJKMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", ST_ID);
  setTrans( ST_nu,         "L",            ST_nul        );
  setTrans( ST_nul,        "ABCDEFGHIJKMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", ST_ID);
  setTrans( ST_nul,        "L",            ST_NUL        );
  setTrans( ST_NUL,        letters+digits, ST_ID         );
  setTrans( ST_START,      "n",            ST_N          );
  setTrans( ST_N,          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdfghijklmnopqrstuvwxyz", ST_ID);
  setTrans( ST_N,          "e",            ST_Ne         );
  setTrans( ST_Ne,         "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvxyz", ST_ID);
  setTrans( ST_Ne,         "w",            ST_NEW        );
  setTrans( ST_NEW,        letters+digits, ST_ID         );
  setTrans( ST_START,      "d",            ST_D          );
  setTrans( ST_D,          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdfghijklmnopqrstuvwxyz", ST_ID);
  setTrans( ST_D,          "e",            ST_DE         );
  setTrans( ST_DE,         "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz", ST_ID);
  setTrans( ST_DE,         "l",            ST_DEL        );
  setTrans( ST_DEL,        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdfghijklmnopqrstuvwxyz", ST_ID);
  setTrans( ST_DEL,        "e",            ST_DELE       );
  setTrans( ST_DELE,       "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrsuvwxyz", ST_ID);
  setTrans( ST_DELE,       "t",            ST_DELET      );
  setTrans( ST_DELET,      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdfghijklmnopqrstuvwxyz", ST_ID);
  setTrans( ST_DELET,      "e",            ST_DELETE     );
  setTrans( ST_DELETE,     letters+digits, ST_ID         ); 
  setTrans( ST_START,      "w",            ST_W          );
  setTrans( ST_W,          "ABCDEFGHIJKLMNOPQRSTUVWXYZbcdefgijklmnopqrstuvwxyz",  ST_ID); //2
  setTrans( ST_W,          "a",            ST_WA         );
  setTrans( ST_WA,         "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjklmnopqrstuvwxyz", ST_ID);
  setTrans( ST_WA,         "i",            ST_WAI        );
  setTrans( ST_WAI,        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmopqrstuvwxyz", ST_ID);
  setTrans( ST_WAI,        "n",            ST_WAIN       );
  setTrans( ST_WAIN,       letters+digits, ST_ID         );
  setTrans( ST_START,      "p",            ST_P          );
  setTrans( ST_P,          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqstuvwxyz", ST_ID);
  setTrans( ST_P,          "r",            ST_PR         );
  setTrans( ST_PR,         "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjklmnopqrstuvwxyz", ST_ID);
  setTrans( ST_PR,         "i",            ST_PRI        );
  setTrans( ST_PRI,        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmopqrstuvwxyz", ST_ID);
  setTrans( ST_PRI,        "n",            ST_PRIN       );
  setTrans( ST_PRIN,       "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrsuvwxyz", ST_ID);
  setTrans( ST_PRIN,       "t",            ST_PRINT      );
  setTrans( ST_PRINT,      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz", ST_ID);
  setTrans( ST_PRINT,      "l",            ST_PRINTL     );
  setTrans( ST_PRINTL,     "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmopqrstuvwxyz", ST_ID);
  setTrans( ST_PRINTL,     "n",            ST_PRINTLN    );
  setTrans( ST_PRINTLN,    letters+digits, ST_ID         );
  setTrans( ST_W,          "h",            ST_WH         );
  setTrans( ST_WH,         "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghjklmnopqrstuvwxyz", ST_ID);
  setTrans( ST_WH,         "i",            ST_WHI        );
  setTrans( ST_WHI,        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz", ST_ID);
  setTrans( ST_WHI,        "l",            ST_WHIL       );
  setTrans( ST_WHIL,       "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdfghijklmnopqrstuvwxyz", ST_ID);
  setTrans( ST_WHIL,       "e",            ST_WHILE      );
  setTrans( ST_WHILE,      letters+digits, ST_ID         );
  setTrans( ST_START,      "e",            ST_E          );
  setTrans( ST_E,          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz", ST_ID);
  setTrans( ST_E,          "l",            ST_EL         );
  setTrans( ST_EL,         "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrtuvwxyz", ST_ID);
  setTrans( ST_EL,         "s",            ST_ELS        );
  setTrans( ST_ELS,        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdfghijklmnopqrstuvwxyz", ST_ID);
  setTrans( ST_ELS,        "e",            ST_ELSE       );
  setTrans( ST_ELSE,       letters+digits, ST_ID         );
  setTrans( ST_START,      "i",            ST_I          );
  setTrans( ST_I,          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdeghijklmopqrstuvwxyz",  ST_ID);  //2
  setTrans( ST_I,          "f",            ST_IF         );
  setTrans( ST_IF,         letters+digits, ST_ID         );
  setTrans( ST_I,          "n",            ST_IN         );
  setTrans( ST_IN,         "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrsuvwxyz", ST_ID);
  setTrans( ST_IN,         "t",            ST_INT        );
  setTrans( ST_INT,        letters+digits, ST_ID         );
  setTrans( ST_START,      "r",            ST_R          );
  setTrans( ST_R,          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdfghijklmnopqrstuvwxyz", ST_ID);
  setTrans( ST_R,          "e",            ST_RE         );
  setTrans( ST_RE,         "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrsuvwxyz", ST_ID);
  setTrans( ST_RE,         "t",            ST_RET        );
  setTrans( ST_RET,        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstvwxyz", ST_ID);
  setTrans( ST_RET,        "u",            ST_RETU       );
  setTrans( ST_RETU,       "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqstuvwxyz", ST_ID);
  setTrans( ST_RETU,       "r",            ST_RETUR      );
  setTrans( ST_RETUR,      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmopqrstuvwxyz", ST_ID); 
  setTrans( ST_RETUR,      "n",            ST_RETURN     );
  setTrans( ST_RETURN,     letters+digits, ST_ID         );
  setTrans( ST_START,      "=",        	   ST_BECOMES    );
  setTrans( ST_BECOMES,    "=",            ST_EQ         );
  setTrans( ST_START,      "<",            ST_LT         );
  setTrans( ST_LT,         "=",            ST_LE         ); 
  setTrans( ST_START,      ">",            ST_GT         );
  setTrans( ST_GT,         "=",            ST_GE         );   
  setTrans( ST_START,      "!",            ST_NOT        );
  setTrans( ST_NOT,        "=",            ST_NE         );
  setTrans( ST_START,      "(",            ST_LPAREN     );
  setTrans( ST_START,      ")",            ST_RPAREN     );
  setTrans( ST_START,      "{",            ST_LBRACE     );
  setTrans( ST_START,      "}",            ST_RBRACE     );
  setTrans( ST_START,      "+",            ST_PLUS       );
  setTrans( ST_START,      "-",            ST_MINUS      );
  setTrans( ST_START,      "*",            ST_STAR       );
  setTrans( ST_START,      "%",            ST_PCT        );
  setTrans( ST_START,      ",",            ST_COMMA      );
  setTrans( ST_START,      ";",            ST_SEMI       );
  setTrans( ST_START,      "[",            ST_LBRACK     );
  setTrans( ST_START,      "]",            ST_RBRACK     );
  setTrans( ST_START,      "&",            ST_AMP        );
  setTrans( ST_START,      letterwoinstr,  ST_ID         );
  setTrans( ST_START,      oneToNine,      ST_NUM        ); 
  setTrans( ST_START,      "0",            ST_ZERO       );
  setTrans( ST_NUM,        digits,         ST_NUM        ); 
  setTrans( ST_NUM,        letters,        ST_CUR        ); 
  setTrans( ST_ID,         letters+digits, ST_ID	 );
  setTrans( ST_START,      "/",            ST_SLASH      );
  setTrans( ST_SLASH,      "/",            ST_COMMENT    );
  setTrans( ST_COMMENT,    letters+digits+other, ST_COMMENT );


  // A comment can only ever lead to the comment state
  for(int j=0; j < maxTrans; ++j) delta[ST_COMMENT][j] = ST_COMMENT;
}

// Set the transitions from one state to another state based upon characters in the
// given string
void ASM::Lexer::setTrans(ASM::State from, const string& chars, ASM::State to){
  for(string::const_iterator it = chars.begin(); it != chars.end(); ++it)
    delta[from][static_cast<unsigned int>(*it)] = to;
}

// Scan a line of input (as a string) and return a vector
// of Tokens representing the MIPS instruction in that line
vector<ASM::Token*> ASM::Lexer::scan(const string& line){
  // Return vector
  vector<Token*> ret;
  if(line.size() == 0) return ret;
  // Always begin at the start state
  State currState = ST_START;
  // startIter represents the beginning of the next Token
  // that is to be recognized. Initially, this is the beginning
  // of the line.
  // Use a const_iterator since we cannot change the input line
  string::const_iterator startIter = line.begin();
  // Loop over the the line
  for(string::const_iterator it = line.begin();;){
    // Assume the next state is the error state
    State nextState = ST_ERR;
    // If we aren't done then get the transition from the current
    // state to the next state based upon the current character of
    //input
    if(it != line.end())
      nextState = delta[currState][static_cast<unsigned int>(*it)];
    // If there is no valid transition then we have reach then end of a
    // Token and can add a new Token to the return vector
    if(ST_ERR == nextState){
      // Get the kind corresponding to the current state
      Kind currKind = stateKinds[currState];
      // If we are in an Error state then we have reached an invalid
      // Token - so we throw and error and delete the Tokens parsed
      // thus far
      if(ERR == currKind){
        vector<Token*>::iterator vit;
        for(vit = ret.begin(); vit != ret.end(); ++vit)
          delete *vit;
        throw "ERROR in lexing after reading " + string(line.begin(),it);
      }
      // If we are not in Whitespace then we push back a new token
      // based upon the kind of the State we end in
      // Whitespace is ignored for practical purposes
      if(WHITESPACE != currKind)
        ret.push_back(Token::makeToken(currKind,string(startIter,it)));
      // Start of next Token begins here
      startIter = it;
      // Revert to start state to begin recognizing next token
      currState = ST_START;
      if(it == line.end()) break;
    } else {
      // Otherwise we proceed to the next state and increment the iterator
      currState = nextState;
      ++it;
    }
  }
  return ret;
}
