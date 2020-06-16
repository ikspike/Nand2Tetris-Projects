#include <iostream>
#include <fstream>
#include <string>
#include "Parser.h"
#include <regex>

using namespace std;

// Private fuctions
void Parser::trim()
{
    ofstream fout;
    fout.open("trimedFile.asm");

    string command;

    while (this->hasMoreCommands())
    {
        this->advance();
        command = this->currentCmd;

        /* hasMoreCommarnd() test */
        cout << command + " has more commands? " << boolalpha << this->hasMoreCommands() << endl;
        cout << this->commandType() << endl;

        /* symbol() test */
        if(this->commandType() == A_COMMAND)
            cout << this->symbol() << endl;

        fout << command << endl;
    }
    fin.close();
    fout.close();
}

// Public functions
Parser::Parser(string asmFile)
{
    //ifstream fin;
    this->fin.open(asmFile);
}

bool Parser::hasMoreCommands()
{
    if (this->fin.peek() != EOF)
        return true;
    else
        return false;
}

// Variables for advance()
void Parser::advance()
{
    string pattern = "\\W*//.+";
    regex r(pattern);
    string fmt = "";
    string line;
    getline(fin, line);
    line = regex_replace(line, r, fmt);
    // Skip empty lines after removing comments.
    while (line.empty())
    {
        getline(fin, line);
        line = regex_replace(line, r, fmt);
    }
    this->currentCmd = line;
}

int Parser::commandType()
{
    const char *firstChar = &this->currentCmd.at(0);
    switch (*firstChar)
    {
    case '@':
        return A_COMMAND;
    case '(':
        return L_COMMAND;
    default:
        break;
    }
    return C_COMMAND;
}

string Parser::symbol()
{
    string pattern = "\\(([^)]+)\\)";
    regex r(pattern);
    smatch result;
    if (this->commandType() == A_COMMAND)
        return this->currentCmd.substr(1);
    else if (this->commandType() == L_COMMAND)
    {
        regex_search(this->currentCmd, result, r);
        return result.str(1);
    }
    else
        throw runtime_error("Not a A or L command!");
}

string Parser::comp()
{
    // Judge whether the current command is a C-Command.
    if (this->commandType() != C_COMMAND)
        throw runtime_error("Not a C command!");

    // Return the comp field
    // Parse command such as "0;JEQ"
    if (int semiPos = this->currentCmd.find(';') != string::npos)
        // If there is a jump field, return the chars before ";" for comp field.
        return this->currentCmd.substr(0, semiPos);
    else
    {
        // Parse command such as "D=A"
        int eqPos = this->currentCmd.find('=');
        return this->currentCmd.substr(eqPos + 1);
    }
}

string Parser::dest()
{
    // Judge whether the current command is a C-Command.
    if (this->commandType() != C_COMMAND)
        throw runtime_error("Not a C command!");

    // Judge whether there is a dest field.
    if (this->currentCmd.find('=') != string::npos)
        return this->currentCmd.substr(0, 1);
    else
        return "null";
}

string Parser::jump()
{
    if (int semiPos = this->currentCmd.find(';') != string::npos)
        // If there is a jump field, return the chars before ";" for comp field.
        return this->currentCmd.substr(semiPos + 1);
    else
        return "null";
}