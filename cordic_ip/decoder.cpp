// decoder.cpp: source file
/********************************************************************
// 
// Module:
//   Instruction Decoding Unit
//
// Implementation:
//   This module decodes the instruction and sets up the 12-stage
//   CORDIC pipeline
//
//
// Authors: 	Winnie Cheng <wwcheng@stanford.edu>,
//		Peter Wu <peter5@stanford.edu>
//
 *********************************************************************/

#include "decoder.h"
#include "opcode.h"
#include "convert.h"

#ifdef MODULE_NAME
#undef MODULE_NAME
#endif

#define MODULE_NAME	"decoder"

#define DEBUG		1
#if DEBUG
#define dprintf		printf
#else
#define dprintf
#endif

#define NUM_STAGES	12
#define IDLE_SLACK	2*NUM_STAGES

void decoder::decoder_process()
{
    short flush;
    sc_uint<UNIT_SEL_WIDTH> ir;
    short op1, op2, op3;

    short x, y, acc_phase, desired_phase;
    short tmp_x;

    // On reset, initialize output
    enable_pipeline.write(false);
    out_x.write(0);
    out_y.write(0);
    out_acc_phase.write(0);
    out_opcode.write(I_NOP);
    out_desired_phase.write(0);
    flush = NUM_STAGES + IDLE_SLACK;

    while(1) {  
        wait();
        if(start.read()==true) {
            flush = NUM_STAGES + IDLE_SLACK;	// reset counter
            
	    // Registered input
	    if(instruction_valid.read()==true) {
                // Read registered input
                ir = opcode.read();
		op1 = operand1.read();
		op2 = operand2.read();
		op3 = operand3.read();

                // Decode the opcode and adjust input
		if(ir == I_ROTATE) {
		
	            // Rotate Instruction Format
	            //   operand1 : orgX
	            //   operand2 : orgY
	            //   operand3 : angle
	            // returns
	            //   result1 : rotated x-coordinate
	            //   result2 : rotated y-coordinate   
		   
		    x = op1;
		    y = op2;
		    acc_phase = op3;

		    if(acc_phase < 0) {
	                // If vector lies in fourth-quadrant, position to first-quadrant
		        x = -op1;
		        y = -op2;
			acc_phase += INT180;
	            }   
		    if (acc_phase > INT90) {
	                // If vector lies in third-quadrant, position to first-quadrant
		         tmp_x = x;
			 x = -y;
		         y = tmp_x;
	                 acc_phase -=INT90;
		    }
	            
                    dprintf("[%s] ROTATE\n", MODULE_NAME);
		
		} else if (ir == I_MAGPHASE) {

	            // Magphase Instruction Format
	            //   operand1 : X
	            //   operand2 : Y
	            // returns
	            //   result1 : magnitude
	            //   result2 : phase

                   x = op1;
		   y = op2;

                   if(x < 0) {
                       // rotate by an initial +/- 90 degrees
		       tmp_x = x; 
		       if( y > 0) {
		           x = y;
			   y = -tmp_x;
			   acc_phase = -INT90;	// subtract 90 deg
		       } else {
		           x = -y;
			   y = tmp_x;
			   acc_phase = INT90;	// add 90 deg
		       }
                   } else {
		       acc_phase = 0;
		   }    
                    dprintf("[%s] MAG-PHASE\n", MODULE_NAME);
		} else if (ir == I_SINCOS) {

	            // SinCos Instruction Format
	            //   operand1 : phase
	            // return
	            //   result1 : sin(phase)
	            //   result2 : cos(phase)

                    desired_phase = op1;

                    // start with +90, -90, or 0 degrees
	            if(desired_phase > INT90) {
	                x = 0;
	                y = START_SINCOS_Y;	// adjust with mult factor
                        acc_phase = INT90;
	            } else if (desired_phase < -INT90) {
	                x = 0;
	                y = -START_SINCOS_Y;
                        acc_phase = -INT90;
	            } else {
	                x = START_SINCOS_Y;
	                y = 0;
	                acc_phase = 0;
	            }   
                    dprintf("[%s] SIN-COS\n", MODULE_NAME);
		} else if (ir == I_SINHCOSH) {
	    
	            // SinhCosh Instruction Format
	            //   operand1 : phase
	            // return
	            //   result1 : sinh(phase)
	            //   result2 : cosh(phase)	 

                    desired_phase = op1;
		    x = START_SINHCOSH_X;
		    y = 0;
		    acc_phase = 0;
                    dprintf("[%s] SINH-COSH\n", MODULE_NAME);
		}
		
		// write output
		enable_pipeline.write(true);
		out_x.write(x);
		out_y.write(y);
		out_acc_phase.write(acc_phase);
		out_opcode.write(ir);
		out_desired_phase.write(desired_phase);
		
	    } else {
                // feed with NOP
		enable_pipeline.write(true);
                out_x.write(0);
                out_y.write(0);
                out_acc_phase.write(0);
                out_opcode.write(I_NOP);
                out_desired_phase.write(0);
	    }
	} else {
            if(flush > 0) {
                // keep pipeline enable, flush with NOP
	        enable_pipeline.write(true);
		flush--;
	    } else {
	        enable_pipeline.write(false);
		printf("[%s] Pipeline shutdown for power-save mode\n", MODULE_NAME);
	    }	
            out_x.write(0);
            out_y.write(0);
            out_acc_phase.write(0);
            out_opcode.write(I_NOP);
            out_desired_phase.write(0);
	}
        	
    } // forever-loop	
}
