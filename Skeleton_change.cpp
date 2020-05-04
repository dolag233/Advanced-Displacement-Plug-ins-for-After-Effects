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

/*	Skeleton.cpp	

	This is a compiling husk of a project. Fill it in with interesting
	pixel processing code.
	
	Revision History

	Version		Change													Engineer	Date
	=======		======													========	======
	1.0			(seemed like a good idea at the time)					bbb			6/1/2002

	1.0			Okay, I'm leaving the version at 1.0,					bbb			2/15/2006
				for obvious reasons; you're going to 
				copy these files directly! This is the
				first XCode version, though.

	1.0			Let's simplify this barebones sample					zal			11/11/2010

	1.0			Added new entry point									zal			9/18/2017

*/

#include "Skeleton.h"


bool isOnScreen(PF_LayerDef* layer, int x, int y) {
	return (x > -1 && y > -1 && x < layer->width && y < layer->height);
}//判断是否在屏幕内



PF_Pixel8* getPixel8(PF_LayerDef* layer, int x, int y) {
	return (PF_Pixel8*)((char*)layer->data + (y * layer->rowbytes) + (x * sizeof(PF_Pixel8)));
};//用于获取指定xy坐标的像素的内存地址


static PF_Err 
About (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg,
											"%s v%d.%d\r%s",
											STR(StrID_Name), 
											MAJOR_VERSION, 
											MINOR_VERSION, 
											STR(StrID_Description));
	return PF_Err_NONE;
}

static PF_Err 
GlobalSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	out_data->my_version = PF_VERSION(	MAJOR_VERSION, 
										MINOR_VERSION,
										BUG_VERSION, 
										STAGE_VERSION, 
										BUILD_VERSION);

	out_data->out_flags =  PF_OutFlag_DEEP_COLOR_AWARE;	// just 16bpc, not 32bpc
	
	return PF_Err_NONE;
}

static PF_Err 
ParamsSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err		err		= PF_Err_NONE;
	PF_ParamDef	def;	

	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUPX("Displace Mode", 4, 1, "Radial|Vector|Direct|Twirl", NULL,isRadial);
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDERX(	"Length", 
							0, 
							30000, 
							0, 
							30000, 
							DIS_LENGTH_DEFT,
							PF_Precision_HUNDREDTHS,
							0,
							0,
							DisplaceLength);

	AEFX_CLR_STRUCT(def);

	PF_ADD_LAYER("Map Layer",0, DisplaceMap);
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT("Displacement Center", in_data->width / 2, in_data->height / 2,false,DisCenter);
	AEFX_CLR_STRUCT(def);
	out_data->num_params = PARAMS_NUM;

	return err;
}

static PF_Err
MySimpleGainFunc16 (
	void		*refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	GainInfo	*giP	= reinterpret_cast<GainInfo*>(refcon);
	PF_FpLong	tempF	= 0;
					
	if (giP){
		tempF = giP->gainF * PF_MAX_CHAN16 / 100.0;
		if (tempF > PF_MAX_CHAN16){
			tempF = PF_MAX_CHAN16;
		};

		outP->alpha		=	inP->alpha;
		outP->red		=	MIN((inP->red	+ (A_u_char) tempF), PF_MAX_CHAN16);
		outP->green		=	MIN((inP->green	+ (A_u_char) tempF), PF_MAX_CHAN16);
		outP->blue		=	MIN((inP->blue	+ (A_u_char) tempF), PF_MAX_CHAN16);
	}

	return PF_Err_NONE;
}

static PF_Err
MySimpleGainFunc8 (
	void		*refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err		err = PF_Err_NONE;

	disInfo	*giP	= reinterpret_cast<disInfo*>(refcon);
	PF_Pixel8* pix;
	PF_Pixel8* mapP;
	mapP= getPixel8(&giP->map, xL % giP->map.width, yL % giP->mapheight);
	if(giP->length==0){
		outP->alpha = inP->alpha;
		outP->red = inP->red;
		outP->green = inP->green;
		outP->blue = inP->blue;
	}
	else {
		if (giP->length >= 0.01) {
			float lumn;
			int x_fin, y_fin;
			float lumn_ori;
			PF_Pixel8* mapP = getPixel8(&giP->map, xL % giP->map.width, yL %giP->map.height);
			 
			//if (isOnScreen(giP->map, xL, yL))  lumn_ori = mapP->red * 0.2 + mapP->green * 0.7 + mapP->blue * 0.1; 
			//else lumn_ori= inP->red * 0.2 + inP->green * 0.7 + inP->blue * 0.1;
			//lumn_ori = inP->red * 0.2 + inP->green * 0.7 + inP->blue * 0.1;

			lumn_ori = (mapP->red + mapP->green + mapP->blue) / 3.0;

			lumn = lumn_ori / (double)PF_MAX_CHAN8;
			int dx;
			int dy;
			float vec_x, vec_y;
			int x_center = giP->center_x;
			int y_center = giP->center_y;
			if (giP->mode == 1) {//radial
				dx = xL - x_center;
				dy = yL - y_center;
				vec_x = sin(atan2((double)dx, (double)dy));
				vec_y = cos(atan2((double)dx, (double)dy));
				x_fin = xL + vec_x * giP->length * lumn;
				y_fin = yL + vec_y * giP->length * lumn;
			}
			if (giP->mode == 2) {//vector
				vec_x = sin(lumn);
				vec_y = cos(lumn);

				x_fin = xL + vec_x * giP->length * lumn;
				y_fin = yL + vec_y * giP->length * lumn;
			}
			if (giP->mode == 3) {//direct
				vec_x = 0;
				vec_y = 1;
				x_fin = xL + vec_x * giP->length * lumn;
				y_fin = yL + vec_y * giP->length * lumn;
			}
			if (giP->mode == 4) {//twirl
				dx = xL - x_center;
				dy = yL - y_center;
				double scr_angle = atan2((double)dy, (double)dx);//开始该像素到中心和x轴正半轴夹角的弧度制
				double des_angle = scr_angle - giP->length * lumn / 10.0;
				int dsts = (int)hypot(dx, dy);
				x_fin = dsts * sin(des_angle);
				y_fin = dsts * cos(des_angle);
			}

			if (isOnScreen(&giP->layer, x_fin, y_fin)) {
				pix = getPixel8(&giP->layer, x_fin, y_fin);

				outP->alpha = pix->alpha;
				outP->red = pix->red;
				outP->green = pix->green;
				outP->blue = pix->blue;
			}
			else {
				outP->alpha = 0;
				outP->red = inP->red;
				outP->green = inP->green;
				outP->blue = inP->blue;
			}
		}
		else
		{
			outP->alpha = inP->alpha;
			outP->red = inP->red;
			outP->green = inP->green;
			outP->blue = inP->blue;
		}
	}
	return err;
}

static PF_Err 
Render (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err				err		= PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);

	/*	Put interesting code here. */
	disInfo disp;
	AEFX_CLR_STRUCT(disp); 
	disp.layer = params[0]->u.ld;
	disp.mode = params[1]->u.pd.value;
	disp.length = params[2]->u.fs_d.value;
	disp.map = params[3]->u.ld;
	disp.center_x = FIX2INT( params[4]->u.td.x_value);
	disp.center_y = FIX2INT(params[4]->u.td.y_value);
	disp.mem = suites.MemorySuite1();
	
	A_long				linesL	= 0;

	linesL 		= output->extent_hint.bottom - output->extent_hint.top;
	if (disp.length != 0) {
		if (PF_WORLD_IS_DEEP(output)) {
			ERR(suites.Iterate16Suite1()->iterate(in_data,
				0,								// progress base
				linesL,							// progress final
				&params[0]->u.ld,	// src 
				NULL,							// area - null for all pixels
				(void*)&disp,				// refcon - your custom data pointer
				MySimpleGainFunc16,				// pixel function pointer
				output));
		}
		else {
			ERR(suites.Iterate8Suite1()->iterate(in_data,
				0,								// progress base
				linesL,							// progress final
				&params[0]->u.ld,	// src 
				NULL,							// area - null for all pixels
				(void*)&disp,					// refcon - your custom data pointer
				MySimpleGainFunc8,				// pixel function pointer
				output));
		}
	}
	return err;
}


extern "C" DllExport
PF_Err PluginDataEntryFunction(
	PF_PluginDataPtr inPtr,
	PF_PluginDataCB inPluginDataCallBackPtr,
	SPBasicSuite* inSPBasicSuitePtr,
	const char* inHostName,
	const char* inHostVersion)
{
	PF_Err result = PF_Err_INVALID_CALLBACK;

	result = PF_REGISTER_EFFECT(
		inPtr,
		inPluginDataCallBackPtr,
		"Skeleton", // Name
		"ADBE Skeleton", // Match Name
		"Sample Plug-ins", // Category
		AE_RESERVED_INFO); // Reserved Info

	return result;
}


PF_Err
EffectMain(
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra)
{
	PF_Err		err = PF_Err_NONE;
	
	try {
		switch (cmd) {
			case PF_Cmd_ABOUT:

				err = About(in_data,
							out_data,
							params,
							output);
				break;
				
			case PF_Cmd_GLOBAL_SETUP:

				err = GlobalSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_PARAMS_SETUP:

				err = ParamsSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_RENDER:

				err = Render(	in_data,
								out_data,
								params,
								output);
				break;
		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}

