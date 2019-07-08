/****************************************************************************************************
Task_Center.cpp

Copyright:      Shanghai INESA Auto Electronics System Co., Ltd.
Created on:    2019-4-24
Author:           Angus Chen
***************************************************************************************************/
#include "FunctionsDeclare.h"
#include "PpsBridge.h"
#include "InesaTiming.h"

enum {
	INDEX_BASIC_INFO = 0,
	INDEX_NAVIGATION,
	INDEX_CHARGE,
	INDEX_MUSIC,
	INDEX_TELEPHONE,
	INDEX_CONTACT,
	INDEX_SETTING,
};

static int sSliceCount = 0;
static int sCenterIndex = 0;
static int sPreCenterIndex = 0;
static PpsBridge* s_pPpsBridge = NULL;

void SetCenter(int nCenter)
{
	if(NULL == s_pPpsBridge) {
		s_pPpsBridge = new PpsBridge("/pps", "M2", PpsBridge::PPS_PUBLISHER);
	}

	if(s_pPpsBridge) {
		s_pPpsBridge->setNumeric("DISP_Center", nCenter);
		s_pPpsBridge->publish();
	}
	//PpsSetValue("/pps/M2", "DISP_Center", nCenter);
}

void Task_Center()
{
	sSliceCount++;
	switch(sCenterIndex) {
	case INDEX_BASIC_INFO:
		if(sSliceCount >= 1000) {
			sCenterIndex++;
			sSliceCount = 0;
		}
		break;
	case INDEX_NAVIGATION:
		if(sSliceCount >= (100 * ProgramEnvVar::m_nCenterNaveTime)) {
			sCenterIndex++;
			sSliceCount = 0;
		}
		break;
	case INDEX_CHARGE:
		if(sSliceCount >= 1000) {
			sCenterIndex++;
			sSliceCount = 0;
		}
		break;
	case INDEX_MUSIC:
		if(sSliceCount >= (100 * ProgramEnvVar::m_nCenterMusicTime)) {
			sCenterIndex++;
			sSliceCount = 0;
		}
		break;
	case INDEX_TELEPHONE:
		if(sSliceCount >= 1000) {
			sCenterIndex++;
			sSliceCount = 0;
		}
		break;
	case INDEX_CONTACT:
		if(sSliceCount >= 1000) {
			sCenterIndex++;
			sSliceCount = 0;
		}
		break;
	case INDEX_SETTING:
		if(sSliceCount >= 1000) {
			sCenterIndex++;
			sSliceCount = 0;
		}
		break;
	default:
		sCenterIndex = 0;
		sSliceCount = 0;
		break;
	}

	if(sCenterIndex != sPreCenterIndex) {
		sPreCenterIndex = sCenterIndex;
		SetCenter(sCenterIndex);
	}
}

