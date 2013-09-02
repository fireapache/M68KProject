#include "m68kassembler.h"

// falta tratar os erros e debuggar mais o codigo e testar
// ============ Auxiliares =======================//
/*
M68KException::M68KException(string msg,int i)
{
    this->msg = msg;
    inst = i;
}

string M68KException::getMsg()
{
    return msg;
}

int M68KException::getInst()
{
    return inst;
}
*/

// ============ Auxiliares =======================//


string M68KAssembler::upper(string str)
{
    char sub = 'A' - 'a';

    for (UINT i = 0; i < str.size(); i++)
    {
        if (str.c_str()[i] >= 'a' && str.c_str()[i] <= 'z')
        {
            str[i] += sub;
        }
    }

    return str;
}

bool M68KAssembler::isNumeral(string str)
{
    bool _return = true;

    for (UINT i = 0; i < str.size(); i++)
    {
        if (str.c_str()[i] < '0' || str.c_str()[i] > '9')
        {
            _return = false;
            break;
        }
    }

    return _return;
}
//================ Operador ====================//

Operador::Operador()
{
    id = 0;
    address = 0;
    operandos = 0;
}
Operador::Operador(short id, UINT o)
{
    this->id = id;
    operandos = o;
}
Operador::~Operador()
{

}

//================ Ponteiro ====================//

Ponteiro::Ponteiro()
{
    val = 0;
    ref = 0;
}
Ponteiro::Ponteiro(short r)
{
    this->ref = r;

}
Ponteiro::~Ponteiro()
{

}

//================ Assembler ====================//

void M68KAssembler::setCode(string code)
{
    entrada << upper(code);

    instcount = 0;
}

M68KAssembler::M68KAssembler()
{
    operadores.insert( pair< string, Operador >( "ADD",	 Operador( 0 , 2 )));
    operadores.insert( pair< string, Operador >( "ADDI", Operador( 1 , 2 )));
    operadores.insert( pair< string, Operador >( "AND",  Operador( 2 , 2 )));
    operadores.insert( pair< string, Operador >( "ANDI", Operador( 3 , 2 )));
    operadores.insert( pair< string, Operador >( "CLR",  Operador( 4 , 1 )));
    operadores.insert( pair< string, Operador >( "CMP",  Operador( 5 , 2 )));
    operadores.insert( pair< string, Operador >( "DIVS", Operador( 6 , 2 )));
    operadores.insert( pair< string, Operador >( "EOR", Operador( 7 , 2 )));
    operadores.insert( pair< string, Operador >( "EORI",Operador( 8 , 2 )));
    operadores.insert( pair< string, Operador >( "EXG", Operador( 9 , 2 )));
    operadores.insert( pair< string, Operador >( "JMP", Operador( 10 , 1 )));
    operadores.insert( pair< string, Operador >( "LSL", Operador( 11 , 2 )));
    operadores.insert( pair< string, Operador >( "LSR",	Operador( 12 , 2 )));
    operadores.insert( pair< string, Operador >( "MOVE", Operador( 13 , 2 )));
    operadores.insert( pair< string, Operador >( "MULS", Operador( 14 , 2 )));
    operadores.insert( pair< string, Operador >( "NEG", Operador( 15 , 1 )));
    operadores.insert( pair< string, Operador >( "NOP", Operador( 16 , 0 )));
    operadores.insert( pair< string, Operador >( "NOT", Operador( 17 , 1 )));
    operadores.insert( pair< string, Operador >( "OR", Operador( 18 , 2 )));
    operadores.insert( pair< string, Operador >( "ORI", Operador( 19 , 2 )));
    operadores.insert( pair< string, Operador >( "SUB", Operador( 20 , 2 )));
    operadores.insert( pair< string, Operador >( "SUBI", Operador( 21 , 2 )));
    operadores.insert( pair< string, Operador >( "BEQ", Operador( 22 , 1 )));
    operadores.insert( pair< string, Operador >( "BGE", Operador( 23 , 1 )));
    operadores.insert( pair< string, Operador >( "BGT", Operador( 24 , 1 )));
    operadores.insert( pair< string, Operador >( "BLE", Operador( 25 , 1 )));
    operadores.insert( pair< string, Operador >( "BLT", Operador( 26 , 1 )));
    operadores.insert( pair< string, Operador >( "BMI", Operador( 27 , 1 )));
    operadores.insert( pair< string, Operador >( "BPL", Operador( 28 , 1 )));
    operadores.insert( pair< string, Operador >( "HALT", Operador( 29 , 0 )));
}

M68KAssembler::M68KAssembler(string code)
{
     // programa a ser traduzido, cria a stream com a string passada.
    entrada << upper(code);

    instcount = 0;

    //cout << code << endl;

    // map aqui, nao chequei
    operadores.insert( pair< string, Operador >( "ADD",	 Operador( 0 , 2 )));
    operadores.insert( pair< string, Operador >( "ADDI", Operador( 1 , 2 )));
    operadores.insert( pair< string, Operador >( "AND",  Operador( 2 , 2 )));
    operadores.insert( pair< string, Operador >( "ANDI", Operador( 3 , 2 )));
    operadores.insert( pair< string, Operador >( "CLR",  Operador( 4 , 1 )));
    operadores.insert( pair< string, Operador >( "CMP",  Operador( 5 , 2 )));
    operadores.insert( pair< string, Operador >( "DIVS", Operador( 6 , 2 )));
    operadores.insert( pair< string, Operador >( "EOR", Operador( 7 , 2 )));
    operadores.insert( pair< string, Operador >( "EORI",Operador( 8 , 2 )));
    operadores.insert( pair< string, Operador >( "EXG", Operador( 9 , 2 )));
    operadores.insert( pair< string, Operador >( "JMP", Operador( 10 , 1 )));
    operadores.insert( pair< string, Operador >( "LSL", Operador( 11 , 2 )));
    operadores.insert( pair< string, Operador >( "LSR",	Operador( 12 , 2 )));
    operadores.insert( pair< string, Operador >( "MOVE", Operador( 13 , 2 )));
    operadores.insert( pair< string, Operador >( "MULS", Operador( 14 , 2 )));
    operadores.insert( pair< string, Operador >( "NEG", Operador( 15 , 1 )));
    operadores.insert( pair< string, Operador >( "NOP", Operador( 16 , 0 )));
    operadores.insert( pair< string, Operador >( "NOT", Operador( 17 , 1 )));
    operadores.insert( pair< string, Operador >( "OR", Operador( 18 , 2 )));
    operadores.insert( pair< string, Operador >( "ORI", Operador( 19 , 2 )));
    operadores.insert( pair< string, Operador >( "SUB", Operador( 20 , 2 )));
    operadores.insert( pair< string, Operador >( "SUBI", Operador( 21 , 2 )));
    operadores.insert( pair< string, Operador >( "BEQ", Operador( 22 , 1 )));
    operadores.insert( pair< string, Operador >( "BGE", Operador( 23 , 1 )));
    operadores.insert( pair< string, Operador >( "BGT", Operador( 24 , 1 )));
    operadores.insert( pair< string, Operador >( "BLE", Operador( 25 , 1 )));
    operadores.insert( pair< string, Operador >( "BLT", Operador( 26 , 1 )));
    operadores.insert( pair< string, Operador >( "BMI", Operador( 27 , 1 )));
    operadores.insert( pair< string, Operador >( "BPL", Operador( 28 , 1 )));
    operadores.insert( pair< string, Operador >( "HALT", Operador( 29 , 0 )));


    //cout << code << endl;

    //passada1();


}
M68KAssembler::~M68KAssembler()
{

}

void M68KAssembler::passada1() //throw M68KAssemblerException;
{

    string palavra,trabckBack;
    Estado q = q0;
    short offset = 0;

    UINT i = 0;

    map<string, Operador>::iterator it, itback;
    map<string, Ponteiro>::iterator itlab1,itlab2;

    short instruction;
    short opcode = 0,typeaddrOp1 = 0,typeaddrOp2 = 0,op1 = 0,op2 = 0;
    short imediato = 0;
    bool lab1 = false,lab2 = false, imm = false;


    entrada >> palavra;

    while(!entrada.eof())
    {

        //cout << palavra << endl;

        it = operadores.find(palavra);

        //cout << q << endl;

        //cout << "offset: " << offset << "\npalavra: " << palavra << endl;

        cout << "palavra: " << palavra << " \toffset:  " << offset << "\t Estado: " << q << endl;


        if(q == q0)
        {
            //cout << q << endl;
            if(operadores.count(palavra) > 0)
            {

                itback = it;

                opcode = it->second.id;

                opcode = opcode << 10;

                // aqui poderia iniciar a descobrir o tipo de endereçamento mas.. talvez arruma na 2 passada
                if(it->second.operandos == 0)
                {

                    typeaddrOp1 = 0;
                    typeaddrOp1 = typeaddrOp1 << 8;

                    typeaddrOp2 = 0;
                    typeaddrOp2 = typeaddrOp2 << 6;

                    op1 = 0;
                    op1 = op1 << 3;

                    op2 = 0;


                    instruction = opcode | typeaddrOp1 | typeaddrOp2 | op1 | op2;

                    instcount++;

                    traduzido.push_back(instruction);
                    i++;

                    instruction = instruction & 0;
                    opcode = opcode & 0;
                    typeaddrOp1 = typeaddrOp1 & 0;
                    typeaddrOp2 = typeaddrOp2 & 0;
                    op1 = op1 & 0;
                    op2 = op2 & 0;

                    q = q0;
                    offset++;

                }
                else
                {
                    q = q2;
                    offset++;
                }


            }
            // igual space
            else if(palavra == "DS")
            {
                q = q0;
                offset++;

                traduzido.push_back(0);
                i++;
            }
            // igual const
            else if(palavra == "DC")
            {
                q = q6;
                offset++;

            }
            else // tratando label
            {
                //cout << palavra << "oasosodas"<< endl;

                if(tabelaSimbolos.count(palavra) == 0)
                {
                    tabelaSimbolos.insert(pair<string,Ponteiro>(palavra,Ponteiro(offset)));

                }
                else
                    tabelaSimbolos.find(palavra)->second.ref = offset;

                q = q1;

                trabckBack = palavra;


            }

        }
        else if(q == q1)
        {
            if(palavra == "DC")
            {
                // começa a resolver referencia
                q = q7;
                offset++;

                cout << trabckBack << " aAA" <<endl;

                //itlab1 = tabelaSimbolos.find(trabckBack);
            }
            else if(operadores.count(palavra) > 0)
            {
                //  leu label e opcode

                itback = operadores.find(palavra);

                opcode = itback->second.id;

                opcode = opcode << 10;

                if(itback->second.operandos == 0)
                {
                    typeaddrOp1 = 0;
                    typeaddrOp1 = typeaddrOp1 << 8;

                    typeaddrOp2 = 0;
                    typeaddrOp2 = typeaddrOp2 << 6;

                    op1 = 0;
                    op1 = op1 << 3;

                    op2 = 0;


                    instruction = opcode | typeaddrOp1 | typeaddrOp2 | op1 | op2;
                    instcount++;

                    traduzido.push_back(instruction);
                    i++;

                    instruction = instruction & 0;
                    opcode = opcode & 0;
                    typeaddrOp1 = typeaddrOp1 & 0;
                    typeaddrOp2 = typeaddrOp2 & 0;
                    op1 = op1 & 0;
                    op2 = op2 & 0;

                    offset++;

                    tabelaSimbolos.find(trabckBack)->second.ref = offset;

                    q = q0;
                }
                else
                {

                    q = q2;
                    offset++;
                }

            }
            else if(palavra == "DS")
            {
                q = q0;
                traduzido.push_back(0);
                offset++;
                i++;
            }
            //else erro: leu label e label

        }
        else if(q == q2)
        {

            if(itback->second.operandos == 1)
            {
                // trata so um operando op1, monta a instrução e vai pra q0

                if(palavra[0] == 'D' && palavra.size() == 2)
                {
                    char x = palavra[1];
                    UINT registerindex = atoi(&x);

                    typeaddrOp1 = 0;
                    typeaddrOp1 = typeaddrOp1 << 8;

                    op1 = registerindex;
                    op1 = op1 << 3;

                }
                else if(palavra[0] == 'A' && palavra.size() == 2)
                {
                    char x = palavra[1];
                    UINT registerindex = atoi(&x);

                    typeaddrOp1 = 1;
                    typeaddrOp1 = typeaddrOp1 << 8;

                    op1 = registerindex;
                    op1 = op1 << 3;

                }
                else if(palavra[0] == '(' && palavra[palavra.size() - 1] == ')' )
                {
                    if(palavra[1] == 'A')
                    {
                        char x = palavra[palavra.size() - 2];
                        UINT registerindex = atoi(&x);

                        typeaddrOp1 = 2;
                        typeaddrOp1 = typeaddrOp1 << 8;

                        op1 = registerindex;
                        op1 = op1 << 3;
                    }
                    //else erro

                }
                else if(palavra[0] == '#')
                {
                    char x = palavra[1];
                    UINT im = atoi(&x);

                    typeaddrOp1 = 0;
                    typeaddrOp1 = typeaddrOp1 << 8;

                    op1 = 0;

                    imm = true;

                    imediato = im;

                }
                else
                {
                    if(tabelaSimbolos.count(palavra) == 0)
                        tabelaSimbolos.insert(pair<string,Ponteiro>(palavra,Ponteiro(offset)));

                    typeaddrOp1 = 3;
                    typeaddrOp1 = typeaddrOp1 << 8;

                    lab1 = true;

                    itlab1 = tabelaSimbolos.find(palavra);

                    op1 = 0;

                    offset++;
                }

                instruction = opcode | typeaddrOp1 | typeaddrOp2 | op1 | op2;

                instcount++;

                traduzido.push_back(instruction);
                i++;

                if(lab1)
                {
                    traduzido.push_back(0);
                    itlab1->second.backTrackIndex.push_back(i);
                    i++;

                    lab1 = false;
                }
                if(imm)
                {
                    traduzido.push_back(imediato);
                    i++;
                    imm = false;
                }

                // reseta todo mundo
                instruction = instruction & 0;
                opcode = opcode & 0;
                typeaddrOp1 = typeaddrOp1 & 0;
                typeaddrOp2 = typeaddrOp2 & 0;
                op1 = op1 & 0;
                op2 = op2 & 0;

                q = q0;

            }
            else
            {
                // trata so um operando op1 e vai pra q3 montar a instrução

                if(palavra[0] == 'D' && palavra.size() == 2)
                {
                    char x = palavra[1];
                    UINT registerindex = atoi(&x);

                    typeaddrOp1 = 0;
                    typeaddrOp1 = typeaddrOp1 << 8;

                    op1 = registerindex;
                    op1 = op1 << 3;

                }
                else if(palavra[0] == 'A' && palavra.size() == 2)
                {
                    char x = palavra[1];
                    UINT registerindex = atoi(&x);

                    typeaddrOp1 = 1;
                    typeaddrOp1 = typeaddrOp1 << 8;

                    op1 = registerindex;
                    op1 = op1 << 3;

                }
                else if(palavra[0] == '(' && palavra[palavra.size() - 1] == ')' )
                {

                    if(palavra[1] == 'A')
                    {
                        char x = palavra[palavra.size() - 2];
                        UINT registerindex = atoi(&x);

                        typeaddrOp1 = 2;
                        typeaddrOp1 = typeaddrOp1 << 8;

                        op1 = registerindex;
                        op1 = op1 << 3;

                    }
                    //else
                        //erro


                }
                else if(palavra[0] == '#')
                {
                    string aux = palavra;

                    aux = aux.erase(0,1);

                    imediato = atoi(aux.c_str());

                    typeaddrOp1 = 0;
                    typeaddrOp1 = typeaddrOp1 << 8;

                    op1 = 0;

                    imm = true;

                }
                else
                {
                    if(tabelaSimbolos.count(palavra) == 0)
                        tabelaSimbolos.insert(pair<string,Ponteiro>(palavra,Ponteiro(offset)));


                    typeaddrOp1 = 3;
                    typeaddrOp1 = typeaddrOp1 << 8;

                    lab1 = true;

                    itlab1 = tabelaSimbolos.find(palavra);

                    op1 = 0;

                    offset++;
                }

                q = q3;

            }

        }
        else if(q == q3)
        {

            if(palavra[0] == 'D' && palavra.size() == 2)
            {
                char x = palavra[1];
                UINT registerindex = atoi(&x);

                typeaddrOp2 = 0;
                typeaddrOp2 = typeaddrOp2 << 6;
                op2 = registerindex;

            }
            else if(palavra[0] == 'A' && palavra.size() == 2)
            {
                char x = palavra[1];
                UINT registerindex = atoi(&x);
                typeaddrOp2 = 1;
                typeaddrOp2 = typeaddrOp2 << 6;

                op2 = registerindex;

            }
            else if(palavra[0] == '(' && palavra[palavra.size() - 1] == ')' )
            {
                if(palavra[1] == 'A')
                {
                    char x = palavra[palavra.size() - 2];
                    UINT registerindex = atoi(&x);


                    typeaddrOp2 = 2;
                    typeaddrOp2 = typeaddrOp2 << 6;

                    op2 = registerindex;
                }
                //else
                //	throw M68KException("Erro de sintaxe.",instcount);
            }
            else
            {

                if(tabelaSimbolos.count(palavra) == 0)
                    tabelaSimbolos.insert(pair<string,Ponteiro>(palavra,Ponteiro(offset)));


                typeaddrOp2 = 3;
                typeaddrOp2 = typeaddrOp2 << 6;

                lab2 = true;

                itlab2 = tabelaSimbolos.find(palavra);

                op2 = 0;

                offset++;
            }


            instruction = opcode | typeaddrOp1 | typeaddrOp2 | op1 | op2;

            instcount++;

            traduzido.push_back(instruction);
            i++;

            if(lab1)
            {
                traduzido.push_back(0);
                itlab1->second.backTrackIndex.push_back(i);
                i++;
                lab1 = false;
            }

            if(lab2)
            {
                traduzido.push_back(0);
                itlab2->second.backTrackIndex.push_back(i);
                i++;
                lab2 = false;
            }

            if(imm)
            {
                traduzido.push_back(imediato);
                i++;
                imm = false;
            }

            // reseta todo mundo
            instruction = instruction & 0;
            opcode = opcode & 0;
            typeaddrOp1 = typeaddrOp1 & 0;
            typeaddrOp2 = typeaddrOp2 & 0;
            op1 = op1 & 0;
            op2 = op2 & 0;

            q = q0;

        }
        else if(q == q6)
        {
            if(isNumeral(palavra))
            {
                short n = atoi(palavra.c_str());

                traduzido.push_back(n);
                i++;

                q = q0;
            }
            //else erro
        }
        else if(q == q7)
        {
            if(isNumeral(palavra))
            {
                short n = atoi(palavra.c_str());

                traduzido.push_back(n);
                i++;

                // tratando referências;
                tabelaSimbolos.find(trabckBack)->second.ref = offset;

                q = q0;
            }
            //else erro
        }

        entrada >> palavra;

    }

    // passada 2, resolvendo as referencias

    for(map<string,Ponteiro>::iterator it = tabelaSimbolos.begin(); it != tabelaSimbolos.end(); it++)
    {
        //cout << it->first << " " << it->second.ref << endl;
        for(unsigned int i = 0; i < it->second.backTrackIndex.size(); i++)
        {
            traduzido[it->second.backTrackIndex[i]] = it->second.ref;
        }
        //cout << "\n";
    }



    for(unsigned int i = 0; i < traduzido.size(); i++)
    {
        cout << traduzido[i] << endl;
    }



    for(map<string,Ponteiro>::iterator it = tabelaSimbolos.begin(); it != tabelaSimbolos.end(); it++)
    {
        cout << it->first << " ref:" << it->second.ref << endl;
        for(unsigned int i = 0; i < it->second.backTrackIndex.size(); i++)
        {
            cout << it->second.backTrackIndex[i] << " ";
        }
        cout << "\n";
    }



}


void M68KAssembler::run()
{

    passada1();
}


vector<unsigned short> M68KAssembler::getProgram()
{
    return traduzido;
}
