#include <iostream>
#include <string>
#include <fstream>

using namespace std;

ifstream file;
string token;

enum StatementTypes
{
  kIf,
  kRepeat,
  kAssign,
  kRead,
  kWrite,
  kError
};

StatementTypes getStatementType(string s)
{
  if (s == "if")
    return kIf;
  if (s == "identifier")
    return kAssign;
  if (s == "read")
    return kRead;
  if (s == "write")
    return kWrite;
  if (s == "repeat")
    return kRepeat;
  else
    return kError;
}

void Match(string s)
{
  if (s == token)
  {
    file >> token;
  }
  else
  {
    cout << endl
         << "*** An Error occured ***" << endl
         << "-- Program not found" << endl;
    exit(1);
  }
}

void Program();
void StatementSequence();
void Statement();
void IfStatement();
void RepeatStatement();
void AssignStatement();
void ReadStatement();
void WriteStatement();
void Expression();
void ComparisonOperation();
void SimpleExpression();
void AddOperation();
void Term();
void MultiplicationOperation();
void Factor();

int main()
{
  file.open("parser_input.txt");
  file >> token;
  Program();
  file.close();
  system("pause");
  return 0;
}

void Program()
{
  StatementSequence();
  cout << endl
       << "-- Program found" << endl;
}

void StatementSequence()
{
  Statement();
  while (token == ";")
  {
    Match(";");
    Statement();
  }
}

void Statement()
{
  switch (getStatementType(token))
  {
  case kIf:
    IfStatement();
    break;
  case kRepeat:
    RepeatStatement();
    break;
  case kAssign:
    AssignStatement();
    break;
  case kRead:
    ReadStatement();
    break;
  case kWrite:
    WriteStatement();
    break;
  case kError:
    cout << "no stmt found";
    exit(1);
    break;
  default:
    break;
  }
}

void IfStatement()
{
  Match("if");
  Expression();
  Match("then");
  StatementSequence();
  if (token == "else")
    StatementSequence();
  Match("end");
  cout << "- if statement found" << endl;
}

void RepeatStatement()
{
  Match("repeat");
  StatementSequence();
  Match("until");
  Expression();
  cout << "- repeat found" << endl;
}

void AssignStatement()
{
  Match("identifier");
  Match(":=");
  Expression();
  cout << "- assignment found" << endl;
}

void ReadStatement()
{
  Match("read");
  Match("identifier");
  cout << "- read found" << endl;
}

void WriteStatement()
{
  Match("write");
  Expression();
  cout << "- write found" << endl;
}

void Expression()
{
  SimpleExpression();
  if (token == "<" || token == "=")
  {
    ComparisonOperation();
    SimpleExpression();
  }
}

void ComparisonOperation()
{
  if (token == "<")
    Match("<");
  if (token == "=")
    Match("=");
}

void SimpleExpression()
{
  Term();
  while (token == "+" || token == "-")
  {
    AddOperation();
    Term();
  }
}

void AddOperation()
{
  if (token == "+")
    Match("+");
  if (token == "-")
    Match("-");
}

void Term()
{
  Factor();
  while (token == "*" || token == "/")
  {
    MultiplicationOperation();
    Factor();
  }
}

void MultiplicationOperation()
{
  if (token == "*")
    Match("*");
  if (token == "/")
    Match("/");
}

void Factor()
{
  if (token == "(")
  {
    Match("(");
    Expression();
    Match(")");
  }
  else if (token == "number")
  {
    Match("number");
  }
  else if (token == "identifier")
  {
    Match("identifier");
  }
}