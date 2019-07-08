/****************************************************************************************************
Task_ADAS.cpp

Copyright:      Shanghai INESA Auto Electronics System Co., Ltd.
Created on:    2019-4-24
Author:           Angus Chen
***************************************************************************************************/
#include "FunctionsDeclare.h"
#include "PpsBridge.h"
#include "InesaTiming.h"

static int sSliceCount = 0;
static int sAdasIndex = 0;
static PpsBridge* s_pPpsBridge = NULL;

void SetAdas(int nAdas)
{
	if(NULL == s_pPpsBridge) {
		s_pPpsBridge = new PpsBridge("/pps", "M2", PpsBridge::PPS_PUBLISHER);
	}

	//PpsSetValue("/pps/M2", "DISP_ADAS", nAdas);
	if(s_pPpsBridge) {
		s_pPpsBridge->setNumeric("DISP_ADAS", nAdas);
		s_pPpsBridge->publish();
	}
}

void Task_ADAS()
{
	if(0 == (sSliceCount % 4000)) {//40s
		sSliceCount = 0;
		sAdasIndex = GetRandomVal(10);
		SetAdas(sAdasIndex);
	}

	sSliceCount++;
}





