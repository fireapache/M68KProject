#ifndef M68KMACROPROCESSOR_H
#define M68KMACROPROCESSOR_H

#include <iostream>
#include <vector>
#include <sstream>
#include <istream>
#include <string>

using namespace std;

class Macro
{

    string macroName;
    vector<string> param;
    vector<string> instruc;

public:

    Macro();
    ~Macro();
    void setName(string name);
    void addParam(string param);
    void addParam(vector<string> param);
    void addInstruc(string instruc);
    string getName();
    vector<string> getParam();
    vector<string> getInstruc();
    vector<string> swapParam(vector<string> paramList);
};

class M68KMacroProcessor
{
    string code;

public:

    M68KMacroProcessor();
    M68KMacroProcessor(string code);
    ~M68KMacroProcessor();

    void run();

    void setCode(string code);

    string getCode();
};

#endif // M68KMACROPROCESSOR_H
