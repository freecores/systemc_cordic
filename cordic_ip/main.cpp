// main.cpp: top-level design file
/********************************************************************
// 
// Module:
//   Top-level module for CORDIC design and testbench
//
// Implementation/Interface:
//   This module connects all submodules of CORDIC design as well as
//   the associated testbench
//
//
// Authors: 	Winnie Cheng <wwcheng@stanford.edu>,
//		Peter Wu <peter5@stanford.edu>
//
 *********************************************************************/

#include "systemc.h"
#include "testbench.h"
#include "decoder.h"
#include "cordpipe.h"
#include "adjust.h"
#include "monitor.h"
#include "opcode.h"
#include "table.h"

#define SIMULATION_TIME		100000

int sc_main(int argc, char* argv[]) {

    // system signals
    sc_signal<bool> reset;
    sc_signal<bool> sys_run; 
    sc_signal<bool> powerdown;

    // signals interfacing between testbench and cordic core
    sc_signal<bool> compute_done;
    sc_signal<sc_uint<UNIT_SEL_WIDTH> > adjust_opcode;
    sc_signal<bool> instruction_valid;
    sc_signal<sc_uint<UNIT_SEL_WIDTH> > engine_select;
    sc_signal<short> operand1;
    sc_signal<short> operand2;
    sc_signal<short> operand3;
    sc_signal<short> result1;
    sc_signal<short> result2;

    // signals interfacing between testbench and monitor
    sc_signal<bool> start_monitor;
    sc_signal<bool> monitor_idle;

    // pipe input
    sc_signal<bool> enable_pipeline;
    sc_signal<short> s0_x;
    sc_signal<short> s0_y;
    sc_signal<short> s0_acc_phase;
    sc_signal<short> s0_desired_phase;
    sc_signal<sc_uint<UNIT_SEL_WIDTH> > s0_opcode;
  
    // pipe output 
    sc_signal<bool> pipe_outvalid;
    sc_signal<short> pipe_x;
    sc_signal<short> pipe_y;
    sc_signal<short> pipe_acc_phase;
    sc_signal<short> pipe_desired_phase;
    sc_signal<sc_uint<UNIT_SEL_WIDTH> > pipe_opcode;
    
    // constants to parametrize pipeline module on system reset
    sc_signal<short> stage0;
    sc_signal<short> stage1;
    sc_signal<short> stage2;
    sc_signal<short> stage3;
    sc_signal<short> stage4;
    sc_signal<short> stage5;
    sc_signal<short> stage6;
    sc_signal<short> stage7;
    sc_signal<short> stage8;
    sc_signal<short> stage9;
    sc_signal<short> stage10;
    sc_signal<short> stage11;

    sc_signal<short> tablep_0;
    sc_signal<short> tablep_1;
    sc_signal<short> tablep_2;
    sc_signal<short> tablep_3;
    sc_signal<short> tablep_4;
    sc_signal<short> tablep_5;
    sc_signal<short> tablep_6;
    sc_signal<short> tablep_7;
    sc_signal<short> tablep_8;
    sc_signal<short> tablep_9;
    sc_signal<short> tablep_10;
    sc_signal<short> tablep_11;

    sc_signal<short> tableh_0;
    sc_signal<short> tableh_1;
    sc_signal<short> tableh_2;
    sc_signal<short> tableh_3;
    sc_signal<short> tableh_4;
    sc_signal<short> tableh_5;
    sc_signal<short> tableh_6;
    sc_signal<short> tableh_7;
    sc_signal<short> tableh_8;
    sc_signal<short> tableh_9;
    sc_signal<short> tableh_10;
    sc_signal<short> tableh_11;

    // system clock 
    sc_clock CLOCK("clock", 10);
  
    // testbench unit (Stimulus + Monitor)
    testbench testbench1("testbench");
    testbench1.clk(CLOCK);
    testbench1.reset(reset);
      // inputs
    testbench1.start(sys_run);
    testbench1.compute_done(compute_done);
    testbench1.result1(result1);
    testbench1.result2(result2);
    testbench1.monitor_idle(monitor_idle);
      // outputs
    testbench1.instructions_valid(instruction_valid);  
    testbench1.done(powerdown); 
    testbench1.start_monitor(start_monitor);
    testbench1.engine_select(engine_select);
    testbench1.operand1(operand1);
    testbench1.operand2(operand2);
    testbench1.operand3(operand3);

    // instantiate decoder
    decoder decoder1("decoder");
    decoder1.clk(CLOCK);
    decoder1.reset(reset);
      // inputs
    decoder1.start(instruction_valid);	// use this for power-save detect
    decoder1.instruction_valid(instruction_valid);
    decoder1.opcode(engine_select);
    decoder1.operand1(operand1);
    decoder1.operand2(operand2);
    decoder1.operand3(operand3);
    decoder1.enable_pipeline(enable_pipeline);
    decoder1.out_x(s0_x);
    decoder1.out_y(s0_y);
    decoder1.out_acc_phase(s0_acc_phase);
    decoder1.out_opcode(s0_opcode);
    decoder1.out_desired_phase(s0_desired_phase);

    // CORDIC pipeline
    cordpipe cordpipe1("cordpipe1");
    cordpipe1.clk(CLOCK);
    cordpipe1.reset(reset);
      // inputs
    cordpipe1.start(enable_pipeline);
    cordpipe1.in_x(s0_x);
    cordpipe1.in_y(s0_y);
    cordpipe1.in_acc_phase(s0_acc_phase);
    cordpipe1.in_opcode(s0_opcode);
    cordpipe1.in_desired_phase(s0_desired_phase);
      // outputs
    cordpipe1.done(pipe_outvalid);
    cordpipe1.out_x(pipe_x);
    cordpipe1.out_y(pipe_y);
    cordpipe1.out_acc_phase(pipe_acc_phase);
    cordpipe1.out_desired_phase(pipe_desired_phase);
    cordpipe1.out_opcode(pipe_opcode);
      // parametrization
    cordpipe1.stage0(stage0);
    cordpipe1.stage1(stage1);
    cordpipe1.stage2(stage2);
    cordpipe1.stage3(stage3);
    cordpipe1.stage4(stage4);
    cordpipe1.stage5(stage5);
    cordpipe1.stage6(stage6);
    cordpipe1.stage7(stage7);
    cordpipe1.stage8(stage8);
    cordpipe1.stage9(stage9);
    cordpipe1.stage10(stage10);
    cordpipe1.stage11(stage11);
    cordpipe1.tablep_0(tablep_0);
    cordpipe1.tablep_1(tablep_1);
    cordpipe1.tablep_2(tablep_2);
    cordpipe1.tablep_3(tablep_3);
    cordpipe1.tablep_4(tablep_4);
    cordpipe1.tablep_5(tablep_5);
    cordpipe1.tablep_6(tablep_6);
    cordpipe1.tablep_7(tablep_7);
    cordpipe1.tablep_8(tablep_8);
    cordpipe1.tablep_9(tablep_9);
    cordpipe1.tablep_10(tablep_10);
    cordpipe1.tablep_11(tablep_11);
    cordpipe1.tableh_0(tableh_0);
    cordpipe1.tableh_1(tableh_1);
    cordpipe1.tableh_2(tableh_2);
    cordpipe1.tableh_3(tableh_3);
    cordpipe1.tableh_4(tableh_4);
    cordpipe1.tableh_5(tableh_5);
    cordpipe1.tableh_6(tableh_6);
    cordpipe1.tableh_7(tableh_7);
    cordpipe1.tableh_8(tableh_8);
    cordpipe1.tableh_9(tableh_9);
    cordpipe1.tableh_10(tableh_10);
    cordpipe1.tableh_11(tableh_11);
    
    // instantiate adjust unit
    adjust adjust1("adjust1");
    adjust1.clk(CLOCK);
    adjust1.reset(reset);
      // inputs
    adjust1.in_valid(pipe_outvalid);
    adjust1.in_x(pipe_x);
    adjust1.in_y(pipe_y);
    adjust1.in_acc_phase(pipe_acc_phase);
    adjust1.in_opcode(pipe_opcode);
      // output
    adjust1.result_valid(compute_done);
    adjust1.out_opcode(adjust_opcode);
    adjust1.out_result1(result1);
    adjust1.out_result2(result2);

    // instantiate monitor unit
    monitor monitor1("monitor1");
    monitor1.clk(CLOCK);
    monitor1.reset(reset);
      // inputs
    monitor1.start(start_monitor);
    monitor1.in_valid(compute_done);
    monitor1.in_opcode(adjust_opcode);
    monitor1.in_result1(result1);
    monitor1.in_result2(result2);
    monitor1.idle(monitor_idle);

    // create trace file
    sc_trace_file * theTraceFile;
    theTraceFile = sc_create_vcd_trace_file("theTraceFile");
  
    // specify signals for trace 
    sc_trace(theTraceFile, CLOCK.signal(), "Clock");

    // parametrize pipeline modules
    stage0 = 0;
    stage1 = 1;
    stage2 = 2;
    stage3 = 3;
    stage4 = 4;
    stage5 = 5;
    stage6 = 6;
    stage7 = 7;
    stage8 = 8;
    stage9 = 9;
    stage10 = 10;
    stage11 = 11;

    tablep_0 = TABLEP_0;
    tablep_1 = TABLEP_1;
    tablep_2 = TABLEP_2;
    tablep_3 = TABLEP_3;
    tablep_4 = TABLEP_4;
    tablep_5 = TABLEP_5;
    tablep_6 = TABLEP_6;
    tablep_7 = TABLEP_7;
    tablep_8 = TABLEP_8;
    tablep_9 = TABLEP_9;
    tablep_10 = TABLEP_10;
    tablep_11 = TABLEP_11;

    tableh_0 = TABLEH_0;
    tableh_1 = TABLEH_1;
    tableh_2 = TABLEH_2;
    tableh_3 = TABLEH_3;
    tableh_4 = TABLEH_4;
    tableh_5 = TABLEH_5;
    tableh_6 = TABLEH_6;
    tableh_7 = TABLEH_7;
    tableh_8 = TABLEH_8;
    tableh_9 = TABLEH_9;
    tableh_10 = TABLEH_10;
    tableh_11 = TABLEH_11;

    // system reset
    reset = true;
    sc_start(3);
    reset = false;

    // simulation runtime
    sys_run = true;
    sc_start(SIMULATION_TIME);

    // close trace file
    sc_close_vcd_trace_file(theTraceFile);

    return(0);
}
