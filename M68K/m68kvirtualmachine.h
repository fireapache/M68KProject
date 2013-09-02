#ifndef M68KVM
#define M68KVM

#include <exception>
#include <string>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

#ifndef UINT
#define UINT unsigned int
#endif

#ifndef USHORT
#define USHORT unsigned short
#endif

enum EOperators
{
	ADD = 0,
	ADDI,
	AND,
	ANDI,
	CLR,
	CMP,
	DIVS,
	EOR,
	EORI,
	EXG,
	JMP,
	LSL,
	LSR,
	MOVE,
	MULS,
	NEG,
	NOP,
	NOT,
	OR,
	ORI,
	SUB,
	SUBI,
	BEQ,
	BGE,
	BGT,
	BLE,
	BLT,
	BMI,
	BPL,
	HALT
};

//class M68KException : public exception
//{
//	string msg;
//	int inst;
//
//public:
//
//	M68KException(string msg, int i);
//	~M68KException();
//
//	string getMsg();
//	int getInst();
//};

class M68KVirtualMachine
{
	vector<USHORT> program;

	map<USHORT, short> dataMemory;

	string output;

	USHORT *A;
	short *D;

	USHORT PC;

	bool *cc;
	bool halt;

	short getData(USHORT address);
	void setData(USHORT address, short data);

	void setCCState(USHORT endtype, USHORT op2);

public:

	M68KVirtualMachine();
	~M68KVirtualMachine();

	void run();
	void nextStep();
	void reset();

	void setProgram(vector<USHORT> program);

	void getOutput() const;
	vector<USHORT> getAs();
	vector<short> getDs();
	vector<pair<USHORT, short> > getMemory();
	USHORT getPC() const;
    vector<bool> getCCs();
};

#endif
