/*
 * ProgramEnvVar.cpp
 *
 *  Created on: 2017-11-28
 *      Author: czn
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include "DebugPrint.h"
#include "ProgramEnvVar.h"

int ProgramEnvVar::m_nPrintTiming = 0;
int ProgramEnvVar::m_nThreadPriority = 0;
int ProgramEnvVar::m_nVerbosity = 0;
int ProgramEnvVar::m_nTimeSlice = 10;
int ProgramEnvVar::m_nMeterSpeed = 10;
int ProgramEnvVar::m_nDoubleFlashSpeed = 10;
int ProgramEnvVar::m_nCenterNaveTime = 10;
int ProgramEnvVar::m_nCenterMusicTime = 10;
double ProgramEnvVar::m_nMeterStep = 5.0;
std::string ProgramEnvVar::m_strVehiclePpsObj = "S2";

ProgramEnvVar::ProgramEnvVar() {
	// TODO Auto-generated constructor stub
}

ProgramEnvVar::~ProgramEnvVar() {
	// TODO Auto-generated destructor stub
}

bool ProgramEnvVar::ParseParams(int argc, char *argv[])
{
	const char *tok = NULL;
	for (int i = 1; i < argc; i++) {
		if (!strncmp(argv[i], "-timing=", strlen("-timing="))) {
			tok = argv[i] + strlen("-timing=");
			m_nPrintTiming = atoi(tok);
		}else if (!strncmp(argv[i], "-priority=", strlen("-priority="))) {
			tok = argv[i] + strlen("-priority=");
			m_nThreadPriority = atoi(tok);
		}else if (!strncmp(argv[i], "-verbosity=", strlen("-verbosity="))) {
			tok = argv[i] + strlen("-verbosity=");
			m_nVerbosity = atoi(tok);
		}else if (!strncmp(argv[i], "-ts=", strlen("-ts="))) {
			tok = argv[i] + strlen("-ts=");
			m_nTimeSlice = atoi(tok);
		}else if (!strncmp(argv[i], "-ms=", strlen("-ms="))) {
			tok = argv[i] + strlen("-ms=");
			m_nMeterSpeed = atoi(tok);
		}else if (!strncmp(argv[i], "-dfs=", strlen("-dfs="))) {
			tok = argv[i] + strlen("-dfs=");
			m_nDoubleFlashSpeed = atoi(tok);
		}else if (!strncmp(argv[i], "-nt=", strlen("-nt="))) {
			tok = argv[i] + strlen("-nt=");
			m_nCenterNaveTime = atoi(tok);
		}else if (!strncmp(argv[i], "-mt=", strlen("-mt="))) {
			tok = argv[i] + strlen("-mt=");
			m_nCenterMusicTime = atoi(tok);
		}else if (!strncmp(argv[i], "-step=", strlen("-step="))) {
			tok = argv[i] + strlen("-step=");
			m_nMeterStep = atof(tok);
		}else if (!strncmp(argv[i], "-pps=", strlen("-pps="))) {
			tok = argv[i]+strlen("-pps=");
			m_strVehiclePpsObj = tok;
		}else if(!strncmp(argv[i], "-help", strlen("-help"))) {
			printf(
					"USAGE:"
					"%s [-verbosity=] [-timing=] [-priority=] [-ts=] [-ms=] [-dfs=] [-nt=] [-mt=] [-step=]\n"
					"-verbosity,       print the debug information\n"
					"-timing,          print the timing\n"
					"-priority,        set the priority of process\n"
					"-ts,              set the time slice interval, default is 10ms\n"
					"-ms,              set the speed of speedo and power meters, default is 10,value range[1,100]\n"
					"-dfs,             set the speed of double flash, default is 10,value range[1,100]\n"
					"-nt,              set the time duration of navigation, default is 10,value range[1,100]\n"
					"-mt,              set the time duration of music, default is 10,value range[1,100]\n"
					"-step,            set the step for speedo meter, default is 5\n"
					"-pps,             set the file name of pps for vehicle information\n"
					,argv[0]
				);
			return false;
		}else {
			printf("invalid parameter:%s\n", argv[i]);
			printf("type %s -help for usage\n", argv[0]);
			return false;
		}
	}

	return true;
}
