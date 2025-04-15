#include <iostream>
#include <cctype>
#include <cstdlib>
#include <string>
using namespace std;

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

// Character Matching
void Match(char x) {
    if (Look == x)
        GetChar();
    else
        Expected("'" + string(1, x) + "'");
}

// Checking for the letter
bool IsAlpha(char c) {
    return isalpha(static_cast<unsigned char>(c));
}

// Checking for a number
bool IsDigit(char c) {
    return isdigit(static_cast<unsigned char>(c));
}

// Get a name (letter)
char GetName() {
    if (!IsAlpha(Look))
        Expected("Name");
    char name = toupper(Look);
    GetChar();
    return name;
}

// Get a number
char GetNum() {
    if (!IsDigit(Look))
        Expected("Integer");
    char num = Look;
    GetChar();
    return num;
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

void Factor() {
    char num = GetNum();
    EmitLn("MOVE #" + string(1, num) + ",D0");
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

void Expression() {
    Term();
    while (Look == '+' || Look == '-') {
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

// Initialization
void Init() {
    GetChar();
}

int main() {
    Init();
    Expression();
    return 0;
}

