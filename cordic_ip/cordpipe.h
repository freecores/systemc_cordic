// cordpipe.h: header file
/********************************************************************
// 
// Module:
//   Cordic Engine 12-stage Pipeline
//
// Interface:
//   This module connects the 12-stages together.
//   Registered input, non-registered output
//
//
// Authors: 	Winnie Cheng <wwcheng@stanford.edu>,
//		Peter Wu <peter5@stanford.edu>
//
 *********************************************************************/

#include "systemc.h"
#include "cordic.h"
#include "opcode.h"

SC_MODULE(cordpipe) {

    // Clock Declaration
    sc_in_clk clk;
    
    // Input ports Declaration
    sc_in<bool> start;	
    sc_in<bool> reset;
    
    // Input data - first stage results
    sc_in<short> in_x;
    sc_in<short> in_y;
    sc_in<short> in_acc_phase;

    // Input data - propagated instruction info
    sc_in<sc_uint<UNIT_SEL_WIDTH> > in_opcode;
    sc_in<short> in_desired_phase; 

    // Output ports Declaration
    sc_out<bool> done;

    // Output data - results of this stage
    sc_out<short> out_x;
    sc_out<short> out_y;
    sc_out<short> out_acc_phase;

    // Output data - propagated instruction info 
    sc_out<sc_uint<UNIT_SEL_WIDTH> > out_opcode;
    sc_out<short> out_desired_phase; 

    // constants to parametrize pipeline module on system reset
    sc_in<short> stage0;
    sc_in<short> stage1;
    sc_in<short> stage2;
    sc_in<short> stage3;
    sc_in<short> stage4;
    sc_in<short> stage5;
    sc_in<short> stage6;
    sc_in<short> stage7;
    sc_in<short> stage8;
    sc_in<short> stage9;
    sc_in<short> stage10;
    sc_in<short> stage11;

    sc_in<short> tablep_0;
    sc_in<short> tablep_1;
    sc_in<short> tablep_2;
    sc_in<short> tablep_3;
    sc_in<short> tablep_4;
    sc_in<short> tablep_5;
    sc_in<short> tablep_6;
    sc_in<short> tablep_7;
    sc_in<short> tablep_8;
    sc_in<short> tablep_9;
    sc_in<short> tablep_10;
    sc_in<short> tablep_11;

    sc_in<short> tableh_0;
    sc_in<short> tableh_1;
    sc_in<short> tableh_2;
    sc_in<short> tableh_3;
    sc_in<short> tableh_4;
    sc_in<short> tableh_5;
    sc_in<short> tableh_6;
    sc_in<short> tableh_7;
    sc_in<short> tableh_8;
    sc_in<short> tableh_9;
    sc_in<short> tableh_10;
    sc_in<short> tableh_11;

    // Signals
    sc_signal<short> out0_x;
    sc_signal<short> out1_x;
    sc_signal<short> out2_x;
    sc_signal<short> out3_x;
    sc_signal<short> out4_x;
    sc_signal<short> out5_x;
    sc_signal<short> out6_x;
    sc_signal<short> out7_x;
    sc_signal<short> out8_x;
    sc_signal<short> out9_x;
    sc_signal<short> out10_x;
    
    sc_signal<short> out0_y;
    sc_signal<short> out1_y;
    sc_signal<short> out2_y;
    sc_signal<short> out3_y;
    sc_signal<short> out4_y;
    sc_signal<short> out5_y;
    sc_signal<short> out6_y;
    sc_signal<short> out7_y;
    sc_signal<short> out8_y;
    sc_signal<short> out9_y;
    sc_signal<short> out10_y;
   
    sc_signal<short> out0_acc_phase;
    sc_signal<short> out1_acc_phase;
    sc_signal<short> out2_acc_phase;
    sc_signal<short> out3_acc_phase;
    sc_signal<short> out4_acc_phase;
    sc_signal<short> out5_acc_phase;
    sc_signal<short> out6_acc_phase;
    sc_signal<short> out7_acc_phase;
    sc_signal<short> out8_acc_phase;
    sc_signal<short> out9_acc_phase;
    sc_signal<short> out10_acc_phase;

    sc_signal<sc_uint<UNIT_SEL_WIDTH> > out0_opcode;
    sc_signal<sc_uint<UNIT_SEL_WIDTH> > out1_opcode;
    sc_signal<sc_uint<UNIT_SEL_WIDTH> > out2_opcode;
    sc_signal<sc_uint<UNIT_SEL_WIDTH> > out3_opcode;
    sc_signal<sc_uint<UNIT_SEL_WIDTH> > out4_opcode;
    sc_signal<sc_uint<UNIT_SEL_WIDTH> > out5_opcode;
    sc_signal<sc_uint<UNIT_SEL_WIDTH> > out6_opcode;
    sc_signal<sc_uint<UNIT_SEL_WIDTH> > out7_opcode;
    sc_signal<sc_uint<UNIT_SEL_WIDTH> > out8_opcode;
    sc_signal<sc_uint<UNIT_SEL_WIDTH> > out9_opcode;
    sc_signal<sc_uint<UNIT_SEL_WIDTH> > out10_opcode;

    sc_signal<short> out0_desired_phase;
    sc_signal<short> out1_desired_phase;
    sc_signal<short> out2_desired_phase;
    sc_signal<short> out3_desired_phase;
    sc_signal<short> out4_desired_phase;
    sc_signal<short> out5_desired_phase;
    sc_signal<short> out6_desired_phase;
    sc_signal<short> out7_desired_phase;
    sc_signal<short> out8_desired_phase;
    sc_signal<short> out9_desired_phase;
    sc_signal<short> out10_desired_phase;

    sc_signal<bool> stage0_done;
    sc_signal<bool> stage1_done;
    sc_signal<bool> stage2_done;
    sc_signal<bool> stage3_done;
    sc_signal<bool> stage4_done;
    sc_signal<bool> stage5_done;
    sc_signal<bool> stage6_done;
    sc_signal<bool> stage7_done;
    sc_signal<bool> stage8_done;
    sc_signal<bool> stage9_done;
    sc_signal<bool> stage10_done;

    // 12 stage instances
    cordic *s0_instance;
    cordic *s1_instance;
    cordic *s2_instance;
    cordic *s3_instance;
    cordic *s4_instance;
    cordic *s5_instance;
    cordic *s6_instance;
    cordic *s7_instance;
    cordic *s8_instance;
    cordic *s9_instance;
    cordic *s10_instance;
    cordic *s11_instance;
   
    // Constructor
    SC_CTOR(cordpipe)
    {
         // instantiate and connect stage 0
         s0_instance = new cordic("stage0_in_pipe");
         s0_instance->clk(clk);
         s0_instance->start(start);
         s0_instance->reset(reset);
         s0_instance->in_x(in_x);
	 s0_instance->in_y(in_y);
	 s0_instance->in_acc_phase(in_acc_phase);
	 s0_instance->in_opcode(in_opcode);
	 s0_instance->in_desired_phase(in_desired_phase);
	 s0_instance->stage_num(stage0);
	 s0_instance->tablep(tablep_0);
	 s0_instance->tableh(tableh_0);
	 s0_instance->done(stage0_done);
	 s0_instance->out_x(out0_x);
	 s0_instance->out_y(out0_y);
	 s0_instance->out_acc_phase(out0_acc_phase);
	 s0_instance->out_opcode(out0_opcode);
	 s0_instance->out_desired_phase(out0_desired_phase);

         // instantiate and connect stage 1
         s1_instance = new cordic("stage1_in_pipe");
         s1_instance->clk(clk);
         s1_instance->start(start);
         s1_instance->reset(reset);
         s1_instance->in_x(out0_x);
	 s1_instance->in_y(out0_y);
	 s1_instance->in_acc_phase(out0_acc_phase);
	 s1_instance->in_opcode(out0_opcode);
	 s1_instance->in_desired_phase(out0_desired_phase);
	 s1_instance->stage_num(stage1);
	 s1_instance->tablep(tablep_1);
	 s1_instance->tableh(tableh_1);
	 s1_instance->done(stage1_done);
	 s1_instance->out_x(out1_x);
	 s1_instance->out_y(out1_y);
	 s1_instance->out_acc_phase(out1_acc_phase);
	 s1_instance->out_opcode(out1_opcode);
	 s1_instance->out_desired_phase(out1_desired_phase);
	 
         // instantiate and connect stage 2
         s2_instance = new cordic("stage2_in_pipe");
         s2_instance->clk(clk);
         s2_instance->start(start);
         s2_instance->reset(reset);
         s2_instance->in_x(out1_x);
	 s2_instance->in_y(out1_y);
	 s2_instance->in_acc_phase(out1_acc_phase);
	 s2_instance->in_opcode(out1_opcode);
	 s2_instance->in_desired_phase(out1_desired_phase);
	 s2_instance->stage_num(stage2);
	 s2_instance->tablep(tablep_2);
	 s2_instance->tableh(tableh_2);
	 s2_instance->done(stage2_done);
	 s2_instance->out_x(out2_x);
	 s2_instance->out_y(out2_y);
	 s2_instance->out_acc_phase(out2_acc_phase);
	 s2_instance->out_opcode(out2_opcode);
	 s2_instance->out_desired_phase(out2_desired_phase);
	  
         // instantiate and connect stage 3
         s3_instance = new cordic("stage3_in_pipe");
         s3_instance->clk(clk);
         s3_instance->start(start);
         s3_instance->reset(reset);
         s3_instance->in_x(out2_x);
	 s3_instance->in_y(out2_y);
	 s3_instance->in_acc_phase(out2_acc_phase);
	 s3_instance->in_opcode(out2_opcode);
	 s3_instance->in_desired_phase(out2_desired_phase);
	 s3_instance->stage_num(stage3);
	 s3_instance->tablep(tablep_3);
	 s3_instance->tableh(tableh_3);
	 s3_instance->done(stage3_done);
	 s3_instance->out_x(out3_x);
	 s3_instance->out_y(out3_y);
	 s3_instance->out_acc_phase(out3_acc_phase);
	 s3_instance->out_opcode(out3_opcode);
	 s3_instance->out_desired_phase(out3_desired_phase);
	  
         // instantiate and connect stage 4
         s4_instance = new cordic("stage4_in_pipe");
         s4_instance->clk(clk);
         s4_instance->start(start);
         s4_instance->reset(reset);
         s4_instance->in_x(out3_x);
	 s4_instance->in_y(out3_y);
	 s4_instance->in_acc_phase(out3_acc_phase);
	 s4_instance->in_opcode(out3_opcode);
	 s4_instance->in_desired_phase(out3_desired_phase);
	 s4_instance->stage_num(stage4);
	 s4_instance->tablep(tablep_4);
	 s4_instance->tableh(tableh_4);
	 s4_instance->done(stage4_done);
	 s4_instance->out_x(out4_x);
	 s4_instance->out_y(out4_y);
	 s4_instance->out_acc_phase(out4_acc_phase);
	 s4_instance->out_opcode(out4_opcode);
	 s4_instance->out_desired_phase(out4_desired_phase);
	  
         // instantiate and connect stage 5
         s5_instance = new cordic("stage5_in_pipe");
         s5_instance->clk(clk);
         s5_instance->start(start);
         s5_instance->reset(reset);
         s5_instance->in_x(out4_x);
	 s5_instance->in_y(out4_y);
	 s5_instance->in_acc_phase(out4_acc_phase);
	 s5_instance->in_opcode(out4_opcode);
	 s5_instance->in_desired_phase(out4_desired_phase);
	 s5_instance->stage_num(stage5);
	 s5_instance->tablep(tablep_5);
	 s5_instance->tableh(tableh_5);
	 s5_instance->done(stage5_done);
	 s5_instance->out_x(out5_x);
	 s5_instance->out_y(out5_y);
	 s5_instance->out_acc_phase(out5_acc_phase);
	 s5_instance->out_opcode(out5_opcode);
	 s5_instance->out_desired_phase(out5_desired_phase);
	  
         // instantiate and connect stage 6
         s6_instance = new cordic("stage6_in_pipe");
         s6_instance->clk(clk);
         s6_instance->start(start);
         s6_instance->reset(reset);
         s6_instance->in_x(out5_x);
	 s6_instance->in_y(out5_y);
	 s6_instance->in_acc_phase(out5_acc_phase);
	 s6_instance->in_opcode(out5_opcode);
	 s6_instance->in_desired_phase(out5_desired_phase);
	 s6_instance->stage_num(stage6);
	 s6_instance->tablep(tablep_6);
	 s6_instance->tableh(tableh_6);
	 s6_instance->done(stage6_done);
	 s6_instance->out_x(out6_x);
	 s6_instance->out_y(out6_y);
	 s6_instance->out_acc_phase(out6_acc_phase);
	 s6_instance->out_opcode(out6_opcode);
	 s6_instance->out_desired_phase(out6_desired_phase);
	  
         // instantiate and connect stage 7
         s7_instance = new cordic("stage7_in_pipe");
         s7_instance->clk(clk);
         s7_instance->start(start);
         s7_instance->reset(reset);
         s7_instance->in_x(out6_x);
	 s7_instance->in_y(out6_y);
	 s7_instance->in_acc_phase(out6_acc_phase);
	 s7_instance->in_opcode(out6_opcode);
	 s7_instance->in_desired_phase(out6_desired_phase);
	 s7_instance->stage_num(stage7);
	 s7_instance->tablep(tablep_7);
	 s7_instance->tableh(tableh_7);
	 s7_instance->done(stage7_done);
	 s7_instance->out_x(out7_x);
	 s7_instance->out_y(out7_y);
	 s7_instance->out_acc_phase(out7_acc_phase);
	 s7_instance->out_opcode(out7_opcode);
	 s7_instance->out_desired_phase(out7_desired_phase);
	  
         // instantiate and connect stage 8
         s8_instance = new cordic("stage8_in_pipe");
         s8_instance->clk(clk);
         s8_instance->start(start);
         s8_instance->reset(reset);
         s8_instance->in_x(out7_x);
	 s8_instance->in_y(out7_y);
	 s8_instance->in_acc_phase(out7_acc_phase);
	 s8_instance->in_opcode(out7_opcode);
	 s8_instance->in_desired_phase(out7_desired_phase);
	 s8_instance->stage_num(stage8);
	 s8_instance->tablep(tablep_8);
	 s8_instance->tableh(tableh_8);
	 s8_instance->done(stage8_done);
	 s8_instance->out_x(out8_x);
	 s8_instance->out_y(out8_y);
	 s8_instance->out_acc_phase(out8_acc_phase);
	 s8_instance->out_opcode(out8_opcode);
	 s8_instance->out_desired_phase(out8_desired_phase);
	  
         // instantiate and connect stage 9
         s9_instance = new cordic("stage9_in_pipe");
         s9_instance->clk(clk);
         s9_instance->start(start);
         s9_instance->reset(reset);
         s9_instance->in_x(out8_x);
	 s9_instance->in_y(out8_y);
	 s9_instance->in_acc_phase(out8_acc_phase);
	 s9_instance->in_opcode(out8_opcode);
	 s9_instance->in_desired_phase(out8_desired_phase);
	 s9_instance->stage_num(stage9);
	 s9_instance->tablep(tablep_9);
	 s9_instance->tableh(tableh_9);
	 s9_instance->done(stage9_done);
	 s9_instance->out_x(out9_x);
	 s9_instance->out_y(out9_y);
	 s9_instance->out_acc_phase(out9_acc_phase);
	 s9_instance->out_opcode(out9_opcode);
	 s9_instance->out_desired_phase(out9_desired_phase);
	  
         // instantiate and connect stage 10
         s10_instance = new cordic("stage10_in_pipe");
         s10_instance->clk(clk);
         s10_instance->start(start);
         s10_instance->reset(reset);
         s10_instance->in_x(out9_x);
	 s10_instance->in_y(out9_y);
	 s10_instance->in_acc_phase(out9_acc_phase);
	 s10_instance->in_opcode(out9_opcode);
	 s10_instance->in_desired_phase(out9_desired_phase);
	 s10_instance->stage_num(stage10);
	 s10_instance->tablep(tablep_10);
	 s10_instance->tableh(tableh_10);
	 s10_instance->done(stage10_done);
	 s10_instance->out_x(out10_x);
	 s10_instance->out_y(out10_y);
	 s10_instance->out_acc_phase(out10_acc_phase);
	 s10_instance->out_opcode(out10_opcode);
	 s10_instance->out_desired_phase(out10_desired_phase);
	  
         // instantiate and connect stage 11
         s11_instance = new cordic("stage11_in_pipe");
         s11_instance->clk(clk);
         s11_instance->start(start);
         s11_instance->reset(reset);
         s11_instance->in_x(out10_x);
	 s11_instance->in_y(out10_y);
	 s11_instance->in_acc_phase(out10_acc_phase);
	 s11_instance->in_opcode(out10_opcode);
	 s11_instance->in_desired_phase(out10_desired_phase);
	 s11_instance->stage_num(stage11);
	 s11_instance->tablep(tablep_11);
	 s11_instance->tableh(tableh_11);
	 s11_instance->done(done);
	 s11_instance->out_x(out_x);
	 s11_instance->out_y(out_y);
	 s11_instance->out_acc_phase(out_acc_phase);
	 s11_instance->out_opcode(out_opcode);
	 s11_instance->out_desired_phase(out_desired_phase);
	 
    }

}; // end module cordpipe 
