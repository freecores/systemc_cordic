// cordic.cpp: source file
/********************************************************************
// 
// Module:
//   Cordic Engine Pipeline Stage
//
// Implementation:
//   This module implements one pipeline stage of the cordic engine.
//
//
// Authors: 	Winnie Cheng <wwcheng@stanford.edu>,
//		Peter Wu <peter5@stanford.edu>
//
 *********************************************************************/

#include "cordic.h"
#include "opcode.h"

#ifdef MODULE_NAME
#undef MODULE_NAME
#endif

#define MODULE_NAME	"cordic"

#define DEBUG		1
#if DEBUG
#define dprintf		printf
#else
#define dprintf
#endif

void cordic::cordic_process()
{
    
    bool enable;
    
    short opcode;
    short desired_phase;

    short x, y, acc_phase;

    short type, tmp_x, table_value, cond;
   
    // stage parameters
    short reg_stage_num, reg_tablep, reg_tableh;
    
    // On Reset, initialize output
    done.write(false);
    out_x.write(0);
    out_y.write(0);
    out_acc_phase.write(0);
    out_opcode.write(I_NOP);
    out_desired_phase.write(0);
    
    // Get stage parameters for this stage
    reg_stage_num = stage_num.read();
    reg_tablep = tablep.read();
    reg_tableh = tableh.read();

    while(1) {

	wait();
        
	// Registered Input
        enable = start.read();
	  // input data from previous stage
        x = in_x.read();
        y = in_y.read();
        acc_phase = in_acc_phase.read();
          // input data propagated
	opcode = in_opcode.read();  
        desired_phase = in_desired_phase.read();

        if(enable) {
	
	    // do stage operation
	  
	    // condition varies for different opcode
            if(opcode == I_ROTATE) {
	        cond = (acc_phase < 0);
	        type = TYPE_NONHYPER;
	        table_value = -reg_tablep;
	        dprintf("[%s%02d] ROTATE\n", MODULE_NAME, reg_stage_num); 
	    } else if (opcode == I_MAGPHASE) {
	        cond = (y >= 0);
	        type = TYPE_NONHYPER;
	        table_value = reg_tablep;
	        dprintf("[%s%02d] MAG-PHASE\n", MODULE_NAME, reg_stage_num); 
	    } else if (opcode == I_SINCOS) {
	        cond = (desired_phase - acc_phase < 0);
	        type = TYPE_NONHYPER;
	        table_value = reg_tablep;
	        dprintf("[%s%02d] SIN-COS\n", MODULE_NAME, reg_stage_num); 
	    } else if (opcode == I_SINHCOSH) {
	        cond = (desired_phase - acc_phase < 0);
	        type = TYPE_HYPER;
	        table_value = reg_tableh;
	        dprintf("[%s%02d] SINH-COSH\n", MODULE_NAME, reg_stage_num); 
	    } else {
	        // treat as NOP
	        type = TYPE_NOP;
	        dprintf("[%s%02d] NOP\n", MODULE_NAME, reg_stage_num); 
            }

            tmp_x = x;
	    if(type == TYPE_NONHYPER) {
                // non-hyperbolic functions 
                if(cond) {
                     x += y >> reg_stage_num;
                     y -= tmp_x >> reg_stage_num;
	             acc_phase -= table_value;
	        } else {
                     x -= y >> reg_stage_num;
		     y += tmp_x >> reg_stage_num;
		     acc_phase += table_value;
	        }
               
		// Output results
                done.write(true);
                out_x.write(x);
                out_y.write(y);
                out_acc_phase.write(acc_phase);
		out_opcode.write(opcode);
                out_desired_phase.write(desired_phase);

            } else if (type == TYPE_HYPER) {
	        // hyperbolic functions
                 
		if(cond) {
                    x -= y >> (reg_stage_num+1);
                    y -= tmp_x >> (reg_stage_num+1);
	  	    acc_phase -= table_value;
	        } else {
                    x += y >> (reg_stage_num+1);
		    y += tmp_x >> (reg_stage_num+1);
		    acc_phase += table_value;
	        }

		// Output results
                done.write(true);
                out_x.write(x);
                out_y.write(y);
                out_acc_phase.write(acc_phase);
		out_opcode.write(opcode);
                out_desired_phase.write(desired_phase);

	    } else {

                // NOP, same output as reset condition except done is true
                done.write(true);
                out_x.write(0);
                out_y.write(0);
                out_acc_phase.write(0);
                out_opcode.write(I_NOP);
                out_desired_phase.write(0);
	    }

	} else {
	
            // same output as reset condition
            done.write(false);
            out_x.write(0);
            out_y.write(0);
            out_acc_phase.write(0);
            out_opcode.write(I_NOP);
            out_desired_phase.write(0);
	}

    } // forever loop	
}
