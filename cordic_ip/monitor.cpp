// monitor.cpp: source file
/********************************************************************
// 
// Module:
//   Pipeline Output Monitor
//
// Implementation:
//   Upon activation of this module, it writes result to a file. The
//   file is closed after a certain cycles of in-activity from
//   the adjust module.
//
//
// Authors: 	Winnie Cheng <wwcheng@stanford.edu>,
//		Peter Wu <peter5@stanford.edu>
//
 *********************************************************************/

#include "monitor.h"
#include "opcode.h"

#ifdef MODULE_NAME
#undef MODULE_NAME
#endif

#define MODULE_NAME	"monitor"

#define DEBUG		1
#if DEBUG
#define dprintf		printf
#else
#define dprintf
#endif

#define LOGFILE		"testpipe.log"
#define IDLE_SLACK	24	// tolerate #NOPs before closing file

void monitor::monitor_process()
{
    sc_uint<UNIT_SEL_WIDTH> opcode;
    short result1, result2;
    short idle_cycles;
    bool oneshot;
    short i;

    FILE *outfile = fopen(LOGFILE, "w");

    // On Reset, initialize output
    idle.write(false); 
    idle_cycles = 0; 
    oneshot = true;	// captures only once 
    wait();

    // wait for signal to capture
    wait_until(start.delayed()==true);
    dprintf("[%s] Monitor Activated\n", MODULE_NAME);
    i = 0;

    while(1) {    
        wait();        
	if(in_valid.read()==true && oneshot == true){
            i++;	   
	    idle_cycles = 0;	// reset
	    
	    // Register input
            opcode = in_opcode.read(); 
	    result1 = in_result1.read();
	    result2 = in_result2.read();
            
	    fprintf(outfile, "%d 0x%x 0x%x\n", (int) opcode, result1, result2);
            dprintf("[%s] Num of captures = %d\n", MODULE_NAME, i);
        } else {
	    
	    if(idle_cycles > IDLE_SLACK) {
	        // turn off monitor
		if(oneshot) {
		    fclose(outfile);	// close file once
		    dprintf("[%s] Closing %s\n", MODULE_NAME, LOGFILE);
		}    
		oneshot = false;
		idle.write(true);
	    } else {
	        idle_cycles++;
	    }	
        }
    } // forever loop	
}
