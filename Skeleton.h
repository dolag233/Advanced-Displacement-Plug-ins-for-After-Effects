/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

/*
	Skeleton.h
*/

#pragma once

#ifndef SKELETON_H
#define SKELETON_H

#define FIX2FLT(x) (x / ((float)(1L << 16)))//定点数到float

typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned short		u_int16;
typedef unsigned long		u_long;
typedef short int			int16;
#define PF_TABLE_BITS	12
#define PF_TABLE_SZ_16	4096

#define PF_DEEP_COLOR_AWARE 1	// make sure we get 16bpc pixels; 
								// AE_Effect.h checks for this.

#include "AEConfig.h"

#ifdef AE_OS_WIN
	typedef unsigned short PixelType;
	#include <Windows.h>
#endif

#include "entry.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_Macros.h"
#include "Param_Utils.h"
#include "AE_EffectCBSuites.h"
#include "String_Utils.h"
#include "AE_GeneralPlug.h"
#include "AEFX_ChannelDepthTpl.h"
#include "AEGP_SuiteHandler.h"

#include "Skeleton_Strings.h"

/* Versioning information */

#define	MAJOR_VERSION	1
#define	MINOR_VERSION	0
#define	BUG_VERSION		0
#define	STAGE_VERSION	PF_Stage_DEVELOP
#define	BUILD_VERSION	1

	//float getLightness(A_u_char R, A_u_char G, A_u_char I)
	//{
	//	return pow(pow(R / 255.0f, 2.2f) + pow(G / 170.0f, 2.2f) + pow(I / 425.0f, 2.2f), 1 / 2.2f) * 0.547373141f;
	//};//伽玛校正后的亮度值


//自己建立的变量结构体
/*
	typedef struct {
		AEGP_MemorySuite1* mem_suite;

		// User settings
		int type;
		int chunk_size;
		double lower_in;
		double upper_in;
		double lower_out;
		double upper_out;
		int sort_by;
		int order_by;
		double direction;
		double centre_x;
		double centre_y;
		double displacement_scale;

		// Derived
		double vec_x;
		double vec_y;
		bool use_displacement;

		// Layer data
		PF_LayerDef* layer;
		PF_LayerDef* map;
		A_long rowbytes;
	} PixelSortInfo;*/
	typedef struct displace {
		float length;
		float vec_x;
		float vec_y;
		AEGP_MemorySuite1* mem;
		int mode;
		PF_ProgPtr ref;
		PF_InData* data;
		PF_LayerDef layer;
		PF_ParamDef check;//the real map
		PF_LayerDef map;
		PF_Pixel8 pix8;
		int rowbytes;
		int center_x;
		int center_y;
		bool isTransparent;
		float angle;//new
}disInfo;
	



/* Parameter defaults */
	const int RADIAL = 1;
	const int VECTOR = 0;
	const int DIS_LENGTH_MIN = 0;
	const int DIS_LENGTH_DEFT = 5;
	const int RADAIL_DEFT = RADIAL;
	const double ANGLE_DEFT = 0.0;
	//少了一个layer的deft



/*enum {
RADIALorVECTOR=1,
DIS_LENGTH,
DIS_LAYER,
DIS_CENTER,
DIS_IS_TRANSPARENT,
DIS_ANGLE,
PARAMS_NUM
};*/

enum {
	isRadial = 1,
	DisplaceLength,
	DisplaceMap,
	DisCenter,
	isTransparent,
	DisAngle,
	PARAMS_NUM
};

typedef struct GainInfo{
	PF_FpLong	gainF;
} GainInfo, *GainInfoP, **GainInfoH;


extern "C" {

	DllExport
	PF_Err
	EffectMain(
		PF_Cmd			cmd,
		PF_InData		*in_data,
		PF_OutData		*out_data,
		PF_ParamDef		*params[],
		PF_LayerDef		*output,
		void			*extra);

}

#endif // SKELETON_H