// resultfifo.h: header file
/********************************************************************
// 
// Module:
//   Pipeline Output Result FIFO
//
// Interface:
//   This module connects to the adjust module and to a "reader" of
//   the result
//
//
// Authors: 	Winnie Cheng <wwcheng@stanford.edu>,
//		Peter Wu <peter5@stanford.edu>
//
 *********************************************************************/

#include "systemc.h"
#include "opcode.h"


SC_MODULE(resultfifo) {

    // Clock Declaration
    sc_in_clk clk;
    
    // Input ports Declaration
    sc_in<bool> reset;
   
    // Input control
    sc_in<bool> read_request;
    sc_in<bool> write_request;
 
    // Input data - adjust module output
    sc_in<sc_uint<UNIT_SEL_WIDTH> > in_opcode;
    sc_in<short> in_result1;
    sc_in<short> in_result2;

    // Output data - results of this stage
    sc_out<bool> out_valid;
    sc_out<sc_uint<UNIT_SEL_WIDTH> > out_opcode;
    sc_out<short> out_result1;
    sc_out<short> out_result2;

    // Declare implementation functions
    void resultfifo_process();

    // Constructor
    SC_CTOR(resultfifo)
    {
        // Register processes and define active clock edge
        SC_CTHREAD(resultfifo_process, clk.pos());

        // Watching for global reset
        watching(reset.delayed()==true);
    }

}; // end module resultfifo 
