#include <iostream>
#include <cctype>
#include <cstdlib>
#include <string>
using namespace std;


void Expression();
void Identificator();
bool isSpace(char c);
void skipSpace();
char Look;  // Lookahead character


// Get a new symbol
void GetChar() {
    cin.get(Look);
}

// Error message
void Error(const string& s) {
    cerr << "\nError: " << s << ".\n";
}

// Error message and completion
void Abort(const string& s) {
    Error(s);
    exit(1);
}

// Waiting for a specific element
void Expected(const string& s) {
    Abort(s + " expected");
}



// Checking for the letter
bool IsAlpha(char c) {
    return isalpha(static_cast<unsigned char>(c));
}

// Checking for a number
bool IsDigit(char c) {
    return isdigit(static_cast<unsigned char>(c));
}


bool isAlNum(char c) {
    return IsAlpha(c) || IsDigit(c);
}

bool isSpace(char c) {
    return c == ' ' || c == '\t';
}

//a function skip space
void skipSpace() {
    while (isSpace(Look)) {
        GetChar();
    }
}

// Character Matching
void Match(char x) {
    if (Look != x) Expected("'" + string(1, x) + "'");
    else {
        GetChar();
        skipSpace();
    }
        
}

// Get a name 
string GetName() {
    string token = "";
    if (!IsAlpha(Look))
        Expected("Name");
    while (isAlNum(Look)) {
        token += toupper(Look);
        GetChar();
    }
    skipSpace();
    return token;
    
}

// Get a number
string GetNum() {
    string value;
    if (!IsDigit(Look))
        Expected("Integer");
    while (IsDigit(Look)) {
        value += Look;
        GetChar();
    }
    skipSpace();
    return value;
    
}

// Printing an indented line
void Emit(const string& s) {
    cout << "\t" << s;
}

// Printing an indented and newline line
void EmitLn(const string& s) {
    Emit(s);
    cout << endl;
}

//Parse and Translate a Math Factor
void Factor() {
    if (Look == '(') {
        Match('(');
        Expression();
        Match(')');
    }

    else if (IsAlpha(Look)) {
        Identificator();
    }
    else {
        string num = GetNum();
        EmitLn("MOVE #" + num + ",D0");
    }
}

void Identificator() {
    string name = GetName();
    if (Look == '(') {
        Match('(');
        Match(')');
        EmitLn("BSR " + name);
    }
    else {
        EmitLn("MOVE " + name + "(PC),D0");
    }

}

void Multiply() {
    Match('*');
    Factor();
    EmitLn("MULS (SP)+,D0");
}

void Divive() {
    Match('/');
    Factor();
    EmitLn("MOVE (SP)+,D1");
    EmitLn("DIVS D1, D0");
}

//Parse and Translate a Math Expression
void Term() {
    Factor();
    while (Look == '*' || Look == '/') {
        EmitLn("MOVE D0,-(SP)");
        switch (Look)
        {
        case '*':Multiply(); break;
        case '/':Divive(); break;
        default:
            Expected("Mulop");
        }
    }
    
}

void Add() {
    Match('+');
    Term();
    EmitLn("ADD (SP)+,D0");
}

void Subtract() {
    Match('-');
    Term();
    EmitLn("SUB (SP)+,D0");
    EmitLn("NEG D0");
}

bool isAddop(char ch) {
    return ch == '+' || ch == '-';
}

//Parse and Translate an Expression
void Expression() {
    if (isAddop(Look)) {
        EmitLn("CLR D0");
    }
    else {
        Term();
    }
    
    while(isAddop(Look)) {
        EmitLn("MOVE D0,-(SP)");
        switch (Look) {
        case '+':
            Add();
            break;
        case '-':
            Subtract();
            break;
        default:
            Expected("Addop");
            break;
        }
    }
}
//assignment function
void Assignment() {
    string name = GetName();

    Match('=');
    Expression();
    EmitLn("LEA " + name + "(PC),A0");
    EmitLn("MOVE D0,(A0)");
    
}

// Initialization
void Init() {
    GetChar();
    skipSpace();
}

int main() {
    Init();
    Expression();
    if (Look != '\n') Expected("NewLine");
    return 0;
}

