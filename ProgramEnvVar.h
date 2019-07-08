/*
 * ProgramEnvVar.h
 *
 *  Created on: 2017-11-28
 *      Author: czn
 */

#ifndef PROGRAMENVVAR_H_
#define PROGRAMENVVAR_H_

#include <string>

class ProgramEnvVar {
public:
	ProgramEnvVar();
	virtual ~ProgramEnvVar();

public:
	static bool ParseParams(int argc, char *argv[]);
	static int m_nPrintTiming;
	static int m_nThreadPriority;
	static int m_nVerbosity;
	static int m_nTimeSlice;
	static int m_nMeterSpeed;
	static int m_nDoubleFlashSpeed;
	static int m_nCenterNaveTime;
	static int m_nCenterMusicTime;
	static double m_nMeterStep;
	static std::string m_strVehiclePpsObj;
};

#endif /* PROGRAMENVVAR_H_ */
