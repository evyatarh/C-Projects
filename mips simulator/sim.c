/***********************************************/
/*Course: Architecture
/**********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "shell.h"
#include "opcodes.h"

typedef enum {J_TYPE,I_TYPE,R_TYPE} Type;
typedef enum {ADD,SUB,OR,AND,XOR,NOR,SHRIGHTL,SHLEFTL,SHRIGHTA} operation;
typedef enum {BYTE_SIZE,HALF_WORD_SIZE,WORD_SIZE}  SIZE;
typedef enum {MASK_2BITS=0x2,MASK_BYTE=0xFF,MASK_HALF_WORD=0xFFFF, MASK_WORD=0xFFFFFFFF}  MASK_SIZE;



#define MASK_HALFWORD 0x0000FFFF
#define MASK_BYTE 0x000000FF
#define IP_SEGMENT 0xF0000000

//main
void runSim();

//functions according to type & action
uint32_t JType(uint32_t opcode,uint32_t imm26);
uint32_t RType(uint32_t _rs,uint32_t _rt,uint32_t _rd,uint32_t shamt,uint32_t funct);
uint32_t IType(uint32_t opcode,uint32_t rs,uint32_t rt,uint32_t imm16, uint32_t instruction);
uint32_t oneReg_IType(uint32_t action, uint32_t rs, uint32_t imm16);
uint32_t twoReg_IType(uint32_t opcode, uint32_t rs,uint32_t rt,uint32_t imm16);
void writeToMemory(uint32_t address, SIZE wordSize,int32_t retReg);
uint32_t checkLessThen(uint32_t num1, uint32_t num2, bool signBit);//change name
uint32_t ALU(bool signBit,operation op,uint32_t num1, uint32_t num2,uint32_t shamt);
void handleException(const char* message);
//helpers functions
Type opcodeAction(uint32_t opcode);
uint32_t get_unsigned_uint32(uint32_t imm16,SIZE size);
int32_t  get_signed_int32(uint32_t imm16,SIZE size);
void readFromMemory(uint32_t address,SIZE wordSize, bool signBit, uint32_t retReg);

//-----------------------------------------------//

void process_instruction(){
  runSim();
}


/***********************runSim function*********************/
void runSim(){

  uint32_t instruction, opcode, rs, rt, rd, shamt, funct, imm16, imm26;
  uint32_t ret = 4;
  Type type;

  instruction = mem_read_32(curr_state.ip);
  opcode = instruction >> SHIFT_OPCODE;
  type = opcodeAction(opcode);
  rs = (instruction & MASK_RS) >> SHIFT_RS;
  rt = (instruction & MASK_RT) >> SHIFT_RT;
  //printf("rs = %d rt = %d ",rs,rt);
  switch (type) {
    case J_TYPE:
  //  printf("J\n");
      imm26 = (instruction & MASK_IMM26) >> SHIFT_IMM26;
      ret = JType(opcode,imm26);
      break;
    case I_TYPE:
  //  printf("I\n");
      imm16 = (instruction & MASK_IMM16) >> SHIFT_IMM16;
      ret = IType(opcode,rs,rt,imm16,instruction);
      break;
    case R_TYPE:
      rd = (instruction & MASK_RD) >> SHIFT_RD;
    //  printf("R rd = %d\n",rd);
      shamt = (instruction & MASK_SHAMT) >> SHIFT_SHAMT;
      funct = (instruction & MASK_FUNCT) >> SHIFT_FUNCT;
      ret = RType(rs,rt,rd,shamt,funct);
      break;
    default:
      printf("Big Error !!!!!\n");
      exit(0);
  }


  next_state.ip = curr_state.ip + ret;
//  printf("next_state.ip = %x\n",next_state.ip);
}


/***********************JType function*********************/
uint32_t JType(uint32_t opcode,uint32_t imm26){
  uint32_t nextIP = 4;
  uint32_t address = ((imm26 << 2) | (curr_state.ip & IP_SEGMENT));

  if(opcode == OPCODE_J){
    if(address & 0x0003){
      handleException("OPCODE_J: Invalid address in operation jump; The two low order bits are not zero.\n");
      return nextIP;
    }
  }

  if(opcode == OPCODE_JAL){
    if(address & 0x0003){
      handleException("OPCODE_JAL: Invalid address in operation jump; The two low order bits are not zero.\n");
      return nextIP;
    }
    next_state.regs[$ra] = curr_state.ip + 4;
  }

  curr_state.ip = address;
  return 0;
}


/***********************RType function*********************/
uint32_t RType(uint32_t _rs,uint32_t _rt,uint32_t _rd,uint32_t shamt,uint32_t funct){
  uint32_t rs = curr_state.regs[_rs];
  uint32_t rt = curr_state.regs[_rt];
  uint32_t rd = curr_state.regs[_rd];
  uint32_t ret,nextIp = 4;

  switch(funct){
    case SPECIAL_ADD:
      ret = ALU(true,ADD,rs,rt,0);
    break;
    case  SPECIAL_ADDU:
      ret = ALU(false,ADD,rs,rt,0);
    break;
    case  SPECIAL_AND:
      ret =  ALU(false,AND,rs,rt,0);
    break;
    case  SPECIAL_JALR:
      if(rs & 0x0003){
        handleException("SPECIAL_JALR <address>;two low-order bits are not zero\n");
      }
      else{
        ret = curr_state.ip + 4;
        curr_state.ip = rs;
        nextIp = 0;
      }
    break;
    case  SPECIAL_JR:
      if(rs & 0x0003){
        handleException("SPECIAL_JR <address>;two low-order bits are not zero\n");
      }
      else{
        curr_state.ip = rs;
        nextIp = 0;
      }
    break;
    case  SPECIAL_NOR:
      ret = ALU(false,NOR,rs,rt,0);
    break;
    case  SPECIAL_OR:
      ret = ALU(false,OR,rs,rt,0);
    break;
    case  SPECIAL_SLL:
      ret = ALU(false,SHLEFTL,0,rt,shamt);
    break;
    case  SPECIAL_SLT:
    	ret = ((int32_t)rs < (int32_t)rt) ? 1 : 0;
    break;
    case  SPECIAL_SLTU:
      ret = (rs < rt) ? 1 : 0;
    break;
    case  SPECIAL_SRA:
      ret = ALU(false,SHRIGHTA,0,rt,shamt);
    break;
    case  SPECIAL_SRAV:
      ret = ALU(true,SHRIGHTA,rs,rt,0);
    break;
    case  SPECIAL_SRL:
      ret = ALU(false,SHRIGHTL,0,rt,shamt);
    break;
    case  SPECIAL_SRLV:
      ret = ALU(true,SHRIGHTL,rs,rt,0);
    break;
    case  SPECIAL_SUB:
      ret = ALU(true,SUB,rs,rt,0);
    break;
    case  SPECIAL_SUBU:
      ret = ALU(false,SUB,rs,rt,0);
    break;
    case  SPECIAL_XOR:
      ret = ALU(false,XOR,rs,rt,0);
    break;
    case  SPECIAL_SYSCALL:
      if(curr_state.regs[$v0] == SYSCALL_EXIT)
        running =  FALSE;
      return nextIp;
    break;
  }
  next_state.regs[_rd] = ret;
  return nextIp;
}


/***********************IType function*********************/
uint32_t IType(uint32_t opcode,uint32_t rs,uint32_t rt,uint32_t imm16, uint32_t instruction){

  if(opcode == OPCODE_REGIMM)
    return oneReg_IType(rt, rs, imm16);
  else
    return twoReg_IType(opcode,rs,rt,imm16);
}

/***********************oneReg_IType function*********************/
uint32_t oneReg_IType(uint32_t action, uint32_t _rs, uint32_t imm16){
  uint32_t nextIp = 4,rs = curr_state.regs[_rs],retVal;

  retVal = (get_signed_int32(imm16,HALF_WORD_SIZE)) << 2 ;

  switch(action){
    case  REGIMM_BGEZ:
      if(!(rs & SIGN_BIT_32))// rs >= 0
        return retVal;
      break;
    case  REGIMM_BGEZAL:
      next_state.regs[$ra] = curr_state.ip + 4;
      if(rs & SIGN_BIT_32)//rs >= 0
        return retVal;
      break;
    case  REGIMM_BLTZ:
      if(!(rs & SIGN_BIT_32))//rs < 0
        return retVal;
      break;
    case  REGIMM_BLTZAL:
      next_state.regs[$ra] = curr_state.ip + 4;
      if(!(rs & SIGN_BIT_32))//rs < 0
        return retVal;
      break;
  }

   return nextIp;
}


/***********************twoReg_IType function*********************/
uint32_t twoReg_IType(uint32_t opcode,uint32_t _rs,uint32_t _rt,uint32_t imm16){
  uint32_t nextIp = 4,rs,rt,uint_imm;
  int32_t int_imm;
  rs = curr_state.regs[_rs];
  rt = curr_state.regs[_rt];
  int_imm = get_signed_int32(imm16,HALF_WORD_SIZE);
  uint_imm = get_unsigned_uint32(imm16,HALF_WORD_SIZE);


  switch (opcode) {
    case OPCODE_ADDI:
      next_state.regs[_rt] = ALU(true,ADD,rs,int_imm,0);
      break;
    case OPCODE_ADDIU:
      next_state.regs[_rt] = ALU(false,ADD,rs,int_imm,0);
      break;
    case OPCODE_ANDI:
      next_state.regs[_rt] = ALU(false, AND,rs, int_imm,0);
      break;
    case OPCODE_BEQ:
      if(rs == rt)
        return (int_imm << 2);
      break;
    case OPCODE_BGTZ:
      if( !(rs & SIGN_BIT_32) && rs != 0)
        return (int_imm << 2);
      break;
    case OPCODE_BLEZ:
      if((rs & SIGN_BIT_32) || rs == 0)
        return (int_imm << 2);
      break;
    case OPCODE_BNE:
      if(rs != rt)
        return (int_imm << 2);
      break;
    case OPCODE_LB:
      readFromMemory( ALU(true,ADD,rs,int_imm,0), BYTE_SIZE, true,_rt);
      break;
    case OPCODE_LBU:
      readFromMemory( ALU(true,ADD,rs,int_imm,0), BYTE_SIZE, false,_rt);
      break;
    case OPCODE_LH:
      readFromMemory( ALU(true,ADD,rs,int_imm,0), HALF_WORD_SIZE, true,_rt);
      break;
    case OPCODE_LHU:
      readFromMemory( ALU(true,ADD,rs,int_imm,0), HALF_WORD_SIZE, false,_rt);
      break;
    case OPCODE_LUI:
      next_state.regs[_rt] = uint_imm << 16;
      break;
    case OPCODE_LW:
      readFromMemory( ALU(true,ADD,rs,int_imm,0), WORD_SIZE, true,_rt);
      break;
    case OPCODE_ORI:
      next_state.regs[_rt] = ALU(false,OR,rs,uint_imm,0);
      break;
    case OPCODE_XORI:
      next_state.regs[_rt] = ALU(false,XOR,rs,uint_imm,0);
      break;
    case OPCODE_SW:
      writeToMemory(ALU(true,ADD,rs,int_imm,0), WORD_SIZE, _rt);
      break;
    case OPCODE_SH:
      writeToMemory(ALU(true,ADD,rs,int_imm,0), HALF_WORD_SIZE, _rt);
      break;
    case OPCODE_SB:
      writeToMemory(ALU(true,ADD,rs,int_imm,0), BYTE_SIZE, _rt);
      break;
    case OPCODE_SLTI:
      next_state.regs[_rt] = checkLessThen(rs,int_imm,true);
      break;
    case OPCODE_SLTIU:
      next_state.regs[_rt] = checkLessThen(rs,uint_imm,false);
      break;
  }
  return nextIp;
}

/***********************ALU function*********************/
uint32_t ALU(bool signBit,operation op,uint32_t num1, uint32_t num2,uint32_t shamt){
  uint32_t ret = 0;
  switch(op){
    case ADD:
    if(signBit == true){
      ret = ((int32_t)num1 + (int32_t)num2);
      if( ((num1 & SIGN_BIT_32) == (num2 & SIGN_BIT_32)) && ((num1 & SIGN_BIT_32) != (ret & SIGN_BIT_32))){
        handleException("SPECIAL_ADD <Overflow>; An overflow exception occured\n");
        return curr_state.regs[shamt]; // rd not Modified if exception occured
      }
      return ret;
    }
    return (num1 + num2);
    break;
    case  SUB:
    if(signBit == true){
      ret = ((int32_t)num1 - (int32_t)num2);
      if( ((num1 & SIGN_BIT_32) != (num2 & SIGN_BIT_32)) && ((num1 & SIGN_BIT_32) != (ret & SIGN_BIT_32))){
        handleException("SPECIAL_SUB <Overflow>; An overflow exception occured\n");
        return curr_state.regs[shamt]; // rd not Modified if exception occured
      }
    return ret;
    }
    return (num1 - num2);
    break;
    case  OR:
      return (num1 | num2);
    break;
    case  AND:
      return (num1 & num2);
    break;
    case  XOR:
      return (num1 ^ num2);
    break;
    case  NOR:
      return (~(num1 | num2));
    break;
    case  SHRIGHTL:
      return ((signBit == true)? ((int32_t)num2 >> (num1 & 0x1f)):(num2 >> shamt));
    break;
    case  SHLEFTL:
      return ((signBit == true)? ((int32_t)num2 << (num1 & 0x1f)):(num2 << shamt));
    break;
    case  SHRIGHTA:
      return (((int32_t)num2 >> shamt));
    break;
    default:
    break;
  }
}

/***********************handleException function*********************/
void handleException(const char* message){
  printf("Exception At: %s\n",message);
}

/***********************opcodeAction function*********************/
Type opcodeAction(uint32_t opcode){
  if(opcode == OPCODE_SPECIAL) return R_TYPE;
  if(opcode == OPCODE_J || opcode == OPCODE_JAL) return J_TYPE;
  return I_TYPE;
}

/***********************get_unsigned_uint32 function*********************/
uint32_t get_unsigned_uint32(uint32_t num,SIZE size){
  uint32_t result;
  if(size == HALF_WORD_SIZE)
    result = (num & MASK_IMM16);
  else
    result = num;
  return result;
}

/***********************get_signed_int32 function*********************/
int32_t get_signed_int32(uint32_t num,SIZE size){
  int32_t result;
  if(size == HALF_WORD_SIZE)
    result = (int16_t)(num & MASK_IMM16);
  else
    result = num;
  return result;
}

/***********************checkLessThen function*********************/
uint32_t checkLessThen(uint32_t num1, uint32_t num2,bool signBit){
	if( (!signBit && num1 < num2) || (signBit && (int32_t)num1 < (int32_t)num2) )
		return 1;

	return 0;
}

/***********************readFromMemory function*********************/
void readFromMemory(uint32_t address,SIZE wordSize, bool signBit, uint32_t retReg){
  uint32_t retVal;
  uint32_t valAddr = mem_read_32(address);

  switch(wordSize){
    case WORD_SIZE:
      if(!(address & 3))
        retVal = valAddr;
      break;
    case HALF_WORD_SIZE:
      if( !(address & 1) ){
        retVal =(signBit)? get_signed_int32(valAddr,HALF_WORD_SIZE): get_unsigned_uint32(valAddr, HALF_WORD_SIZE);
      }
      break;
    case BYTE_SIZE:
      retVal = (signBit)?
      (int8_t)get_signed_int32(valAddr, HALF_WORD_SIZE):
      (uint8_t)get_unsigned_uint32(valAddr, HALF_WORD_SIZE);
      break;
  }
  next_state.regs[retReg]  = retVal;
}


/***********************writeToMemory function*********************/
void writeToMemory(uint32_t address, SIZE wordSize,int32_t retReg){
	uint32_t retVal;
	switch(wordSize)
	{
		case WORD_SIZE:
			if(address & 3) return;
      retVal = curr_state.regs[retReg];
			break;
		case HALF_WORD_SIZE:
			if(address & 1) return;
		  retVal = curr_state.regs[retReg]& MASK_HALFWORD;
			break;
		case BYTE_SIZE:
      retVal = curr_state.regs[retReg]& MASK_BYTE;
			break;
	}

  mem_write_32( address, retVal);
}
