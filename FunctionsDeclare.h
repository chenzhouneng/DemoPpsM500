/****************************************************************************************************
FunctionsDeclare.h

Copyright:      Shanghai INESA Auto Electronics System Co., Ltd.
Created on:    2019-4-24
Author:           Angus Chen
***************************************************************************************************/

#ifndef FUNCTIONSDECLARE_H_
#define FUNCTIONSDECLARE_H_
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void PpsSetValue(const char* file, const char* field, int val);
unsigned int GetRandomVal(unsigned int  base);

void Task_ADAS();
void Task_Center();
void Task_ModeSwitch();
void Task_PowerAndSpeedo();
void Task_Telltales();
void Task_WarningPopup();

#endif /* FUNCTIONSDECLARE_H_ */
