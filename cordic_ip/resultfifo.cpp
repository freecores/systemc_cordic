// resultfifo.cpp: source file
/********************************************************************
// 
// Module:
//   Pipeline Result FIFO 
//
// Implementation:
//   This module buffers the result from the cordic pipeline.
//
//
// Authors: 	Winnie Cheng <wwcheng@stanford.edu>,
//		Peter Wu <peter5@stanford.edu>
//
 *********************************************************************/

#include "resultfifo.h"
#include "opcode.h"

#ifdef MODULE_NAME
#undef MODULE_NAME
#endif

#define MODULE_NAME	"resultfifo"

#define DEBUG		1
#if DEBUG
#define dprintf		printf
#else
#define dprintf
#endif

#define FIFODEPTH	12

void resultfifo::resultfifo_process()
{
    bool readreq; 
    bool writereq;
    short num_elements, first;
    
    // FIFO
    sc_uint<UNIT_SEL_WIDTH> opcode[FIFODEPTH];
    short result1[FIFODEPTH];
    short result2[FIFODEPTH];

    // initialize fifo status 
    num_elements = 0;
    first = 0;

    // On Reset, initialize output
    out_valid.write(false);
    out_opcode.write(I_NOP);
    out_result1.write(0);
    out_result2.write(0);
    wait();

    while(1) {

        wait();
        
        // sample the requests
        readreq = read_request.read();
        writereq = write_request.read();  

        // handle write requests if any
        if(writereq) {
            if(num_elements == FIFODEPTH) {
                // no room left
                printf("[%s] WARNING: FIFO Overflow, result dropped\n", MODULE_NAME); 
            } else {
                // space available, store the data
                opcode[(first+num_elements)%FIFODEPTH] = in_opcode.read();
                result1[(first+num_elements)%FIFODEPTH] = in_result1.read();
                result2[(first+num_elements)%FIFODEPTH] = in_result2.read();
                num_elements++;
            }
        }

        // then handle read requests if any
        if(readreq) {
            if(num_elements == 0) {
                // fifo is empty
                out_valid.write(false);
            } else {
                // read out earliest entry
                out_opcode.write(opcode[first]); 
                out_result1.write(result1[first]); 
                out_result2.write(result2[first]); 
                out_valid.write(true);
                printf("[%s] Read opcode 0x%x\n", MODULE_NAME, (unsigned short) opcode[first]); 
		// update fifo status
                num_elements--;
                first = (first+1)%FIFODEPTH; 
            }
        }
    
    } // forever loop	
}
