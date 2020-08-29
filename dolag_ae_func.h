//都是些自定义的函数
/*
extern void Swap(int* dst, int* src);
extern void Swap(A_u_char* dst, A_u_char* src);
extern void Swap(A_u_short* dst, A_u_short* src);


extern bool SwapPixel8(PF_Pixel8* dst, PF_Pixel8* src);

extern bool SwapPixel16(PF_Pixel16* dst, PF_Pixel16* src);

//写完发现并没有什么用orz，个像素之间的插值果然还是太弱了
extern A_Color Bilinear8(PF_LayerDef* layer, float x, float y);



extern A_Color Bilinear16(PF_LayerDef* layer, float x, float y);

extern bool isOnScreen(PF_LayerDef* layer, int x, int y);
extern PF_Pixel16* getPixel16(PF_LayerDef* layer, int x, int y);
//然后你发现本身就有获取像素的函数,不过有、麻烦
//PF_GET_PIXEL_DATA8()
// PF_GET_PIXEL_DATA16();不过好像得有*in_data才能用
extern PF_Pixel8* getPixel8(PF_LayerDef* layer, int x, int y);

extern float lerp(float Dmin, float Dmax, float Smin, float Smax, float value);

extern typedef struct {
	u_short x;//最大是32768 * 2
	u_short y;//没有负数
}Do_pixelPos;




//#pragma once
//
//
//#define FIX2FLT(x) (x / ((float)(1L << 16)))//定点数到float
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

PF_Pixel8 Bilinear8(PF_LayerDef* layer, double x, double y);

PF_Pixel16 Bilinear16(PF_LayerDef* layer, double x, double y);

void Swap(int* dst, int* src);
void Swap(A_u_char* dst, A_u_char* src);
void Swap(A_u_short* dst, A_u_short* src);

bool SwapPixel8(PF_Pixel8* dst, PF_Pixel8* src);

bool SwapPixel16(PF_Pixel16* dst, PF_Pixel16* src);

void Normalize(float* x, float* y);

bool isOnScreen(PF_LayerDef* layer, int x, int y);

PF_Pixel8* getPixel8(PF_LayerDef* layer, int x, int y);

PF_Pixel16* getPixel16(PF_LayerDef* layer, int x, int y);

float lerp(float Dmin, float Dmax, float Smin, float Smax, float value);

void DrawWaterMark(PF_LayerDef* output, PF_LayerDef* src_layer, int time,bool isLisensed);//画水印

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

void RepeatEdge(float* x, float* y,short width,short height,int mode);

void BlendPix(const PF_Pixel8 src, PF_Pixel8* dst, float coe);
void BlendPix(const PF_Pixel16 src, PF_Pixel16 * dst, float coe);

float GetLumn(PF_Pixel8 pix);
float GetLumn(PF_Pixel16 pix);

void GetPseudoFXAA(PF_LayerDef* layer,short x,short y, float MINLIMIT);

void PseduoFXAA(PF_LayerDef* layer, float MINLIMIT);