#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
  unsigned int result = 0;

  // addition
  if (ALUControl == 0)
  {
      result = A + B;
  }
  // subtraction
  else if (ALUControl == 1)
  {
    result = A - B;
  }

  else if (ALUControl == 2)
  {
    if ((int)A < (int)B)
    {
      result = 1;
    }
    else
    {
      result = 0;
    }
  }
  else if (ALUControl == 3)
  {
    if (A < B)
    {
      result = 1;
    }
    else
    {
      result = 0;
    }
  }
  else if (ALUControl == 4)
  {
    result = A & B;
  }
  else if (ALUControl == 5)
  {
    result = A | B;
  }
  else if (ALUControl == 6)
  {
    result = B << 16;
  }
  else if (ALUControl == 7)
  {
    result = ~A;
  }

  *ALUresult = result;

  if (result == 0)
  {
    *Zero = 1;
  }
  else
  {
    *Zero = 0;
  }

}



/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
	//test for word alignment
	if ((PC % 4) != 0) {
		return 1;//halt the program PC is not word alligned
	}
	

	//check ranges of PC
	if (PC < 0x0000 || PC > 0xFFFF) {
		return 1; //halt the program, PC not in range
	}

	//get instruction from memory
	*instruction = Mem[PC >> 2];
	return 0; //don't halt
}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
	//We don't know the specific instruction yet, so we partition every possible part
	//need to bitmask with instruction to extract bits out 
	
	
	//first lets create the mask : fieldM is the mask of that field
	unsigned opM = 0xfc000000; //bit [31-26]
	unsigned r1M = 0x03e00000; //bit [25-21]
	unsigned r2M = 0x001f0000; //bit [20-16]
	unsigned r3M = 0x0000f800; //bit [15-11]
	unsigned functM = 0x0000003f; //bit [5-0]
	unsigned offsetM = 0x0000ffff; //bit [15-0]
	unsigned jsecM = 0x03ffffff; //bit [25-0]


	//we bitmask here to extract bits
	*op = instruction & opM; //opcode
	*r1 = (instruction & r1M) >> 21; //register 1
	*r2 = (instruction & r2M) >> 16; //register 2
	*r3 = (instruction & r3M) >> 11; //register 3
	*funct = instruction & functM; //function field
	*offset = instruction & offsetM; //offset field
	*jsec = instruction & jsecM; // jump address field

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
	//use op to assign control signals

	switch (op) {
	case 0x000000000 : //r-type instruction (add, sub, or, and, slt, sltu)
		controls->RegDst = 0x1; //want to write to register in bit 15-11
		controls->Jump = 0x0; //not jumping
		controls->Branch = 0x0; //not branching
		controls->MemRead = 0x0; //not reading memory
		controls->MemtoReg = 0x0; //want alu output to go to write data register
		controls->ALUOp = 0x7; //don't know yet, look at our funct bit 5-0
		controls->MemWrite = 0x0; //not writing to memory
		controls->ALUSrc = 0x0; //read data 2 to alu
		controls->RegWrite = 0x1; //write data to register
		break;
	case 0x20000000: //i-type (addi)
		controls->RegDst = 0x0; //want to write to register in bit 20-16
		controls->Jump = 0x0; //not jumping
		controls->Branch = 0x0; //not branching
		controls->MemRead = 0x0; //not reading memory
		controls->MemtoReg = 0x0; //want alu output to go to write data register
		controls->ALUOp = 0x0; //add for alu
		controls->MemWrite = 0x0; //not writing to memory
		controls->ALUSrc = 0x1; //read data 2 to alu
		controls->RegWrite = 0x1; //write data to register
		break;
	case 0x8c000000: //i-type (load word)
		controls->RegDst = 0x0; //want to write to register in bit 20-16
		controls->Jump = 0x0; //not jumping
		controls->Branch = 0x0; //not branching
		controls->MemRead = 0x1; //reading memory
		controls->MemtoReg = 0x1; //want memory output to go to write data register
		controls->ALUOp = 0x0; //add register address with offset
		controls->MemWrite = 0x0; //not writing to memory
		controls->ALUSrc = 0x1; //send off set to alu
		controls->RegWrite = 0x1; //write data to register
		break;
	case 0xac000000: //i-type (store word)
		controls->RegDst = 0x0; //want to write to register in bit 20-16
		controls->Jump = 0x0; //not jumping
		controls->Branch = 0x0; //not branching
		controls->MemRead = 0x0; //not reading memory
		controls->MemtoReg = 0x0; //don't want memory output to go to write data register
		controls->ALUOp = 0x0; //add register address with offset
		controls->MemWrite = 0x1; //writing to memory
		controls->ALUSrc = 0x1; //send off set to alu
		controls->RegWrite = 0x0; //don't write data to register
		break;
	case 0x3c000000: //i-type (load upper immediate
		controls->RegDst = 0x0; //want to write to register in bit 20-16
		controls->Jump = 0x0; //not jumping
		controls->Branch = 0x0; //not branching
		controls->MemRead = 0x0; //reading memory
		controls->MemtoReg = 0x0; //want alu output to go to write data register
		controls->ALUOp = 0x6; //shift offset 16 places to the left
		controls->MemWrite = 0x0; //not writing to memory
		controls->ALUSrc = 0x1; //send off set to alu
		controls->RegWrite = 0x1; //write data to register
		break;
	case 0x28000000: //(i-type) set less than immediate
		controls->RegDst = 0x0; //want to write to register in bit 20-16
		controls->Jump = 0x0; //not jumping
		controls->Branch = 0x0; //not branching
		controls->MemRead = 0x0; //not reading memory
		controls->MemtoReg = 0x0; //want alu output to go to write data register
		controls->ALUOp = 0x2; //slt
		controls->MemWrite = 0x0; //not writing to memory
		controls->ALUSrc = 0x1; //send offset to alu
		controls->RegWrite = 0x1; //write data to register
		break;
	case 0x2c000000: //i-type set less than immediate unsigned
		controls->RegDst = 0x0; //want to write to register in bit 20-16
		controls->Jump = 0x0; //not jumping
		controls->Branch = 0x0; //not branching
		controls->MemRead = 0x0; //not reading memory
		controls->MemtoReg = 0x0; //want alu output to go to write data register
		controls->ALUOp = 0x3; //sltu
		controls->MemWrite = 0x0; //not writing to memory
		controls->ALUSrc = 0x1; //send offset to alu
		controls->RegWrite = 0x1; //write data to register
		break;
	case 0x10000000://branch on equal
		controls->RegDst = 0x0; //not writing to register don't care
		controls->Jump = 0x0; //not jumping
		controls->Branch = 0x1; // branching
		controls->MemRead = 0x0; //not reading memory
		controls->MemtoReg = 0x0; //want alu output to go to write data register (don't care)
		controls->ALUOp = 0x1; //subtract
		controls->MemWrite = 0x0; //not writing to memory
		controls->ALUSrc = 0x0; //send data 2 to alu
		controls->RegWrite = 0x0; //don't write data to register
		break;
	case 0x08000000: //jump
		controls->RegDst = 0x0; //dc
		controls->Jump = 0x1; // jumping
		controls->Branch = 0x0; //not branching
		controls->MemRead = 0x0; //not reading memory
		controls->MemtoReg = 0x0; //dc
		controls->ALUOp = 0x0; //add
		controls->MemWrite = 0x0; //not writing to memory
		controls->ALUSrc = 0x0; //dc
		controls->RegWrite = 0x0; //dont't write data to register
		break;
	default: //no other case must halt
		return 1; //halt 
	}

	return 0; //don't halt
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
	//check to see if offset is a negative number 
	unsigned mask1 = 0x00008000; // mask to check sign
	unsigned mask2 = 0xffff0000; //mask to add 1 to first 16 bits
	unsigned result = offset & mask1; // bit mask to see the 16th bit
	if (result > 1) { //have a negative number 
		*extended_value = offset | mask2; //put 1 in the first 16 bits
	}
	if (result == 0) { //have a positive number
		*extended_value = offset; //put 0 in the first 16 bits
	}
}


int rType_Decode(unsigned data1, unsigned data2, unsigned funct, unsigned *ALUresult, char *Zero) {
	//decode a r-type using funct

	switch (funct) {
	case 0x20: //add
		ALU(data1, data2, 0, ALUresult, Zero);
		break;
	case 0x22: //subtrat
		ALU(data1, data2, 1, ALUresult, Zero);
		break;
	case 0x25: //or
		ALU(data1, data2, 5, ALUresult, Zero);
		break;
	case 0x24: //and
		ALU(data1, data2, 4, ALUresult, Zero);
		break;
	case 0x2A: //slt
		ALU(data1, data2, 2, ALUresult, Zero);
		break;
	case 0x2B: //sltu
		ALU(data1, data2, 3, ALUresult, Zero);
		break;
	default:
		return 1; //halt function is not recognized
	}

	return 0;
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
	//look up alu op
	switch (ALUOp) {

	case 0x0: //alu will do addition
		if (ALUSrc == 1)//send extended value
			ALU(data1, extended_value, ALUOp, ALUresult, Zero); 
		else //send data 2
			ALU(data1, data2, ALUOp, ALUresult, Zero);
		
		break;
	case 0x1: //alu will do subtraction
		if (ALUSrc == 1)//send extended value
			ALU(data1, extended_value, ALUOp, ALUresult, Zero);
		else //send data 2
			ALU(data1, data2, ALUOp, ALUresult, Zero);
		
		break;
	case 0x2: //alu will do slt
		if (ALUSrc == 1)//send extended value
			ALU(data1, extended_value, ALUOp, ALUresult, Zero);
		else //send data 2
			ALU(data1, data2, ALUOp, ALUresult, Zero);
		
		break;
	case 0x3: //alu will do sltu
		if (ALUSrc == 1)//send extended value
			ALU(data1, extended_value, ALUOp, ALUresult, Zero);
		else //send data 2
			ALU(data1, data2, ALUOp, ALUresult, Zero);
		
		break;
	case 0x4: //alu wll do AND
		if (ALUSrc == 1)//send extended value
			ALU(data1, extended_value, ALUOp, ALUresult, Zero);
		else //send data 2
			ALU(data1, data2, ALUOp, ALUresult, Zero);
		
		break;
	case 0x5: //alu will do OR
		if (ALUSrc == 1)//send extended value
			ALU(data1, extended_value, ALUOp, ALUresult, Zero);
		else //send data 2
			ALU(data1, data2, ALUOp, ALUresult, Zero);
		
		break;
	case 0x6: //alu will SLL extendedvalue
		if (ALUSrc == 1)//send extended value
			ALU(data1, extended_value, ALUOp, ALUresult, Zero);
		else //send data 2
			ALU(data1, data2, ALUOp, ALUresult, Zero);

		break;
	case 0x7: //alu will do an r-type instruction
		return rType_Decode(data1, data2, funct, ALUresult, Zero);

		break;
	default:
		return 1; //halt the program aluop not recognized
	}

	return 0; //don't halt

}



/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

	//check to see if aluresult is a valid address (only when it is a address)
	if (MemRead == 0x1 || MemWrite == 0x1) {//we are reading or storing
		//address is not word alligned
		if ((ALUresult % 4) != 0) 
			return 1; //halt

		//check ranges of PC
		if (ALUresult < 0x0000 || ALUresult > 0xFFFF)
			return 1; //halt the program, address not in range
	}



	//load word into memory
	if (MemRead == 0x1 && MemWrite == 0x0) {
		*memdata = Mem[ALUresult >> 2];
	}


	//store word into memory
	if (MemRead == 0x0 && MemWrite == 0x1) {
		Mem[ALUresult >> 2] = data2;
	}


	return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
	//r-type write to register 3
	if (RegDst == 0x1 && MemtoReg == 0x0 && RegWrite == 0x1) {
		if (r3 == 0) //can't write to reg0
			return;
		
		Reg[r3] = ALUresult; //r-types get aluresult in reg3
		return;
	}

	//load word, write to register 2
	if (RegDst == 0x0 && MemtoReg == 0x1 && RegWrite == 0x1) {
		if (r2 == 0) //can't write to reg0
			return;

		Reg[r2] = memdata; //lw gets memdata in reg2
		return;
	}

	//addi, lui, slti, sltui, write to register 2
	if (RegDst == 0x0 && MemtoReg == 0x0 && RegWrite == 0x1) {
		if (r2 == 0) //can't write to reg0
			return;

		Reg[r2] = ALUresult; //lw gets ALU result in reg2
		return;
	}
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
	//next instruction
	if (Branch == 0x0 && Jump == 0x0) {
		*PC = *PC + 4; //go to next instruction
	}

	//branch on equal
	if (Branch == 0x1 && Jump == 0x0 && Zero == 0x1) {
		*PC = (extended_value << 2) + (*PC + 4); //multiple extended_value by 4 nd add to pc + 4
	}

	//jump
	if (Branch == 0x0 && Jump == 0x1) {
		unsigned jsecShift = jsec << 2; //shift jsec to the left 2 bits
		unsigned PC4Bits = (*PC + 4) & 0xF0000000; //bitmask to get PC + 4 first 4 bits
		*PC = PC4Bits | jsecShift; //combine the 4 bits if pc with the 28 bit jsec
	}

}

