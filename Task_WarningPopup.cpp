/****************************************************************************************************
Task_WarningPopup.cpp

Copyright:      Shanghai INESA Auto Electronics System Co., Ltd.
Created on:    2019-4-24
Author:           Angus Chen
***************************************************************************************************/
#include "FunctionsDeclare.h"
#include "PpsBridge.h"
#include "InesaTiming.h"

static int sSliceCount = 0;
static int sPopupIndex = 0;
static int sPopupStatus = 0;
static PpsBridge* s_pPpsBridge = NULL;

void SetWarningPopup()
{
	if(NULL == s_pPpsBridge) {
		s_pPpsBridge = new PpsBridge("/pps", "M2", PpsBridge::PPS_PUBLISHER);
	}

	if(s_pPpsBridge) {
		s_pPpsBridge->setNumeric("POPUP_Status", sPopupStatus);
		s_pPpsBridge->setNumeric("POPUP_TextWarning", sPopupIndex);
		s_pPpsBridge->publish();
	}
	//PpsSetValue("/pps/M2", "POPUP_Status", sPopupStatus);
	//PpsSetValue("/pps/M2", "POPUP_TextWarning", sPopupIndex);
}

void Task_WarningPopup()
{
	if(0 == (sSliceCount % 6000)) {//60s
		sSliceCount = 0;
		sPopupStatus = 1;
		sPopupIndex = GetRandomVal(100);
		if(0 == sPopupIndex) {
			sPopupIndex = 1;
		}
		SetWarningPopup();
	}else {
		if(300 == sSliceCount) {//3s after showed
			sPopupStatus = 0;
			SetWarningPopup();
		}
	}

	sSliceCount++;
}


