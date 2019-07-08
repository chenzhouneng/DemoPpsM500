/*
 * InesaTiming.h
 *
 *  Created on: 2017-7-20
 *      Author: czn
 */

#ifndef INESATIMING_H_
#define INESATIMING_H_

#include <sys/neutrino.h>
#include <sys/syspage.h>
#include <stdint.h>
#include "ProgramEnvVar.h"

#define DEBUG_TIMING
#ifdef DEBUG_TIMING
extern uint64_t it_cycles_per_sec;
extern uint64_t it_start;

#define INESA_TIMING_INIT()		\
	do { \
		if(0 < ProgramEnvVar::m_nPrintTiming) { \
			it_cycles_per_sec = SYSPAGE_ENTRY(qtime)->cycles_per_sec; \
		} \
	} while(0)

#define INESA_TIMING_START() \
	do { \
		if(0 < ProgramEnvVar::m_nPrintTiming) { \
			 it_start = ClockCycles(); \
		} \
	} while(0)

#define INESA_TIMING_STOP(msg) \
	do { \
		if(0 < ProgramEnvVar::m_nPrintTiming) { \
			uint64_t it_stop = ClockCycles(); \
			double it_deltatime = (it_stop - it_start); \
			it_deltatime = (it_deltatime  / it_cycles_per_sec) * 1000; \
			printf("INESA TIMING->%s %f ms\n", msg, it_deltatime); \
		} \
	} while(0)

#else

#define INESA_TIMING_INIT()
#define INESA_TIMING_START()
#define INESA_TIMING_STOP()

#endif


#endif /* INESATIMING_H_ */
