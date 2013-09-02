// no motorola o imadiato vem direto depois da instrução
#ifndef M68KASSEMBLER_H
#define M68KASSEMBLER_H
//
#include <iostream>
#include <istream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <exception>

#include <fstream>

using namespace std;

#ifndef NULL
#define NULL 0
#endif

#ifndef UINT
#define UINT unsigned int
#endif

enum Estado
{
    q0,
    q1,
    q2,
    q3,
    q4,
    q5,
    q6,
    q7,
};
/*
class M68KException : public exception
{
    string msg;
    int inst;

public:

    M68KException(string msg,int i);
    ~M68KException();

    string getMsg();
    int getInst();
};
*/


class Operador
{

public:

    short id; // identificador da instrução
    short address; // calcula o tipo de endereçamento da instrução
    UINT operandos; // numero de operandos

    Operador();
    Operador(short id, UINT o);
    ~Operador();

};

// mexer nessa classe para adicionar o backtrack

class Ponteiro
{
public:

    short ref,val;

    vector<int> backTrackIndex;

    Ponteiro();
    Ponteiro(short r);
    ~Ponteiro();
};



class M68KAssembler
{
    // string de entrada
    stringstream entrada;
    // escrever o resultado da traducao em um vetor
    vector< unsigned short> traduzido;

    int instcount;

    // tabela mapeada de operadores e seus atributos
    map<string,Operador> operadores;
    // tabela de simbolos para a segunda passada
    map<string,Ponteiro> tabelaSimbolos;

    string upper(string str);
    bool isNumeral(string str);
    void passada1(); //throw M68KAssemblerException;

public:

    M68KAssembler();
    M68KAssembler(string code);
    ~M68KAssembler();

    // roda passada 1 e 2
    void run(); //throw M68KAssemblerException;
    void setCode(string code);

    // retorna o programa traduzido
    vector<unsigned short> getProgram();
};


#endif
