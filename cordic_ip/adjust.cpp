// adjust.cpp: source file
/********************************************************************
// 
// Module:
//   Pipeline Output Adjusting Unit
//
// Implementation:
//   This module performs some of the shifts there are needed to
//   obtain final results.
//
//
// Authors: 	Winnie Cheng <wwcheng@stanford.edu>,
//		Peter Wu <peter5@stanford.edu>
//
 *********************************************************************/

#include "adjust.h"
#include "opcode.h"

#ifdef MODULE_NAME
#undef MODULE_NAME
#endif

#define MODULE_NAME	"adjust"

#define DEBUG		1
#if DEBUG
#define dprintf		printf
#else
#define dprintf
#endif

void adjust::adjust_process()
{
    sc_uint<UNIT_SEL_WIDTH> opcode;
    short x, y, acc_phase;
    short result1, result2;

    // On Reset, initialize output
    result_valid.write(true);
    out_opcode.write(I_NOP);
    out_result1.write(0);
    out_result2.write(0);
    wait();

    while(1) {

        wait();
        
	// propagate through
	opcode = in_opcode.read();
        out_opcode.write(opcode);

	if(in_valid.read()==true){
	    // Register input
            x = in_x.read();
	    y = in_y.read();
	    acc_phase = in_acc_phase.read();

            // Adjust according to opcode
            if(opcode == I_ROTATE) {

	        // Rotate Instruction Format
	        //   operand1 : orgX
	        //   operand2 : orgY
	        //   operand3 : angle
	        // returns
	        //   result1 : rotated x-coordinate
	        //   result2 : rotated y-coordinate

	        // Reduce vector size by multiplier
		//   (1/2+1/8-1/64-1/512)
               
	        // adjust
		result1=(x>>1)+(x>>3)-(x>>6)-(x>>9); 
  	        result2=(y>>1)+(y>>3)-(y>>6)-(y>>9);

                // output results
		out_result1.write(result1);
		out_result2.write(result2);
                result_valid.write(true);
                
		dprintf("[%s] Adjust ROTATE\n", MODULE_NAME);

	    } else if (opcode == I_MAGPHASE) {

	        // Magphase Instruction Format
	        //   operand1 : X
	        //   operand2 : Y
	        // returns
	        //   result1 : magnitude
	        //   result2 : phase

                // adjust
                result1 = (x>>1)+(x>>3)-(x>>6)-(x>>9);
		result2 = -acc_phase;

                // output results
		out_result1.write(result1);
		out_result2.write(result2);
                result_valid.write(true);

		dprintf("[%s] Adjust MAG-PHASE\n", MODULE_NAME);

            } else if (opcode == I_SINCOS || opcode == I_SINHCOSH) {
	    
	        // SinCos/SinhCosh Instruction Format
	        //   operand1 : phase
	        // return
	        //   result1 : sin/sinh(phase)
	        //   result2 : cos/cosh(phase)

                // no adjustment needed

                // output results
                out_result1.write(y);	// sin/sinh result
		out_result2.write(x);	// cos/cosh result
                result_valid.write(true);

		dprintf("[%s] Adjust SINCOS or SINHCOSH\n", MODULE_NAME);

            } else {
	        // filter out NOP results
	        result_valid.write(false);
		
		dprintf("[%s] Adjust ignored NOP\n", MODULE_NAME);
	    }	

	} else {
	    result_valid.write(false);
	    dprintf("[%s] Nothing to adjust\n", MODULE_NAME);
        }
    } // forever loop	
}
