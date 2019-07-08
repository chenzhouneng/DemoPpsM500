/****************************************************************************************************
Task_Power.cpp

Copyright:      Shanghai INESA Auto Electronics System Co., Ltd.
Created on:    2019-4-24
Author:           Angus Chen
***************************************************************************************************/
#include "FunctionsDeclare.h"
#include "PpsBridge.h"
#include "InesaTiming.h"

static PpsBridge* s_pPpsBridge = NULL;
static int sSliceCount = 0;
static double sSpeedo = 0;
static double sPower = 0;
static bool bForward = true;

void SetSpeedoAndPower(double nSpeedo, double nPower)
{
	if(NULL == s_pPpsBridge) {
		s_pPpsBridge = new PpsBridge("/pps", "S2", PpsBridge::PPS_PUBLISHER);
	}

	//PpsSetValue("/pps/S2", "GAU_N_VehicleSpeed", nSpeedo * 10);
	//PpsSetValue("/pps/S2", "GAU_N_PowerValue", nPower + 200);
	if(s_pPpsBridge) {
		s_pPpsBridge->setNumeric("GAU_N_VehicleSpeed", int(nSpeedo * 10));
		s_pPpsBridge->setNumeric("GAU_N_PowerValue", nPower + 200);
		s_pPpsBridge->publish();
	}
}


void Task_PowerAndSpeedo()
{
	if(0 == (sSliceCount % (500 / (ProgramEnvVar::m_nMeterSpeed)))) {
		sSliceCount = 0;
		if(bForward) {
			if(sSpeedo >=100) {
				sSpeedo += ProgramEnvVar::m_nMeterStep * 2;
				sPower += ProgramEnvVar::m_nMeterStep * 2;
			}else {
				sSpeedo += ProgramEnvVar::m_nMeterStep;
				sPower += ProgramEnvVar::m_nMeterStep;
			}
			if((sSpeedo >= 200)) {
				if(sSpeedo >= 200) {
					sSpeedo = 200;
				}
				if(sPower >= 180) {
					sPower = 180;
				}
				bForward = false;
			}
			SetSpeedoAndPower(sSpeedo,sPower);
		}else {
			if(sSpeedo >=100) {
				sSpeedo -= ProgramEnvVar::m_nMeterStep * 2;
				sPower -= ProgramEnvVar::m_nMeterStep * 2;
			}else {
				sSpeedo -= ProgramEnvVar::m_nMeterStep;
				sPower -= ProgramEnvVar::m_nMeterStep;
			}
			if((sSpeedo <= 0) ) {
				if(sSpeedo <= 0) {
					sSpeedo = 0;
				}
				if(sPower <= 0) {
					sPower = 0;
				}
				bForward = true;
			}
			SetSpeedoAndPower(sSpeedo,sPower);
		}
	}
	sSliceCount++;
}





