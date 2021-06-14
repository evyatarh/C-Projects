/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   Architecture course                                       */
/*                                                             */
/*                                                             */
/***************************************************************/

#ifndef _SIM_OPCODES_H_
#define _SIM_OPCODES_H_

/**
 * Masks and shift counts for accessing the fields of an instruction
 */
#define MASK_OPCODE           0xFC000000
#define SHIFT_OPCODE          26

#define MASK_RS               0x03E00000
#define SHIFT_RS              21

#define MASK_RT               0x001F0000
#define SHIFT_RT              16

#define MASK_RD               0x0000F800
#define SHIFT_RD              11

#define MASK_SHAMT            0x000007C0
#define SHIFT_SHAMT           06

#define MASK_FUNCT            0x0000003F
#define SHIFT_FUNCT           0

#define MASK_IMM16            0x0000FFFF
#define SHIFT_IMM16           0

#define MASK_IMM26            0x03FFFFFF
#define SHIFT_IMM26           0

#define SIGN_BIT_8            0x00000080
#define SIGN_BIT_16           0x00008000
#define SIGN_BIT_32           0x80000000


/**
 * MIPS opcodes
 *
 * These values may appear in the opcode field (bits 26-31) of an instruction.
 *
 */
#define OPCODE_ADDI           0x08
#define OPCODE_ADDIU          0x09
#define OPCODE_ANDI           0x0C
#define OPCODE_BEQ            0x04
#define OPCODE_BGTZ           0x07
#define OPCODE_BLEZ           0x06
#define OPCODE_BNE            0x05
#define OPCODE_J              0x02
#define OPCODE_JAL            0x03
#define OPCODE_LB             0x20
#define OPCODE_LBU            0x24
#define OPCODE_LH             0x21
#define OPCODE_LHU            0x25
#define OPCODE_LUI            0x0F
#define OPCODE_LW             0x23
#define OPCODE_ORI            0x0D
#define OPCODE_SB             0x28
#define OPCODE_SH             0x29
#define OPCODE_SLTI           0x0A
#define OPCODE_SLTIU          0x0B
#define OPCODE_SW             0x2B
#define OPCODE_XORI           0x0E

/*
 * SPECIAL: the opcode for instructions with 2 register parameters (R-type).
 *          The actual operation is determined by the funct field (bits 0-5).
 *          (See SPECIAL_xxxx symbols below.)
 */
#define OPCODE_SPECIAL        0x00

/*
 * REGIMM: the opcode for those I-type instructions which depend on a 16-bit
 *         immediate but only one register (e.g., some branch instructions).
 *         Opcode values may be saved if the field usually reserved for a
 *         second register is used to determine the required action.
 *         Thus, in these instructions the register is determined by rs field
 *         (bits 21-25) and the actual operation by the rt field (bits 16-20).
 *         (See REGIMM_xxxx symbols below.)
 */
#define OPCODE_REGIMM         0x01

/**
 * MIPS function codes, associated with OPCODE_SPECIAL
 */
#define SPECIAL_ADD           0x20
#define SPECIAL_ADDU          0x21
#define SPECIAL_AND           0x24
#define SPECIAL_JALR          0x09
#define SPECIAL_JR            0x08
#define SPECIAL_NOR           0x27
#define SPECIAL_OR            0x25
#define SPECIAL_SLL           0x00
#define SPECIAL_SLLV          0x04
#define SPECIAL_SLT           0x2A
#define SPECIAL_SLTU          0x2B
#define SPECIAL_SRA           0x03
#define SPECIAL_SRAV          0x07
#define SPECIAL_SRL           0x02
#define SPECIAL_SRLV          0x06
#define SPECIAL_SUB           0x22
#define SPECIAL_SUBU          0x23
#define SPECIAL_SYSCALL       0x0C
#define SPECIAL_XOR           0x26

/*
 * There a few more SPECIAL function codes which deal with multiplication
 * and division, that are not to be implemented right now...
 */
//#define SPECIAL_DIV           0x1A
//#define SPECIAL_DIVU          0x1B
//#define SPECIAL_MFHI          0x10
//#define SPECIAL_MFLO          0x12
//#define SPECIAL_MTHI          0x11
//#define SPECIAL_MTLO          0x13
//#define SPECIAL_MULT          0x18
//#define SPECIAL_MULTU         0x19


/**
 * MIPS function codes, associated with OPCODE_REGIMM
 */
#define REGIMM_BGEZ           0x01
#define REGIMM_BGEZAL         0x11
#define REGIMM_BLTZ           0x00
#define REGIMM_BLTZAL         0x10


/**
 * SYSCALL codes, to be loaded into $v0 when opcode is OPCODE_SPECIAL
 * and funct is SPECIAL_SYSCALL
 */
#define SYSCALL_CLOSE         0x10
#define SYSCALL_EXIT          0x0A
#define SYSCALL_OPEN          0x0D
#define SYSCALL_PRINT_CHAR    0x0B
#define SYSCALL_PRINT_DOUBLE  0x03
#define SYSCALL_PRINT_FLOAT   0x02
#define SYSCALL_PRINT_INT     0x01
#define SYSCALL_READ          0x0E
#define SYSCALL_READ_CHAR     0x0C
#define SYSCALL_READ_DOUBLE   0x07
#define SYSCALL_READ_FLOAT    0x06
#define SYSCALL_READ_INT      0x05
#define SYSCALL_WRITE         0x0F


/**
 * register names
 */
#define $0     0	 // always 0
#define $at    1	 // reserved for the assembler
#define $v0    2	 // result of function
#define $v1    3	 // result of function
#define $a0    4	 // 1st argument to a function
#define $a1    5	 // 2nd argument to a function
#define $a2    6	 // 3rd argument to a function
#define $a3    7	 // 4th argument to a function
#define $t0    8	 // temporary (not preserved across calls)
#define $t1    9	 // temporary (not preserved across calls)
#define $t2   10	 // temporary (not preserved across calls)
#define $t3   11	 // temporary (not preserved across calls)
#define $t4   12	 // temporary (not preserved across calls)
#define $t5   13	 // temporary (not preserved across calls)
#define $t6   14	 // temporary (not preserved across calls)
#define $t7   15	 // temporary (not preserved across calls)
#define $s0   16	 // saved temporary (preserved across calls)
#define $s1   17	 // saved temporary (preserved across calls)
#define $s2   18	 // saved temporary (preserved across calls)
#define $s3   19	 // saved temporary (preserved across calls)
#define $s4   20	 // saved temporary (preserved across calls)
#define $s5   21	 // saved temporary (preserved across calls)
#define $s6   22	 // saved temporary (preserved across calls)
#define $s7   23	 // saved temporary (preserved across calls)
#define $t8   24	 // temporary (not preserved across calls)
#define $t9   25	 // temporary (not preserved across calls)
#define $k0   26	 // reserved for kernel
#define $k1   27	 // reserved for kernel
#define $gp   28	 // pointer to global area
#define $sp   29	 // pointer to top of stack
#define $fp   30	 // frame pointer
#define $ra   31	 // return address


#endif
