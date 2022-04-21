/*
AUTHORS:
Anthony Arronte Lugo
Brooklyn Butner
Benjamin Richards
Kennedy Torrent
*/

#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
  unsigned int Z = 0;

   //addition
  if (ALUControl == 0)
  {
      Z = A + B;
  }
  // subtraction
  else if (ALUControl == 1)
  {
    Z = A - B;
  }

  // signed int comparison
  else if (ALUControl == 2)
  {
    if ((int)A < (int)B)
    {
      Z = 1;
    }
    else
    {
      Z = 0;
    }
  }

  // unsigned else if comparison
  else if (ALUControl == 3)
  {
    if (A < B)
    {
      Z = 1;
    }
    else
    {
      Z = 0;
    }
  }

  // bitwise "AND" operator
  else if (ALUControl == 4)
  {
    Z = A & B;
  }

  // bitwise "OR" operator
  else if (ALUControl == 5)
  {
    Z = A | B;
  }

  // bitwise "LEFT SHIFT" operator
  else if (ALUControl == 6)
  {
    Z = B << 16;
  }

  // bitwise "NOT" operator
  else if (ALUControl == 7)
  {
    Z = ~A;
  }

  *ALUresult = Z;

  if (Z == 0)
  {
    *Zero = 1;
  }
  else
  {
    *Zero = 0;
  }

} // end ALU function

/* instruction fetch */
/* 10 Points */
// gets instruction from Mem and places in "instruction"
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
  if (PC % 4 == 0) // if there is nothing within the 4 bits
  {
    *instruction = Mem[PC >> 2]; // right shift PC by 2 to get the correct index for instruction within Mem
    return 0;
  }
  else
    return 1;

} // end instruction fetch function

/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned int instruction, unsigned int* op, unsigned int* r1,unsigned int* r2, unsigned int* r3, unsigned int* funct, unsigned int* offset, unsigned int* jsec)
{
	// creating temporary bitmasks for each variable so we dont change the original values
	unsigned int opTemp = 0xfc000000; // difference (2^32 -1) - (2^26 - 1)
	unsigned int r1Temp = 0x03e00000; // difference (2^26 - 1) - (2^21 - 1)
	unsigned int r2Temp = 0x001f0000; // difference (2^21 -1) - (2^16 - 1)
	unsigned int r3Temp = 0x0000f800; // difference (2^16 -1) - (2^11 - 1)
	unsigned int functTemp = 0x0000003f; // difference (2^6 -1) - (2^0 - 1)
	unsigned int offsetTemp = 0x0000ffff; // difference (2^16 -1) - (2^0 - 1)
	unsigned int jsecTemp = 0x03ffffff; // difference (2^26 -1) - (2^0 - 1)


	*op = instruction & opTemp; //opcode
	*r1 = (instruction & r1Temp) >> 21;
	*r2 = (instruction & r2Temp) >> 16;
	*r3 = (instruction & r3Temp) >> 11;
	*funct = instruction & functTemp;
	*offset = instruction & offsetTemp;
	*jsec = instruction & jsecTemp;

}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned int op,struct_controls *controls)
{

	switch (op) {
	case 0x000000000 :
		controls->RegDst = 0x1;
		controls->Jump = 0x0;
		controls->Branch = 0x0;
		controls->MemRead = 0x0;
		controls->MemtoReg = 0x0;
		controls->ALUOp = 0x7;
		controls->MemWrite = 0x0;
		controls->ALUSrc = 0x0;
		controls->RegWrite = 0x1;
		break;
	case 0x20000000:
		controls->RegDst = 0x0;
		controls->Jump = 0x0;
		controls->Branch = 0x0;
		controls->MemRead = 0x0;
		controls->MemtoReg = 0x0;
		controls->ALUOp = 0x0;
		controls->MemWrite = 0x0;
		controls->ALUSrc = 0x1;
		controls->RegWrite = 0x1;
		break;
	case 0x8c000000:
		controls->RegDst = 0x0;
		controls->Jump = 0x0;
		controls->Branch = 0x0;
		controls->MemRead = 0x1;
		controls->MemtoReg = 0x1;
		controls->ALUOp = 0x0;
		controls->MemWrite = 0x0;
		controls->ALUSrc = 0x1;
		controls->RegWrite = 0x1;
		break;
	case 0xac000000:
		controls->RegDst = 0x0;
		controls->Jump = 0x0;
		controls->Branch = 0x0;
		controls->MemRead = 0x0;
		controls->MemtoReg = 0x0;
		controls->ALUOp = 0x0;
		controls->MemWrite = 0x1;
		controls->ALUSrc = 0x1;
		controls->RegWrite = 0x0;
		break;
	case 0x3c000000:
		controls->RegDst = 0x0;
		controls->Jump = 0x0;
		controls->Branch = 0x0;
		controls->MemRead = 0x0;
		controls->MemtoReg = 0x0;
		controls->ALUOp = 0x6;
		controls->MemWrite = 0x0;
		controls->ALUSrc = 0x1;
		controls->RegWrite = 0x1;
		break;
	case 0x28000000:
		controls->RegDst = 0x0;
		controls->Jump = 0x0;
		controls->Branch = 0x0;
		controls->MemRead = 0x0;
		controls->MemtoReg = 0x0;
		controls->ALUOp = 0x2;
		controls->MemWrite = 0x0;
		controls->ALUSrc = 0x1;
		controls->RegWrite = 0x1;
		break;
	case 0x2c000000:
		controls->RegDst = 0x0;
		controls->Jump = 0x0;
		controls->Branch = 0x0;
		controls->MemRead = 0x0;
		controls->MemtoReg = 0x0;
		controls->ALUOp = 0x3;
		controls->MemWrite = 0x0;
		controls->ALUSrc = 0x1;
		controls->RegWrite = 0x1;
		break;
	case 0x10000000:
		controls->RegDst = 0x0;
		controls->Jump = 0x0;
		controls->Branch = 0x1;
		controls->MemRead = 0x0;
		controls->MemtoReg = 0x0;
		controls->ALUOp = 0x1;
		controls->MemWrite = 0x0;
		controls->ALUSrc = 0x0;
		controls->RegWrite = 0x0;
		break;
	case 0x08000000:
		controls->RegDst = 0x0;
		controls->Jump = 0x1;
		controls->Branch = 0x0;
		controls->MemRead = 0x0;
		controls->MemtoReg = 0x0;
		controls->ALUOp = 0x0;
		controls->MemWrite = 0x0;
		controls->ALUSrc = 0x0;
		controls->RegWrite = 0x0;
		break;
	default:
		return 1;
	}

	return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	*data1 = Reg[r1];
	*data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{

	unsigned int temp1 = 0x00008000;
	unsigned int temp2 = 0xffff0000;
	unsigned int result = offset & temp1;
	if (result > 1)
  {
		*extended_value = offset | temp2;
	}

	if (result == 0)
  {
		*extended_value = offset;
	}
}



int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

	if(ALUOp == 0)
  {
    if (ALUSrc == 1)
			ALU(data1, extended_value, ALUOp, ALUresult, Zero);
		else
			ALU(data1, data2, ALUOp, ALUresult, Zero);
  }

	else if(ALUOp == 1)
  {
    if (ALUSrc == 1)
			ALU(data1, extended_value, ALUOp, ALUresult, Zero);
		else
			ALU(data1, data2, ALUOp, ALUresult, Zero);
  }

	else if(ALUOp == 2)
  {
    if (ALUSrc == 1)
    ALU(data1, extended_value, ALUOp, ALUresult, Zero);
  else
    ALU(data1, data2, ALUOp, ALUresult, Zero);
  }

	else if(ALUOp == 3)
  {
    if (ALUSrc == 1)
			ALU(data1, extended_value, ALUOp, ALUresult, Zero);
		else
			ALU(data1, data2, ALUOp, ALUresult, Zero);

  }

	else if(ALUOp == 4)
  {
    if (ALUSrc == 1)
			ALU(data1, extended_value, ALUOp, ALUresult, Zero);
		else
			ALU(data1, data2, ALUOp, ALUresult, Zero);
  }


  else if(ALUOp == 5)
  {
    if (ALUSrc == 1)
      ALU(data1, extended_value, ALUOp, ALUresult, Zero);
    else
      ALU(data1, data2, ALUOp, ALUresult, Zero);

  }

  else if(ALUOp == 6)
  {
    if (ALUSrc == 1)
      ALU(data1, extended_value, ALUOp, ALUresult, Zero);
    else
      ALU(data1, data2, ALUOp, ALUresult, Zero);
  }


	else if (ALUOp == 7)
  {
    if (funct == 32)
    {
      ALU(data1, data2, 0, ALUresult, Zero);
    }

    else if(funct == 34)
    {
      ALU(data1, data2, 1, ALUresult, Zero);
    }

    else if(funct == 36)
    {
      ALU(data1, data2, 4, ALUresult, Zero);
    }

    else if(funct == 37)
    {
      ALU(data1, data2, 5, ALUresult, Zero);
    }

    else if(funct == 42)
    {
      ALU(data1, data2, 2, ALUresult, Zero);
    }

    else if(funct == 43)
    {
      ALU(data1, data2, 3, ALUresult, Zero);
    }


  }

	else
  {
    return 1;
  }

	return 0;
}


int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

	if (MemRead == 1 || MemWrite == 1)
  {
		if ((ALUresult % 4) != 0)
			return 1;

		if (ALUresult < 0 || ALUresult > 0xFFFF)
			return 1;
	}

	if (MemRead == 1 && MemWrite == 0)
  {
		*memdata = Mem[ALUresult >> 2];
	}

	if (MemRead == 0 && MemWrite == 1)
  {
		Mem[ALUresult >> 2] = data2;
	}


	return 0;
}



void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

	if (RegDst == 1 && MemtoReg == 0 && RegWrite == 1)
  {
		if (r3 == 0)
			return;

		Reg[r3] = ALUresult;
		return;
	}


	if (RegDst == 0 && MemtoReg == 1 && RegWrite == 1)
  {
		if (r2 == 0)
			return;

		Reg[r2] = memdata;
		return;
	}


	if (RegDst == 0 && MemtoReg == 0 && RegWrite == 1)
  {
		if (r2 == 0)
			return;

		Reg[r2] = ALUresult;
		return;
	}

}

void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

	if (Branch == 0 && Jump == 0) {
		*PC = *PC + 4;
	}


	if (Branch == 1 && Jump == 0 && Zero == 1) {
		*PC = (extended_value << 2) + (*PC + 4);
	}


	if (Branch == 0 && Jump == 1) {
		unsigned int jsecLeftShift = jsec << 2;
		unsigned int PC4Bits = (*PC + 4) & 0xF0000000;
		*PC = PC4Bits | jsecLeftShift;
	}

}

