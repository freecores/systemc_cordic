#ifndef _OPCODE_H_
#define _OPCODE_H_
// opcode.h: header file
/********************************************************************
// 
// Module:
//   A collection of opcode related decodings and settings 
//
// Authors: 	Winnie Cheng<wwcheng@stanford.edu>,
//		Peter Wu<peter5@stanford.edu>
 *********************************************************************/

// specification in user program file
#define ROT_OPNAME	"rot"
#define MAG_OPNAME	"mgp"
#define SIN_OPNAME	"sin"
#define COS_OPNAME	"cos"
#define SINH_OPNAME	"sinh"
#define COSH_OPNAME	"cosh"
#define NOP_OPNAME	"nop"

// decode which function unit
#define UNIT_SEL_WIDTH  4	
#define I_NOP		(short)0
#define I_ROTATE	(short)1
#define I_MAGPHASE	(short)2
#define I_SINCOS	(short)8	// selects the SIN-COS unit	
#define I_SIN		(short)8	
#define I_COS		(short)9	
#define I_SINHCOSH	(short)10	// selects the SINH-COSH unit	
#define I_SINH		(short)10	
#define I_COSH		(short)11	

// differentiate operations within a pipeline stage
#define TYPE_NOP	0
#define TYPE_NONHYPER	1
#define TYPE_HYPER	2

#endif // _OPCODE_H_ 
