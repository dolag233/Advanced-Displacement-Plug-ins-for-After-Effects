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
#include "AEFX_SuiteHandlerTemplate.h"
#include "DolagDisp_Strings.h"
#include "dolag_ae_func.h"
//#include "AEFX_SuiteHelper.h"
/* Versioning information */
#define	MAJOR_VERSION	1
#define	MINOR_VERSION	0
#define	BUG_VERSION		0
#define	STAGE_VERSION	PF_Stage_DEVELOP
#define	BUILD_VERSION	1


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
		int repeat_mode;
		int max_iter;
		float dis_vel;//运动快慢
		float max_distance;
		//PF_Sampling8Suite1 *sample_suite8;
		//PF_Sampling16Suite1 *sample_suite16;
		//PF_SampPB samp_pb;
		//bool apply_mode;
}disInfo;
	



/* Parameter defaults */
	const int RADIAL = 1;
	const int VECTOR = 0;
	const int DIS_LENGTH_MIN = 0;
	const int DIS_LENGTH_DEFT = 5;
	const int RADAIL_DEFT = RADIAL;
	const double ANGLE_DEFT = 0.0;
	//const int DIS_EDGE_DEFT = 
	//少了一个layer的deft

struct Do_pixelPos {
	u_short x;//最大是32768 * 2
	u_short y;//没有负数
};

/*enum {
RADIALorVECTOR=1,
DIS_LENGTH,
DIS_LAYER,
DIS_CENTER,
DIS_IS_TRANSPARENT,
DIS_ANGLE,
PARAMS_NUM
};*/
//下面是index，可以任意顺序随意插入
enum {
	DisInputLayer = 0,
	DisButton,
	DisMode,
	DisLength,
	DisMap,
	DisCenter,
	DisIsTransparent,
	DisApplyAA,
	DisAAThreshold,
	DisAngle,
	DisEdge,
	DisDynamicsTopic,
	DisVelocity,
	DisMaxIter,
	DisMaxDistance,
	DisDynamicsTopicEnd,
	PARAMS_NUM
};
//下面是DIsk ID，只能append，不要改变前面的顺序
enum {
	DisModeID = 1,
	DisLengthID,
	DisMapID,
	DisCenterID,
	DisIsTransparentID,
	DisAngleID,
	DisEdgeID,
	DisVelocityID,
	DisMaxIterID,
	DisApplyModeID,
	DisDynamicsTopicID,
	DisButtonID,
	DisDynamicsTopicEndID,
	DisMaxDistanceID,
	DisApplyAAID,
	DisAAThresholdID

};
typedef struct GainInfo{
	PF_FpLong	gainF;
} GainInfo, *GainInfoP, **GainInfoH;


static PF_Err
HandleEvent(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output,
	PF_EventExtra* extra);//函数实现实在Dolag_UI里面的

//释放套件内存
//测试用的path结构体
typedef struct {
	int vert_num;
	PF_PathVertex* p_vert_head;
	PF_FpLong* p_x;
	PF_FpLong* p_y;
	int step;
}path_info;

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