/****************************************************************************************************
PpsToolFunc.cpp

Copyright:      Shanghai INESA Auto Electronics System Co., Ltd.
Created on:    2019-4-24
Author:           Angus Chen
***************************************************************************************************/
#include <time.h>
#include "FunctionsDeclare.h"
#include "InesaTiming.h"

void PpsSetValue(const char* file, const char* field, int val)
{
	char program[256];
	sprintf(program, "echo %s:n:%d>>%s", field, val, file);
	system(program);
}

unsigned int GetRandomVal(unsigned int  base)
{
	 srand( (int) time( NULL ) );
	 return (rand() % base);
}


