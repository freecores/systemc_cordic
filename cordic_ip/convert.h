#ifndef _CONVERT_H_
#define _CONVERT_H_
// convert.h: header file
/********************************************************************
// 
// Module:
//   Collection of type conversion parameters and functions 
//
// Authors: 	Winnie Cheng<wwcheng@stanford.edu>,
//		Peter Wu<peter5@stanford.edu>
 *********************************************************************/

/* common angles in fixed point format */
#define INT90  (0x2d00) /* Predefine 90 degrees in fixed point format  */
#define INT180 (0x5a00) /* Predefine 180 degrees in fixed point format */

/* start values for some rotation */
#define START_SINCOS_Y		0x004e
#define START_SINHCOSH_X	0x009b	// 1.2074

/* specification for fixed point format */
#define FRACTBITS 7          /* Number of fractional bits */
#define CONV (1<<FRACTBITS)  /* Conversion factor to fixed-point format */

/* fixed-point <-> floating point conversion functions */
  /* convert from fixed point to float */
#define tofp(i)   ((double)(1.0*i)/(1.0*CONV))
  /* convert from float to fixed point */
#define toint(fp) ((short)(fp*CONV+0.5))

#endif /* _CONVERT_H_ */
