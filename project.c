/***************************************************
AUTHORS:
Anthony Arronte Lugo
Benjamin Richards
Brooklyn Butner
Kennedy Torrent
***************************************************/
#include "spimcore.h"

// ALU
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

// instruction fetch
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

// instruction partition
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
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
	*r1 = (instruction & r1Temp) >> 21; // right shift instruction 21 bits according to register 1
	*r2 = (instruction & r2Temp) >> 16; // right shift instruction 16 bits according to register 2
	*r3 = (instruction & r3Temp) >> 11; // right shift instruction 11 bits according to register 3
	
	// reassigning values
	*funct = instruction & functTemp;
	*offset = instruction & offsetTemp;
	*jsec = instruction & jsecTemp;

} // end instruction_partition function

// instruction decode
// switch case that turns on / off the control pointers according to the value given from op
int instruction_decode(unsigned op,struct_controls *controls)
{
	switch (op)
  {
	// turning off all of the control pointers except for "RegDst" and "RegWrite" and assigning "ALUOP" to be 7
	case 0x00000000 : // "0000 0000"
		controls->RegDst = 1;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 7;
		controls->MemWrite = 0;
		controls->ALUSrc = 0;
		controls->RegWrite = 1;
		break;
	// turning off all of the control pointers except for "ALUSrc" and "RegWrite" and assigning "ALUOP" to be 0
	case 0x20000000: // "2000 0000"
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 0;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
		break;
	// turning off all of the control pointers except for "MemRead", "MemtoReg", "ALUSrc", and "RegWrite" and assigning "ALUOP" to be 0
	case 0x8c000000: // "8c00 0000"
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 1;
		controls->MemtoReg = 1;
		controls->ALUOp = 0;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
		break;
	// turning off all of the control pointers except for "Memwrite" and "ALUSrc" and assigning "ALUOP" to be 0
	case 0xac000000: // "ac00 0000"
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 0;
		controls->MemWrite = 1;
		controls->ALUSrc = 1;
		controls->RegWrite = 0;
		break;
	// turning off all of the control pointers except for "ALUSrc" and "RegWrite" and assigning "ALUOP" to be 6
	case 0x3c000000: // "3c00 0000"
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 6;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
		break;
	// turning off all of the control pointers except for "ALUSrc" and "RegWrite" and assigning "ALUOP" to be 2
	case 0x28000000: // "2800 0000"
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 2;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
		break;
	// turning off all of the control pointers except for "ALUSrc" and "RegWrite" and assigning "ALUOP" to be 3
	case 0x2c000000: // "2c00 0000"
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 3;
		controls->MemWrite = 0;
		controls->ALUSrc = 1;
		controls->RegWrite = 1;
		break;
	// turning off all of the control pointers except for "Branch" and assigning "ALUOP" to be 1
	case 0x10000000: // "1000 0000"
		controls->RegDst = 0;
		controls->Jump = 0;
		controls->Branch = 1;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 1;
		controls->MemWrite = 0;
		controls->ALUSrc = 0;
		controls->RegWrite = 0;
		break;
	// turning off all of the control pointers except for "Jump" and assigning "ALUOP" to be 0
	case 0x08000000: // "0800 0000"
		controls->RegDst = 0;
		controls->Jump = 1;
		controls->Branch = 0;
		controls->MemRead = 0;
		controls->MemtoReg = 0;
		controls->ALUOp = 0;
		controls->MemWrite = 0;
		controls->ALUSrc = 0;
		controls->RegWrite = 0;
		break;
	default:
		return 1;
	}

	return 0;
} // end instruction_decode function

// Read Register
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	*data1 = Reg[r1]; // stores information from register 1 within "data1" variable
	*data2 = Reg[r2]; // stores information from register 2 within "data2" variable
} // end read_register function


// Sign Extend
void sign_extend(unsigned offset,unsigned *extended_value)
{
	// creates temporary variables so that we dont change the actual values
	unsigned int temp1 = 0x00008000;
	unsigned int temp2 = 0xffff0000;
	
	// calculates the result of the bitwise AND between the offset and temp1
	unsigned int result = offset & temp1;
	
	// if the result is greater than 1 then change the value within "extended_value" to be the bitwise OR of offset and temp2
	if (result > 1)
  	{
		*extended_value = offset | temp2; // changes the value of "extended_value" to be the bitwise OR between the offset and temp2
	}

	// if the result is 0 then change the value within "extended_value" to be equal to the offset
	if (result == 0)
  	{
		*extended_value = offset; // changes the value of "extended_value" to be equal to the offset
	}
} // end sign_extend function

// ALU Operations
// takes in the ALU value 1 - 7 from the "instruction_decode" function and checks if the ALUSrc is 1
// if the ALUSrc is 1 then it sends the "ALU" function "extended_value" otherwise the "ALU" function is sent "data2"
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
	// checks the ALUOp value that was given from the "instruction_decode" function
	if(ALUOp == 0)
  	{
    		if (ALUSrc == 1) // if the ALUSrc is 1 then send the "ALU" function "extended_value"
			ALU(data1, extended_value, ALUOp, ALUresult, Zero);
		else // if the ALUSrc1 is 0 then send the "ALU" function "data2"
			ALU(data1, data2, ALUOp, ALUresult, Zero);
  	}

	// checks the ALUOp value that was given from the "instruction_decode" function
	else if(ALUOp == 1)
  	{
    		if (ALUSrc == 1) // if the ALUSrc is 1 then send the "ALU" function "extended_value"
			ALU(data1, extended_value, ALUOp, ALUresult, Zero);
		else // if the ALUSrc1 is 0 then send the "ALU" function "data2"
			ALU(data1, data2, ALUOp, ALUresult, Zero);
  	}

	// checks the ALUOp value that was given from the "instruction_decode" function
	else if(ALUOp == 2)
  	{
    		if (ALUSrc == 1) // if the ALUSrc is 1 then send the "ALU" function "extended_value"
    			ALU(data1, extended_value, ALUOp, ALUresult, Zero);
  		else // if the ALUSrc1 is 0 then send the "ALU" function "data2"
   			ALU(data1, data2, ALUOp, ALUresult, Zero);
  	}

	// checks the ALUOp value that was given from the "instruction_decode" function
	else if(ALUOp == 3)
 	{
    		if (ALUSrc == 1) // if the ALUSrc is 1 then send the "ALU" function "extended_value"
			ALU(data1, extended_value, ALUOp, ALUresult, Zero);
		else // if the ALUSrc1 is 0 then send the "ALU" function "data2"
			ALU(data1, data2, ALUOp, ALUresult, Zero);
  	}

	// checks the ALUOp value that was given from the "instruction_decode" function
	else if(ALUOp == 4)
  	{
    		if (ALUSrc == 1) // if the ALUSrc is 1 then send the "ALU" function "extended_value"
			ALU(data1, extended_value, ALUOp, ALUresult, Zero);
		else // if the ALUSrc1 is 0 then send the "ALU" function "data2"
			ALU(data1, data2, ALUOp, ALUresult, Zero);
  	}

	// checks the ALUOp value that was given from the "instruction_decode" function
 	else if(ALUOp == 5)
  	{
    		if (ALUSrc == 1) // if the ALUSrc is 1 then send the "ALU" function "extended_value"
      			ALU(data1, extended_value, ALUOp, ALUresult, Zero);
    		else // if the ALUSrc1 is 0 then send the "ALU" function "data2"
      			ALU(data1, data2, ALUOp, ALUresult, Zero);
  	}
	
	// checks the ALUOp value that was given from the "instruction_decode" function
  	else if(ALUOp == 6)
  	{
   		if (ALUSrc == 1) // if the ALUSrc is 1 then send the "ALU" function "extended_value"
      			ALU(data1, extended_value, ALUOp, ALUresult, Zero);
    		else // if the ALUSrc1 is 0 then send the "ALU" function "data2"
      			ALU(data1, data2, ALUOp, ALUresult, Zero);
  	}

	/// checks the ALUOp value that was given from the "instruction_decode" function
	// then checks for the value of the function and sends a new ALUOp to the "ALU" function
	else if (ALUOp == 7)
  	{
		// checks for the value of "funct" variable then sends the "ALU" function 0 - 5 accordingly
    		if (funct == 32)
    		{
      			ALU(data1, data2, 0, ALUresult, Zero); // sends the "ALU" function 0 for ALUOp
    		}

    		else if(funct == 34)
    		{
      			ALU(data1, data2, 1, ALUresult, Zero); // sends the "ALU" function 1 for ALUOp
    		}

    		else if(funct == 36)
    		{
      			ALU(data1, data2, 4, ALUresult, Zero); // sends the "ALU" function 4 for ALUOp
    		}

    		else if(funct == 37)
    		{
      			ALU(data1, data2, 5, ALUresult, Zero); // sends the "ALU" function 5 for ALUOp
    		}

    		else if(funct == 42)
    		{
      			ALU(data1, data2, 2, ALUresult, Zero); // sends the "ALU" function 2 for ALUOp
    		}

    		else if(funct == 43)
    		{
      			ALU(data1, data2, 3, ALUresult, Zero); // sends the "ALU" function 3 for ALUOp
    		}


  	} // end else if (ALUOp == 7)

	else
  	{
    		return 1;
  	}

	return 0;
} // end ALU_operations function

// rw_memory
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
	// if MemRead is on or if MemWrite is on then check the ALUresult
	if (MemRead == 1 || MemWrite == 1)
  	{
		if ((ALUresult % 4) != 0) // checks if the ALUresult bits are all 0
			return 1;

		if (ALUresult < 0 || ALUresult > 0xFFFF) // checks if the ALUresult is between (0 - 2^16)
			return 1;
	}
	
	// if MemRead is on and MemWrite is off then alter the memdata according to the Mem array
	if (MemRead == 1 && MemWrite == 0)
  	{
		*memdata = Mem[ALUresult >> 2]; // change value of memdata to left shift ALUresult index in the Mem array
	}
	
	// if MemRead is off and MemWrite is on then change the value within the Mem array to the value of data2
	if (MemRead == 0 && MemWrite == 1)
	{
		Mem[ALUresult >> 2] = data2; // the left shift ALUresult index of the Mem array changes to data2
	}

	return 0;
} // end rw_memory function

// write_register
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
	// if RegDst is on, MemtoReg is off, and RegWrite is on 
	// check the value of register 3 and reassign values in the Reg array
	if (RegDst == 1 && MemtoReg == 0 && RegWrite == 1)
  	{
		if (r3 == 0) // if register 3 is off
			return;

		Reg[r3] = ALUresult; // reassign the register 3 index within the Reg array
		return;
	}

	// if RegDst is off, MemtoReg is on, and RegWrite is on
	// check the value of register 2 and reassgin values in the Reg array
	if (RegDst == 0 && MemtoReg == 1 && RegWrite == 1)
  	{
		if (r2 == 0) // if register 2 is off
			return;

		Reg[r2] = memdata; // reassign the register 2 index within the Reg array
		return;
	}

	// if RegDst is off, MemtoReg is off, and RegWrite is on
	// check the value of register 2 and reassgin values in the Reg array
	if (RegDst == 0 && MemtoReg == 0 && RegWrite == 1)
  	{
		if (r2 == 0) // if register 2 is off
			return;

		Reg[r2] = ALUresult; // reassign the register 2 index within the Reg array
		return;
	}

} // end write_register function

// PC_update
// checks if certain variables are on or off then updates the PC array accordingly
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
	// if Branch is off and Jump is off then we update the PC array
	if (Branch == 0 && Jump == 0) {
		*PC = *PC + 4;
	}
	
	// if Branch is on, Jump is off, and Zero is on then we left shift the extended_value and add the answer to the updated PC array
	if (Branch == 1 && Jump == 0 && Zero == 1) {
		*PC = (extended_value << 2) + (*PC + 4);
	}

	// if the Branch is off and the Jump is on we left shift the jsec and update the PC array
	if (Branch == 0 && Jump == 1) {
		unsigned int jsecLeftShift = jsec << 2;
		unsigned int PC4Bits = (*PC + 4) & 0xF0000000;
		*PC = PC4Bits | jsecLeftShift;
	}

} // end PC_update function
