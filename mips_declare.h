//The head file is for the functions and structures defined by myself


#ifndef mips_declare_header
#define mips_declare_header

#include "mips_mem.h"
#include "mips_cpu.h"
#include "mips_test.h"



struct mips_cpu_impl{
	uint32_t pc, pcN;
	uint32_t regs[32];
	mips_mem_h mem;
	uint8_t currentIns[10];
	uint32_t HI;
	uint32_t LO;
};

void get_instruction(uint8_t *InstructionSet, int num);

mips_error set_CurInstruction(uint8_t *CurrentIns, mips_cpu_h cpu);

//Function distinguishes diverse instructioons and implement it
mips_error encodeAndImpl(mips_cpu_h cpu);

mips_cpu_h set_cpu(mips_cpu_h cpu);

//Functions implement the R type instructions
//R Add
mips_error R_add_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func);
//R AddU
mips_error R_addu_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func);
//R And
mips_error R_and_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func);
//R Or
mips_error R_or_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func);
//R Xor
mips_error R_xor_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func);
//R Slt
mips_error R_slt_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func);
//R Sltu
mips_error R_sltu_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func);
//R Sll
mips_error R_sll_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func);
//R Sllv
mips_error R_sllv_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func);
//R Sra
mips_error R_sra_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func);
//R Srl
mips_error R_srl_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func);
//R Srlv
mips_error R_srlv_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func);
//R Sub
mips_error R_sub_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func);
//R Subu
mips_error R_subu_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t shift, uint32_t func);
//I addi
mips_error I_addi_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm);
//I addiu
mips_error I_addiu_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm);
//I andi
mips_error I_andi_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm);
//I ori
mips_error I_ori_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm);
//I xori
mips_error I_xori_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm);
//I beq
mips_error I_beq_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm);
//I bgez
mips_error I_bgez_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm);
//I bgezal
mips_error I_bgezal_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm);
//I bgtz
mips_error I_bgtz_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm);
//I blez
mips_error I_blez_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm);
//I bltz
mips_error I_bltz_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm);
//I bltzal
mips_error I_bltzal_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm);
//I bne
mips_error I_bne_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm);
//J jump
mips_error J_impl(mips_cpu_h cpu, uint32_t target);
//JR jump
mips_error JR_impl(mips_cpu_h cpu, uint32_t rs);
//JAL jump
mips_error JAL_impl(mips_cpu_h cpu, uint32_t target);
//LB 
mips_error LB_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t offset);
//LBU 
mips_error LBU_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t offset);
//LUI
mips_error LUI_impl(mips_cpu_h cpu, uint32_t rt, uint32_t offset);
//LW 
mips_error LW_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t offset);
//SB 
mips_error SB_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t offset);
//SH
mips_error SH_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t offset);
//SW
mips_error SW_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t offset);
//DIV
mips_error DIV_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt);
//DIVU
mips_error DIVU_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt);
//MFHI
mips_error MFHI_impl(mips_cpu_h cpu, uint32_t rd);
//MFLO
mips_error MFLO_impl(mips_cpu_h cpu, uint32_t rd);
//LWL
mips_error LWL_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t offset);
//LWR
mips_error LWR_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t offset);
//MULT
mips_error MULT_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt);
//MULTU
mips_error MULTU_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt);
//SLTI
mips_error I_slti_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm);
//SLTIU
mips_error I_sltiu_impl(mips_cpu_h cpu, uint32_t rs, uint32_t rt, uint32_t imm);
#endif