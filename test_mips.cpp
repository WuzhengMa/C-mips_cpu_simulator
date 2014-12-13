#include "mips.h"
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
using namespace std;

uint8_t InstructionSet[10];
uint8_t InstructionSet_next[10];
uint8_t CurrentIns[10];


int main(){

	//Initialise
	for(int i=0; i<10; i++){
		InstructionSet[i] = 0;
		InstructionSet_next[i] = 0;
		CurrentIns[i] = 0;
	}

	mips_mem_h ram = mips_mem_create_ram(1<<20, 4);
	mips_cpu_h cpu = mips_cpu_create(ram);
	
	//Start tests
	mips_test_begin_suite();

//-----------------------------------------------------------------------
	//ADD $1, $1, $2	
	// 0000 0000 0010 0010 0000 1000 0010 0000	
	//Test ADD with positive number + negative number
	get_instruction(InstructionSet, 0);	
	int testId = mips_test_begin_test("ADD");
	int passed = 0;
	uint32_t pcOrig;
	//Set values into registers and write instruction code into memory
	mips_error err = mips_cpu_set_register(cpu, 1, 5);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 0xFFFFFFFFD);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);

	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){		//if execeptions happen, pc value should be stayed the same
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	uint32_t got = 0;
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 2);	
	if(err == 0){
		mips_test_end_test(testId, passed, NULL);
	}

//-----------------------------------------------------------------------
	//ADD $1, $1, $2
	// 0000 0000 0010 0010 0000 1000 0010 0000
	//Test ADD with two positive numbers
	get_instruction(InstructionSet, 0);
	testId = mips_test_begin_test("ADD");
	passed = 0;
	got = 0;
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 5);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 55);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 60);	
	if(err == 0){
		mips_test_end_test(testId, passed, NULL);
	}

//-----------------------------------------------------------------------
	//ADD $1, $1, $2
	// 0000 0000 0010 0010 0000 1000 0010 0000\
	//Test ADD with two negative numbers that will cause a signed overflow
	get_instruction(InstructionSet, 0);
	testId = mips_test_begin_test("ADD");
	passed = 0;
	got = 0;
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 0x80000000);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 0xFFFFFFFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);

	passed = (err == mips_ExceptionArithmeticOverflow);	
	mips_test_end_test(testId, passed, NULL);

//-------------------------------------------------------------------	
	//AND $3, $3, $4
	//0000 0000 0110 0100 0001 1000 0010 0100
	get_instruction(InstructionSet, 4);


	passed = 0;
	testId = mips_test_begin_test("AND");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 3, &got);
	
	passed = (err == mips_Success) && (got == 0x0F);
	mips_test_end_test(testId, passed, NULL);

//-------------------------------------------------------
	//OR $3, $3, $4
	get_instruction(InstructionSet, 8);
	//0000 0000 0110 0100 0001 1000 0010 0101
	passed = 0;
	testId = mips_test_begin_test("OR");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 3, &got);

	passed = (err == mips_Success) && (got == 0xFF);
	mips_test_end_test(testId, passed, NULL);

//------------------------------------------------------------
	//XOR $3, $3, $4
	get_instruction(InstructionSet, 12);

	//0000 0000 0110 0100 0001 1000 0010 0110
	passed = 0;
	testId = mips_test_begin_test("XOR");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 3, &got);

	passed = (err == mips_Success) && (got == 0xF0);
	mips_test_end_test(testId, passed, NULL);

//------------------------------------------------------------
	//ADDU $1, $1, $2
	get_instruction(InstructionSet, 16);
	//0000 0000 0010 0010 0000 1000 0010 0001
	passed = 0;
	testId = mips_test_begin_test("ADDU");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 0xFFFF);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 0xFFFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 131070);
	mips_test_end_test(testId, passed, NULL);

//------------------------------------------------------------
	//ADDU $1, $1, $2
	get_instruction(InstructionSet, 16);
	//0000 0000 0010 0010 0000 1000 0010 0001
	passed = 0;
	testId = mips_test_begin_test("ADDU");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 0xFFFFFFFF);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 0xFFFFFFFE);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);

	passed = (err == mips_ErrorInvalidArgument);
	mips_test_end_test(testId, passed, NULL);	

//------------------------------------------------------------
	//SLT $1, $1, $2
	get_instruction(InstructionSet, 20);
	//0000 0000 0010 0010 0000 1000 0010 1010
	passed = 0;
	testId = mips_test_begin_test("SLT");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 0xFFFFFFF0);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 0xFFFFFFFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);

//------------------------------------------------------------
	//SLT $1, $1, $2
	get_instruction(InstructionSet, 20);
	//0000 0000 0010 0010 0000 1000 0010 1010
	passed = 0;
	testId = mips_test_begin_test("SLT");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 0xFFFFFFF0);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 0xFFFFFFF0);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);

	passed = (err == mips_ErrorInvalidArgument);
	mips_test_end_test(testId, passed, NULL);

//------------------------------------------------------------
	//SLTU $1, $1, $2
	get_instruction(InstructionSet, 24);
	//0000 0000 0010 0010 0000 1000 0010 1011
	passed = 0;
	testId = mips_test_begin_test("SLTU");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 20);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 50);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
//------------------------------------------------------------
	//SLTU $1, $1, $2
	get_instruction(InstructionSet, 24);
	//0000 0000 0010 0010 0000 1000 0010 1011
	passed = 0;
	testId = mips_test_begin_test("SLTU");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 50);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 50);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 0);
	mips_test_end_test(testId, passed, NULL);

//------------------------------------------------------------
	//SLTU $1, $1, $2
	get_instruction(InstructionSet, 24);
	//0000 0000 0010 0010 0000 1000 0010 1011
	passed = 0;
	testId = mips_test_begin_test("SLTU");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 100);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 20);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 0);
	mips_test_end_test(testId, passed, NULL);
	
//------------------------------------------------------------
	//SLL S1, S2, h
	get_instruction(InstructionSet, 28);
	//0000 0000 0000 0010 0000 1010 0000 0000
	passed = 0;
	testId = mips_test_begin_test("SLL");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 1);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 256);
	mips_test_end_test(testId, passed, NULL);

//------------------------------------------------------------
	//SLLV $1, $2, $3
	get_instruction(InstructionSet, 32);
	//0000 0000 0110 0010 0000 1000 0000 0100
	passed = 0;
	testId = mips_test_begin_test("SLLV");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 3, 8);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 1);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 256);
	mips_test_end_test(testId, passed, NULL);
		
//------------------------------------------------------------
	//SRA S1, S2, h
	get_instruction(InstructionSet, 36);
	//0000 0000 0000 0010 0000 1000 1100 0011
	passed = 0;
	testId = mips_test_begin_test("SRA");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 0xFFFFFFFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 0xFFFFFFFF);
	mips_test_end_test(testId, passed, NULL);

//------------------------------------------------------------
	//SRL S1, S2, h
	get_instruction(InstructionSet, 40);
	//0000 0000 0000 0010 0000 1001 0000 0010
	passed = 0;
	testId = mips_test_begin_test("SRL");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 0x4F);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 4);
	mips_test_end_test(testId, passed, NULL);
	
//------------------------------------------------------------
	//SRLV $1, $2, $3
	get_instruction(InstructionSet, 44);
	//0000 0000 0110 0010 0000 1000 0000 0110
	passed = 0;
	testId = mips_test_begin_test("SRLV");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 3, 4);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 0xFFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 255);
	mips_test_end_test(testId, passed, NULL);
		
//-----------------------------------------------------------------------
	//SUB $1, $2, $3	
	get_instruction(InstructionSet, 48);
	//0000 0000 0100 0011 0000 1000 0010 0010
	testId = mips_test_begin_test("SUB");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 0xFFFFFFFD);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0xFFFFFFFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == -2);	
	if(err == 0){
		mips_test_end_test(testId, passed, NULL);
	}

//-----------------------------------------------------------------------
	//SUB $1, $2, $3	
	get_instruction(InstructionSet, 48);
	//0000 0000 0100 0011 0000 1000 0010 0010
	testId = mips_test_begin_test("SUB");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 100);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 20);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 80);	
	if(err == 0){
		mips_test_end_test(testId, passed, NULL);
	}

//-----------------------------------------------------------------------
	//SUB $1, $2, $3	
	get_instruction(InstructionSet, 48);
	//0000 0000 0100 0011 0000 1000 0010 0010
	testId = mips_test_begin_test("SUB");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 0xFFFFFFFF);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0xFFFFFFFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 0);	
	if(err == 0){
		mips_test_end_test(testId, passed, NULL);
	}

//-----------------------------------------------------------------------
	//SUBU $1, $2, $3	
	get_instruction(InstructionSet, 52);
	//0000 0000 0100 0011 0000 1000 0010 0011
	testId = mips_test_begin_test("SUBU");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 0xFFFFFFFF);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x4);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 0xFFFFFFFB);	
	if(err == 0){
		mips_test_end_test(testId, passed, NULL);
	}

//-----------------------------------------------------------------------
	//SUBU $1, $2, $3	
	get_instruction(InstructionSet, 52);
	//0000 0000 0100 0011 0000 1000 0010 0011
	testId = mips_test_begin_test("SUBU");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 0xFFFFFFFF);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0xFFFFFFFE);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 1);	
	if(err == 0){
		mips_test_end_test(testId, passed, NULL);
	}

//-----------------------------------------------------------------------
	//ADDi $1, $2, 0xFFFF	
	get_instruction(InstructionSet, 56);
	//0010 0000 0100 0001 1111 1111 1111 1111;
	testId = mips_test_begin_test("ADDI");
	passed = 0;
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 0xFFFFFFFD);	
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 65532);	
	if(err == 0){
		mips_test_end_test(testId, passed, NULL);
	}

//-----------------------------------------------------------------------
	//ADDIU $1, $2, 0xFFFF	
	get_instruction(InstructionSet, 60);
	//0010 0000 0100 0001 1111 1111 1111 1111;
	testId = mips_test_begin_test("ADDIU");
	passed = 0;
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 0);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 0xFFFF);	
	if(err == 0){
		mips_test_end_test(testId, passed, NULL);
	}

//-------------------------------------------------------------------	
	//ANDI $1, $2, 0xFF
	get_instruction(InstructionSet, 64);
	//0011 0000 0100 0001 0000 0000 1111 1111
	passed = 0;
	testId = mips_test_begin_test("ANDI");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 0x0F);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);
	
	passed = (err == mips_Success) && (got == 0x0F);
	mips_test_end_test(testId, passed, NULL);

//-------------------------------------------------------------------	
	//0011 0100 0010 0010 1111 1111 0000 0000
	//ORI $1, $2, 0xFF00
	get_instruction(InstructionSet, 68);
	passed = 0;
	testId = mips_test_begin_test("ORI");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 0);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);
	
	passed = (err == mips_Success) && (got == 0xFF00);
	mips_test_end_test(testId, passed, NULL);

//-------------------------------------------------------------------	
	//0011 1000 0100 0001 1111 1111 1111 1111
	//XORI $1, $2, 0xFFFF
	get_instruction(InstructionSet, 72);
	passed = 0;
	testId = mips_test_begin_test("XORI");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 0xF0F0);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);
	
	passed = (err == mips_Success) && (got == 0x0F0F);
	mips_test_end_test(testId, passed, NULL);

//-------------------------------------------------------------------	
	//0001 0000 0010 0010 0000 0000 0000 1111
	//BEQ $1, $2, 0xF
	get_instruction(InstructionSet, 76);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction
	passed = 0;
	got = 0;
	uint32_t pcGot = 0;
	testId = mips_test_begin_test("BEQ");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 0xF0F0);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 0xF0F0);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if(pcGot == (0xF<<2))
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests

//-------------------------------------------------------------------	
	//0001 0000 0010 0010 0000 0000 0000 1111
	//BEQ $1, $2, 0xF
	get_instruction(InstructionSet, 76);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	got = 0;
	pcGot = 0;
	testId = mips_test_begin_test("BEQ");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 0x0);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 0xF0F0);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);

	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if(pcGot == pcOrig + 4)
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests

//-------------------------------------------------------------------	
	//0000 0100 0010 0001 0000 0000 0000 000F
	//BGEZ $1, 0xF0F0
	get_instruction(InstructionSet, 80);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	testId = mips_test_begin_test("BGEZ");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 0xF0F0);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if(pcGot ==(0xF<<2))
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests

//-------------------------------------------------------------------	
	//0000 0100 0010 0001 0000 0000 0000 000F
	//BGEZ $1, 0xF0F0
	get_instruction(InstructionSet, 80);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	testId = mips_test_begin_test("BGEZ");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 0xFFFFFF00);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if(pcGot == pcOrig + 4)
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests

//-------------------------------------------------------------------	
	//0000 0100 0011 0001 0000 0000 0000 FFFF
	//BGEZAL $1, 0xF
	get_instruction(InstructionSet, 84);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	got = 0;
	uint32_t pcGot1 = 0;
	testId = mips_test_begin_test("BGEZAL");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 0xF0F0);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err == 0)
		err = mips_cpu_get_register(cpu, 31, &pcGot1);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if((pcGot == (0xF<<2)) && (pcGot1 == pcOrig + 8))
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests

//-------------------------------------------------------------------	
	//0000 0100 0011 0001 0000 0000 0000 FFFF
	//BGEZAL $1, 0xF
	get_instruction(InstructionSet, 84);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	got = 0;
	pcGot1 = 0;
	testId = mips_test_begin_test("BGEZAL");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 0);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err == 0)
		err = mips_cpu_get_register(cpu, 31, &pcGot1);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if((pcGot == (0xF<<2)) && (pcGot1 == pcOrig + 8))
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests	

//-------------------------------------------------------------------	
	//0000 0100 0011 0001 0000 0000 0000 FFFF
	//BGEZAL $1, 0xF
	get_instruction(InstructionSet, 84);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BGEZAL");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 0xFFFF0000);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);

	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if(pcGot == pcOrig + 4)
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests

//-------------------------------------------------------------------	
	//0001 1100 0010 0000 0000 0000 1010 1110
	//BGTZ $1, 0xAE
	get_instruction(InstructionSet, 88);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BGTZ");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 0xF0F0);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if(pcGot ==(0xAE<<2))
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests

//-------------------------------------------------------------------	
	//0001 1100 0010 0000 0000 0000 1010 1110
	//BGTZ $1, 0xAE
	get_instruction(InstructionSet, 88);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BGTZ");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 0xFFFFF000);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if(pcGot == pcOrig + 4)
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests


//-------------------------------------------------------------------	
	//0001 1000 0010 0000 0000 0000 1010 1110
	//BLEZ $1, 0xAE
	get_instruction(InstructionSet, 92);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BLEZ");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 0xFFFFFFFF);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if(pcGot == (0xAE<<2))
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests


//-------------------------------------------------------------------	
	//0001 1000 0010 0000 0000 0000 1010 1110
	//BLEZ $1, 0xAE
	get_instruction(InstructionSet, 92);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BLEZ");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 0);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if(pcGot == (0xAE<<2))
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests


//-------------------------------------------------------------------	
	//0001 1000 0010 0000 0000 0000 1010 1110
	//BLEZ $1, 0xAE
	get_instruction(InstructionSet, 92);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BLEZ");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 0xFFFF);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if(pcGot == pcOrig + 4)
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests

//-------------------------------------------------------------------	
	//0000 0100 0010 0000 0000 0000 1010 1110
	//BLTZ $1, 0xAE
	get_instruction(InstructionSet, 96);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BLTZ");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, -10);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if(pcGot == (0xAE<<2))
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests

//-------------------------------------------------------------------	
	//0000 0100 0010 0000 0000 0000 1010 1110
	//BLTZ $1, 0xAE
	get_instruction(InstructionSet, 96);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BLTZ");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 0xFFFF);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if(pcGot == pcOrig + 4)
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests
	
//-------------------------------------------------------------------	
	//0000 0100 0011 0000 0000 0000 1010 1110
	//BLTZAL $1, 0xAE
	get_instruction(InstructionSet, 100);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BLTZAL");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 0xFFFF0000);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err == 0)
		err = mips_cpu_get_register(cpu, 31, &pcGot1);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if((pcGot == (0xAE<<2)) && (pcGot1 == pcOrig + 4))
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests	

//-------------------------------------------------------------------	
	//0000 0100 0011 0000 0000 0000 1010 1110
	//BLTZAL $1, 0xAE
	get_instruction(InstructionSet, 100);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BLTZAL");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 0x0);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if(pcGot == pcOrig + 4)
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);

	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests	

//-------------------------------------------------------------------	
	//0001 0100 0010 0010 0000 0000 1010 1110
	//BNE $1, $2, 0xAE
	get_instruction(InstructionSet, 104);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BNE");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 0xF0F0);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 0xFFFFFF0F);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if(pcGot == (0xAE<<2))
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests

//-------------------------------------------------------------------	
	//0001 0100 0010 0010 0000 0000 1010 1110
	//BNE $1, $2, 0xAE
	get_instruction(InstructionSet, 104);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("BNE");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 0xF0F0);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 0xF0F0);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if(pcGot == pcOrig + 4)
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests

//-------------------------------------------------------------------	
	//0000 1000 0000 0000 0000 0000 1111 1111
	//J	0xFF
	get_instruction(InstructionSet, 108);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("J");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_get_pc(cpu, &pcOrig);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if(pcGot == ((pcOrig & 0xF0000000) | (0xFF<<2)))
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests

//-------------------------------------------------------------------	
	//0000 0000 0010 0000 0000 0000 0000 1000
	//JR $1
	get_instruction(InstructionSet, 112);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("JR");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 1, 0xF0F0);
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if(cpu->pc == 0xF0F0)
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests

//-------------------------------------------------------------------	
	//0000 1100 0000 0000 1111 0000 1111 0000
	//JAL 0xF0F0
	get_instruction(InstructionSet, 116);
	get_instruction(InstructionSet_next, 4); // get the instruction for the instruction follwing the branch instruction 
										// Assume that AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
										//to be the following instruction

	passed = 0;
	got = 0;
	testId = mips_test_begin_test("JAL");
	//Set values into registers and write instruction code into memory
	//Set the registers that will be used in the next instruction following the branch instruction
	if(err == 0)
		err = mips_cpu_set_register(cpu, 3, 0x0F);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 4, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);

	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig+4, 4, InstructionSet_next);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcGot);
	if(err == 0)
		err = mips_cpu_get_register(cpu, 31, &pcGot1);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if((pcGot == ((pcOrig & 0xF0000000) | (0xF0F0<<2))) && (pcGot1 == pcOrig +8))
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
	if(err == 0)
		err = mips_cpu_set_pc(cpu, pcOrig+4); //Set the PC back after the Branch tests

//-------------------------------------------------------------------	
	//1000 0000 0100 0001 1111 1111 1111 1111
	//LB $1, offset($2)
	get_instruction(InstructionSet, 120);
	passed = 0;
	got = 0;
	testId = mips_test_begin_test("LB");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 2);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		err = mips_cpu_get_register(cpu, 1, &got);
	}

	passed = (err == mips_Success) && (got == 0x22);
	mips_test_end_test(testId, passed, NULL);

//-------------------------------------------------------------------	
	//1001 0000 0100 0001 0000 0000 0000 1111
	//LBU $1, offset($2)
	get_instruction(InstructionSet, 124);
	passed = 0;
	got = 0;
	testId = mips_test_begin_test("LBU");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 2);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		err = mips_cpu_get_register(cpu, 1, &got);

	}

	passed = (err == mips_Success) && (got == 100);
	mips_test_end_test(testId, passed, NULL);

//-------------------------------------------------------------------	
	//0011 1100 0000 0001 1111 0000 1111 0000
	//LUI $1, imm
	get_instruction(InstructionSet, 128);
	passed = 0;
	got = 0;
	testId = mips_test_begin_test("LUI");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		err = mips_cpu_get_register(cpu, 1, &got);

	}

	passed = (err == mips_Success) && (got == 0xF0F00000);
	mips_test_end_test(testId, passed, NULL);

//-------------------------------------------------------------------	
	//1000 1100 0100 0001 0000 0000 0000 1111
	//LW $1, 0ffset($2)
	get_instruction(InstructionSet, 132);
	passed = 0;
	got = 0;
	testId = mips_test_begin_test("LW");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 2);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		err = mips_cpu_get_register(cpu, 1, &got);

	}

	passed = (err == mips_Success) && (got == 0x00641826);
	mips_test_end_test(testId, passed, NULL);


//-------------------------------------------------------------------	
	//1010 0000 0100 0001 0000 0000 0000 1111
	//SB $1, offset($2)
	get_instruction(InstructionSet, 136);
	passed = 0;
	got = 0;
	uint32_t temp = 0;
	testId = mips_test_begin_test("SB");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 15);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 0x874931AE);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_mem_read(ram, 28, 4, CurrentIns);
	if(err == 0){
		for(int i=0; i<4; i++){
			temp = temp << 8;
			temp = temp | CurrentIns[i];
		}
		if(temp == 0x0022AE21)
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);

//-------------------------------------------------------------------	
	//1010 0100 0100 0001 0000 0000 0000 1100
	//SH $1, offset($2)
	get_instruction(InstructionSet, 140);
	passed = 0;
	temp = 0;
	got = 0;
	testId = mips_test_begin_test("SH");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 28);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 0x123799AE);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_mem_read(ram, 40, 4, CurrentIns);
	if(err == 0){
		for(int i=0; i<4; i++){
			temp = temp << 8;
			temp = temp | CurrentIns[i];
		}
		if(temp == 0x000099AE)
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
//-------------------------------------------------------------------	
	//1010 1100 0100 0001 0000 0000 11000 1000
	//SW $1, offset(20)
	get_instruction(InstructionSet, 144);
	passed = 0;
	temp = 0;
	got = 0;
	testId = mips_test_begin_test("SW");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 20);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 0x1234567F);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_mem_read(ram, 220, 4, CurrentIns);
	if(err == 0){
		for(int i=0; i<4; i++){
			temp = temp << 8;
			temp = temp | CurrentIns[i];
		}
		if(temp == 0x1234567F)
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
	
//-------------------------------------------------------------------	
	//0000 0000 0010 0010 0000 0000 0001 1010
	//DIV $1, $2
	get_instruction(InstructionSet, 148);
	passed = 0;
	temp = 0;
	got = 0;
	mips_cpu_h cpu_used = set_cpu(cpu);
	testId = mips_test_begin_test("DIV");
	//Set values into registers and write instruction code into memory
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 0xFFFFFFFF);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 10);
	if(err == 0)	
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if((cpu_used->LO == 0) && (cpu_used->HI == -1))
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);

//-------------------------------------------------------------------	
	//0000 0000 0010 0010 0000 0000 0001 1010
	//DIV $1, $2
	get_instruction(InstructionSet, 148);
	passed = 0;
	temp = 0;
	got = 0;
	cpu_used = set_cpu(cpu);
	testId = mips_test_begin_test("DIV");
	//Set values into registers and write instruction code into memory
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 2);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 0xFFFFFFFB);
	if(err == 0)	
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if((cpu_used->LO == 0) && (cpu_used->HI == 2))
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);

//-------------------------------------------------------------------	
	//0000 0000 0010 0010 0000 0000 0001 1010
	//DIV $1, $2
	get_instruction(InstructionSet, 148);
	passed = 0;
	temp = 0;
	got = 0;
	cpu_used = set_cpu(cpu);
	testId = mips_test_begin_test("DIV");
	//Set values into registers and write instruction code into memory
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 0xFFFFFFF8);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 0xFFFFFFFB);
	if(err == 0)	
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if((cpu_used->LO == 1) && (cpu_used->HI == -3))
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);

//-------------------------------------------------------------------	
	//0000 0000 0010 0010 0000 0000 0001 1010
	//DIV $1, $2
	get_instruction(InstructionSet, 148);
	passed = 0;
	temp = 0;
	got = 0;
	cpu_used = set_cpu(cpu);
	testId = mips_test_begin_test("DIV");
	//Set values into registers and write instruction code into memory
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 36);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 4);
	if(err == 0)	
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if((cpu_used->LO == 9) && (cpu_used->HI == 0))
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
//-------------------------------------------------------------------	
	//0000 0000 0010 0010 0000 0000 0001 1011
	//DIVU $1, $2
	get_instruction(InstructionSet, 152);
	passed = 0;
	temp = 0;
	got = 0;
	testId = mips_test_begin_test("DIVU");
	//Set values into registers and write instruction code into memory
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 0xFFFF0000);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 0xFFFFFFFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if((cpu_used->LO == 0) && (cpu_used->HI == 0xFFFF0000))
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);	

//-------------------------------------------------------------------	
	//0000 0000 0010 0010 0000 0000 0001 1011
	//DIVU $1, $2
	get_instruction(InstructionSet, 152);
	passed = 0;
	temp = 0;
	got = 0;
	testId = mips_test_begin_test("DIVU");
	//Set values into registers and write instruction code into memory
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 0xFFFF0000);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 4);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if((cpu_used->LO == 1073725440) && (cpu_used->HI == 0))
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);		

//-------------------------------------------------------------------	
	//0000 0000 0010 0010 0000 0000 0001 1011
	//DIVU $1, $2
	get_instruction(InstructionSet, 152);
	passed = 0;
	temp = 0;
	got = 0;
	testId = mips_test_begin_test("DIVU");
	//Set values into registers and write instruction code into memory
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 50);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 21);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if((cpu_used->LO == 2) && (cpu_used->HI == 8))
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);

//-------------------------------------------------------------------	
	//0000 0000 0000 0000 0000 1000 0001 0000
	//MFHI, $1
	get_instruction(InstructionSet, 156);
	passed = 0;
	temp = 0;
	got = 0;
	testId = mips_test_begin_test("MFHI");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}

	passed = (err == mips_Success) && (got == 8);
	mips_test_end_test(testId, passed, NULL);	

//-------------------------------------------------------------------	
	//0000 0000 0000 0000 0000 1000 0001 0010
	//MFLO, $1
	get_instruction(InstructionSet, 160);
	passed = 0;
	temp = 0;
	got = 0;
	testId = mips_test_begin_test("MFLO");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}

	passed = (err == mips_Success) && (got == 2);
	mips_test_end_test(testId, passed, NULL);	

//-------------------------------------------------------------------	
	//1000 1010 1000 0001 0000 0000 0010 1000
	//LWL $1, offset(20)
	get_instruction(InstructionSet, 164);
	passed = 0;
	temp = 0;
	got = 0;
	testId = mips_test_begin_test("LWL");
	//Set values into registers and write instruction code into memory
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 0x1234ABCD);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);
	//0000 0000 0110 0100 0001 1000 0010 0110
	passed = (err == mips_Success) && (got == 0x0064ABCD);
	mips_test_end_test(testId, passed, NULL);
	
//-------------------------------------------------------------------	
	//1001 1010 1000 0001 1111 1111 1111 1000
	//LWR $1, offset(20)
	get_instruction(InstructionSet, 168);
	passed = 0;
	temp = 0;
	got = 0;
	testId = mips_test_begin_test("LWR");
	//Set values into registers and write instruction code into memory
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 0x1234ABCD);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);
	//0000 0000 0110 0100 0001 1000 0010 0110
	passed = (err == mips_Success) && (got == 0x12341824);
	mips_test_end_test(testId, passed, NULL);

//-------------------------------------------------------------------	
	//0000 0000 0010 0010 0000 0000 0001 1000
	//MULT $1, $2
	get_instruction(InstructionSet, 172);
	passed = 0;
	temp = 0;
	got = 0;
	cpu_used = set_cpu(cpu);
	testId = mips_test_begin_test("MULT");
	//Set values into registers and write instruction code into memory
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 0xFFFFFFFF);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 10);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if((cpu_used->LO) == -10)
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);
		
//-------------------------------------------------------------------	
	//0000 0000 0010 0010 0000 0000 0001 1000
	//MULT $1, $2
	get_instruction(InstructionSet, 172);
	passed = 0;
	temp = 0;
	got = 0;
	cpu_used = set_cpu(cpu);
	testId = mips_test_begin_test("MULT");
	//Set values into registers and write instruction code into memory
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, -20);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, -1);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if((cpu_used->LO) == 20)
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);

//-------------------------------------------------------------------	
	//0000 0000 0010 0010 0000 0000 0001 1001
	//MULTU $1, $2
	get_instruction(InstructionSet, 176);
	passed = 0;
	temp = 0;
	got = 0;
	cpu_used = set_cpu(cpu);
	testId = mips_test_begin_test("MULTU");
	//Set values into registers and write instruction code into memory
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 0xFF);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 10);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if((cpu_used->LO) == 2550)
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);

//-------------------------------------------------------------------	
	//0000 0000 0010 0010 0000 0000 0001 1001
	//MULTU $1, $2
	get_instruction(InstructionSet, 176);
	passed = 0;
	temp = 0;
	got = 0;
	cpu_used = set_cpu(cpu);
	testId = mips_test_begin_test("MULTU");
	//Set values into registers and write instruction code into memory
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 0x80000000);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 1);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0){
		if((cpu_used->LO) == 0x80000000)
			got = 1;
	}

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);

//-------------------------------------------------------------------	
	//0000 0000 0010 0010 0000 0000 0001 1001
	//MULTU $1, $2
	get_instruction(InstructionSet, 176);
	passed = 0;
	temp = 0;
	got = 0;
	cpu_used = set_cpu(cpu);
	testId = mips_test_begin_test("MULTU");
	//Set values into registers and write instruction code into memory
	if(err == 0)
		err = mips_cpu_set_register(cpu, 1, 0x80000000);
	if(err == 0)
		err = mips_cpu_set_register(cpu, 2, 0x80000000);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err = mips_cpu_step(cpu);

	passed = (err == mips_ErrorInvalidArgument);
	mips_test_end_test(testId, passed, NULL);

//------------------------------------------------------------
	//0010 1000 0100 0001 FFFF FFFF 0000 0000
	//SLTI $1, $2, 0xFF00
	get_instruction(InstructionSet, 180);
	passed = 0;
	temp = 0;
	got = 0;
	testId = mips_test_begin_test("SLTI");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 0xAA);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);

//------------------------------------------------------------
	//0010 1100 0100 0001 FFFF FFFF FFFF FFFF
	//SLTIU $1, $2, 0xFFFF
	get_instruction(InstructionSet, 184);
	passed = 0;
	temp = 0;
	got = 0;
	testId = mips_test_begin_test("SLTIU");
	//Set values into registers and write instruction code into memory
	err = mips_cpu_set_register(cpu, 2, 0xFF);
	if(err == 0)
		err = mips_cpu_get_pc(cpu, &pcOrig);
	if(err == 0)
		err = mips_mem_write(ram, pcOrig, 4, InstructionSet);
	
	//Reads instruction from the memory and implements it
	if(err == 0)
		err = mips_mem_read(ram, pcOrig, 4, CurrentIns);
	if(err == 0)
		err = set_CurInstruction(CurrentIns, cpu);
	if(err == 0)
		err=mips_cpu_step(cpu);
	if(err != 0){		
		err = mips_cpu_set_pc(cpu, pcOrig);
	}
	if(err == 0)
		err = mips_cpu_get_register(cpu, 1, &got);

	passed = (err == mips_Success) && (got == 1);
	mips_test_end_test(testId, passed, NULL);

	//End of all tests
	mips_test_end_suite();
	
	//Free
	mips_cpu_free(cpu);
	mips_mem_free(ram);

	return 0;
}