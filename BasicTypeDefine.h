/*
 * BasicTypeDefine.h
 *
 *  Created on: 2017-8-27
 *      Author: czn
 */

#ifndef BASICTYPEDEFINE_H_
#define BASICTYPEDEFINE_H_

typedef bool  							BOOLEAN;
typedef unsigned char  			INT8U;
typedef signed  char  				INT8S;
typedef unsigned short 			INT16U;
typedef signed  short 				INT16S;
typedef unsigned int   			INT32U;
typedef unsigned long long	INT64U;
typedef signed  int   				INT32S;
typedef float								FP32;
typedef double							FP64;
typedef BOOLEAN					BOOL;

#ifndef NULL
#define NULL  (void*)0L
#endif

#ifndef FALSE
#define FALSE  false
#endif

#ifndef TRUE
#define TRUE   true
#endif

#endif /* BASICTYPEDEFINE_H_ */
