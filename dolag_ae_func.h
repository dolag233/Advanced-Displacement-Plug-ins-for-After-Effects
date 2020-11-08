//都是些自定义的函数
/*
extern void Swap(int* dst, int* src);
extern void Swap(A_u_char* dst, A_u_char* src);
extern void Swap(A_u_short* dst, A_u_short* src);


extern bool SwapPixel8(PF_Pixel8* dst, PF_Pixel8* src);

extern bool SwapPixel16(PF_Pixel16* dst, PF_Pixel16* src);

//写完发现并没有什么用orz，个像素之间的插值果然还是太弱了
extern A_Color Bilinear8(PF_LayerDef* layer, PF_FpShort x, PF_FpShort y);



extern A_Color Bilinear16(PF_LayerDef* layer, PF_FpShort x, PF_FpShort y);

extern bool isOnScreen(PF_LayerDef* layer, int x, int y);
extern PF_Pixel16* getPixel16(PF_LayerDef* layer, int x, int y);
//然后你发现本身就有获取像素的函数,不过有、麻烦
//PF_GET_PIXEL_DATA8()
// PF_GET_PIXEL_DATA16();不过好像得有*in_data才能用
extern PF_Pixel8* getPixel8(PF_LayerDef* layer, int x, int y);

extern PF_FpShort lerp(PF_FpShort Dmin, PF_FpShort Dmax, PF_FpShort Smin, PF_FpShort Smax, PF_FpShort value);

extern typedef struct {
	u_short x;//最大是32768 * 2
	u_short y;//没有负数
}Do_pixelPos;




//#pragma once
//
//
//#define FIX2FLT(x) (x / ((PF_FpShort)(1L << 16)))//定点数到PF_FpShort
//
//typedef unsigned char		u_char;
//typedef unsigned short		u_short;
//typedef unsigned short		u_int16;
//typedef unsigned long		u_long;
//typedef short int			int16;
//#define PF_TABLE_BITS	12
//#define PF_TABLE_SZ_16	4096
//
//#define PF_DEEP_COLOR_AWARE 1	// make sure we get 16bpc pixels; 
//// AE_Effect.h checks for this.
*/
#ifndef _H_DOLAG_AE_FUNC_
#define _H_DOLAG_AE_FUNC_

	#include "AEConfig.h"
	//
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
	//#include "AEFX_ChannelDepthTpl.h"
	#include "AEGP_SuiteHandler.h"
	#include "AEFX_SuiteHandlerTemplate.h"

#endif
//
//

PF_Pixel8 Bilinear8(PF_LayerDef* layer, PF_FpLong x, PF_FpLong y);

PF_Pixel16 Bilinear16(PF_LayerDef* layer, PF_FpLong x, PF_FpLong y);

void Swap(A_long* dst, A_long* src);
void Swap(A_u_char* dst, A_u_char* src);
void Swap(A_u_short* dst, A_u_short* src);

bool SwapPixel8(PF_Pixel8* dst, PF_Pixel8* src);

bool SwapPixel16(PF_Pixel16* dst, PF_Pixel16* src);

void Normalize(PF_FpShort* x, PF_FpShort* y);

bool isOnScreen(PF_LayerDef* layer, A_long x, A_long y);

PF_Pixel8* getPixel8(PF_LayerDef* layer, A_long x, A_long y);

PF_Pixel16* getPixel16(PF_LayerDef* layer, A_long x, A_long y);

PF_FpShort lerp(PF_FpShort Dmin, PF_FpShort Dmax, PF_FpShort Smin, PF_FpShort Smax, PF_FpShort value);

void DrawWaterMark(PF_LayerDef* output, PF_LayerDef* src_layer, A_long time,bool isLisensed);//画水印

PF_Err DOLAG_ReleaseSuite(PF_InData* in_data,
	PF_OutData* out_data,
	const char* name,
	int32_t			version,
	const char* error_stringPC0);

enum EdgeRepeatMode//环绕方式的枚举
{
	REPEAT_BASE = 2,
	REPEAT_MIRROR
};

void RepeatEdge(PF_FpShort* x, PF_FpShort* y,short width,short height,A_long mode);

void BlendPix(const PF_Pixel8 src, PF_Pixel8* dst, PF_FpShort coe);
void BlendPix(const PF_Pixel16 src, PF_Pixel16 * dst, PF_FpShort coe);

PF_FpShort GetLumn(PF_Pixel8 pix);
PF_FpShort GetLumn(PF_Pixel16 pix);

void GetPseudoFXAA(PF_LayerDef* layer,short x,short y, PF_FpShort MINLIMIT);

void PseduoFXAA(PF_LayerDef* layer, PF_FpShort MINLIMIT);