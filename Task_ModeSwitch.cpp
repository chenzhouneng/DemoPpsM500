/****************************************************************************************************
Task_ModeSwitch.cpp

Copyright:      Shanghai INESA Auto Electronics System Co., Ltd.
Created on:    2019-4-24
Author:           Angus Chen
***************************************************************************************************/
#include "FunctionsDeclare.h"
#include "PpsBridge.h"
#include "InesaTiming.h"

static int sSliceCount = 0;
static int sMode = 0;
static PpsBridge* s_pPpsBridge = NULL;

void SetMode(int nMode)
{
	if(NULL == s_pPpsBridge) {
		s_pPpsBridge = new PpsBridge("/pps", "S2", PpsBridge::PPS_PUBLISHER);
	}

	if(s_pPpsBridge) {
		s_pPpsBridge->setNumeric("DISP_StyleType", nMode);
		s_pPpsBridge->publish();
	}

	//PpsSetValue("/pps/S2", "DISP_StyleType", nMode);
}

void Task_ModeSwitch()
{
	if(0 == (sSliceCount % 3000)) {//30s
		sSliceCount = 0;
		sMode = (++sMode) % 3;
		SetMode(sMode);
	}
	sSliceCount++;
}





