// testbench.cpp: source file
/********************************************************************
// 
// Module:
//   Testbench feeding instructions into Cordic core
//
// Implementation:
//   This module tests the Rotate, Magnitude-Phase, Sin-Cos and
//   Sinh-Cosh CORDIC engines.
//
//
// Authors: 	Winnie Cheng<wwcheng@stanford.edu>,
//		Peter Wu<peter5@stanford.edu>
//
 *********************************************************************/

#include "testbench.h"
#include "opcode.h"
#include "convert.h"
#include <math.h>

#define ROTATE_NUM_TEST		100	
#define MAGPHASE_NUM_TEST       100	
#define SINCOS_NUM_TEST         100	
#define SINHCOSH_NUM_TEST       100	

#ifdef MODULE_NAME
#undef MODULE_NAME
#endif

#define MODULE_NAME	"[testbench]"

#define PROGFILE	"userprog.dat"
#define LOGFILE		"testpipe.log"
#define MAX_PROGSIZE	24
#define MAX_STR_LEN	128

void testbench::testbench_process()
{
    short i;
    double x, y, a;
    double rad, correct_x, correct_y;
    double computed_v1, computed_v2;
    int fixed1, fixed2;
    bool success, skip;
    int retval;
    char ir[MAX_STR_LEN];
    char op[3][MAX_STR_LEN];
    int file_opcode;
    int num_instr;
    FILE *logfile;

    // Opens user program file for reading
    FILE *infile = fopen(PROGFILE, "r");
    
    if(!infile) {
        cout << MODULE_NAME << "File does not exist: " << PROGFILE << endl;
	exit(-1);
    }
    
    // Initialize output
    
    done.write(false);
    instructions_valid.write(false);
    start_monitor.write(false);
    wait();

    wait_until(start.delayed()==true);
    printf("%s Testbench for CORDIC engines\n", MODULE_NAME);
    success = true;

    // Test Rotate Engine
    for(i = 0; i < ROTATE_NUM_TEST; i++) {
        wait();
    
        // Generate random input
        a = 1.0*((rand()>>2)%360)-180.0;
        x = 1.0*((rand()>>2)%100);
        y = 1.0*((rand()>>2)%100);     
        cout << MODULE_NAME << "Rotate(" << x << ", " << y << ", " << a << ")" << endl;

        // activate the rotate engine
        engine_select.write(I_ROTATE);	
        operand1.write(toint(x));	// orgX
        operand2.write(toint(y));	// orgY
        operand3.write(toint(a));	// angle
        instructions_valid.write(true);
        wait();

        // pulse instructions valid
        instructions_valid.write(false);
        wait_until(compute_done.delayed()==true);

	// Read Computed Results
	computed_v1 = tofp(result1.read());
	computed_v2 = tofp(result2.read());

	// Check Result
        rad=(3.1415926*a)/180.0;
        correct_x=x*cos(rad)-y*sin(rad); /* Perform "perfect" rotation by  */
        correct_y=y*cos(rad)+x*sin(rad); /* using f.p. and transcendentals */

        if ((fabs(computed_v1-correct_x)>0.15) || 
	    (fabs(computed_v2-correct_y)>0.15)) {
	    cout << MODULE_NAME << "ERROR computed=" << computed_v1 << ",";
	    cout << computed_v2 << " expected=" << correct_x << ",";
	    cout << correct_y << endl;
	    success = false;
        } else {
	    cout << MODULE_NAME << "CORRECT" << endl;
        }

	wait();

    } // end Test Rotate Engine

    // Test MagPhase Engine
    for(i = 0; i < MAGPHASE_NUM_TEST; i++) {
        wait();
        x = 2.0*((rand()>>2)%100) - 100;
        y = 2.0*((rand()>>2)%100) - 100;     
        cout << MODULE_NAME << "Mag and Phase(" << x << ", " << y << ") " << endl;

        // activate the rotate engine
        engine_select.write(I_MAGPHASE);	
        operand1.write(toint(x));	// orgX
        operand2.write(toint(y));	// orgY
        instructions_valid.write(true);
        wait();

        // pulse instructions valid
        instructions_valid.write(false);
        wait_until(compute_done.delayed()==true);

	// Read Computed Results
	computed_v1 = tofp(result1.read());
	computed_v2 = tofp(result2.read());

	// Check Result
        correct_x=sqrt(x*x+y*y); /* Perform "perfect" magnitude */ 
        correct_y=atan(y/x)*180/3.1415926;

        if (x<0 && y>0)
           correct_y = 180 + correct_y;
        if (x<0 && y<=0)
           correct_y = -180 + correct_y;

	if ((fabs(computed_v1-correct_x)>0.15) || 
	    (fabs(computed_v2-correct_y)>0.15)) {
	    cout << MODULE_NAME << "ERROR computed=" << computed_v1 << ",";
	    cout << computed_v2 << " expected=" << correct_x << ",";
	    cout << correct_y << endl;
	    success = false;
        } else {
	    cout << MODULE_NAME << "CORRECT" << endl;
        }

	wait();

    } // end Test Magnitude and Phase Engine

    // Test Sine Cosine Engine
    for(i = 0; i < SINCOS_NUM_TEST; i++) {
        wait();
        a = 1.0*((rand()>>2)%360)-180.0;
        cout << MODULE_NAME << "Sine and Cosine(" << a << ") " << endl;

        // activate the rotate engine
        engine_select.write(I_SINCOS);	
        operand1.write(toint(a));	// orgX
        instructions_valid.write(true);
        wait();

        // pulse instructions valid
        instructions_valid.write(false);
        wait_until(compute_done.delayed()==true);

	// Read Computed Results
	computed_v1 = tofp(result1.read());
	computed_v2 = tofp(result2.read());

	// Check Result
        rad=(3.1415926*a)/180.0;
        correct_x=sin(rad); /* Perform "perfect" Cosine */ 
        correct_y=cos(rad);
        if ((fabs(computed_v1-correct_x)>0.06) || 
	    (fabs(computed_v2-correct_y)>0.06)) {
	    cout << MODULE_NAME << "ERROR computed=" << computed_v1 << ",";
	    cout << computed_v2 << " expected=" << correct_x << ",";
	    cout << correct_y << endl;
	    success = false;
        } else {
	    cout << MODULE_NAME << "CORRECT" << endl;
        }

	wait();

    } // end Test Sine and Cosine Engine

    // Test Sinh Cosh Engine
    for(i = 0; i < SINHCOSH_NUM_TEST; i++) {
        wait();
        a = 1.0*((rand()>>2)%90)-45.0;
	if(a < 0 && a > -3) a = -3;	// adjust for algorithm inaccuracies
	cout << MODULE_NAME << "Sinh and Cosh(" << a << ") " << endl;
        // activate the rotate engine
        engine_select.write(I_SINHCOSH);	
        operand1.write(toint(a));	// orgX
        instructions_valid.write(true);
        wait();

        // pulse instructions valid
        instructions_valid.write(false);
        wait_until(compute_done.delayed()==true);

	// Read Computed Results
	computed_v1 = tofp(result1.read());
	computed_v2 = tofp(result2.read());

	// Check Result
        rad=(3.1415926*a)/180.0;
        correct_x=sinh(rad); /* Perform "perfect" Cosine */ 
        correct_y=cosh(rad);

        if ((fabs(computed_v1-correct_x)>0.06) || 
	    (fabs(computed_v2-correct_y)>0.06)) {
	    cout << MODULE_NAME << "ERROR computed=" << computed_v1 << ",";
	    cout << computed_v2 << " expected=" << correct_x << ",";
	    cout << correct_y << endl;
	    success = false;
        } else {
	    cout << MODULE_NAME << "CORRECT" << endl;
        }

	wait();

    } // end Test Sinh and Cosh Engine
 
    // Testing Pipeline
    wait();
    cout << MODULE_NAME << "Testing Pipeline Mode" << endl;
    start_monitor.write(true);
    wait();

    // Translates user program
    i = 0; 

    while(i < MAX_PROGSIZE) {
      wait();

      retval = fscanf(infile, "%s", ir);
      if(retval==EOF) break;
      if(!strcmp(ir, "rot")) {
	  // Rotate type functions
	  printf("%s Opcode ROTATE[%s]: ", MODULE_NAME, ir);
	  // get operands
	  if((retval=fscanf(infile, "%s %s %s\n", op[0], op[1], op[2]))!= 3) {
	    printf("Parse Error\n");
	    break;
	  } else {
            printf("[%s], [%s], [%s]\n", op[0], op[1], op[2]);
	    
	    engine_select.write(I_ROTATE);
	    operand1.write(toint(atof(op[0])));
	    operand2.write(toint(atof(op[1])));
	    operand3.write(toint(atof(op[2])));
	    instructions_valid.write(true);
          }	    
      } else if(!strcmp(ir, MAG_OPNAME)){
	  // Magnitude-Phase type functions
	  printf("%s Opcode MAG-PHASE[%s]: ", MODULE_NAME, ir);
          // get operands
	  if((retval=fscanf(infile, "%s %s\n", op[0], op[1]))!= 2) {
	    printf("Parse Error\n");
	    break;
	  } else {
            printf("%s, %s\n", op[0], op[1]);
	    engine_select.write(I_MAGPHASE);
	    operand1.write(toint(atof(op[0])));
	    operand2.write(toint(atof(op[1])));
	    instructions_valid.write(true);
	  }
      } else if(!strcmp(ir, SIN_OPNAME) || !strcmp(ir, COS_OPNAME)) {
	  // Trig type functions
	  printf("%s Opcode SIN-COS[%s]: ", MODULE_NAME, ir);
          // get operands
	  if((retval=fscanf(infile, "%s\n", op[0]))!= 1) { 
	    printf("Parse Error\n");
	    break;
	  } else {
            printf("%s\n", op[0]);
	    engine_select.write(I_SINCOS);
	    operand1.write(toint(atof(op[0])));
            instructions_valid.write(true);
	  }
      } else if(!strcmp(ir, SINH_OPNAME) || !strcmp(ir, COSH_OPNAME)) {
	  // Trig Hyperbolic type functions
	  printf("%s Opcode SINH-COSH[%s]: ", MODULE_NAME, ir);
          // get operands
	  if((retval=fscanf(infile, "%s\n", op[0]))!= 1) {
	    printf("Parse Error\n");
	    break;
	  } else {
            printf("%s\n", op[0]);
	    engine_select.write(I_SINHCOSH);
	    operand1.write(toint(atof(op[0])));
	    instructions_valid.write(true);
	  }	  
      } else if (!strcmp(ir, NOP_OPNAME)) {
	  // NOP function
	  printf("%s Opcode NOP[%s]\n", MODULE_NAME, ir);
	  engine_select.write(I_NOP);
	  instructions_valid.write(true);
      } else {
          printf("Unknown Opcode %s\n", ir);
	  engine_select.write(I_NOP);
	  instructions_valid.write(false);
	  break;
      }
      i++;
    }

    if(i == MAX_PROGSIZE) wait();

    instructions_valid.write(false);
    fclose(infile);
    cout << MODULE_NAME << "Interpreted " << i << " instructions\n";     
    num_instr = i;

    wait_until(monitor_idle.delayed()==true);
    start_monitor.write(false);
    wait();
    
    // Check results
    cout << MODULE_NAME << "Checking Pipeline Mode results" << endl; 
    infile = fopen(PROGFILE, "r");
    logfile = fopen(LOGFILE, "r");
    if(!infile) {
        cout << MODULE_NAME << "Prog file cannot be opened: " << PROGFILE << endl;
	exit(-1);
    }
    if(!logfile) {
        cout << MODULE_NAME << "Log file cannot be opened: " << LOGFILE << endl;
	exit(-1);
    }	

    i = 0;
    skip = false;

    while(i < MAX_PROGSIZE) {
      wait();

      retval = fscanf(infile, "%s", ir);
      if(retval==EOF) break;
      if(skip==false) {
          retval = fscanf(logfile, "%d %x %x\n", &file_opcode, &fixed1, &fixed2);
          if(retval!=3) {
	      cout << MODULE_NAME << "ERROR mismatch num_arg in " << LOGFILE << " found " << retval << endl;
	      break;
	  }
	  computed_v1 = tofp((short)fixed1);
	  computed_v2 = tofp((short)fixed2);
      }
      skip = false;
      if(!strcmp(ir, "rot")) {
	  // Rotate type functions
	  printf("%s Opcode ROTATE[%s]: ", MODULE_NAME, ir);
	  // get operands
	  if((retval=fscanf(infile, "%s %s %s\n", op[0], op[1], op[2]))!= 3) {
	    printf("Parse Error\n");
	    break;
	  } else {
            printf("[%s], [%s], [%s]\n", op[0], op[1], op[2]);
            // Check 
	    rad=(3.1415926*atof(op[2]))/180.0;
            correct_x=atof(op[0])*cos(rad)-atof(op[1])*sin(rad);
            correct_y=atof(op[1])*cos(rad)+atof(op[0])*sin(rad);

            if ((fabs(computed_v1-correct_x)>0.15) || 
	        (fabs(computed_v2-correct_y)>0.15)) {
	        cout << MODULE_NAME << "ERROR computed=" << computed_v1 << ",";
	        cout << computed_v2 << " expected=" << correct_x << ",";
	        cout << correct_y << endl;
	        success = false;
            } else {
	        cout << MODULE_NAME << "CORRECT" << endl;
            }
          }	    
      } else if(!strcmp(ir, MAG_OPNAME)){
	  // Magnitude-Phase type functions
	  printf("%s Opcode MAG-PHASE[%s]: ", MODULE_NAME, ir);
          // get operands
	  if((retval=fscanf(infile, "%s %s\n", op[0], op[1]))!= 2) {
	    printf("Parse Error\n");
	    break;
	  } else {
            printf("%s, %s\n", op[0], op[1]);
	    // Check
	    x = atof(op[0]);
	    y = atof(op[1]);
            correct_x=sqrt(x*x+y*y); 
            correct_y=atan(y/x)*180/3.1415926;

            if (x<0 && y>0)
                correct_y = 180 + correct_y;
            if (x<0 && y<=0)
                correct_y = -180 + correct_y;

	    if ((fabs(computed_v1-correct_x)>0.15) || 
	        (fabs(computed_v2-correct_y)>0.15)) {
	        cout << MODULE_NAME << "ERROR computed=" << computed_v1 << ",";
	        cout << computed_v2 << " expected=" << correct_x << ",";
	        cout << correct_y << endl;
	        success = false;
            } else {
	        cout << MODULE_NAME << "CORRECT" << endl;
            }
	  }
      } else if(!strcmp(ir, SIN_OPNAME) || !strcmp(ir, COS_OPNAME)) {
	  // Trig type functions
	  printf("%s Opcode SIN-COS[%s]: ", MODULE_NAME, ir);
          // get operands
	  if((retval=fscanf(infile, "%s\n", op[0]))!= 1) { 
	    printf("Parse Error\n");
	    break;
	  } else {
            printf("%s\n", op[0]);
	    // Check
            rad=(3.1415926*atof(op[0]))/180.0;
            correct_x=sin(rad); /* Perform "perfect" Cosine */ 
            correct_y=cos(rad);
            if ((fabs(computed_v1-correct_x)>0.06) || 
	        (fabs(computed_v2-correct_y)>0.06)) {
	        cout << MODULE_NAME << "ERROR computed=" << computed_v1 << ",";
	        cout << computed_v2 << " expected=" << correct_x << ",";
	        cout << correct_y << endl;
	        success = false;
            } else {
	        cout << MODULE_NAME << "CORRECT" << endl;
            }
	  }
      } else if(!strcmp(ir, SINH_OPNAME) || !strcmp(ir, COSH_OPNAME)) {
	  // Trig Hyperbolic type functions
	  printf("%s Opcode SINH-COSH[%s]: ", MODULE_NAME, ir);
          // get operands
	  if((retval=fscanf(infile, "%s\n", op[0]))!= 1) {
	      printf("Parse Error\n");
	      break;
	  } else {
             printf("%s\n", op[0]);
	     // Check
             rad=(3.1415926*atof(op[0]))/180.0;
             correct_x=sinh(rad); /* Perform "perfect" Cosine */ 
             correct_y=cosh(rad);
	    
             if ((fabs(computed_v1-correct_x)>0.06) || 
	        (fabs(computed_v2-correct_y)>0.06)) {
	         cout << MODULE_NAME << "ERROR computed=" << computed_v1 << ",";
	         cout << computed_v2 << " expected=" << correct_x << ",";
	         cout << correct_y << endl;
	         success = false;
             } else {
	         cout << MODULE_NAME << "CORRECT" << endl;
             }
	  }	  
      } else if (!strcmp(ir, NOP_OPNAME)) {
	  // NOP function
	  printf("%s Opcode NOP[%s]\n", MODULE_NAME, ir);
	  skip = true;
      } else {
          printf("Unknown Opcode %s\n", ir);
	  break;
      }
      i++;
    }
    if(i != num_instr) {
        cout << MODULE_NAME << "ERROR num of instruction mismatched " ;
        cout << i << " vs " << num_instr << endl;    
	success = false;
    } else {
        cout << MODULE_NAME << "Pipeline processed " << i << " instructions" <<endl;
    }	
    fclose(infile);
    fclose(logfile);

    cout << MODULE_NAME << "Testbench completed - ";
    if(success) {
        cout << "SUCCESS" << endl;
    } else {
        cout << "FAILED" << endl;
    }
    done.write(true);
    // end of test vectors
}
