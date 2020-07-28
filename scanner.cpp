#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void getToken(string);
bool isDigit(char);
bool isLetter(char);
bool isSpace(char);
bool isSymbol(char);

enum DFAStates
{
  kStart,
  kInComment,
  kInNumber,
  kInID,
  kInAssignment,
  kDone
};

DFAStates state = kStart;

const string kReservedWords[8] = {"if", "then", "else", "end", "repeat", "until", "read", "write"};

int main()
{
  ifstream file;
  string line, code;
  file.open("sample_code.txt");
  while (!file.eof())
  {
    getline(file, line);
    code += line;
  }

  freopen("parser_input.txt", "w", stdout);
  getToken(code);
  fclose(stdout);

  return 0;
}

void getToken(string l)
{
  ofstream output_file("scanner_output.txt");

  string my_token;
  bool reserved_flag = 0;
  int i = 0;
  while (state != kDone)
  {
    switch (state)
    {
    case kStart:
      if (isSpace(l[i]))
      {
        i++;
        if (i == l.length())
          state = kDone;
        else
          state = kStart;
      }
      else if (isDigit(l[i]))
      {
        state = kInNumber;
      }
      else if (isLetter(l[i]))
      {
        state = kInID;
      }
      else if (l[i] == ':')
      {
        state = kInAssignment;
      }
      else if (l[i] == '{')
      {
        i++;
        state = kInComment;
      }
      else if (isSymbol(l[i]))
      {
        switch (l[i])
        {
        case ';':
          cout << l[i] << endl;
          output_file << l[i] << " "
                      << "->"
                      << " "
                      << "End of Statement"
                      << "\n";
          break;
        default:
          cout << l[i] << " ";
          output_file << l[i] << " "
                      << "->"
                      << " "
                      << "Special Symbol"
                      << "\n";
          break;
        }
        i++;
        if (i == l.length())
          state = kDone;
        else
          state = kStart;
      }
      else
        state = kDone;
      break;

    case kInComment:
      if (state == kInComment)
      {
        while (l[i] != '}')
        {
          i++;
        }
        i++;
        if (i == l.length())
          state = kDone;
        else
          state = kStart;
      }
      break;

    case kInNumber:
      while (isDigit(l[i]))
      {
        my_token += l[i];
        i++;
      }
      cout << "number"
           << " ";
      output_file << my_token << " "
                  << "->"
                  << " "
                  << "number"
                  << "\n";
      my_token = "";
      if (i == l.length())
        state = kDone;
      else
        state = kStart;
      break;

    case kInID:
      while (isLetter(l[i]))
      {
        my_token += l[i];
        i++;
      }
      for (int i = 0; i < 8; i++)
      {
        if (kReservedWords[i] == my_token)
          reserved_flag = 1;
      }
      if (reserved_flag)
      {
        cout << endl
             << my_token << " ";
        output_file << my_token << " "
                    << "->"
                    << " "
                    << "Reserved Word"
                    << "\n";
      }
      else
      {
        cout << "identifier"
             << " ";
        output_file << my_token << " "
                    << "->"
                    << " "
                    << "Identifier"
                    << "\n";
      }
      my_token = "";
      reserved_flag = 0;
      if (i == l.length())
        state = kDone;
      else
        state = kStart;
      break;

    case kInAssignment:
      if (l[i] == ':')
      {
        i += 2;
        cout << ":="
             << " ";
        output_file << ":="
                    << " "
                    << "->"
                    << " "
                    << "Special Symbol"
                    << "\n";
        state = kStart;
      }
      else
      {
        if (i == l.length())
          state = kDone;
        else
          state = kStart;
      }
      break;
    case kDone:
      break;
    }
  }
}

bool isDigit(char d) { return (d >= '0' && d <= '9'); }

bool isLetter(char l) { return (l >= 'a' && l <= 'z' || l >= 'A' && l <= 'Z'); }

bool isSpace(char s) { return (s == ' ' || s == '\t' || s == '\n'); }

bool isSymbol(char c) { return (c == '+' || '-' || '*' || '/' || '=' || '<' || '(' || ')' || ';'); }