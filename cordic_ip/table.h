#ifndef _TABLE_H_
#define _TABLE_H_
// table.h: header file
/********************************************************************
// 
// Module:
//   A collection of lookup tables used in CORDIC computation 
//
//
// Authors: 	Winnie Cheng<wwcheng@stanford.edu>,
//		Peter Wu<peter5@stanford.edu>
 *********************************************************************/

// table for non-hyperbolic functions
#define TABLEP_0	0x1680
#define TABLEP_1	0x0d48
#define TABLEP_2	0x0705
#define TABLEP_3	0x0390
#define TABLEP_4	0x01ca
#define TABLEP_5	0x00e5
#define TABLEP_6	0x0073
#define TABLEP_7	0x0039
#define TABLEP_8	0x001d
#define TABLEP_9	0x000e
#define TABLEP_10	0x0007
#define TABLEP_11	0x0004

// table for hyperbolic functions
#define TABLEH_0	0x0fbd
#define TABLEH_1	0x0751
#define TABLEH_2	0x039a
#define TABLEH_3	0x01cb
#define TABLEH_4	0x00e5
#define TABLEH_5	0x0073
#define TABLEH_6	0x0039
#define TABLEH_7	0x001d
#define TABLEH_8	0x000e
#define TABLEH_9	0x0007
#define TABLEH_10	0x0004
#define TABLEH_11	0x0002

#endif // _TABLE_H_ 
