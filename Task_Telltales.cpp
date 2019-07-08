/****************************************************************************************************
Task_Telltales.cpp

Copyright:      Shanghai INESA Auto Electronics System Co., Ltd.
Created on:    2019-4-24
Author:           Angus Chen
***************************************************************************************************/
#include "FunctionsDeclare.h"
#include "PpsBridge.h"
#include "InesaTiming.h"

static unsigned int sSliceCount = 0;
static unsigned int nOnOff = 0;
static unsigned int nOnOffDouble = 0;
static bool bStartDouble = false;
static PpsBridge* s_pPpsBridge = NULL;

void SetDoubleFlash(int nOnOff)
{
	if(NULL == s_pPpsBridge) {
		s_pPpsBridge = new PpsBridge("/pps", "S1", PpsBridge::PPS_PUBLISHER);
	}

	if(s_pPpsBridge) {
		s_pPpsBridge->setNumeric("TEL_LeftTurnLamp", nOnOff);
		s_pPpsBridge->setNumeric("TEL_RightTurnLamp", nOnOff);
		s_pPpsBridge->publish();
	}

	//PpsSetValue("/pps/S1", "TEL_LeftTurnLamp", nOnOff);
	//PpsSetValue("/pps/S1", "TEL_RightTurnLamp", nOnOff);
}

void SwithTelltales(int nOnOff)
{
	if(NULL == s_pPpsBridge) {
		s_pPpsBridge = new PpsBridge("/pps", "S1", PpsBridge::PPS_PUBLISHER);
	}

	if(s_pPpsBridge) {
		s_pPpsBridge->setNumeric("TEL_ESP", nOnOff);
		s_pPpsBridge->setNumeric("TEL_HighBeam", nOnOff);
		s_pPpsBridge->setNumeric("TEL_TPMSFault", nOnOff);
		s_pPpsBridge->setNumeric("TEL_SpeedLimit", nOnOff);
		s_pPpsBridge->setNumeric("TEL_RightTurnLamp", nOnOff);
		s_pPpsBridge->setNumeric("TEL_PositionLamp", nOnOff);
		s_pPpsBridge->publish();
	}
}

void Task_Telltales()
{
	 if(0 == (sSliceCount % 3000)) {//30s
		bStartDouble  = true;
	 }else if(0 == (sSliceCount % 4000)) {//40s
		nOnOff = nOnOff ? 0 : 1;
		bStartDouble = false;
		SwithTelltales(nOnOff);
		sSliceCount = 0;
	}

	if(bStartDouble) {
		if(0 == (sSliceCount % (500 / ProgramEnvVar::m_nMeterSpeed))) {//500ms
			nOnOffDouble = nOnOffDouble ?  0 : 1;
			SetDoubleFlash(nOnOffDouble);
		}
	}else {
		//SetDoubleFlash(0);
	}

	sSliceCount++;
}





