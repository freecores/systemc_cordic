// adjust.h: header file
/********************************************************************
// 
// Module:
//   Pipeline Output Adjusting Unit
//
// Interface:
//   This module interfaces to the pipeline and the testbench result
//   monitor.
//   Registered input, non-registered output
//
//
// Authors: 	Winnie Cheng <wwcheng@stanford.edu>,
//		Peter Wu <peter5@stanford.edu>
//
 *********************************************************************/

#include "systemc.h"
#include "opcode.h"

SC_MODULE(adjust) {

    // Clock Declaration
    sc_in_clk clk;
    
    // Input ports Declaration
    sc_in<bool> reset;
    
    // Input port declaration
    sc_in<bool> in_valid;

    // Input data - pipeline results
    sc_in<short> in_x;
    sc_in<short> in_y;
    sc_in<short> in_acc_phase;

    // Input data - propagated instruction info
    sc_in<sc_uint<UNIT_SEL_WIDTH> > in_opcode;

    // Output data - results of this stage
    sc_out<bool> result_valid;
    sc_out<sc_uint<UNIT_SEL_WIDTH> > out_opcode;
    sc_out<short> out_result1;
    sc_out<short> out_result2;

    // Declare implementation functions
    void adjust_process();

    // Constructor
    SC_CTOR(adjust)
    {
        // Register processes and define active clock edge
        SC_CTHREAD(adjust_process, clk.pos());

        // Watching for global reset
        watching(reset.delayed()==true);
    }

}; // end module adjust 
