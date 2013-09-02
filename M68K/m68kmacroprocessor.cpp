#include "m68kmacroprocessor.h"

using namespace std;
using std::vector;

vector<string> &split(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, char delim);

Macro::Macro()
{

}

Macro::~Macro()
{

}

void Macro::setName(string name)
{
    this->macroName = name;
}

void Macro::addParam(string param)
{
    this->param.push_back(param);
}

void Macro::addParam(vector<string> param)
{
    for (int i = 0; i < (int)param.size(); i++)
        this->param.push_back(param[i]);
}

void Macro::addInstruc(string instruc)
{
    this->instruc.push_back(instruc);
}

string Macro::getName()
{
    return this->macroName;
}

vector<string> Macro::getParam()
{
    return this->param;
}

vector<string> Macro::getInstruc()
{
    return this->instruc;
}

vector<string> Macro::swapParam(vector<string> paramList)
{
    vector<string> final;

    for (int i = 0; i < (int)instruc.size(); i++)
    {
        vector<string> splitted = split(instruc[i], ' ');
        string line = "";

        for (int j = 0; j < (int)splitted.size(); j++)
        {
            for (int h = 0; h < (int)this->param.size(); h++)
            {
                if (splitted[j] == this->param[h])
                {
                    splitted[j] = paramList[h];
                }
            }

            line += splitted[j] + ' ';
        }

        final.push_back(line);
    }

    final.erase(final.begin());

    return final;
}

M68KMacroProcessor::M68KMacroProcessor()
{

}

M68KMacroProcessor::M68KMacroProcessor(string code)
{
    this->code = code;
}

M68KMacroProcessor::~M68KMacroProcessor()
{

}

void M68KMacroProcessor::run()
{
    stringstream strs(code);
    stringstream saida;
    vector<string> content;
    string line;
    int i,j,h,k;

    Macro *macro;
    vector<Macro> macroList;
    bool macroFlag = false;

    vector<string> splitted;
    string anterior;
    string posterior;

    while(getline(strs, line))
    {
        content.push_back(line);
    }

    int countContent = content.size();

    //Laço para achar macros e armazanar seus nomes
    for (i = 0; i < countContent ; i++)
    {
        splitted = split(content[i], ' ');

        for (j = 0; j < (int)splitted.size(); j++)
        {
            if (macroFlag == false && j + 1 != (int)splitted.size())
            {
                anterior = splitted[j];
                posterior = splitted[j + 1];
                if (posterior == "MCDEF")	//Achei a macro \o/
                {
                    macro = new Macro();
                    macro->setName(anterior);	//Nomearemos
                    macro->addParam(split(splitted[j + 2], ','));
                    macroFlag = true;
                }
            }
            else
            {
                if(splitted[j] == "MCEND")
                {
                    macroFlag = false;
                    macroList.push_back(*macro); //Tá dando problemas, não consigo pegar toda macro criada e jogar pra lista de macros
                }
                /*else
                {
                    macro->addInstruc(content[i]);
                }*/
            }
        }

        if (macroFlag) macro->addInstruc(content[i]);
    }

    vector<string>::iterator it;

    for (it = content.begin(); it != content.end();)
    {
        if (it->find("MCDEF") != string::npos)
        {
            while (it->find("MCEND") == string::npos)
                it = content.erase(it);
            it = content.erase(it);
        }
        else it++;
    }

    countContent = (int)content.size();
    bool found;

    //laço que troca os parâmetros e tira as macros e coloca suas definições
    for (i = 0; i < countContent; i++)
    {
        splitted = split(content[i],' ');

        for (j = 0; j < (int)splitted.size(); j++)
        {
            found = false;

            if (splitted[j].size() > 0)
            {
                while (splitted[j][0] == '\t')
                {
                    splitted[j].replace(0, 1, "");
                }
            }

            for(h = 0; h < (int)macroList.size(); h++)
            {
                if(splitted[j] == macroList[h].getName())
                {
                    vector<string> aux = split(splitted[j+1], ',');
                    aux = macroList[h].swapParam(aux);
                    found = true;
                    for(k = 0; k < (int)aux.size(); k++)
                    {
                        saida << aux[k] << endl;
                    }
                }
            }

            if (!found) saida << splitted[j] << ' ';
            else break;
        }

        saida << endl;
    }

    code = saida.str();
}

void M68KMacroProcessor::setCode(string code)
{
    this->code = code;
}

string M68KMacroProcessor::getCode()
{
    return this->code;
}

vector<string> &split(const string &s, char delim, vector<string> &elems)
{
    stringstream ss(s);
    string item;
    while(getline(ss,item,delim))
    {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim)
{
    vector<string> elems;
    split(s,delim,elems);
    return elems;
}
