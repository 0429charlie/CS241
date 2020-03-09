#ifndef __KIND_H__
#define __KIND_H__
#include <string>
#include <iostream>
namespace ASM{
  // The different kinds of Tokens that are part of different WLP4 instructions
  // Used for determining the correct Token to create in the WLP4 recognizer
  enum Kind{
    ID,          // variables
    INT,         // Decimal integer
   // HEXINT,      // Hexadecimal integer
   // REGISTER,    // Register number
    COMMA,       // Comma
    LPAREN,      // (
    RPAREN,      // )
   // LABEL,       // Declaration of a label (with a colon)
    //DOTWORD,     // .word directive
    WHITESPACE,  // Whitespace
	NUM,	//integer in string
	LBRACE,	//{
	RBRACE,	//}
	RETURN,	//return
	IF,	//if
	ELSE, 	//else
	WHILE,	//while
	PRINTLN,	//println
	WAIN,	//wain
	BECOMES,	//=
	EQ,	//==
	NE,	//!=
	LT,	//<
	GT,	//>
	LE,	//<=
	GE,	//>=
	PLUS,	//+
	MINUS,	//-
	STAR,	//*
	SLASH,	//"/"
	PCT,	//%
	SEMI,	//;
	NEW,	//new
	DELETE,	//delete
	LBRACK,	//[
	RBRACK,	//]
	AMP,	//&
	NUL,	//null
	ZERO,
	COMMENT,
	CUR, //	
    ERR          // Bad/Invalid Token
  };

  // A Token class representing the concrete functions we
  // might want to apply to a WLP4 Token

  class Token{
  protected:
    // The kind of the Token
    Kind kind;
    // The actual string representing the Token
    std::string lexeme;
  public:
    // A Factory Method that allows us to make specific Tokens
    // based upon the Kind
    static Token* makeToken(Kind kind, std::string lexeme);
    Token(Kind kind, std::string lexeme);
    // Convenience functions for operations we might like to
    // use on a Token
    virtual int toInt() const;
    std::string toString() const;
    std::string getLexeme() const;
    Kind getKind() const;
  };

  // Subclass the Token class to allow specialized implementations
  // of the toInt method, since each type of token has different ranges
  // of valid values.

  // Token class for Integers
  class IntToken : public Token{
  public:
    IntToken(Kind kind, std::string lexeme);
    virtual int toInt() const;
  };

  // Token class for Hexidecimal Integers
  //class HexIntToken : public Token{
 // public:
 //   HexIntToken(Kind kind, std::string lexeme);
 //   virtual int toInt() const;
 // };

  // Token class for Integers representing Registers
 // class RegisterToken : public Token{
 // public:
 //   RegisterToken(Kind kind, std::string lexeme);
 //   virtual int toInt() const;
 // };

  // Overload the output operator for Tokens
  std::ostream& operator<<(std::ostream& out, const Token& t);
}
#endif
