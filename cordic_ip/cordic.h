// cordic.h: header file
/********************************************************************
// 
// Module:
//   Cordic Engine Pipeline Stage
//
// Interface:
//   Registered input, non-registered output
//
//
// Authors: 	Winnie Cheng <wwcheng@stanford.edu>,
//		Peter Wu <peter5@stanford.edu>
//
 *********************************************************************/

#include "systemc.h"
#include "opcode.h"

SC_MODULE(cordic) {

    // Clock Declaration
    sc_in_clk clk;
    
    // Input ports Declaration
    sc_in<bool> start;		// enables the stage
    sc_in<bool> reset;
    
    // Input data - previous stage results
    sc_in<short> in_x;
    sc_in<short> in_y;
    sc_in<short> in_acc_phase;

    // Input data - propagated instruction info
    sc_in<sc_uint<UNIT_SEL_WIDTH> > in_opcode;
    sc_in<short> in_desired_phase; 

    // Input data - stage parametrization
    sc_in<short> stage_num;
    sc_in<short> tablep;
    sc_in<short> tableh;

    // Output ports Declaration
    sc_out<bool> done;

    // Output data - results of this stage
    sc_out<short> out_x;
    sc_out<short> out_y;
    sc_out<short> out_acc_phase;

    // Output data - propagated instruction info 
    sc_out<sc_uint<UNIT_SEL_WIDTH> > out_opcode;
    sc_out<short> out_desired_phase; 

    // Declare implementation functions
    void cordic_process();

    // Constructor
    SC_CTOR(cordic)
    {
        // Register processes and define active clock edge
        SC_CTHREAD(cordic_process, clk.pos());

        // Watching for global reset
        watching(reset.delayed()==true);
    }

}; // end module cordic 
