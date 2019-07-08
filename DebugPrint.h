/*
 * DebugPrint.h
 *
 *  Created on: 2017-8-22
 *      Author: czn
 */

#ifndef DEBUGPRINT_H_
#define DEBUGPRINT_H_
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "ProgramEnvVar.h"

#define USE_DEBUG
#ifdef USE_DEBUG
#define DEBUG_ERR(fmt, args...)  \
		printf("[%s:%d] "#fmt" errno=%d, %s\n", __func__, __LINE__, ##args, errno, strerror(errno));

#define DEBUG_INFO(fmt, args...)  \
	if(0 < ProgramEnvVar::m_nVerbosity) { \
		printf("[INFO]"#fmt"\n",  ##args); \
	}

#define DEBUG_PRINT_HEX(buf, len) \
	if(0 < ProgramEnvVar::m_nVerbosity) { \
		do { \
			for(int i = 0 ; i <(len); i++) { \
				printf("%02X ", buf[i]); \
			} \
			printf("\n"); \
		} while(0); \
	}

#define DEBUG_INFO_LEVEL(level,fmt, args...)  \
	if((level) <= ProgramEnvVar::m_nVerbosity) { \
		printf("[INFO]"#fmt"\n",  ##args); \
	}

#else
#define DEBUG_ERR(fmt, ...)
#define DEBUG_INFO(fmt, ...)
#define DEBUG_PRINT_HEX(buf, len)
#define DEBUG_INFO_LEVEL(level,fmt, args...)
#endif


#endif /* DEBUGPRINT_H_ */
