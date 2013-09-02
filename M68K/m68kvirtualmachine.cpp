#include "m68kvirtualmachine.h"

//M68KException::M68KException(string msg, int i)
//{
//	this->msg = msg;
//	this->inst = i;
//}
//
//M68KException::~M68KException()
//{
//
//}
//
//string M68KException::getMsg()
//{
//	return msg;
//}
//
//int M68KException::getInst()
//{
//	return inst;
//}

M68KVirtualMachine::M68KVirtualMachine()
{
	output = "";

	A = new USHORT[8];
	D = new short[8];

	for (int i = 0; i < 8; i++)
	{
		A[i] = 0;
		D[i] = 0;
	}

	PC = 0;
	cc = new bool[5];

	for (int i = 0; i < 5; i++)
		cc[i] = 0;

	halt = false;
}

M68KVirtualMachine::~M68KVirtualMachine()
{
	delete [] A;
	delete [] D;
	delete [] cc;
}

vector<bool> M68KVirtualMachine::getCCs()
{
    vector<bool> _return;

    for (int i = 0; i < 5; i++)
    {
        _return.push_back(cc[i]);
    }

    return _return;
}

short M68KVirtualMachine::getData(USHORT address)
{
	if (dataMemory.count(address) == 0)
	{
		dataMemory.insert(pair<USHORT, short>(address, program[address]));
	}

	return dataMemory.find(address)->second;
}

void M68KVirtualMachine::setData(USHORT address, short data)
{
	if (dataMemory.count(address) == 0)
	{
		dataMemory.insert(pair<USHORT, short>(address, program[address]));
	}

	dataMemory.find(address)->second = data;
}

void M68KVirtualMachine::setCCState(USHORT endtype, USHORT op2)
{
	short result;

	endtype &= 3;

	if (endtype == 0)			result = D[op2];
	else if (endtype == 1)		result = A[op2];
	else if (endtype == 2)		result = getData(A[op2]);
	else if (endtype == 3)		result = getData(program[PC - 1]);

	if (result < 0) cc[1] = true;
	else result = false;

	if (result == 0) cc[2] = true;
	else result = false;
}

void M68KVirtualMachine::run()
{
	while (!halt) nextStep();
}

void M68KVirtualMachine::nextStep()
{
	short inst, opcode, endtype, op1, op2;
	short cmp1, cmp2, comp;
	int icomp;
	short aux;

	if (program.size() == 0) return;
    if (halt) return;

	inst = program[PC];

	PC++;

	opcode = inst >> 10;
	endtype = (inst >> 6) & 0xF;
	op1 = (inst >> 3) & 0x7;
	op2 = inst & 0x7;

	output = "";

	if (opcode == ADD)
	{
		if (endtype == 0)			D[op2] += D[op1];
		else if (endtype == 1)		A[op2] += D[op1];
		else if (endtype == 2)		setData(A[op2],	D[op1] + getData(A[op2]));
		else if (endtype == 3)		{setData(program[PC], D[op1] + getData(program[PC])); PC++;}
		else if (endtype == 4)		D[op2] += A[op1];
		else if (endtype == 5)		A[op2] += A[op1];
		else if (endtype == 6)		setData(A[op2], A[op1] + getData(A[op2]));
		else if (endtype == 7)		{setData(program[PC], A[op1] + getData(program[PC])); PC++;}
		else if (endtype == 8)		D[op2] += getData(A[op1]);
		else if (endtype == 9)		A[op2] += getData(A[op1]);
		else if (endtype == 10)		setData(A[op2], getData(A[op1]) + getData(A[op2]));
		else if (endtype == 11)		{setData(program[PC], getData(A[op1]) + getData(program[PC])); PC++;}
		else if (endtype == 12)		{D[op2] += getData(program[PC]); PC++;}
		else if (endtype == 13)		{A[op2] += getData(program[PC]); PC++;}
		else if (endtype == 14)		{setData(A[op2], getData(program[PC]) + getData(A[op2])); PC++;}
		else if (endtype == 15)		{setData(program[PC + 1], getData(program[PC]) + getData(program[PC + 1])); PC += 2;}

		setCCState(endtype, op2);

	}
	else if (opcode == ADDI)
	{
		if (endtype == 0)			{D[op2] += program[PC]; PC++;}
		else if (endtype == 1)		{A[op2] += program[PC]; PC++;}
		else if (endtype == 2)		{setData(A[op2], program[PC] + getData(A[op2])); PC++;}
		else if (endtype == 3)		{setData(program[PC + 1], program[PC] + getData(program[PC + 1])); PC += 2;}

		setCCState(endtype, op2);
	}
	else if (opcode == AND)
	{
		if (endtype == 0)			D[op2] &= D[op1];
		else if (endtype == 1)		A[op2] &= D[op1];
		else if (endtype == 2)		setData(A[op2],	D[op1] & getData(A[op2]));
		else if (endtype == 3)		{setData(program[PC], D[op1] & getData(program[PC])); PC++;}
		else if (endtype == 4)		D[op2] &= A[op1];
		else if (endtype == 5)		A[op2] &= A[op1];
		else if (endtype == 6)		setData(A[op2], A[op1] & getData(A[op2]));
		else if (endtype == 7)		{setData(program[PC], A[op1] & getData(program[PC])); PC++;}
		else if (endtype == 8)		D[op2] &= getData(A[op1]);
		else if (endtype == 9)		A[op2] &= getData(A[op1]);
		else if (endtype == 10)		setData(A[op2], getData(A[op1]) & getData(A[op2]));
		else if (endtype == 11)		{setData(program[PC], getData(A[op1]) & getData(program[PC])); PC++;}
		else if (endtype == 12)		{D[op2] &= getData(program[PC]); PC++;}
		else if (endtype == 13)		{A[op2] &= getData(program[PC]); PC++;}
		else if (endtype == 14)		{setData(A[op2], getData(program[PC]) & getData(A[op2])); PC++;}
		else if (endtype == 15)		{setData(program[PC + 1], getData(program[PC]) & getData(program[PC + 1])); PC += 2;}

		cc[3] = cc[4] = false;

		setCCState(endtype, op2);
	}
	else if (opcode == ANDI)
	{
		if (endtype == 0)			{D[op2] &= program[PC]; PC++;}
		else if (endtype == 1)		{A[op2] &= program[PC]; PC++;}
		else if (endtype == 2)		{setData(A[op2], program[PC] & getData(A[op2])); PC++;}
		else if (endtype == 3)		{setData(program[PC + 1], program[PC] & getData(program[PC + 1])); PC += 2;}

		cc[3] = cc[4] = false;

		setCCState(endtype, op2);
	}
	else if (opcode == CLR)
	{
		endtype &= 12;

		if (endtype == 0)			D[op1] = 0;
		else if (endtype == 4)		A[op1] = 0;
		else if (endtype == 8)		setData(A[op1], 0);
		else if (endtype == 12)		{setData(program[PC], 0); PC++;}

		cc[1] = false;
		cc[2] = true;
		cc[3] = false;
		cc[4] = false;
	}
	else if (opcode == CMP)
	{
		if (endtype == 0)			{cmp1 = D[op1]; cmp2 = D[op2];}
		else if (endtype == 1)		{cmp1 = D[op1]; cmp2 = A[op2];}
		else if (endtype == 2)		{cmp1 = D[op1]; cmp2 = getData(A[op2]);}
		else if (endtype == 3)		{cmp1 = D[op1]; cmp2 = getData(program[PC]); PC++;}
		else if (endtype == 4)		{cmp1 = A[op1]; cmp2 = D[op2];}
		else if (endtype == 5)		{cmp1 = A[op1]; cmp2 = A[op2];}
		else if (endtype == 6)		{cmp1 = A[op1]; cmp2 = getData(A[op2]);}
		else if (endtype == 7)		{cmp1 = A[op1]; cmp2 = getData(program[PC]); PC++;}
		else if (endtype == 8)		{cmp1 = getData(A[op1]); cmp2 = D[op2];}
		else if (endtype == 9)		{cmp1 = getData(A[op1]); cmp2 = A[op2];}
		else if (endtype == 10)		{cmp1 = getData(A[op1]); cmp2 = getData(A[op2]);}
		else if (endtype == 11)		{cmp1 = getData(A[op1]); cmp2 = getData(program[PC]); PC++;}
		else if (endtype == 12)		{cmp1 = getData(program[PC]); cmp2 = D[op2]; PC++;}
		else if (endtype == 13)		{cmp1 = getData(program[PC]); cmp2 = A[op2]; PC++;}
		else if (endtype == 14)		{cmp1 = getData(program[PC]); cmp2 = getData(A[op2]); PC++;}
		else if (endtype == 15)		{cmp1 = getData(program[PC]); cmp2 = getData(program[PC + 1]); PC += 2;}

		comp = cmp2 - cmp1;
		icomp = (int)cmp2 - (int)cmp1;

		if (comp < 0) cc[1] = true;
		else cc[1] = false;

		if (comp == 0) cc[2] = true;
		else cc[2] = false;

		if (icomp != comp) cc[3] = true;
		else cc[3] = false;
	}
	else if (opcode == DIVS)
	{
		if (endtype == 0)			D[op2] /= D[op1];
		else if (endtype == 1)		A[op2] /= D[op1];
		else if (endtype == 2)		setData(A[op2],	D[op1] / getData(A[op2]));
		else if (endtype == 3)		{setData(program[PC], D[op1] / getData(program[PC])); PC++;}
		else if (endtype == 4)		D[op2] /= A[op1];
		else if (endtype == 5)		A[op2] /= A[op1];
		else if (endtype == 6)		setData(A[op2], A[op1] / getData(A[op2]));
		else if (endtype == 7)		{setData(program[PC], A[op1] / getData(program[PC])); PC++;}
		else if (endtype == 8)		D[op2] /= getData(A[op1]);
		else if (endtype == 9)		A[op2] /= getData(A[op1]);
		else if (endtype == 10)		setData(A[op2], getData(A[op1]) / getData(A[op2]));
		else if (endtype == 11)		{setData(program[PC], getData(A[op1]) / getData(program[PC])); PC++;}
		else if (endtype == 12)		{D[op2] /= getData(program[PC]); PC++;}
		else if (endtype == 13)		{A[op2] /= getData(program[PC]); PC++;}
		else if (endtype == 14)		{setData(A[op2], getData(program[PC]) / getData(A[op2])); PC++;}
		else if (endtype == 15)		{setData(program[PC + 1], getData(program[PC]) / getData(program[PC + 1])); PC += 2;}

		cc[4] = false;

		setCCState(endtype, op2);
	}
	else if (opcode == EOR)
	{
		if (endtype == 0)			D[op2] |= D[op1];
		else if (endtype == 1)		A[op2] |= D[op1];
		else if (endtype == 2)		setData(A[op2],	D[op1] | getData(A[op2]));
		else if (endtype == 3)		{setData(program[PC], D[op1] | getData(program[PC])); PC++;}
		else if (endtype == 4)		D[op2] |= A[op1];
		else if (endtype == 5)		A[op2] |= A[op1];
		else if (endtype == 6)		setData(A[op2], A[op1] | getData(A[op2]));
		else if (endtype == 7)		{setData(program[PC], A[op1] | getData(program[PC])); PC++;}
		else if (endtype == 8)		D[op2] |= getData(A[op1]);
		else if (endtype == 9)		A[op2] |= getData(A[op1]);
		else if (endtype == 10)		setData(A[op2], getData(A[op1]) | getData(A[op2]));
		else if (endtype == 11)		{setData(program[PC], getData(A[op1]) | getData(program[PC])); PC++;}
		else if (endtype == 12)		{D[op2] |= getData(program[PC]); PC++;}
		else if (endtype == 13)		{A[op2] |= getData(program[PC]); PC++;}
		else if (endtype == 14)		{setData(A[op2], getData(program[PC]) | getData(A[op2])); PC++;}
		else if (endtype == 15)		{setData(program[PC + 1], getData(program[PC]) | getData(program[PC + 1])); PC += 2;}

		cc[3] = cc[4] = false;

		setCCState(endtype, op2);
	}
	else if (opcode == EORI)
	{
		if (endtype == 0)			{D[op2] |= program[PC]; PC++;}
		else if (endtype == 1)		{A[op2] |= program[PC]; PC++;}
		else if (endtype == 2)		{setData(A[op2], program[PC] | getData(A[op2])); PC++;}
		else if (endtype == 3)		{setData(program[PC + 1], program[PC] | getData(program[PC + 1])); PC += 2;}

		cc[3] = cc[4] = false;

		setCCState(endtype, op2);
	}
	else if (opcode == EXG)
	{
		if (endtype == 0)			{aux = D[op2]; D[op2] = D[op1]; D[op1] = aux;}
		else if (endtype == 1)		{aux = A[op2]; A[op2] = D[op1]; D[op1] = aux;}
		else if (endtype == 4)		{aux = D[op2]; D[op2] = A[op1]; A[op1] = aux;}
		else if (endtype == 5)		{aux = A[op2]; A[op2] = A[op1]; A[op1] = aux;}
	}
	else if (opcode == JMP)
	{
		PC = program[PC];

        //PC++;
	}
	else if (opcode == LSL)
	{
		if (endtype == 0)			{D[op2] = D[op2] << program[PC]; PC++;}
		else if (endtype == 1)		{A[op2] = A[op2] << program[PC]; PC++;}
		else if (endtype == 2)		{setData(A[op2], getData(A[op2]) << program[PC]); PC++;}
		else if (endtype == 3)		{setData(program[PC + 1], getData(program[PC + 1]) << program[PC]); PC += 2;}

		setCCState(endtype, op2);
	}
	else if (opcode == LSR)
	{
		if (endtype == 0)			{D[op2] = D[op2] >> program[PC]; PC++;}
		else if (endtype == 1)		{A[op2] = A[op2] >> program[PC]; PC++;}
		else if (endtype == 2)		{setData(A[op2], getData(A[op2]) >> program[PC]); PC++;}
		else if (endtype == 3)		{setData(program[PC + 1], getData(program[PC + 1]) >> program[PC]); PC += 2;}

		setCCState(endtype, op2);
	}
	else if (opcode == MOVE)
	{
		if (endtype == 0)			D[op2] = D[op1];
		else if (endtype == 1)		A[op2] = D[op1];
		else if (endtype == 2)		setData(A[op2],	D[op1]);
		else if (endtype == 3)		{setData(program[PC], D[op1]); PC++;}
		else if (endtype == 4)		D[op2] = A[op1];
		else if (endtype == 5)		A[op2] = A[op1];
		else if (endtype == 6)		setData(A[op2], A[op1]);
		else if (endtype == 7)		{setData(program[PC], A[op1]); PC++;}
		else if (endtype == 8)		D[op2] = getData(A[op1]);
		else if (endtype == 9)		A[op2] = getData(A[op1]);
		else if (endtype == 10)		setData(A[op2], getData(A[op1]));
		else if (endtype == 11)		{setData(program[PC], getData(A[op1])); PC++;}
		else if (endtype == 12)		{D[op2] = getData(program[PC]); PC++;}
		else if (endtype == 13)		{A[op2] = getData(program[PC]); PC++;}
		else if (endtype == 14)		{setData(A[op2], getData(program[PC])); PC++;}
		else if (endtype == 15)		{setData(program[PC + 1], getData(program[PC])); PC += 2;}

		setCCState(endtype, op2);
	}
	else if (opcode == MULS)
	{
		if (endtype == 0)			D[op2] *= D[op1];
		else if (endtype == 1)		A[op2] *= D[op1];
		else if (endtype == 2)		setData(A[op2],	D[op1] * getData(A[op2]));
		else if (endtype == 3)		{setData(program[PC], D[op1] * getData(program[PC])); PC++;}
		else if (endtype == 4)		D[op2] *= A[op1];
		else if (endtype == 5)		A[op2] *= A[op1];
		else if (endtype == 6)		setData(A[op2], A[op1] * getData(A[op2]));
		else if (endtype == 7)		{setData(program[PC], A[op1] * getData(program[PC])); PC++;}
		else if (endtype == 8)		D[op2] *= getData(A[op1]);
		else if (endtype == 9)		A[op2] *= getData(A[op1]);
		else if (endtype == 10)		setData(A[op2], getData(A[op1]) * getData(A[op2]));
		else if (endtype == 11)		{setData(program[PC], getData(A[op1]) * getData(program[PC])); PC++;}
		else if (endtype == 12)		{D[op2] *= getData(program[PC]); PC++;}
		else if (endtype == 13)		{A[op2] *= getData(program[PC]); PC++;}
		else if (endtype == 14)		{setData(A[op2], getData(program[PC]) * getData(A[op2])); PC++;}
		else if (endtype == 15)		{setData(program[PC + 1], getData(program[PC]) * getData(program[PC + 1])); PC += 2;}

		setCCState(endtype, op2);
	}
	else if (opcode == NEG)
	{
		if (endtype == 0)			D[op1] = ~D[op1];
		else if (endtype == 4)		A[op1] = ~A[op1];
		else if (endtype == 8)		setData(A[op1], ~getData(A[op1]));
		else if (endtype == 12)		{setData(program[PC], ~getData(program[PC])); PC++;}

		setCCState(endtype, op2);
	}
	else if (opcode == NOP)
	{

	}
	else if (opcode == NOT)
	{
		if (endtype == 0)			D[op1] = ~D[op1];
		else if (endtype == 4)		A[op1] = ~A[op1];
		else if (endtype == 8)		setData(A[op1], ~getData(A[op1]));
		else if (endtype == 12)		{setData(program[PC], ~getData(program[PC])); PC++;}

		setCCState(endtype, op2);
	}
	else if (opcode == OR)
	{
		if (endtype == 0)			D[op2] |= D[op1];
		else if (endtype == 1)		A[op2] |= D[op1];
		else if (endtype == 2)		setData(A[op2],	D[op1] | getData(A[op2]));
		else if (endtype == 3)		{setData(program[PC], D[op1] | getData(program[PC])); PC++;}
		else if (endtype == 4)		D[op2] |= A[op1];
		else if (endtype == 5)		A[op2] |= A[op1];
		else if (endtype == 6)		setData(A[op2], A[op1] | getData(A[op2]));
		else if (endtype == 7)		{setData(program[PC], A[op1] | getData(program[PC])); PC++;}
		else if (endtype == 8)		D[op2] |= getData(A[op1]);
		else if (endtype == 9)		A[op2] |= getData(A[op1]);
		else if (endtype == 10)		setData(A[op2], getData(A[op1]) | getData(A[op2]));
		else if (endtype == 11)		{setData(program[PC], getData(A[op1]) | getData(program[PC])); PC++;}
		else if (endtype == 12)		{D[op2] |= getData(program[PC]); PC++;}
		else if (endtype == 13)		{A[op2] |= getData(program[PC]); PC++;}
		else if (endtype == 14)		{setData(A[op2], getData(program[PC]) | getData(A[op2])); PC++;}
		else if (endtype == 15)		{setData(program[PC + 1], getData(program[PC]) | getData(program[PC + 1])); PC += 2;}

		setCCState(endtype, op2);
	}
	else if (opcode == ORI)
	{
		if (endtype == 0)			{D[op2] = D[op2] | program[PC]; PC++;}
		else if (endtype == 1)		{A[op2] = A[op2] | program[PC]; PC++;}
		else if (endtype == 2)		{setData(A[op2], getData(A[op2]) | program[PC]); PC++;}
		else if (endtype == 3)		{setData(program[PC + 1], getData(program[PC + 1]) | program[PC]); PC += 2;}

		setCCState(endtype, op2);
	}
	else if (opcode == SUB)
	{
		if (endtype == 0)			D[op2] -= D[op1];
		else if (endtype == 1)		A[op2] -= D[op1];
		else if (endtype == 2)		setData(A[op2],	D[op1] - getData(A[op2]));
		else if (endtype == 3)		{setData(program[PC], D[op1] - getData(program[PC])); PC++;}
		else if (endtype == 4)		D[op2] -= A[op1];
		else if (endtype == 5)		A[op2] -= A[op1];
		else if (endtype == 6)		setData(A[op2], A[op1] - getData(A[op2]));
		else if (endtype == 7)		{setData(program[PC], A[op1] - getData(program[PC])); PC++;}
		else if (endtype == 8)		D[op2] -= getData(A[op1]);
		else if (endtype == 9)		A[op2] -= getData(A[op1]);
		else if (endtype == 10)		setData(A[op2], getData(A[op1]) - getData(A[op2]));
		else if (endtype == 11)		{setData(program[PC], getData(A[op1]) - getData(program[PC])); PC++;}
		else if (endtype == 12)		{D[op2] -= getData(program[PC]); PC++;}
		else if (endtype == 13)		{A[op2] -= getData(program[PC]); PC++;}
		else if (endtype == 14)		{setData(A[op2], getData(program[PC]) - getData(A[op2])); PC++;}
		else if (endtype == 15)		{setData(program[PC + 1], getData(program[PC]) - getData(program[PC + 1])); PC += 2;}

		setCCState(endtype, op2);
	}
	else if (opcode == SUBI)
	{
		if (endtype == 0)			{D[op2] = D[op2] - program[PC]; PC++;}
		else if (endtype == 1)		{A[op2] = A[op2] - program[PC]; PC++;}
		else if (endtype == 2)		{setData(A[op2], getData(A[op2]) - program[PC]); PC++;}
		else if (endtype == 3)		{setData(program[PC + 1], getData(program[PC + 1]) - program[PC]); PC += 2;}

		setCCState(endtype, op2);
	}
	else if (opcode == BEQ)
	{
		if (cc[2] == true)
		{
			if (endtype == 0)			PC = D[op1];
			else if (endtype == 4)		PC = A[op1];
			else if (endtype == 8)		PC = getData(A[op1]);
            else if (endtype == 12)		PC = program[PC];
		}
        else
        {
            PC++;
        }
	}
	else if (opcode == BGE)
	{
		if (cc[1] == false || cc[2] == true)
		{
			if (endtype == 0)			PC = D[op1];
			else if (endtype == 4)		PC = A[op1];
			else if (endtype == 8)		PC = getData(A[op1]);
            else if (endtype == 12)		PC = program[PC];
		}
        else
        {
            PC++;
        }
	}
	else if (opcode == BGT)
	{
		if (cc[1] == false && cc[2] == false)
		{
			if (endtype == 0)			PC = D[op1];
			else if (endtype == 4)		PC = A[op1];
			else if (endtype == 8)		PC = getData(A[op1]);
            else if (endtype == 12)		PC = program[PC];
		}
        else
        {
            PC++;
        }
	}
	else if (opcode == BLE)
	{
		if (cc[1] == true || cc[2] == true)
		{
			if (endtype == 0)			PC = D[op1];
			else if (endtype == 4)		PC = A[op1];
			else if (endtype == 8)		PC = getData(A[op1]);
            else if (endtype == 12)		PC = program[PC];
		}
        else
        {
            PC++;
        }
	}
	else if (opcode == BLT)
	{
		if (cc[1] == true && cc[2] == false)
		{
			if (endtype == 0)			PC = D[op1];
			else if (endtype == 4)		PC = A[op1];
			else if (endtype == 8)		PC = getData(A[op1]);
            else if (endtype == 12)		PC = program[PC];
		}
        else
        {
            PC++;
        }
	}
	else if (opcode == BMI)
	{
		if (cc[1] == true)
		{
			if (endtype == 0)			PC = D[op1];
			else if (endtype == 4)		PC = A[op1];
			else if (endtype == 8)		PC = getData(A[op1]);
            else if (endtype == 12)		PC = program[PC];
		}
        else
        {
            PC++;
        }
	}
	else if (opcode == BPL)
	{
		if (cc[1] == false)
		{
			if (endtype == 0)			PC = D[op1];
			else if (endtype == 4)		PC = A[op1];
			else if (endtype == 8)		PC = getData(A[op1]);
            else if (endtype == 12)		PC = program[PC];
		}
        else
        {
            PC++;
        }
	}
	else if (opcode == HALT)
	{
		halt = true;
	}

	/*
	if (opcode == ADD)			output.append("ADD");
	else if (opcode == ADDI)	output.append("ADDI");
	else if (opcode == AND)		output.append("AND");
	else if (opcode == ANDI)	output.append("ANDI");
	else if (opcode == CLR)		output.append("CLR");
	else if (opcode == CMP)		output.append("CMP");
	else if (opcode == DIVS)	output.append("DIVS");
	else if (opcode == EOR)		output.append("EOR");
	else if (opcode == EORI)	output.append("EORI");
	else if (opcode == EXG)		output.append("EXG");
	else if (opcode == JMP)		output.append("JMP");
	else if (opcode == LSL)		output.append("LSL");
	else if (opcode == LSR)		output.append("LSR");
	else if (opcode == MOVE)	output.append("MOVE");
	else if (opcode == MULS)	output.append("MULS");
	else if (opcode == NEG)		output.append("NEG");
	else if (opcode == NOP)		output.append("NOP");
	else if (opcode == NOT)		output.append("NOT");
	else if (opcode == OR)		output.append("OR");
	else if (opcode == ORI)		output.append("ORI");
	else if (opcode == SUB)		output.append("SUB");
	else if (opcode == SUBI)	output.append("SUBI");
	else if (opcode == BEQ)		output.append("BEQ");
	else if (opcode == BGE)		output.append("BGE");
	else if (opcode == BGT)		output.append("BGT");
	else if (opcode == BLE)		output.append("BLE");
	else if (opcode == BLT)		output.append("BLT");
	else if (opcode == BMI)		output.append("BMI");
	else if (opcode == BPL)		output.append("BPL");
	else if (opcode == HALT)	output.append("HALT");

	output.append(" ");

	stringstream sstr(output);

	if (endtype == 0)			sstr << "D" << op1 << " D" << op2;
	else if (endtype == 1)		sstr << "D" << op1 << " A" << op2;
	else if (endtype == 2)		sstr << "D" << op1 << " (A" << op2 << ")\n\n\tA(" << op2 << ") = " << getData(A[op2]);
	else if (endtype == 3)		{setData(program[PC], D[op1] - getData(program[PC])); PC++;}
	else if (endtype == 4)		D[op2] -= A[op1];
	else if (endtype == 5)		A[op2] -= A[op1];
	else if (endtype == 6)		setData(A[op2], A[op1] - getData(A[op2]));
	else if (endtype == 7)		{setData(program[PC], A[op1] - getData(program[PC])); PC++;}
	else if (endtype == 8)		D[op2] -= getData(A[op1]);
	else if (endtype == 9)		A[op2] -= getData(A[op1]);
	else if (endtype == 10)		setData(A[op2], getData(A[op1]) - getData(A[op2]));
	else if (endtype == 11)		{setData(program[PC], getData(A[op1]) - getData(program[PC])); PC++;}
	else if (endtype == 12)		{D[op2] -= getData(program[PC]); PC++;}
	else if (endtype == 13)		{A[op2] -= getData(program[PC]); PC++;}
	else if (endtype == 14)		{setData(A[op2], getData(program[PC]) - getData(A[op2])); PC++;}
	else if (endtype == 15)		{setData(program[PC + 1], getData(program[PC]) - getData(program[PC + 1])); PC += 2;}

	output = sstr.str();
	*/
}

void M68KVirtualMachine::reset()
{
	output = "";

	dataMemory.clear();

	for (int i = 0; i < 8; i++)
	{
		A[i] = 0;
		D[i] = 0;
	}

	PC = 0;

	for (int i = 0; i < 5; i++)
		cc[i] = 0;

	halt = false;
}

void M68KVirtualMachine::setProgram(vector<USHORT> program)
{
	reset();
	this->program = program;
}

vector<pair<USHORT, short> > M68KVirtualMachine::getMemory()
{
	map<USHORT, short>::iterator it;
	pair<USHORT, short> data;
	vector<pair<USHORT, short> > _return;

	for (it = dataMemory.begin(); it != dataMemory.end(); it++)
	{
		data.first = it->first;
		data.second = it->second;

		_return.push_back(data);
	}

	return _return;
}

vector<USHORT> M68KVirtualMachine::getAs()
{
	vector<USHORT> _return;

	for (int i = 0; i < 8; i++)
		_return.push_back(A[i]);

	return _return;
}

vector<short> M68KVirtualMachine::getDs()
{
	vector<short> _return;

	for (int i = 0; i < 8; i++)
		_return.push_back(D[i]);

	return _return;
}

USHORT M68KVirtualMachine::getPC() const
{
	return PC;
}
