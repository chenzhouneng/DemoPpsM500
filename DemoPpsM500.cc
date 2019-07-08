#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "FunctionsDeclare.h"
#include "ProgramEnvVar.h"
#include "InesaTiming.h"

int main(int argc, char *argv[]) {
	if(	!ProgramEnvVar::ParseParams(argc, argv)) {
			return false;
	}

	system("slay McuMessenger");
	system("/emmc/usr/bin/McuMessenger &");
	usleep(500000);
	system("slay McuMessenger");
	INESA_TIMING_INIT();

	while(true) {
		usleep(ProgramEnvVar::m_nTimeSlice  * 1000 - 1000);
		INESA_TIMING_START();
		Task_ADAS();
		Task_Center();
		Task_ModeSwitch();
		Task_PowerAndSpeedo();
		Task_Telltales();
		Task_WarningPopup();
		INESA_TIMING_STOP("TASK LOOP USE TIME");
	}

	return EXIT_SUCCESS;
}
