// monitor.h: header file
/********************************************************************
// 
// Module:
//   Pipeline Output Monitor 
//
// Interface:
//   This module interfaces to the adjust unit and captures result
//   to a file for testbench comparison.
//
//
// Authors: 	Winnie Cheng <wwcheng@stanford.edu>,
//		Peter Wu <peter5@stanford.edu>
//
 *********************************************************************/

#include "systemc.h"
#include "opcode.h"

SC_MODULE(monitor) {

    // Clock Declaration
    sc_in_clk clk;
    
    // Input ports Declaration
    sc_in<bool> start;
    sc_in<bool> reset;
    
    // Input port declaration
    sc_in<bool> in_valid;
    sc_in<sc_uint<UNIT_SEL_WIDTH> > in_opcode;
    sc_in<short> in_result1;
    sc_in<short> in_result2;

    // Output data - other data written to file
    sc_out<bool> idle;

    // Declare implementation functions
    void monitor_process();

    // Constructor
    SC_CTOR(monitor)
    {
        // Register processes and define active clock edge
        SC_CTHREAD(monitor_process, clk.pos());

        // Watching for global reset
        watching(reset.delayed()==true);
    }

}; // end module monitor 
