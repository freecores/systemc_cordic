// decoder.h: header file
/********************************************************************
// 
// Module:
//   Instruction Decoding Unit
//
// Interface:
//   This module interfaces to the testbench and the pipeline.
//   Registered input, non-registered output
//
//
// Authors: 	Winnie Cheng <wwcheng@stanford.edu>,
//		Peter Wu <peter5@stanford.edu>
//
 *********************************************************************/

#include "systemc.h"
#include "opcode.h"

SC_MODULE(decoder) {

    // Clock Declaration
    sc_in_clk clk;
    
    // Input ports Declaration
    sc_in<bool> start;	
    sc_in<bool> reset;
   
    // Input data - instruction
    sc_in<bool> instruction_valid;
    sc_in<sc_uint<UNIT_SEL_WIDTH> > opcode;
    sc_in<short> operand1;
    sc_in<short> operand2;
    sc_in<short> operand3;

    // Output ports declaration
    sc_out<bool> enable_pipeline;

    // Output data - feed to first stage of pipeline
    sc_out<short> out_x;
    sc_out<short> out_y;
    sc_out<short> out_acc_phase;

    // Output data - propagated instruction info 
    sc_out<sc_uint<UNIT_SEL_WIDTH> > out_opcode;
    sc_out<short> out_desired_phase; 

    // Declare implementation functions
    void decoder_process();

    // Constructor
    SC_CTOR(decoder)
    {
        // Register processes and define active clock edge
        SC_CTHREAD(decoder_process, clk.pos());

        // Watching for global reset
        watching(reset.delayed()==true);
    }

}; // end module cordic 
