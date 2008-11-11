#include "lolevel.h"
#include "platform.h"
#include "core.h"

#define RAWDATA_AVAILABLE (1)
#define RAWDATA_SAVED (2)

#define NR_ON (2)
#define NR_OFF (1)

static long raw_save_stage;

void capt_seq_hook_raw_here()
{
    raw_save_stage = RAWDATA_AVAILABLE;
    core_rawdata_available();
    while (raw_save_stage != RAWDATA_SAVED){
	_SleepTask(10);
    }
}

void hook_raw_save_complete()
{
    raw_save_stage = RAWDATA_SAVED;
}


void capt_seq_hook_set_nr()
{
    long *nrflag = (long*)0xCF74;

    switch (core_get_noise_reduction_value()){
    case NOISE_REDUCTION_AUTO_CANON:
	// leave it alone
	break;
    case NOISE_REDUCTION_OFF:
	*nrflag = 1;
	break;
    case NOISE_REDUCTION_ON:
	*nrflag = 2;
	break;
    };
}

void __attribute__((naked,noinline)) sub_FFB0D8CC_my(long p)
{
    asm volatile (
                
								"STMFD   SP!, {R4,LR}\n"
								"MOV     R4, R0\n"
								"SUB     SP, SP, #0xC\n"
								"BL      sub_FFB0E33C\n"
								"MVN     R1, #0\n"
								"BL      sub_FFB1EAAC\n"	// ClearEventFlag
								"MOV     R0, #0x8A\n"
								"ADD     R1, SP, #4\n"
								"MOV     R2, #4\n"
								"BL      sub_FF81BCAC\n"
								"TST     R0, #1\n"
								"BEQ     loc_FFB0D90C\n"
								"MOV     R1, #0x1D0\n"
								"LDR     R0, =0xFFB0D750\n"	// aSscaptureseq_c
								"ADD     R1, R1, #2\n"
								"BL      sub_FFB2C138\n"		// DebugAssert
"loc_FFB0D90C:\n"
								"LDR     R3, =0xBE160\n"
								"LDR     R2, =0xBE220\n"
								"LDR     R0, [R3,#0x7C]\n"
								"LDRSH   R1, [R2,#0xE]\n"
								"BL      sub_FFA44DE0\n"
								"MOV     R0, R4\n"
								"BL      sub_FFB0D6D4\n"
								"BL      capt_seq_hook_set_nr\n"  // +
								"LDR     R3, =0xCF78\n"
								"LDR     R0, [R3]\n"
								"B       sub_FFB0D930\n"
    );
}

void __attribute__((naked,noinline)) sub_FFB0A6F4_my(long p)
{
    asm volatile (
                    
							"STMFD   SP!, {R4,R5,LR}\n"
							"LDR     R3, =0xBE160\n"
							"LDR     R5, [R0,#0xC]\n"
							"LDR     R1, [R3,#0x24]\n"
							"LDR     R2, [R5,#8]\n"
							"CMP     R1, #0\n"
							"ORR     R2, R2, #1\n"
							"STR     R2, [R5,#8]\n"
							"BNE     loc_FFB0A748\n"
							"MOV     R0, #0xC\n"
							"BL      sub_FFB14C44\n"
							"TST     R0, #1\n"
							"BEQ     loc_FFB0A748\n"
							"LDR     R3, [R5,#8]\n"
							"MOV     R0, #1\n"
							"ORR     R3, R3, #0x40000000\n"
							"STR     R3, [R5,#8]\n"
"loc_FFB0A738:\n"
							"MOV     R2, R5\n"
							"MOV     R1, #1\n"
							"LDMFD   SP!, {R4,R5,LR}\n"
							"B       sub_FFB08E9C\n"
"loc_FFB0A748:\n"
							"LDR     R4, =0xBE160\n"
							"BL      sub_FFB0B1EC\n"		// Set_CMD25Write_62
							"LDR     R3, [R4,#0x24]\n"
							"CMP     R3, #0\n"
							"BNE     loc_FFB0A790\n"
							"MOV     R0, R5\n"
							"BL      sub_FFB0C3D0\n"
							"TST     R0, #1\n"
							"BNE     loc_FFB0A738\n"
							"BL      sub_FF82668C\n"
							"BL      sub_FF81BEA8\n"
							"STR     R0, [R5,#0x14]\n"
							"MOV     R0, R5\n"
							"BL      sub_FFB0D7B4\n"
							"BL      sub_FFB0E1A8\n"
							"MOV     R0, R5\n"
							"BL      sub_FFB0D8CC_my\n"	  //------------>
              "BL      capt_seq_hook_raw_here\n"  // +							
							"B       loc_FFB0A7A4\n"
"loc_FFB0A790:\n"
							"LDR     R3, =0xCF60\n"
							"LDR     R2, [R3]\n"
							"CMP     R2, #0\n"
							"MOVNE   R0, #0x1D\n"
							"MOVEQ   R0, #0\n"
"loc_FFB0A7A4:\n"
							"MOV     R1, #1\n"
							"MOV     R2, R5\n"
							"BL      sub_FFB08E9C\n"
							"BL      sub_FFB0DC48\n"
							"CMP     R0, #0\n"
							"LDRNE   R3, [R5,#8]\n"
							"ORRNE   R3, R3, #0x2000\n"
							"STRNE   R3, [R5,#8]\n"
							"LDMFD   SP!, {R4,R5,PC}\n"                
    );
}

void __attribute__((naked,noinline)) capt_seq_task()
{
	asm volatile (
	                
							"STMFD   SP!, {R4,LR}\n"
							"SUB     SP, SP, #4\n"
							"MOV     R4, SP\n"
							"B       loc_FFB0AD2C\n"
"loc_FFB0ABB0:\n"
							"LDR     R2, [SP]\n"
							"LDR     R3, [R2]\n"
							"MOV     R0, R2\n"
							"CMP     R3, #0x15\n"
							"LDRLS   PC, [PC,R3,LSL#2]\n"
							"B       loc_FFB0AD00\n"
							".long   loc_FFB0AC20\n"
							".long   loc_FFB0AC40\n"
							".long   loc_FFB0AC54\n"
							".long   loc_FFB0AC68\n"
							".long   loc_FFB0AC60\n"
							".long   loc_FFB0AC70\n"
							".long   loc_FFB0AC78\n"
							".long   loc_FFB0AC84\n"
							".long   loc_FFB0AC8C\n"
							".long   loc_FFB0AC98\n"
							".long   loc_FFB0ACA0\n"
							".long   loc_FFB0ACA8\n"
							".long   loc_FFB0ACB0\n"
							".long   loc_FFB0ACB8\n"
							".long   loc_FFB0ACC0\n"
							".long   loc_FFB0ACCC\n"
							".long   loc_FFB0ACD4\n"
							".long   loc_FFB0ACDC\n"
							".long   loc_FFB0ACE4\n"
							".long   loc_FFB0ACF0\n"
							".long   loc_FFB0ACF8\n"
							".long   loc_FFB0AD14\n"
"loc_FFB0AC20:\n"
							"BL      sub_FFB0B214\n"
              "BL      shooting_expo_param_override\n" // +							
							"BL      sub_FFB08924\n"
							"LDR     R3, =0xBE160\n"
							"LDR     R2, [R3,#0x24]\n"
							"CMP     R2, #0\n"
							"BEQ     loc_FFB0AD10\n"
							"BL      sub_FFB0A7D0\n"
							"B       loc_FFB0AD10\n"
"loc_FFB0AC40:\n"
							"BL      sub_FFB0A6F4_my\n"	//------------>
"loc_FFB0AC44:\n"
							"LDR     R2, =0xBE160\n"
							"MOV     R3, #0\n"
							"STR     R3, [R2,#0x24]\n"
							"B       loc_FFB0AD10\n"
"loc_FFB0AC54:\n"
							"MOV     R0, #1\n"
							"BL      sub_FFB0B434\n"
							"B       loc_FFB0AD10\n"
"loc_FFB0AC60:\n"
							"BL      sub_FFB0AE14\n"
							"B       loc_FFB0AC44\n"
"loc_FFB0AC68:\n"
							"BL      sub_FFB0B1CC\n"		// BackLightDrv_LcdBackLightOff_15
							"B       loc_FFB0AC44\n"
"loc_FFB0AC70:\n"
							"BL      sub_FFB0B1DC\n"
							"B       loc_FFB0AD10\n"
"loc_FFB0AC78:\n"
							"BL      sub_FFB0B32C\n"
							"BL      sub_FFB08924\n"
							"B       loc_FFB0AD10\n"
"loc_FFB0AC84:\n"
							"BL      sub_FFB0A8AC\n"
							"B       loc_FFB0AD10\n"
"loc_FFB0AC8C:\n"
							"BL      sub_FFB0B39C\n"
							"BL      sub_FFB08924\n"
							"B       loc_FFB0AD10\n"
"loc_FFB0AC98:\n"
							"BL      sub_FFB0B1CC\n"		// BackLightDrv_LcdBackLightOff_15
							"B       loc_FFB0AD10\n"
"loc_FFB0ACA0:\n"
							"BL      sub_FFB0CB04\n"
							"B       loc_FFB0AD10\n"
"loc_FFB0ACA8:\n"
							"BL      sub_FFB0CCD8\n"
							"B       loc_FFB0AD10\n"
"loc_FFB0ACB0:\n"
							"BL      sub_FFB0CD6C\n"
							"B       loc_FFB0AD10\n"
"loc_FFB0ACB8:\n"
							"BL      sub_FFB0CE68\n"
							"B       loc_FFB0AD10\n"
"loc_FFB0ACC0:\n"
							"MOV     R0, #0\n"
							"BL      sub_FFB0D0D4\n"
							"B       loc_FFB0AD10\n"
"loc_FFB0ACCC:\n"
							"BL      sub_FFB0D2A8\n"
							"B       loc_FFB0AD10\n"
"loc_FFB0ACD4:\n"
							"BL      sub_FFB0D344\n"
							"B       loc_FFB0AD10\n"
"loc_FFB0ACDC:\n"
							"BL      sub_FFB0D404\n"
							"B       loc_FFB0AD10\n"
"loc_FFB0ACE4:\n"
							"BL      sub_FFB0B588\n"
							"BL      sub_FFB0A618\n"
							"B       loc_FFB0AD10\n"
"loc_FFB0ACF0:\n"
							"BL      sub_FFB0CFA8\n"
							"B       loc_FFB0AD10\n"
"loc_FFB0ACF8:\n"
							"BL      sub_FFB0D004\n"
							"B       loc_FFB0AD10\n"
"loc_FFB0AD00:\n"
							"MOV     R1, #0x4C0\n"
							"LDR     R0, =0xFFB0A474\n"		// aSsshoottask_c
							"ADD     R1, R1, #0xE\n"
							"BL      sub_FFB2C138\n"		// DebugAssert
"loc_FFB0AD10:\n"
							"LDR     R2, [SP]\n"
"loc_FFB0AD14:\n"
							"LDR     R3, =0x97A30\n"
							"LDR     R1, [R2,#4]\n"
							"LDR     R0, [R3]\n"
							"BL      sub_FFB1E910\n"		// SetEventFlag
							"LDR     R0, [SP]\n"
							"BL      sub_FFB0A4F4\n"
"loc_FFB0AD2C:\n"
							"LDR     R3, =0x97A34\n"
							"MOV     R1, R4\n"
							"LDR     R0, [R3]\n"
							"MOV     R2, #0\n"
							"BL      sub_FFB1F028\n"		// ReceiveMessageQueue
							"TST     R0, #1\n"
							"BEQ     loc_FFB0ABB0\n"
							"LDR     R0, =0xFFB0A474\n"		// aSsshoottask_C
							"MOV     R1, #0x400\n"
							"BL      sub_FFB2C138\n"		// DebugAssert
							"BL      sub_FFB205EC\n"
							"ADD     SP, SP, #4\n"
							"LDMFD   SP!, {R4,PC}\n"                
	);
}

