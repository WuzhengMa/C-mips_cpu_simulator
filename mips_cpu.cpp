/*
 * Mips_simu.cpp
 *
 *  Created on: 21 Oct 2014
 *      Author: wm813
 */

#include "mips.h"
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <string>
using namespace std;

mips_cpu_h mips_cpu_create(mips_mem_h mem){
	mips_cpu_h res = (mips_cpu_h) malloc(sizeof(struct mips_cpu_impl));
	res->pc = 0;
	res->pcN = 4;
	res->HI = 0;
	res->LO = 0;
	for (unsigned i = 0; i < 32; i++){
		res->regs[i] = 0;
		if(i<10){
			res->currentIns[i] = 0;
		}
	}
	res->mem = mem;
	return res;
}

mips_cpu_h set_cpu(mips_cpu_h cpu){
	return cpu;
}

mips_error mips_cpu_reset(mips_cpu_h state){
	state->pc = 0;
	for(int i=0; i<32; i++){
		state->regs[i] = 0;
	}
	return mips_Success;
}

mips_error mips_cpu_set_pc(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	uint32_t pc			//!< Address of the next instruction to exectute.
	){
		state->pc = pc;
		return mips_Success;
}

mips_error mips_cpu_get_pc(mips_cpu_h state, uint32_t *pc){
	*pc = state->pc;
	return mips_Success;
}

void mips_cpu_free(mips_cpu_h state){
	state->pc = 0;
	state->pcN = 0;
	for(int i=0; i<32; i++){
		state->regs[i] = 0;
		if(i<10){
			state->currentIns[i] = 0;
		}
	}
	state->HI = 0;
	state->LO = 0;
	free(state);
}


/*! Returns the current value of one of the 32 general purpose MIPS registers */
mips_error mips_cpu_get_register(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	unsigned index,		//!< Index from 0 to 31
	uint32_t *value		//!< Where to write the value to
){
	if(state == 0)
		return mips_ErrorInvalidHandle;
	if(index >= 32)
		return mips_ExceptionInvalidAlignment;

	*value = state->regs[index];
	return mips_Success;
}


mips_error mips_cpu_step(mips_cpu_h state){
	state->pc = state->pc + state->pcN;		//Advance the pc by 4
	mips_error err5 = encodeAndImpl(state);		//Then encodes and implements instruction
	return err5;
}


/*! Modifies one of the 32 general purpose MIPS registers. */
mips_error mips_cpu_set_register(
	mips_cpu_h state,	//!< Valid (non-empty) handle to a CPU
	unsigned index,		//!< Index from 0 to 31
	uint32_t value		//!< New value to write into register file
){
	if(state == 0)
		return mips_ErrorInvalidHandle;
	if(index >= 32)
		return mips_ExceptionInvalidAlignment;


	state->regs[index] = value;
	return mips_Success;
}

mips_error set_CurInstruction(uint8_t *CurrentIns, mips_cpu_h cpu){
	for(int i=0; i<10; i++){
		cpu->currentIns[i] = CurrentIns[i];
	}

	return mips_Success;
}

void get_instruction(uint8_t *InstructionSet, int num){
	uint8_t InstructionIn[500];
	
	InstructionIn[0] = 0x00;
	InstructionIn[1] = 0x22;
	InstructionIn[2] = 0x08;
	InstructionIn[3] = 0x20;
	// 0000 0000 0010 0010 0000 1000 0010 0000
	//ADD $1, $1, $2

	InstructionIn[4] = 0x00;
	InstructionIn[5] = 0x64;
	InstructionIn[6] = 0x18;
	InstructionIn[7] = 0x24;
	//0000 0000 0110 0100 0001 1000 0010 0100
	//AND $3, $3, $4
	
	InstructionIn[8] = 0x00;
	InstructionIn[9] = 0x64;
	InstructionIn[10] = 0x18;
	InstructionIn[11] = 0x25;
	//0000 0000 0110 0100 0001 1000 0010 0101
	//OR $3, $3, $4

	InstructionIn[12] = 0x00;
	InstructionIn[13] = 0x64;
	InstructionIn[14] = 0x18;
	InstructionIn[15] = 0x26;
	//0000 0000 0110 0100 0001 1000 0010 0110
	//Xor $3, $3, $4

	InstructionIn[16] = 0x00;
	InstructionIn[17] = 0x22;
	InstructionIn[18] = 0x08;
	InstructionIn[19] = 0x21;
	//0000 0000 0010 0010 0000 1000 0010 0001
	//ADDU $1, $1, $2

	InstructionIn[20] = 0x00;
	InstructionIn[21] = 0x22;
	InstructionIn[22] = 0x08;
	InstructionIn[23] = 0x2A;
	//0000 0000 0010 0010 0000 1000 0010 1010
	//SLT $1, $1, $2

	InstructionIn[24] = 0x00;
	InstructionIn[25] = 0x22;
	InstructionIn[26] = 0x08;
	InstructionIn[27] = 0x2B;
	//0000 0000 0010 0010 0000 1000 0010 1011
	//SLTU $1, $1, $2

	InstructionIn[28] = 0x00;
	InstructionIn[29] = 0x02;
	InstructionIn[30] = 0x0A;
	InstructionIn[31] = 0x00;
	//0000 0000 0000 0010 0000 1010 0000 0000
	//SLL $2, $1, h

	InstructionIn[32] = 0x01;
	InstructionIn[33] = 0x62;
	InstructionIn[34] = 0x08;
	InstructionIn[35] = 0x04;
	//0000 0000 0110 0010 0000 1000 0000 0100
	//SLLV $1, $2, $3
	
	InstructionIn[36] = 0x00;
	InstructionIn[37] = 0x02;
	InstructionIn[38] = 0x08;
	InstructionIn[39] = 0xC3;
	//0000 0000 0000 0010 0000 1000 1100 0011
	//SRA $1, $2, h

	InstructionIn[40] = 0x00;
	InstructionIn[41] = 0x02;
	InstructionIn[42] = 0x09;
	InstructionIn[43] = 0x02;
	//0000 0000 0000 0010 0000 1001 0000 0010
	//SRL $1, $2, h

	InstructionIn[44] = 0x00;
	InstructionIn[45] = 0x62;
	InstructionIn[46] = 0x08;
	InstructionIn[47] = 0x06;
	//0000 0000 0110 0010 0000 1000 0000 0110
	//SRLV $1, $2, $3

	InstructionIn[48] = 0x00;
	InstructionIn[49] = 0x43;
	InstructionIn[50] = 0x08;
	InstructionIn[51] = 0x22;
	//0000 0000 0100 0011 0000 1000 0010 0010
	//SUB $1, $2, $3

	InstructionIn[52] = 0x00;
	InstructionIn[53] = 0x43;
	InstructionIn[54] = 0x08;
	InstructionIn[55] = 0x23;
	//0000 0000 0100 0011 0000 1000 0010 0011
	//SUBU $1, $2, $3	

	InstructionIn[56] = 0x20;
	InstructionIn[57] = 0x41;
	InstructionIn[58] = 0xFF;
	InstructionIn[59] = 0xFF;
	//0010 0000 0100 0001 1111 1111 1111 1111;
	//ADDI $1, $2, 0xFFFF

	InstructionIn[60] = 0x24;
	InstructionIn[61] = 0x41;
	InstructionIn[62] = 0xFF;
	InstructionIn[63] = 0xFF;
	//0010 0100 0100 0001 1111 1111 1111 1111;
	//ADDIU $1, $2, 0xFFFF

	InstructionIn[64] = 0x30;
	InstructionIn[65] = 0x41;
	InstructionIn[66] = 0x00;
	InstructionIn[67] = 0xFF;
	//0011 0000 0100 0001 0000 0000 1111 1111
	//ANDI $1, $2, 0xFF

	InstructionIn[68] = 0x34;
	InstructionIn[69] = 0x41;
	InstructionIn[70] = 0xFF;
	InstructionIn[71] = 0x00;
	//0011 0100 0100 0001 1111 1111 0000 0000
	//ORI $1, $2, 0xF0

	InstructionIn[72] = 0x38;
	InstructionIn[73] = 0x41;
	InstructionIn[74] = 0xFF;
	InstructionIn[75] = 0xFF;
	//0011 1000 0100 0001 1111 1111 1111 1111
	//XORI $1, $2, 0xFF

	InstructionIn[76] = 0x10;
	InstructionIn[77] = 0x22;
	InstructionIn[78] = 0x00;
	InstructionIn[79] = 0x0F;
	//0001 0000 0010 0010 0000 0000 0000 1111
	//BEQ $1, $2, 0xF

	InstructionIn[80] = 0x04;
	InstructionIn[81] = 0x21;
	InstructionIn[82] = 0x00;
	InstructionIn[83] = 0x0F;
	//0000 0100 0010 0001 0000 0000 0000 000F
	//BGEZ $1, 0xF

	InstructionIn[84] = 0x04;
	InstructionIn[85] = 0x31;
	InstructionIn[86] = 0x00;
	InstructionIn[87] = 0x0F;	
	//0000 0100 0011 0001 0000 0000 0000 FFFF
	//BGEZAL $1, 0xF
	
	InstructionIn[88] = 0x1C;
	InstructionIn[89] = 0x20;
	InstructionIn[90] = 0x00;
	InstructionIn[91] = 0xAE;	
	//0001 1100 0010 0000 0000 0000 1010 1110
	//BGTZ $1, 0xAE

	InstructionIn[92] = 0x18;
	InstructionIn[93] = 0x20;
	InstructionIn[94] = 0x00;
	InstructionIn[95] = 0xAE;	
	//0001 1000 0010 0000 0000 0000 1010 1110
	//BLEZ $1, 0xAE

	InstructionIn[96] = 0x04;
	InstructionIn[97] = 0x20;
	InstructionIn[98] = 0x00;
	InstructionIn[99] = 0xAE;	
	//0000 0100 0010 0000 0000 0000 1010 1110
	//BLTZ $1, 0xAE

	InstructionIn[100] = 0x04;
	InstructionIn[101] = 0x30;
	InstructionIn[102] = 0x00;
	InstructionIn[103] = 0xAE;	
	//0000 0100 0011 0000 0000 0000 1010 1110
	//BLTZAL $1, 0xAE

	InstructionIn[104] = 0x14;
	InstructionIn[105] = 0x22;
	InstructionIn[106] = 0x00;
	InstructionIn[107] = 0xAE;		
	//0001 0100 0010 0010 0000 0000 1010 1110
	//BNE $1, $2, 0xAE

	InstructionIn[108] = 0x08;
	InstructionIn[109] = 0x00;
	InstructionIn[110] = 0x00;
	InstructionIn[111] = 0xFF;			
	//0000 1000 0000 0000 0000 0000 1111 1111
	//J	0xFF

	InstructionIn[112] = 0x00;
	InstructionIn[113] = 0x20;
	InstructionIn[114] = 0x00;
	InstructionIn[115] = 0x08;			
	//0000 0000 0010 0000 0000 0000 0000 1000
	//JR $1

	InstructionIn[116] = 0x0C;
	InstructionIn[117] = 0x00;
	InstructionIn[118] = 0xF0;
	InstructionIn[119] = 0xF0;
	//0000 1100 0000 0000 1111 0000 1111 0000
	//JAL 0xF0F0
	
	InstructionIn[120] = 0x80;
	InstructionIn[121] = 0x41;
	InstructionIn[122] = 0xFF;
	InstructionIn[123] = 0xFF;
	//1000 0000 0100 0001 1111 1111 1111 1111
	//LB $1, offset($2)

	InstructionIn[124] = 0x90;
	InstructionIn[125] = 0x41;
	InstructionIn[126] = 0x00;
	InstructionIn[127] = 0x0F;
	//1001 0000 0100 0001 0000 0000 0000 1111
	//LBU $1, offset($2)

	InstructionIn[128] = 0x3C;
	InstructionIn[129] = 0x01;
	InstructionIn[130] = 0xF0;
	InstructionIn[131] = 0xF0;
	//0011 1100 0000 0001 1111 0000 1111 0000
	//LUI $1, imm

	InstructionIn[132] = 0x8C;
	InstructionIn[133] = 0x41;
	InstructionIn[134] = 0x00;
	InstructionIn[135] = 0x12;
	//1000 1100 0100 0001 0000 0000 0001 0010
	//LW $1, 0ffset($2)

	InstructionIn[136] = 0xA0;
	InstructionIn[137] = 0x41;
	InstructionIn[138] = 0x00;
	InstructionIn[139] = 0x0F;
	//1010 0000 0100 0001 0000 0000 0000 1111
	//SB $1, offset($2)

	InstructionIn[140] = 0xA4;
	InstructionIn[141] = 0x41;
	InstructionIn[142] = 0x00;
	InstructionIn[143] = 0x0C;
	//1010 0100 0100 0001 0000 0000 0000 1100
	//SH $1, offset($2)

	InstructionIn[144] = 0xAC;
	InstructionIn[145] = 0x41;
	InstructionIn[146] = 0x00;
	InstructionIn[147] = 0xC8;
	//1010 1100 0100 0001 0000 0000 1100 1000
	//SW $1, offset($2)

	InstructionIn[148] = 0x00;
	InstructionIn[149] = 0x22;
	InstructionIn[150] = 0x00;
	InstructionIn[151] = 0x1A;
	//0000 0000 0010 0010 0000 0000 0001 1010
	//DIV $1, $2

	InstructionIn[152] = 0x00;
	InstructionIn[153] = 0x22;
	InstructionIn[154] = 0x00;
	InstructionIn[155] = 0x1B;
	//0000 0000 0010 0010 0000 0000 0001 1011
	//DIVU $1, $2

	InstructionIn[156] = 0x00;
	InstructionIn[157] = 0x00;
	InstructionIn[158] = 0x08;
	InstructionIn[159] = 0x10;
	//0000 0000 0000 0000 0000 1000 0001 0000
	//MFHI, $1

	InstructionIn[160] = 0x00;
	InstructionIn[161] = 0x00;
	InstructionIn[162] = 0x08;
	InstructionIn[163] = 0x12;
	//0000 0000 0000 0000 0000 1000 0001 0010
	//MFLO, $1

	InstructionIn[164] = 0x8A;
	InstructionIn[165] = 0x81;
	InstructionIn[166] = 0xFF;
	InstructionIn[167] = 0xF8;
	//1000 1010 1000 0001 1111 1111 1111 1000
	//LWL $1, offset(20)

	InstructionIn[168] = 0x9A;
	InstructionIn[169] = 0x81;
	InstructionIn[170] = 0xFF;
	InstructionIn[171] = 0xF8;
	//1001 1010 1000 0001 1111 1111 1111 1000
	//LWR $1, offset(20)

	InstructionIn[172] = 0x00;
	InstructionIn[173] = 0x22;
	InstructionIn[174] = 0x00;
	InstructionIn[175] = 0x18;
	//0000 0000 0010 0010 0000 0000 0001 1000
	//MULT $1, $2

	InstructionIn[176] = 0x00;
	InstructionIn[177] = 0x22;
	InstructionIn[178] = 0x00;
	InstructionIn[179] = 0x19;
	//0000 0000 0010 0010 0000 0000 0001 1001
	//MULTU $1, $2

	InstructionIn[180] = 0x28;
	InstructionIn[181] = 0x41;
	InstructionIn[182] = 0xFF;
	InstructionIn[183] = 0x00;
	//0010 1000 0100 0001 FFFF FFFF 0000 0000
	//SLTI $1, $2, 0xFF00

	InstructionIn[184] = 0x2C;
	InstructionIn[185] = 0x41;
	InstructionIn[186] = 0xFF;
	InstructionIn[187] = 0xFF;
	//0010 1100 0100 0001 FFFF FFFF FFFF FFFF
	//SLTIU $1, $2, 0xFFFF

	for(int i=0; i<4; i++){
		InstructionSet[i] = InstructionIn[num+i];
	}
}

mips_error encodeAndImpl(mips_cpu_h cpu){
	uint32_t ins = 0;
	uint32_t temp = 0;
	uint32_t rs, rt, rd, shift, func, imm, target, offset;
	mips_error err1 = mips_Success;
	for(int i=0; i<4; i++){
		ins = ins << 8;
		ins = ins + cpu->currentIns[i];
		//ins = ins << 8;
	}
	
	temp = ins;
	temp = temp>>26;

	if(temp == 0){ //R type
		rs = ins;
		rs = rs & 0x04E00000;
		rs = rs>>21; // get rs
		
		rt = ins;
		rt = rt & 0x001F0000;
		rt = rt>>16; // get rt

		rd = ins;
		rd = rd & 0x0000F800;
		rd = rd>>11; // get rd

		shift = ins;
		shift = shift & 0x000007C0;
		shift = shift>>6; // get shift

		func = ins;
		func = func & 0x0000003F;
		func = func; // get function

		target = ins;
		target = target & 0x001FFFFF; //get target



		if(func == 32)
			err1 = R_add_impl(cpu, rs, rt, rd, shift, func);
		if(func == 33)
			err1 = R_addu_impl(cpu, rs, rt, rd, shift, func);
		if(func == 36)
			err1 = R_and_impl(cpu, rs, rt, rd, shift, func);
		if(func == 37)
			err1 = R_or_impl(cpu, rs, rt, rd, shift, func);
		if(func == 38)
			err1 = R_xor_impl(cpu, rs, rt, rd, shift, func);
		if(func == 42)
			err1 = R_slt_impl(cpu, rs, rt, rd, shift, func);
		if(func == 43)
			err1 = R_sltu_impl(cpu, rs, rt, rd, shift, func);
		if(func == 0)
			err1 = R_sll_impl(cpu, rs, rt, rd, shift, func);
		if(func == 4)
			err1 = R_sllv_impl(cpu, rs, rt, rd, shift, func);
		if(func == 3)
			err1 = R_sra_impl(cpu, rs, rt, rd, shift, func);
		if(func == 2)
			err1 = R_srl_impl(cpu, rs, rt, rd, shift, func);
		if(func == 6)
			err1 = R_srlv_impl(cpu, rs, rt, rd, shift, func);
		if(func == 34)
			err1 = R_sub_impl(cpu, rs, rt, rd, shift, func);
		if(func == 35)
			err1 = R_subu_impl(cpu, rs, rt, rd, shift, func);
		if(target == 8)
			err1 = JR_impl(cpu, rs);
		if(func == 26)
			err1 = DIV_impl(cpu, rs, rt);
		if(func == 27)
			err1 = DIVU_impl(cpu, rs, rt);
		if(func == 16)
			err1 = MFHI_impl(cpu, rd);
		if(func == 18)
			err1 = MFLO_impl(cpu, rd);
		if(func == 24)
			err1 = MULT_impl(cpu, rs, rt);
		if(func == 25)
			err1 = MULTU_impl(cpu, rs, rt);


	}else{

		rs = ins;
		rs = rs & 0x03E00000;
		rs = rs>>21; // get rs			
			
		rt = ins;
		rt = rt & 0x001F0000;
		rt = rt>>16; // get rt

		imm = ins;
		imm = imm & 0x0000FFFF; //get imm

		target = ins;
		target = target & 0x03FFFFFF; //get target

		offset = ins;
		offset = offset & 0x0000FFFF; //get target

		if(temp == 8) //I type Addi
			err1 = I_addi_impl(cpu, rs, rt, imm);
		if(temp == 9) //I type ADDIU
			err1 = I_addiu_impl(cpu, rs, rt, imm);
		if(temp == 12) //I type ANDI
			err1 = I_andi_impl(cpu, rs, rt, imm);
		if(temp == 13)
			err1 = I_ori_impl(cpu, rs, rt, imm);
		if(temp == 14)
			err1 = I_xori_impl(cpu, rs, rt, imm);
		if(temp == 4)
			err1 = I_beq_impl(cpu, rs, rt, imm);
		if((temp == 1) && (rt == 1))
			err1 = I_bgez_impl(cpu, rs, rt, imm);
		if((temp == 1) && (rt == 17))
			err1 = I_bgezal_impl(cpu, rs, rt, imm);
		if((temp == 7) && (rt == 0))
			err1 = I_bgtz_impl(cpu, rs, rt, imm);
		if((temp == 6) && (rt == 0))
			err1 = I_blez_impl(cpu, rs, rt, imm);
		if((temp == 1) && (rt == 0))
			err1 = I_bltz_impl(cpu, rs, rt, imm);
		if((temp == 1) && (rt == 16))
			err1 = I_bltzal_impl(cpu, rs, rt, imm);
		if(temp == 5)
			err1 = I_bne_impl(cpu, rs, rt, imm);
		if(temp == 2)
			err1 = J_impl(cpu, target);
		if((temp == 0) && (target == 8))
			err1 = JR_impl(cpu, rs);
		if(temp == 3)
			err1 = JAL_impl(cpu, target);
		if(temp == 32)
			err1 = LB_impl(cpu, rs, rt, offset);
		if(temp == 36)
			err1 = LBU_impl(cpu, rs, rt, offset);
		if(temp == 15)
			err1 = LUI_impl(cpu, rt, offset);
		if(temp == 35)
			err1 = LW_impl(cpu, rs, rt, offset);
		if(temp == 40)
			err1 = SB_impl(cpu, rs, rt, offset);
		if(temp == 41)
			err1 = SH_impl(cpu, rs, rt, offset);
		if(temp == 43)
			err1 = SW_impl(cpu, rs, rt, offset);
		if(temp == 34)
			err1 = LWL_impl(cpu, rs, rt, offset);
		if(temp == 38)
			err1 = LWR_impl(cpu, rs, rt, offset);
		if(temp == 10)
			err1 = I_slti_impl(cpu, rs, rt, imm);
		if(temp == 11)
			err1 = I_sltiu_impl(cpu, rs, rt, imm);

	}
	return err1;
}

mips_error R_add_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func){
	uint32_t num1_added = 0; 
	uint32_t num2_added = 0;

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_added);
	if(err2 == 0)
		err2 = mips_cpu_get_register(cpu, rt, &num2_added);
	
	if((num1_added & 0x80000000) == 0x80000000){
		num1_added = ~num1_added + 1;
		num1_added = -num1_added;
	}
	if((num2_added & 0x80000000) == 0x80000000){
		num2_added = ~num2_added + 1;
		num2_added = -num2_added;
	}

	uint32_t num3 = num1_added + num2_added;

	if(err2 == 0){
		if(((num1_added & 0x80000000) == 0x80000000) && ((num2_added & 0x80000000) == 0x80000000)
			&&((num3 & 0x80000000) == 0)){			
				return mips_ExceptionArithmeticOverflow;  //If MSB1 = 1, MSB2 = 1, MSB_result = 0, signed overflowed

		}else if(((num1_added & 0x80000000) == 0) && ((num2_added & 0x80000000) == 0)
			&&((num3 & 0x80000000) == 0x80000000)){		
				return mips_ExceptionArithmeticOverflow;  //If MSB1 = 0, MSB2 = 0, MSB_result = 1, signed overflowed
				
		}else{		
			err2 = mips_cpu_set_register(cpu, rd, num3);
			return mips_Success;
		}
	}
	return mips_ExceptionAccessViolation;	
}

mips_error R_addu_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func){
	uint32_t num1_addued = 0; 
	uint32_t num2_addued = 0;

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_addued);
	if(err2 == 0)
		err2 = mips_cpu_get_register(cpu, rt, &num2_addued);
	
	uint32_t num3 = num1_addued + num2_addued;

	if((num3 - num1_addued) != num1_addued){
		return mips_ErrorInvalidArgument;
	}else{
		if(err2 == 0){
			err2 = mips_cpu_set_register(cpu, rd, num3);
			return mips_Success;	
		}else{
			return mips_ExceptionAccessViolation;
		}
	}
}


mips_error R_and_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func){
	uint32_t num1_anded = 0; 
	uint32_t num2_anded = 0;

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_anded);
	if(err2 == 0)
		err2 = mips_cpu_get_register(cpu, rt, &num2_anded);

	uint32_t num3 = num1_anded & num2_anded;

	if(err2 == 0){
		err2 = mips_cpu_set_register(cpu, rd, num3);
		return mips_Success;
	}
	return mips_ExceptionAccessViolation;
}	

mips_error R_or_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func){
	uint32_t num1_ored = 0; 
	uint32_t num2_ored = 0;

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_ored);
	if(err2 == 0)
		err2 = mips_cpu_get_register(cpu, rt, &num2_ored);

	uint32_t num3 = num1_ored | num2_ored;

	if(err2 == 0){
		err2 = mips_cpu_set_register(cpu, rd, num3);
		return mips_Success;
	}
	return mips_ExceptionAccessViolation;
}	

mips_error R_xor_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func){
	uint32_t num1_xored = 0; 
	uint32_t num2_xored = 0;

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_xored);
	if(err2 == 0)
		err2 = mips_cpu_get_register(cpu, rt, &num2_xored);

	uint32_t num3 = num1_xored ^ num2_xored;

	if(err2 == 0){
		err2 = mips_cpu_set_register(cpu, rd, num3);
		return mips_Success;
	}
	return mips_ExceptionAccessViolation;
}	

mips_error R_slt_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func){
	uint32_t num1_slted = 0; 
	uint32_t num2_slted = 0;
	uint32_t num3 = 0;
	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_slted);
	if(err2 == 0)
		err2 = mips_cpu_get_register(cpu, rt, &num2_slted);

	if(num1_slted == num2_slted)
		return mips_ErrorInvalidArgument;

	if((num1_slted & 0x80000000) == (num2_slted & 0x80000000)){
		if((num2_slted - num1_slted) > 0)
			num3 = 1;
		else
			num3 = 0;
	}else{
		if((num2_slted & 0x80000000) == 0x80000000)
			num3 = 1;
		else
			num3 = 0;
	}


	if(err2 == 0){
		err2 = mips_cpu_set_register(cpu, rd, num3);
		return mips_Success;
	}
	return mips_ExceptionAccessViolation;
}	

mips_error R_sltu_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func){
	uint32_t num1_sltued = 0; 
	uint32_t num2_sltued = 0;
	uint32_t num_less = 0;
	uint32_t num3 = 0;
	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_sltued);
	if(err2 == 0)
		err2 = mips_cpu_get_register(cpu, rt, &num2_sltued);

	num_less = num2_sltued - num1_sltued;

	if(((num_less & 0x80000000) == 0x80000000) || (num_less == 0)){
		num3 = 0;
	}else{
		num3 = 1;
	}

	if(err2 == 0){
		err2 = mips_cpu_set_register(cpu, rd, num3);
		return mips_Success;
	}
	return mips_ExceptionAccessViolation;
}	

mips_error R_sll_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func){
	uint32_t num1_slled = 0; 
	uint32_t num2_slled = 0;
	uint32_t num3 = 0;
	mips_error err2 = mips_cpu_get_register(cpu, rt, &num2_slled);

	if(err2 == 0)
		num3 = num2_slled << shift;	
	if(err2 == 0){
		err2 = mips_cpu_set_register(cpu, rd, num3);
		return mips_Success;	
	}
	return mips_ExceptionAccessViolation;
}

mips_error R_sllv_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func){
	uint32_t num1_sllved = 0; 
	uint32_t num2_sllved = 0;
	uint32_t num3 = 0;

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_sllved);
	if(err2 == 0)
		err2 = mips_cpu_get_register(cpu, rt, &num2_sllved);

	if(err2 == 0)
		num3 = num2_sllved << num1_sllved;
	if(err2 == 0){
		err2 = mips_cpu_set_register(cpu, rd, num3);
		return mips_Success;	
	}
	return mips_ExceptionAccessViolation;
}

mips_error R_sra_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func){
	uint32_t num1_sraed = 0; 
	uint32_t num2_sraed = 0;
	uint32_t num3 = 0;
	mips_error err2 = mips_cpu_get_register(cpu, rt, &num2_sraed);

	if(err2 == 0){
		if((num2_sraed & 0x80000000) == 0x80000000){
			num1_sraed = num2_sraed >> shift;	
			for(unsigned int i=0; i<shift; i++){																		
				num1_sraed = num1_sraed | (1 << ((32-shift)+i));
				num3 = num1_sraed;
			}
		}else{
			num3 = num2_sraed >> shift;
		}
	}

	if(err2 == 0){
		err2 = mips_cpu_set_register(cpu, rd, num3);
		return mips_Success;	
	}
	return mips_ExceptionAccessViolation;
}

mips_error R_srl_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func){
	uint32_t num1_srled = 0; 
	uint32_t num2_srled = 0;
	uint32_t num3 = 0;
	mips_error err2 = mips_cpu_get_register(cpu, rt, &num2_srled);

	if(err2 == 0)
			num3 = num2_srled >> shift;	
	
	if(err2 == 0){
		err2 = mips_cpu_set_register(cpu, rd, num3);
		return mips_Success;	
	}
	return mips_ExceptionAccessViolation;
}

mips_error R_srlv_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func){
	uint32_t num1_srlved = 0; 
	uint32_t num2_srlved = 0;
	uint32_t num3 = 0;

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_srlved);
	if(err2 == 0)
		err2 = mips_cpu_get_register(cpu, rt, &num2_srlved);

	if(err2 == 0)
		num3 = num2_srlved >> num1_srlved;
	if(err2 == 0){
		err2 = mips_cpu_set_register(cpu, rd, num3);
		return mips_Success;	
	}
	return mips_ExceptionAccessViolation;
}

mips_error R_sub_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func){
	uint32_t num1_subed = 0; 
	uint32_t num2_subed = 0;

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_subed);
	if(err2 == 0)
		err2 = mips_cpu_get_register(cpu, rt, &num2_subed);
	
	if((num1_subed & 0x80000000) == 0x80000000){
		num1_subed = ~num1_subed + 1;
		num1_subed = -num1_subed;
	}
	if((num2_subed & 0x80000000) == 0x80000000){
		num2_subed = ~num2_subed + 1;
		num2_subed = -num2_subed;
	}

	uint32_t num3 = num1_subed - num2_subed;

	if(err2 == 0){
		err2 = mips_cpu_set_register(cpu, rd, num3);
		return mips_Success;
	}
	return mips_ExceptionAccessViolation;
}

mips_error R_subu_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func){
	uint32_t num1_subued = 0; 
	uint32_t num2_subued = 0;

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_subued);
	if(err2 == 0)
		err2 = mips_cpu_get_register(cpu, rt, &num2_subued);

	uint32_t num3 = num1_subued - num2_subued;

	if((num3 + num2_subued) != num1_subued){
		return mips_ErrorInvalidArgument;
	}else{
		if(err2 == 0){
			err2 = mips_cpu_set_register(cpu, rd, num3);
			return mips_Success;
		}else{
			return mips_ExceptionAccessViolation;
		}
	}
}

mips_error I_addi_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm){
	uint32_t num1_addied = 0; 
	uint32_t num3 = 0;

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_addied);

	if((num1_addied & 0x80000000) == 0x80000000){
		num1_addied = ~num1_addied + 1;
		num1_addied = -num1_addied;
	}
	if((imm & 0x80000000) == 0x80000000){
		imm = ~imm + 1;
		imm = -imm;
	}

	num3 = num1_addied + imm;

	if(err2 == 0){
		if(((num1_addied & 0x80000000) == 0x80000000) && ((imm & 0x80000000) == 0x80000000)
		&&((num3 & 0x80000000) == 0)){			
			return mips_ExceptionArithmeticOverflow;  //If MSB1 = 1, MSB2 = 1, MSB_result = 0, signed overflowed

		}else if(((num1_addied & 0x80000000) == 0) && ((imm & 0x80000000) == 0)
			&&((num3 & 0x80000000) == 0x80000000)){		
				return mips_ExceptionArithmeticOverflow;  //If MSB1 = 0, MSB2 = 0, MSB_result = 1, signed overflowed
		
		}else{
			err2 = mips_cpu_set_register(cpu, rt, num3);
			return mips_Success;
		}
	}else{
		return mips_ExceptionAccessViolation;
	}
}

mips_error I_addiu_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm){
	uint32_t num1_addiued = 0; 
	uint32_t num3 = 0;

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_addiued);

	num3 = num1_addiued + imm;

	if((num3 - num1_addiued) != imm){
		return mips_ErrorInvalidArgument;
	}else{

		if(err2 == 0){
			err2 = mips_cpu_set_register(cpu, rt, num3);
			return mips_Success;
		}else{
			return mips_ExceptionAccessViolation;
		}
	}
}

mips_error I_andi_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm){
	uint32_t num1_andied = 0; 

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_andied);

	uint32_t num3 = num1_andied & imm;

	if(err2 == 0){
		err2 = mips_cpu_set_register(cpu, rt, num3);
		return mips_Success;
	}
	return mips_ExceptionAccessViolation;
}

mips_error I_ori_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm){
	uint32_t num1_oried = 0; 

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_oried);

	uint32_t num3 = num1_oried | imm;

	if(err2 == 0){
		err2 = mips_cpu_set_register(cpu, rt, num3);
		return mips_Success;
	}
	return mips_ExceptionAccessViolation;
}

mips_error I_xori_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm){
	uint32_t num1_xoried = 0; 

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_xoried);

	uint32_t num3 = num1_xoried ^ imm;

	if(err2 == 0)
		err2 = mips_cpu_set_register(cpu, rt, num3);
	if(err2 == 0)
		return mips_Success;

	return mips_ExceptionAccessViolation;
}

mips_error I_beq_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm){
	uint32_t num1_beqed = 0; 
	uint32_t num2_beqed = 0;
	uint8_t followingIns[4];

	for(int i=0; i<4; i++){
		followingIns[i] = 0;
	}

	imm = imm << 2;		//imm<<2

	mips_error err_tobe_exe = mips_mem_read(cpu->mem, cpu->pc, 4, followingIns);
	if(err_tobe_exe == 0){	
		for(int i=0; i<4; i++){
			cpu->currentIns[i] = followingIns[i];		//Assume that the next instruction is
														//AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
		} 
		err_tobe_exe = encodeAndImpl(cpu);	//Implements the AND instruction
	}

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_beqed);
	if(err2 == 0)
		err2 = mips_cpu_get_register(cpu, rt, &num2_beqed);
	if(err2 == 0){
		if(num1_beqed == num2_beqed){		//If $s == $t, cpu-> imm<<2
											//else advance pc by 4. Which has been done within mips_cpu_step(); 
			cpu->pc = imm;
			return mips_Success;
		}else{
			return mips_Success;
		}

	}
	
	return mips_ExceptionAccessViolation;
	
}

mips_error I_bgez_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm){
	uint32_t num1_bgezed = 0; 
	uint8_t followingIns[4];

	for(int i=0; i<4; i++){
		followingIns[i] = 0;
	}

	imm = imm << 2;		//imm<<2

	mips_error err_tobe_exe = mips_mem_read(cpu->mem, cpu->pc, 4, followingIns);
	if(err_tobe_exe == 0){	
		for(int i=0; i<4; i++){
			cpu->currentIns[i] = followingIns[i];		//Assume that the next instruction is
														//AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
		} 
		err_tobe_exe = encodeAndImpl(cpu);		//Implements the AND instruction
	}

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_bgezed);
	if(err2 == 0){
		if(((num1_bgezed & 0x80000000) == 0) || (num1_bgezed == 0)){	//If $s >= 0, cpu-> imm<<2
																		//else advance pc by 4. Which has been done within mips_cpu_step(); 
			cpu->pc =imm;
			return mips_Success;
		}else{
			return mips_Success;
		}
	}else{
		return mips_ExceptionAccessViolation;
	}
}

mips_error I_bgezal_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm){
	uint32_t num1_bgezaled = 0;
	uint8_t followingIns[4];

	for(int i=0; i<4; i++){
		followingIns[i] = 0;
	}

	imm = imm << 2;		//imm<<2

	mips_error err_tobe_exe = mips_mem_read(cpu->mem, cpu->pc, 4, followingIns);
	if(err_tobe_exe == 0){	
		for(int i=0; i<4; i++){
			cpu->currentIns[i] = followingIns[i];		//Assume that the next instruction is
														//AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
		} 
		err_tobe_exe = encodeAndImpl(cpu);		//Implements the AND instruction
	}

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_bgezaled);
	if(err2 == 0){
		if(((num1_bgezaled & 0x80000000) == 0) || (num1_bgezaled == 0)){
				err2 = mips_cpu_set_register(cpu, 31, (cpu->pc+4)); //PC value has been advanced by 4 within function mips_cpu_step 
				cpu->pc = imm;
				return mips_Success;
		}else{
			return mips_Success;
		}
	}else{
		return mips_ExceptionAccessViolation;
	}
}

mips_error I_bgtz_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm){
	uint32_t num1_bgtzed = 0; 
	uint8_t followingIns[4];

	for(int i=0; i<4; i++){
		followingIns[i] = 0;
	}

	imm = imm << 2;		//imm<<2

	mips_error err_tobe_exe = mips_mem_read(cpu->mem, cpu->pc, 4, followingIns);
	if(err_tobe_exe == 0){	
		for(int i=0; i<4; i++){
			cpu->currentIns[i] = followingIns[i];		//Assume that the next instruction is
														//AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
		} 
		err_tobe_exe = encodeAndImpl(cpu);		//Implements the AND instruction
	}

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1_bgtzed);
	if(err2 == 0){
		if(((num1_bgtzed & 0x80000000) == 0)){
			cpu->pc = imm;
			return mips_Success;
		}else{
			return mips_Success;	
		}
	}else{
		return mips_ExceptionAccessViolation;
	}
}

mips_error I_blez_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm){
	uint32_t num1 = 0; 
	uint8_t followingIns[4];

	for(int i=0; i<4; i++){
		followingIns[i] = 0;
	}

	imm = imm << 2;		//imm<<2

	mips_error err_tobe_exe = mips_mem_read(cpu->mem, cpu->pc, 4, followingIns);
	if(err_tobe_exe == 0){	
		for(int i=0; i<4; i++){
			cpu->currentIns[i] = followingIns[i];		//Assume that the next instruction is
														//AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
		} 
		err_tobe_exe = encodeAndImpl(cpu);		//Implements the AND instruction
	}

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1);
	if(err2 == 0){
		if(((num1 & 0x80000000) == 0x80000000) || (num1 == 0)){
			cpu->pc = imm;
			return mips_Success;
		}else{
			return mips_Success;
		}
	}else{
		return mips_ExceptionAccessViolation;
	}
}

mips_error I_bltz_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm){
	uint32_t num1 = 0; 
	uint8_t followingIns[4];

	for(int i=0; i<4; i++){
		followingIns[i] = 0;
	}

	imm = imm << 2;		//imm<<2

	mips_error err_tobe_exe = mips_mem_read(cpu->mem, cpu->pc, 4, followingIns);
	if(err_tobe_exe == 0){	
		for(int i=0; i<4; i++){
			cpu->currentIns[i] = followingIns[i];		//Assume that the next instruction is
														//AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
		} 
		err_tobe_exe = encodeAndImpl(cpu);		//Implements the AND instruction
	}

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1);
	if(err2 == 0){
		if(((num1 & 0x80000000) == 0x80000000)){
			cpu->pc = imm;
			return mips_Success;
		}else{
			return mips_Success;
		}
	}else{
		return mips_ExceptionAccessViolation;
	}
}

mips_error I_bltzal_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm){
	uint32_t num1 = 0; 
	uint8_t followingIns[4];

	for(int i=0; i<4; i++){
		followingIns[i] = 0;
	}

	imm = imm << 2;		//imm<<2

	mips_error err_tobe_exe = mips_mem_read(cpu->mem, cpu->pc, 4, followingIns);
	if(err_tobe_exe == 0){	
		for(int i=0; i<4; i++){
			cpu->currentIns[i] = followingIns[i];		//Assume that the next instruction is
														//AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
		} 
		err_tobe_exe = encodeAndImpl(cpu);		//Implements the AND instruction
	}

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1);
	if(err2 == 0){
		if(((num1 & 0x80000000) == 0x80000000)){
			err2 = mips_cpu_set_register(cpu, 31, cpu->pc); //PC value has been updated within function cpu_step 
			cpu->pc = imm;
			return mips_Success;
		}else{
			return mips_Success;
		}
	}else{
		return mips_ExceptionAccessViolation;
	}
}

mips_error I_bne_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm){
	uint32_t num1 = 0; 
	uint32_t num2 = 0;
	uint8_t followingIns[4];

	for(int i=0; i<4; i++){
		followingIns[i] = 0;
	}

	imm = imm << 2;		//imm<<2

	mips_error err_tobe_exe = mips_mem_read(cpu->mem, cpu->pc, 4, followingIns);
	if(err_tobe_exe == 0){	
		for(int i=0; i<4; i++){
			cpu->currentIns[i] = followingIns[i];		//Assume that the next instruction is
														//AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
		} 
		err_tobe_exe = encodeAndImpl(cpu);		//Implements the AND instruction
	}

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1);
	if(err2 == 0)
		err2 = mips_cpu_get_register(cpu, rt, &num2);
	if(err2 == 0){	
		if(num1 != num2){
			cpu->pc = imm;
			return mips_Success;
		}else{
			return mips_Success;
		}
	}
	return mips_ExceptionInvalidAlignment;
}

mips_error J_impl(mips_cpu_h cpu, uint32_t target){
	uint8_t followingIns[4];

	for(int i=0; i<4; i++){
		followingIns[i] = 0;
	}

	target = target << 2;		//target<<2

	mips_error err_tobe_exe = mips_mem_read(cpu->mem, cpu->pc, 4, followingIns);
	if(err_tobe_exe == 0){	
		for(int i=0; i<4; i++){
			cpu->currentIns[i] = followingIns[i];		//Assume that the next instruction is
														//AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
		} 
		err_tobe_exe = encodeAndImpl(cpu);		//Implements the AND instruction
	}

	cpu->pc = ((cpu->pc-4) & 0xF0000000) | (target);
	return mips_Success;
}

mips_error JR_impl(mips_cpu_h cpu, uint32_t rs){
	uint32_t num1;
	uint8_t followingIns[4];

	for(int i=0; i<4; i++){
		followingIns[i] = 0;
	}

	mips_error err_tobe_exe = mips_mem_read(cpu->mem, cpu->pc, 4, followingIns);
	if(err_tobe_exe == 0){	
		for(int i=0; i<4; i++){
			cpu->currentIns[i] = followingIns[i];		//Assume that the next instruction is
														//AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
		} 
		err_tobe_exe = encodeAndImpl(cpu);		//Implements the AND instruction
	}

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1);
	if(err2 == 0){
		cpu->pc = num1;
		return mips_Success;
	}
	return mips_ExceptionAccessViolation;
}

mips_error JAL_impl(mips_cpu_h cpu, uint32_t target){
	uint32_t got = 0;
	uint8_t followingIns[4];

	for(int i=0; i<4; i++){
		followingIns[i] = 0;
	}

	target = target << 2;		//target<<2

	mips_error err_tobe_exe = mips_mem_read(cpu->mem, cpu->pc, 4, followingIns);
	if(err_tobe_exe == 0){	
		for(int i=0; i<4; i++){
			cpu->currentIns[i] = followingIns[i];		//Assume that the next instruction is
														//AND $3, $3, $4 (0000 0000 0110 0100 0001 1000 0010 0100) 
		} 
		err_tobe_exe = encodeAndImpl(cpu);		//Implements the AND instruction
	}

	mips_error err2 = mips_cpu_set_register(cpu, 31, cpu->pc+4); //PC value has been advanced by 4 within function mips_cpu_step() 
	
	if(err2 == 0){
		cpu->pc = ((cpu->pc-4) & 0xF0000000) | target;
		return mips_Success;
	}else{
		return mips_ExceptionAccessViolation;
	}
}

mips_error LB_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t offset){
	uint32_t num1 = 0;
	uint32_t num2 = 0;
	uint32_t wd_address = 0;
	uint32_t bt_address = 0;
	uint8_t num3[4];
	uint32_t byte = 0;
	//mips_error err2 = mips_cpu_get_register(cpu, rs, &num1);
	
	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1);
	
	if(err2 == 0){
		if((offset & 0x00008000) == 0x00008000){
				offset = offset | 0xFFFF0000;
				offset = offset;
			}
		
		num2 = num1 + offset;

		if((num2 & 0x80000000) == 0){
			wd_address = num2 - (num2 % 4);
			bt_address = num2 % 4;
		}else{
			return mips_ErrorInvalidArgument;
		}
	}
	err2 = mips_mem_read(cpu->mem, wd_address, 4, num3);	
	
	if(err2 == 0){
		byte = num3[bt_address];
		err2 = mips_cpu_set_register(cpu, rt, byte);
		if(err2 == 0)
			return mips_Success;
		else
			return mips_ExceptionAccessViolation;
	}
	return mips_ExceptionAccessViolation;
}

mips_error LBU_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t offset){;
	uint32_t num1 = 0;
	uint32_t num2 = 0;
	uint32_t wd_address;
	uint32_t bt_address;
	uint8_t num3[4];
	uint32_t byte;
	for(int i=0; i<4; i++){
		num3[i] = 0;
	}

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1);

	if(err2 == 0){
		num2 = num1 + offset;
		wd_address = num2 - (num2 % 4);
		bt_address = num2 % 4;
		err2 = mips_mem_read(cpu->mem, wd_address, 4, num3);
	}

	if(err2 == 0){
		byte = num3[bt_address];
		err2 = mips_cpu_set_register(cpu, rt, byte);
		if(err2 == 0)
			return mips_Success;
		else
			return mips_ExceptionAccessViolation;
	}
	return mips_ExceptionAccessViolation;
}

mips_error LUI_impl(mips_cpu_h cpu, uint32_t rt, uint32_t offset){
	uint32_t num1 = offset << 16;
	mips_error err2 = mips_cpu_set_register(cpu, rt, num1);
	if(err2 == 0)
		return mips_Success;
	else
		return mips_ExceptionAccessViolation;
	
}

mips_error LW_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t offset){
	uint32_t num1;
	uint32_t num2;
	uint32_t word = 0;
	uint8_t num3[4];
	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1);

	if(err2 == 0){
		num2 = num1 + offset;

		if((num2 % 4) == 0){
			err2 = mips_mem_read(cpu->mem, num2, 4, num3);
			for(int i=0; i<4; i++){
				word = word << 8;
				word = word | num3[i];	
			}
		}else{
			return mips_ErrorInvalidArgument;
		}
	}

	if(err2 == 0){
		err2 = mips_cpu_set_register(cpu, rt, word);
		return mips_Success;
	}
	return mips_ExceptionAccessViolation;	
}

mips_error SB_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t offset){
	uint32_t num1 = 0; 
	uint32_t num2 = 0;
	uint8_t num3 = 0;
	uint8_t num4[4];
	uint32_t wd_address = 0;
	uint32_t bt_address = 0;
	for(int i=0; i<4; i++){
		num4[i] = 0;
	}

	mips_error err2 = mips_cpu_get_register(cpu, rt, &num1);
	if(err2 == 0){
		err2 = mips_cpu_get_register(cpu, rs, &num2);
		num1= num1 & 0xFF;
		num3 = num1;
	}
	if(err2 == 0){
		num2 = num2 + offset;
		wd_address = num2 - (num2 % 4);
		bt_address = num2 % 4;

		err2 = mips_mem_read(cpu->mem, wd_address, 4, num4); //Read the content from the memory,
														     //change the spcific byte and store it back
		if(err2 == 0){
			num4[bt_address] = num3; 
			err2 = mips_mem_write(cpu->mem, wd_address, 4, num4);
		}
	}
	if(err2 == 0)
		return mips_Success;
	else
		return mips_ExceptionAccessViolation;
}

mips_error SH_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t offset){
	uint32_t num1 = 0; 
	uint32_t num2 = 0; 
	uint8_t num3[4];
	for(int i=0; i<4; i++){
		num3[i] = 0;
	}

	mips_error err2 = mips_cpu_get_register(cpu, rt, &num1);
	if(err2 == 0)
		err2 = mips_cpu_get_register(cpu, 2, &num2);

	if(err2 == 0){
		num3[3] = num1 & 0xFF;
		num3[2] = (num1 & 0xFF00) >> 8;
	}
	if(err2 == 0){
		num2 = num2 + offset;
		if((num2 % 4) == 0){
			err2 = mips_mem_write(cpu->mem, num2, 4, num3);
			return mips_Success;
		}else{
			return mips_ErrorInvalidArgument;
		}
	}
	return mips_ExceptionAccessViolation;
}

mips_error SW_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t offset){
	uint32_t num1 = 0; 
	uint32_t num2 = 0; 
	uint8_t num3[4];
	for(int i=0; i<4; i++){
		num3[i] = 0;
	}

	mips_error err2 = mips_cpu_get_register(cpu, rt, &num1);
	if(err2 == 0)
		err2 = mips_cpu_get_register(cpu, rs, &num2);
	if(err2 == 0){
		num3[3] = num1 & 0xFF;
		num3[2] = (num1 & 0xFF00) >> 8;
		num3[1] = (num1 & 0xFF0000) >> 16; 
		num3[0] = (num1 & 0xFF000000) >> 24;
	}
	if(err2 == 0){
		num2 = num2 + offset;
		if((num2 % 4) == 0){
			err2 = mips_mem_write(cpu->mem, num2, 4, num3);
			return mips_Success;
		}else{
			return mips_ErrorInvalidArgument;
		}
	}
	return mips_ExceptionAccessViolation;
}

mips_error DIV_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt){
	uint32_t num1 = 0;
	uint32_t num2 = 0;
	uint32_t num1_temp = 0;
	uint32_t num2_temp = 0;

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1);
	
	if(err2 == 0){
		err2 = mips_cpu_get_register(cpu, rt, &num2);

		if((num1 & 0x80000000) == 0x80000000){
			num1_temp = ~num1 + 1;
		}
		if((num2 & 0x80000000) == 0x80000000){
			num2_temp = ~num2 + 1;
		}
	}

	if(err2 == 0){
		if(((num1 & 0x80000000) == 0x80000000) && ((num2 & 0x80000000) == 0x80000000)){
			cpu->LO = num1_temp / num2_temp;
			cpu->HI = num1_temp % num2_temp;
			cpu->HI = -cpu->HI;
			return mips_Success;
		}else if((((num1 & 0x80000000) == 0x80000000) && ((num2 & 0x80000000) == 0))){
			cpu->LO = num1_temp / num2;
			cpu->LO = -cpu->LO;
			cpu->HI = num1_temp % num2;
			cpu->HI = -cpu->HI;
			return mips_Success;
		}else if((((num1 & 0x80000000) == 0) && ((num2 & 0x80000000) == 0x80000000))){
			cpu->LO = num1 / num2_temp;
			cpu->LO = -cpu->LO;
			cpu->HI = num1 % num2_temp;
			cpu->HI = cpu->HI;
			return mips_Success;
		}else{
			cpu->LO = num1 / num2;
			cpu->HI = num1 % num2;
			return mips_Success;
		}

	}
	return mips_ExceptionAccessViolation;

}

mips_error DIVU_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt){
	uint32_t num1 = 0;
	uint32_t num2 = 0;

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1);
	if(err2 == 0)
		err2 = mips_cpu_get_register(cpu, rt, &num2);
	
	if(err2 == 0){
		cpu->LO = num1 / num2;
		cpu->HI = num1 % num2;
		return mips_Success;
	}
	return mips_ExceptionAccessViolation;
}

mips_error MFHI_impl(mips_cpu_h cpu, uint32_t rd){
	uint32_t num1 = cpu->HI;
	mips_error err2 = mips_cpu_set_register(cpu, rd, num1);
	if(err2 == 0)
		return mips_Success;
	else
		return mips_ExceptionAccessViolation;
}

mips_error MFLO_impl(mips_cpu_h cpu, uint32_t rd){
	uint32_t num1 = cpu->LO;
	mips_error err2 = mips_cpu_set_register(cpu, rd, num1);
	if(err2 == 0)
		return mips_Success;
	else
		return mips_ExceptionAccessViolation;
}

mips_error LWL_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t offset){
	uint32_t num1 = 0; 
	uint8_t num2[4];
	uint32_t num3 = 0;
	uint32_t sum = 0;
	for(int i=0; i<4; i++){
		num2[i] = 0;
	}

	//Cases of that offset could be positive or negative
	if((offset & 0x00008000) == 0x00008000){
		offset = offset | 0xFFFF0000;
		offset = ~offset + 1;
		sum = rs - offset;
	}else{
		sum = rs + offset;
	}

	mips_error err2 = mips_cpu_get_register(cpu, rt, &num1);
	if(err2 == 0){
		if((sum % 4) == 0){
			err2 = mips_mem_read(cpu->mem, sum, 4, num2);
		}else{
			return mips_ErrorInvalidArgument;
		}
	}
	if(err2 == 0){
		num1 = num1 & 0x0000FFFF;
		
		num3 = num3 | num2[0];
		num3 = num3 << 8;

		num3 = num3 | num2[1];
		num3 = num3 << 16;

		num1 = num1 | num3;

		err2 = mips_cpu_set_register(cpu, rt, num1);
	}

	if(err2 == 0){
		return mips_Success;
	}else{
		return mips_ErrorInvalidArgument;
	}
	return mips_ExceptionAccessViolation;
}

mips_error LWR_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t offset){
	uint32_t num1 = 0; 
	uint8_t num2[4];
	uint32_t num3 = 0;
	uint32_t sum = 0;
	for(int i=0; i<4; i++){
		num2[i] = 0;
	}

	//Cases of that offset could be positive or negative
	if((offset & 0x00008000) == 0x00008000){
		offset = offset | 0xFFFF0000;
		offset = ~offset + 1;
		sum = rs - offset;
	}else{
		sum = rs + offset;
	}

	mips_error err2 = mips_cpu_get_register(cpu, rt, &num1);
	if(err2 == 0){
		if((sum % 4) == 0){
			err2 = mips_mem_read(cpu->mem, sum, 4, num2);
		}else{
			return mips_ErrorInvalidArgument;
		}
	}
	if(err2 == 0){
		num1 = num1 & 0xFFFF0000;
		
		num3 = num3 | num2[2];
		num3 = num3 << 8;

		num3 = num3 | num2[3];

		num1 = num1 | num3;

		err2 = mips_cpu_set_register(cpu, rt, num1);
	}

	if(err2 == 0){
		return mips_Success;
	}else{
		return mips_ExceptionAccessViolation;
	}
}

mips_error MULT_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt){
	uint32_t num1 = 0;
	uint32_t num2 = 0;
	uint32_t num1_temp = 0;
	uint32_t num2_temp = 0;

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1);
	if(err2 == 0){
		err2 = mips_cpu_get_register(cpu, rt, &num2);

		if((num1 & 0x80000000) == 0x80000000){
			num1_temp = ~num1 + 1;
		}
		if((num2 & 0x80000000) == 0x80000000){
			num2_temp = ~num2 + 1;
		}
	}

	if(err2 == 0){
		if(((num1 & 0x80000000) == 0x80000000) && ((num2 & 0x80000000) == 0x80000000)){
			cpu->LO = num1_temp * num2_temp;
			if((cpu->LO / num1_temp) != num2_temp){
				return mips_ErrorInvalidArgument;
			}else{
				cpu->LO = cpu->LO;
				return mips_Success;
			}			
		}else if((((num1 & 0x80000000) == 0x80000000) && ((num2 & 0x80000000) == 0))){
			cpu->LO = num1_temp * num2;
			if((cpu->LO / num1_temp) != num2){
				return mips_ErrorInvalidArgument;
			}else{
				cpu->LO = -cpu->LO;
				return mips_Success;
			}
		}else if((((num1 & 0x80000000) == 0) && ((num2 & 0x80000000) == 0x80000000))){
			cpu->LO = num1 * num2_temp;
			if((cpu->LO / num1) != num2_temp){
				return mips_ErrorInvalidArgument;
			}else{
				cpu->LO = -cpu->LO;
				return mips_Success;
			}			
		}else{
			cpu->LO = num1 * num2;
			if((cpu->LO / num1) != num2){
				return mips_ErrorInvalidArgument;
			}else{
				return mips_Success;
			}			
		}

	}

	return mips_ExceptionAccessViolation;	
}

mips_error MULTU_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt){
	uint32_t num1 = 0;
	uint32_t num2 = 0;

	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1);
	if(err2 == 0){
		err2 = mips_cpu_get_register(cpu, rt, &num2);
	}
	if(err2 == 0){
		cpu->LO = num1 * num2;
		if((cpu->LO / num1) != num2){
			return mips_ErrorInvalidArgument;
		}else{
			return mips_Success;
		}
	}else{
		return mips_ExceptionAccessViolation;
	}

	
}

mips_error I_slti_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm){
	uint32_t num1= 0; 
	uint32_t num2= 0;
	uint32_t num3 = 0;
	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1);

	if(num1 == imm)
		return mips_ExceptionInvalidAlignment;
	
	if((imm & 0x00008000) == 0x00008000){
		imm = imm | 0xFFFF0000;
		imm = ~imm + 1;
	}
	if((num1 & 0x80000000) == 0x80000000){
		num1 = ~num1 + 1;
	}

	if(((num1 & 0x80000000) == 0x80000000) && ((imm & 0x00008000) == 0x00008000)){
		if(imm < num1)
			num3 = 1;
		else
			num3 = 0;
	}else if((((num1 & 0x80000000) == 0x80000000) && ((imm & 0x00008000) == 0))){
		num3 = 1;
	}else if((((num1 & 0x80000000) == 0) && ((imm & 0x00008000) == 0x00008000))){
		num3 = 0;
	}else{
		if(imm > num1){
			num3 = 1;
		}else{
			num3 = 0;
		}
	}

	if(err2 == 0){
		err2 = mips_cpu_set_register(cpu, rt, num3);
		return mips_Success;
	}
	return mips_ExceptionAccessViolation;
}	

mips_error I_sltiu_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm){
	uint32_t num1= 0; 
	uint32_t num2= 0;
	mips_error err2 = mips_cpu_get_register(cpu, rs, &num1);

	if(num1 == imm)
		return mips_ExceptionInvalidAlignment;
	
	if(imm > num1){
		num2 = 1;
	}else{
		num2 = 0;
	}

	if(err2 == 0){
		err2 = mips_cpu_set_register(cpu, rt, num2);
		return mips_Success;
	}
	return mips_ExceptionAccessViolation;
}	
