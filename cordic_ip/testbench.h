// testbench.h: header file
/********************************************************************
// 
// Module:
//   Testbench   
//
// Interface:
//   This module generates random input vectors to test the CORDIC
//   core and checks the result with C math library functions.
//
//
// Authors: 	Winnie Cheng <wwcheng@stanford.edu>,
//		Peter Wu <peter5@stanford.edu>
//
 *********************************************************************/
#include "systemc.h"
#include "opcode.h"

SC_MODULE(testbench) {

    // Clock Declaration
    sc_in_clk clk;
    
    // Input ports Declaration
    sc_in<bool> start; 
    sc_in<bool> reset;
    sc_in<bool> compute_done;
    sc_in<short> result1;
    sc_in<short> result2;
    sc_in<bool> monitor_idle;

    // Input ports from result fifo
    sc_in<bool> fifo_valid;
    sc_in<sc_uint<UNIT_SEL_WIDTH> > fifo_opcode;
    sc_in<short> fifo_result1;
    sc_in<short> fifo_result2;

    // Output ports Declaration
    sc_out<bool> instructions_valid;
    sc_out<bool> done;
    sc_out<bool> start_monitor;

    // Output data
    sc_out<sc_uint<UNIT_SEL_WIDTH> > engine_select;
    sc_out<short> operand1;
    sc_out<short> operand2;
    sc_out<short> operand3;

    // Output to result fifo
    sc_out<bool> fifo_read_request;
    
    // Declare implementation functions
    void testbench_process();
    void readfifo_process();

    // Constructor
    SC_CTOR(testbench)
    {
        // Register processes and define active clock edge
        SC_CTHREAD(testbench_process, clk.pos());
        SC_CTHREAD(readfifo_process, clk.pos());

        // Watching for global reset
        watching(reset.delayed()==true);
    }

}; // end module testbench 
