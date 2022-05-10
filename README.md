# Simple-Processor-In-C
This project was worked on by 4 people in a group for Computer Logic and Organization at UCF

In this project, you are asked to write the core part of a mini processor simulator called MySPIM using C language on a Unix or a PC platform. Your MySPIM will demonstrate some functions of MIPS processor as well as the principle of the datapath and the control signals of MIPS processor. The MySPIM simulator should read in a file containing MIPS machine codes (in the format specified below) and simulate what the MIPS does cycle-by-cycle. You are required to implement the MySPIM with a single-cycle datapath. You are asked to fill in the body of several functions in a given file.



### 9 Functions Implemented in project.c:


•	instruction_fetch(…)
1.	Fetch the instruction addressed by PC from Mem and write it to instruction.
2.	Return 1 if a halt condition occurs; otherwise, return 0.


•	instruction_partition(…)
1.	Partition instruction into several parts (op, r1, r2, r3, funct, offset, jsec).
2.	Read line 41 to 47 of spimcore.c for more information.



•	instruction_decode(…)
1.	Decode the instruction using the opcode (op).
2.	Assign the values of the control signals to the variables in the structure controls (See spimcore.h file).
The meanings of the values of the control signals:
For MemRead, MemWrite or RegWrite, the value 1 means that enabled, 0 means that disabled, 2 means “don’t care”.
For RegDst, Jump, Branch, MemtoReg or ALUSrc, the value 0 or 1 indicates the selected path of the multiplexer; 2 means “don’t care”.
The following table shows the meaning of the values of ALUOp.



Value (Binary)	Meaning:


000	ALU will do addition or “don’t care”


001	ALU will do subtraction


010	ALU will do “set less than” operation


011	ALU will do “set less than unsigned” operation


100	ALU will do “AND” operation


101	ALU will do “OR” operation


110	ALU will shift left extended_value by 16 bits


111	The instruction is an R-type instruction

3.	Return 1 if a halt condition occurs; otherwise, return 0.




•	read_register(…)
1.	Read the registers addressed by r1 and r2 from Reg, and write the read values to data1 and data2 respectively.





•	sign_extend(…)
1.	Assign the sign-extended value of offset to extended_value.





•	ALU_operations(…)
1.	Apply ALU operations on data1, and data2 or extended_value (determined by ALUSrc).
2.	The operation performed is based on ALUOp and funct.
3.	Apply the function ALU(…).
4.	Output the result to ALUresult.
5.	Return 1 if a halt condition occurs; otherwise, return 0.





•	rw_memory(…)
1.	Base on the value of MemWrite or MemRead to determine memory write operation or memory read operation.
2.	Read the content of the memory location addressed by ALUresult to memdata.
3.	Write the value of data2 to the memory location addressed by ALUresult.
4.	Return 1 if a halt condition occurs; otherwise, return 0.




•	write_register(…)
1.	Write the data (ALUresult or memdata) to a register (Reg) addressed by r2 or r3.




•	PC_update(…)
1.	Update the program counter (PC).






### Operation of the spimcore

For your convenience, here is how you could do it in UNIX environment. First compile:

$ gcc -o spimcore spimcore.c project.c

After compilation, to use MySPIM, you would type the following command in UNIX:

$ ./spimcore <input_filename>.asc

The command prompt

cmd:

should appear. spimcore works like a simple debugger with the following commands:



r	Dump registers contents


m	Dump memory contents (in Hexadecimal format)


s[n]	Step n instructions (simulate the next n instruction). If n is not typed, 1 is assumed


c	Continue (carry on the simulation until the program halts (with illegal instruction))


H	Check if the program has halted


d	ads1 ads2 Hexadecimal dump from address ads1 to ads2


I	Inquire memory size


P	Print the input file


g	Display all control signals


X, X, q, Q	Quit
















