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

#include "DolagDispMainHead.h"

//PF_Pixel8 Mirror(PF_LayerDef layer, int x, int y) {

	//PF_GET_PIXEL_DATA8();
	//*in_data->utils->get_pixel_data8();
//};

//bool myIteration8(PF_LayerDef* inL,)

//释放套件内存的东西，其实在 AEFX_SuiteHelper.h里面就有现成的


const PF_FpShort PI = 3.14159265358;
bool isLisensed = 0;/*判断是否有效购买的全局变量*/
static PF_Err
About(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{
	//short map;
	AEGP_SuiteHandler suites(in_data->pica_basicP);

	suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
		"%s v%d.%d\r%s",
		STR(StrID_Name),
		MAJOR_VERSION,
		MINOR_VERSION,
		STR(StrID_Description));
	return PF_Err_NONE;
}

static PF_Err
GlobalSetup(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{
	out_data->my_version = PF_VERSION(MAJOR_VERSION,
		MINOR_VERSION,
		BUG_VERSION,
		STAGE_VERSION,
		BUILD_VERSION);
	out_data->out_flags = PF_OutFlag_DEEP_COLOR_AWARE |
		PF_OutFlag_WIDE_TIME_INPUT |
		PF_OutFlag_NON_PARAM_VARY |
		PF_OutFlag_I_EXPAND_BUFFER |
		PF_OutFlag_I_DO_DIALOG;	// just 16bpc, not 32bpc
	out_data->out_flags2 |= PF_OutFlag2_OUTPUT_IS_WATERMARKED | PF_OutFlag2_PARAM_GROUP_START_COLLAPSED_FLAG;

	return PF_Err_NONE;
}

static PF_Err
ParamsSetup(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{
	PF_Err		err = PF_Err_NONE;
	PF_ParamDef	def;

	static bool* add_params;
	bool NOPARAMS = false;
	add_params = &NOPARAMS;
	AEFX_CLR_STRUCT(def);
	PF_ADD_BUTTON("Water Mark Toggle", "Water Mark Toggle", 0, PF_ParamFlag_SUPERVISE, DisButtonID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUPX("Displace Mode", 5, 1, "Radial|Vector(Only Red and Blue Channel)|Direct|Twirl|Dynamics", 0, DisModeID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDERX("Length",
		-250,
		250,
		-100,
		100,
		DIS_LENGTH_DEFT,
		PF_Precision_HUNDREDTHS,
		0,//displace flag,如果设置为1的话，就是以百分数的形式显示
		//改了一下这里的flag，使得该变量在改变时会产生params changed的cmd
		//是有用的，可以留彩蛋
		PF_ParamFlag_SUPERVISE,
		DisLengthID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_LAYER("Map Layer", PF_LayerDefault_MYSELF, DisMapID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT("Displacement Center", in_data->width / 2, in_data->height / 2, false, DisCenterID);
	//AEFX_CLR_STRUCT(def);//这行可以删除，因为checkboxx本身就有清零
	PF_ADD_CHECKBOXX("Apply Transparent", 1, 0, DisIsTransparentID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOXX("Apply Anti-aliasing", 1, 0, DisApplyAAID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDERX("Anti-aliasing Threshold", 0, 1, 0, 1, .5f, PF_Precision_HUNDREDTHS, 0, 0, DisAAThresholdID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE("Direct Angle", ANGLE_DEFT, DisAngleID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUPX("Edge Repeat", 3, 1, "None|Repeat|Mirror", NULL, DisEdgeID);//新增了一个边缘环绕模式，还没有实现
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC("Dynamics Options", DisDynamicsTopicID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDERX("Displace Velocity", 0. - 1000, 1000.0, 0.0, 10.0, 1, PF_Precision_HUNDREDTHS, 0, PF_ParamFlag_SUPERVISE, DisVelocityID);
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER("Displace Max Iterations", 0, 2000, 0, 200, 2, DisMaxIterID);
	PF_ADD_FLOAT_SLIDERX("Displace Max Distance", 0, 3500, 0, 1000, 500, PF_Precision_HUNDREDTHS, 0, 0, DisMaxDistanceID);
	AEFX_CLR_STRUCT(def);
	//PF_ADD_POPUPX("Displace Apply Mode", 2, 1, "Sample|Trans", NULL, DisApplyModeID);
	//PF_ADD_CHECKBOXX("Apply Trans Pixels", 0, 0, DisApplyModeID);
	//AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(DisDynamicsTopicID);
	AEFX_CLR_STRUCT(def);
	/*if (add_params) {
		PF_ADD_TOPICX("PV一键生成器", 0, 8);
	}*/

	out_data->num_params = PARAMS_NUM;

	//选项按钮
	if (in_data->appl_id != 'PrMr')
	{
		AEFX_SuiteScoper<PF_EffectUISuite1> effect_ui_suiteP = AEFX_SuiteScoper<PF_EffectUISuite1>(
			in_data,
			kPFEffectUISuite,
			kPFEffectUISuiteVersion1,
			out_data);

		ERR(effect_ui_suiteP->PF_SetOptionsButtonName(in_data->effect_ref, "Doalg"));
	}


	return err;
}

static PF_Err
FrameSetup(PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output) {

	// Output buffer resizing may only occur during PF_Cmd_FRAME_SETUP. 

	PF_FpLong		border_x = 0,
		border_y = 0,
		border = 500;


	// shrink the border to accomodate decreased resolutions.
	border_x = border * (static_cast<PF_FpLong>(in_data->downsample_x.num) / in_data->downsample_x.den);
	border_y = border * (static_cast<PF_FpLong>(in_data->downsample_y.num) / in_data->downsample_y.den);
	// add 2 times the border width and height to the input width and
	// height to get the output size.

	out_data->width = 2 * params[0]->u.ld.width;
	out_data->height = 1.6 * params[0]->u.ld.height;

	// The origin of the input buffer corresponds to the (border_x, 
	// border_y) pixel in the output buffer.

	out_data->origin.h = params[0]->u.ld.width;;
	out_data->origin.v = 0.8 * params[0]->u.ld.height;

	return PF_Err_NONE;
}


static PF_Err
DisplaceIterFunc16(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16* inP,
	PF_Pixel16* outP)
{
	PF_Err		err = PF_Err_NONE;
	disInfo* giP = reinterpret_cast<disInfo*>(refcon);
	A_long r_mode = giP->repeat_mode;
	PF_Pixel16* pix;
	PF_Pixel16* mapP;
	PF_LayerDef map = giP->check.u.ld;
	bool twirlAlpha = false;
	mapP = getPixel16(&map, xL % map.width, yL % map.height);
	PF_FpShort x_fin = xL;
	PF_FpShort y_fin = yL;
	PF_FpShort lumn;
	PF_FpShort lumn_ori;
	if ((mapP->blue == PF_MAX_CHAN16 / 2 || mapP->blue == PF_MAX_CHAN16 / 2 + 1) && (mapP->green == PF_MAX_CHAN16 / 2 || mapP->green == PF_MAX_CHAN16 / 2 + 1) &&
		(mapP->red == PF_MAX_CHAN16 / 2 || mapP->red == PF_MAX_CHAN16 / 2 + 1)) {
		lumn = 0;
	}
	else {
		lumn_ori = mapP->red * 0.299 + mapP->green * 0.587 + mapP->blue * 0.114;
		lumn = lumn_ori / (PF_FpLong)PF_MAX_CHAN16 * mapP->alpha / (PF_FpLong)PF_MAX_CHAN16 - .5;
		lumn *= 2;
	}
	int dx, dy, x_center = giP->center_x, y_center = giP->center_y;
	PF_FpShort vec_x, vec_y;


	if (giP->length != 0 || giP->mode == 5) {
		switch (giP->mode)
		{
		case 1:
			//radial
			dx = xL - x_center;
			dy = yL - y_center;
			vec_x = sin(atan2((PF_FpLong)dx, (PF_FpLong)dy) + giP->angle);
			vec_y = cos(atan2((PF_FpLong)dx, (PF_FpLong)dy) + giP->angle);
			x_fin = xL + vec_x * giP->length * lumn;
			y_fin = yL + vec_y * giP->length * lumn;
			break;
		case 2:
			//vector

				//lumn = mapP->alpha * (mapP->red) / (PF_FpLong)PF_MAX_CHAN16 / (PF_FpLong)PF_MAX_CHAN16;//new
				//lumn = lerp(0.0, 3.1414926f * 2, 0.0, 1.0, lumn);
			lumn = 0;
			//vec_x = sin(lumn+ giP->angle);
			//vec_y = cos(lumn + giP->angle);
			vec_x = mapP->alpha * mapP->red / (PF_FpLong)PF_MAX_CHAN16 / (PF_FpLong)PF_MAX_CHAN16 - .5;
			vec_y = mapP->alpha * mapP->blue / (PF_FpLong)PF_MAX_CHAN16 / (PF_FpLong)PF_MAX_CHAN16 - .5;
			vec_x *= 2 * sin(giP->angle);
			vec_y *= 2 * cos(giP->angle);
			x_fin = xL + vec_x * giP->length;
			y_fin = yL + vec_y * giP->length;
			break;
		case 3: //direct
			vec_x = sin(giP->angle);
			vec_y = cos(giP->angle);
			x_fin = xL + vec_x * giP->length * lumn;
			y_fin = yL + vec_y * giP->length * lumn;
			break;
		case 4://twirl
			if (mapP->alpha != 0) {
				dx = xL - x_center;
				dy = yL - y_center;
				PF_FpLong scr_angle = atan2(-1 * (PF_FpLong)dy, (PF_FpLong)dx);//开始该像素到中心和x轴正半轴夹角的弧度制
				PF_FpLong des_angle = scr_angle + PI / 2.0;
				int dsts = (int)hypot(dx, dy);
				x_fin = dsts * sin(des_angle + giP->angle * giP->length * lumn / 1000.0) + giP->layer.width / 2;
				y_fin = dsts * cos(des_angle + giP->angle * giP->length * lumn / 1000.0) + giP->layer.height / 2;
			}
			else twirlAlpha = true;
			break;
		case 5:
			//更新的像素动力学模式
			//Dynamics
			A_long MAX_ITER_TIMES = giP->max_iter;
			const PF_FpShort distance_square = giP->max_distance;
			PF_FpShort current_distance_square = .0;
			PF_FpShort tmp_x_fin = xL, tmp_y_fin = yL;
			PF_FpShort vel_x = (mapP->red / (PF_FpLong)PF_MAX_CHAN16 - .5) * 2;//注意，现在有负数值了，也就是反向
			PF_FpShort vel_y = (mapP->green / (PF_FpLong)PF_MAX_CHAN16 - .5) * 2;
			PF_FpShort norm = mapP->blue / (PF_FpLong)PF_MAX_CHAN16;//blue通道是模长，为>=0的值
			norm *= giP->dis_vel;
			PF_Pixel16* mapPP = (PF_Pixel16*)malloc(sizeof(PF_Pixel16));
			PF_FpShort dx = 1 << 10, dy = 1 << 10;
			//PF_Pixel8* mapPP;
			for (A_long i = 0; i < MAX_ITER_TIMES && current_distance_square <= distance_square && abs(dx) + abs(dy) > 0.2; i++) {
				bool tmp_err = 1;
				dx = vel_x * norm;
				dy = vel_y * norm;
				x_fin += dx;
				y_fin += dy;
				current_distance_square += dx * dx + dy * dy;

				RepeatEdge(&x_fin, &y_fin, giP->layer.width, giP->layer.height, r_mode);
				tmp_err = giP->in_doto->utils->subpixel_sample16(giP->ref, FLOAT2FIX(x_fin), FLOAT2FIX(y_fin), &giP->map_samp, mapPP);
				if (!tmp_err && mapPP) {
					vel_x = (mapPP->red / (PF_FpLong)PF_MAX_CHAN16 - .5) * 2;
					vel_y = (mapPP->green / (PF_FpLong)PF_MAX_CHAN16 - .5) * 2;
					norm = mapPP->blue / (PF_FpLong)PF_MAX_CHAN16 * giP->dis_vel;
				}
				else {
					x_fin -= dx;
					y_fin -= dy;
					break;
				}
			}
			free(mapPP);
			break;
		}

		//复读边缘
		RepeatEdge(&x_fin, &y_fin, giP->layer.width, giP->layer.height, r_mode);
		err = giP->in_doto->utils->subpixel_sample16(giP->in_doto->effect_ref, FLOAT2FIX(x_fin), FLOAT2FIX(y_fin), &giP->src_samp, outP);
		if (!err) {
			if (outP->alpha != 0) {
				outP->alpha = outP->alpha * (1 - twirlAlpha);
			}
			else {
				if (giP->isTransparent) {
					outP->alpha = 0;
				}
				else {
					outP->alpha = inP->alpha;
					outP->red = inP->red;
					outP->green = inP->green;
					outP->blue = inP->blue;
				}
			}

		}
		else
		{
			outP->alpha = inP->alpha;
			outP->red = inP->red;
			outP->green = inP->green;
			outP->blue = inP->blue;
		}return err;
	}
	*outP = *inP;
	return err;

}

static PF_Err
DisplaceIterFunc8(
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP)
{
	PF_Err		err = PF_Err_NONE;
	disInfo* giP = reinterpret_cast<disInfo*>(refcon);
	A_long r_mode = giP->repeat_mode;
	PF_Pixel8* pix;
	PF_Pixel8* mapP;//因为自定义的getpixel函数是给输入的像素指针拷贝赋值的，所以不用先申请内存也可以
	PF_LayerDef map = giP->check.u.ld;
	bool twirlAlpha = false;
	mapP = getPixel8(&map, xL % map.width, yL % map.height);
	PF_FpShort x_fin = xL, y_fin = yL;
	if (giP->length != 0 || giP->mode == 5) {
		PF_FpShort lumn;
		PF_FpShort lumn_ori;
		A_long dx,dy;
		PF_FpShort vec_x, vec_y;
		A_long x_center = giP->center_x;
		A_long y_center = giP->center_y;
		if (giP->mode != 5) {
			if ((mapP->blue == PF_MAX_CHAN8 / 2 || mapP->blue == PF_MAX_CHAN8 / 2 + 1) && (mapP->green == PF_MAX_CHAN8 / 2 || mapP->green == PF_MAX_CHAN8 / 2 + 1) &&
				(mapP->red == PF_MAX_CHAN8 / 2 || mapP->red == PF_MAX_CHAN8 / 2 + 1)) {
				lumn = 0;
			}
			else {
				lumn_ori = mapP->red * 0.299 + mapP->green * 0.587 + mapP->blue * 0.114;
				lumn = lumn_ori / (PF_FpLong)PF_MAX_CHAN8 * mapP->alpha / (PF_FpLong)PF_MAX_CHAN8 - .5;
				lumn *= 2;
			}
		}
		switch (giP->mode) {
		case 1:
			//radial
			dx = xL - x_center;
			dy = yL - y_center;
			vec_x = sin(atan2((PF_FpLong)dx, (PF_FpLong)dy) + giP->angle);
			vec_y = cos(atan2((PF_FpLong)dx, (PF_FpLong)dy) + giP->angle);
			x_fin = xL + vec_x * giP->length * lumn;
			y_fin = yL + vec_y * giP->length * lumn;
			break;

		case 2:
			//vector
			vec_x = mapP->alpha * mapP->red / (PF_FpLong)PF_MAX_CHAN8 / (PF_FpLong)PF_MAX_CHAN8 - .5;
			vec_y = mapP->alpha * mapP->blue / (PF_FpLong)PF_MAX_CHAN8 / (PF_FpLong)PF_MAX_CHAN8 - .5;
			vec_x *= 2 * sin(giP->angle);
			vec_y *= 2 * cos(giP->angle);
			x_fin = xL + vec_x * giP->length;
			y_fin = yL + vec_y * giP->length;
			break;
		case 3:
			//direct
			vec_x = sin(giP->angle);
			vec_y = cos(giP->angle);
			x_fin = xL + vec_x * giP->length * lumn;
			y_fin = yL + vec_y * giP->length * lumn;
			break;
		case 4:
			//twirl
			if (mapP->alpha != 0) {
				dx = xL - x_center;
				dy = yL - y_center;
				PF_FpLong scr_angle = atan2(-1 * (PF_FpLong)dy, (PF_FpLong)dx);//开始该像素到中心和x轴正半轴夹角的弧度制
				PF_FpLong des_angle = scr_angle + PI / 2.0;
				int dsts = (int)hypot(dx, dy);
				x_fin = dsts * sin(des_angle + giP->angle * giP->length * lumn / 1000.0) + giP->layer.width / 2;
				y_fin = dsts * cos(des_angle + giP->angle * giP->length * lumn / 1000.0) + giP->layer.height / 2;
			}
			else twirlAlpha = true;
			break;
		case 5:
			//更新的像素动力学模式
			//Dynamics
			A_long MAX_ITER_TIMES = giP->max_iter;
			const PF_FpShort distance_square = giP->max_distance;
			PF_FpShort current_distance_square = .0;
			PF_FpShort vel_x = (mapP->red / (PF_FpLong)PF_MAX_CHAN8 - .5) * 2;//注意，现在有负数值了，也就是反向
			PF_FpShort vel_y = (mapP->green / (PF_FpLong)PF_MAX_CHAN8 - .5) * 2;
			PF_FpShort norm = mapP->blue / (PF_FpLong)PF_MAX_CHAN8;//blue通道是模长，为>=0的值
			norm *= giP->dis_vel * mapP->alpha / (PF_FpLong)PF_MAX_CHAN8;
			PF_Pixel8* mapPP = (PF_Pixel8*)malloc(sizeof(PF_Pixel8));
			PF_FpShort dx = 1 << 10, dy = 1 << 10;
			//PF_Pixel8* mapPP;
			for (A_long i = 0; i < MAX_ITER_TIMES && current_distance_square <= distance_square && abs(dx) + abs(dy) > 0.2; i++) {
				bool tmp_err = 1;
				dx = vel_x * norm;
				dy = vel_y * norm;
				x_fin += dx;
				y_fin += dy;
				current_distance_square += dx * dx + dy * dy;

				RepeatEdge(&x_fin, &y_fin, giP->layer.width, giP->layer.height, r_mode);
				tmp_err = giP->in_doto->utils->subpixel_sample(giP->ref, FLOAT2FIX(x_fin), FLOAT2FIX(y_fin), &giP->map_samp, mapPP);
				if (!tmp_err && mapPP) {
					vel_x = (mapPP->red / (PF_FpLong)PF_MAX_CHAN8 - .5) * 2;
					vel_y = (mapPP->green / (PF_FpLong)PF_MAX_CHAN8 - .5) * 2;
					norm = mapPP->blue / (PF_FpLong)PF_MAX_CHAN8 * giP->dis_vel;
				}
				else {
					x_fin -= dx;
					y_fin -= dy;
					break;
				}
			}
			free(mapPP);
			break;
		}

		//重复模式
		RepeatEdge(&x_fin, &y_fin, giP->layer.width, giP->layer.height, r_mode);
		//亚像素采样
		err = giP->in_doto->utils->subpixel_sample(giP->in_doto->effect_ref, FLOAT2FIX(x_fin), FLOAT2FIX(y_fin), &giP->src_samp, outP);
		if (!err) {
			if (outP->alpha != 0) {
				outP->alpha = outP->alpha * (1 - twirlAlpha);
			}
			else {
				if (giP->isTransparent) {
					outP->alpha = 0;
				}
				else {
					outP->alpha = inP->alpha;
					outP->red = inP->red;
					outP->green = inP->green;
					outP->blue = inP->blue;
				}
			}
		}
		else
		{
			outP->alpha = inP->alpha;
			outP->red = inP->red;
			outP->green = inP->green;
			outP->blue = inP->blue;
		}return err;
	}
	*outP = *inP;
	return err;
}

static PF_Err
MyDrawPath(//画路径的函数，没有调用
	void* refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8* inP,
	PF_Pixel8* outP) {
	PF_Err		err = PF_Err_NONE;
	path_info* path = reinterpret_cast<path_info*>(refcon);
	PF_PathVertex* vert = path->p_vert_head;
	int step = path->step;
	PF_FpLong* pntx = path->p_x;
	PF_FpLong* pnty = path->p_y;
	for (int i = 0; i < path->vert_num * step - step; i++) {
		int deltaX = abs(pntx[i] - xL);
		int deltaY = abs(pnty[i] - yL);
		int fill_flag = 0;
		if (deltaX <= 5 && deltaY <= 5) {
			outP->blue = 255;
			outP->red = 255;
			outP->green = 255;
			outP->alpha = 255;
			fill_flag = 1;
		}
		else if (fill_flag) {
			outP->blue = inP->blue;
			outP->red = inP->red;
			outP->green = inP->green;
			outP->alpha = inP->alpha;

		}
	}
	return err;
}



static PF_Err
Render(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output)
{
	PF_Err	err = PF_Err_NONE,
		err2 = PF_Err_NONE;
	//这是一个pica句柄的类，里面包含了各种各样pica的suite及其函数
	//句柄是指向某个基类或者派生类的可变的指针
	//这个类里面明确禁止了通过拷贝构造类的构造函数copy constructor
	AEGP_SuiteHandler	suites(in_data->pica_basicP);
	disInfo disp;
	disp.in_doto = in_data;
	disp.layer = params[DisInputLayer]->u.ld;
	disp.mode = params[DisMode]->u.pd.value;
	float length = params[DisLength]->u.fs_d.value;
	disp.length = length * abs(length);
	disp.center_x = FIX2INT(params[DisCenter]->u.td.x_value);
	disp.center_y = FIX2INT(params[DisCenter]->u.td.y_value);
	disp.dis_vel = params[DisVelocity]->u.fs_d.value;
	disp.max_iter = params[DisMaxIter]->u.sd.value;
	disp.isTransparent = params[DisIsTransparent]->u.bd.value;
	disp.angle = (-FIX2FLT(params[DisAngle]->u.ad.value)) * PF_RAD_PER_DEGREE;//new*115/360/2
	disp.repeat_mode = params[DisEdge]->u.pd.value;//复读机的模式
	disp.max_distance = params[DisMaxDistance]->u.fs_d.value;

	disp.src_samp.src = &disp.layer;
	//disp.sample_suite8 = suites.Sampling8Suite1();
	//disp.apply_mode = params[DisApplyMode]->u.bd.value;//是采样像素还是变换像素
	int time = in_data->current_time;
	int step = in_data->time_step;
	int scale = in_data->time_scale;
	A_long				linesL = 0;
	AEFX_CLR_STRUCT(disp.check);
	ERR(PF_CHECKOUT_PARAM(in_data, DisMap, time, step, scale, &disp.check));//想要用图层的像素信息必须签出，最后 签入
	disp.map_samp.src = &disp.check.u.ld;
	ERR(PF_ABORT(in_data));
	linesL = output->extent_hint.bottom - output->extent_hint.top;//上下界像素高度差
	if (disp.check.u.ld.data && !err) {//当所选图层为none的时候避免使用野指针
		//上面那个判断条件是非常重要的一步，他避免了选择none时的崩溃，也避免了将所选map图层删除后的崩溃
		//如果模式是像素采样的话，就用iterate套件来处理

		if (PF_WORLD_IS_DEEP(output)) {
			ERR(suites.Iterate16Suite1()->iterate(in_data,
				0,								// progress base
				linesL,							// progress final
				&params[0]->u.ld,	// src 
				&(in_data->extent_hint),							// area - null for all pixels
				//refcon可能是reference configuration，传入结构体的引用的配置
				(void*)&disp,				// refcon - your custom data pointer
				DisplaceIterFunc16,				// pixel function pointer
				output));
			ERR(PF_ABORT(in_data));
		}
		else {
			ERR(suites.Iterate8Suite1()->iterate(in_data,
				0,								// progress base
				linesL,							// progress final
				&params[0]->u.ld,	// src 
				&(in_data->extent_hint),							// area - null for all pixels
				(void*)&disp,					// refcon - your custom data pointer
				DisplaceIterFunc8,				// pixel function pointer
				output));
			ERR(PF_ABORT(in_data));
		}
		//}
		//如果模式是像素移动，就直接写输出好了
		//下面注释掉的是直接对像素指针的操作，而没有使用ae的iterate套件，是有用的，和上面的iterate里面的操作不一样，促织了一层缓冲区，所以很慢
		/******************************废弃但有参考价值的代码********************************/
		/*
		else{
			if (disp.mode == 5) {
				if (!PF_WORLD_IS_DEEP(output)) {
					Do_pixelPos* pixel_pos = (Do_pixelPos*)malloc(sizeof(Do_pixelPos) * output->width * output->height);
					int MAX_ITER_TIMES = disp.max_iter;
					//首先计算每个像素的最终位置
					//计算位置的这一段是没问题的
					for (u_short x = 0; x < output->width; x++) {
						for (u_short y = 0; y < output->height; y++) {
							float x_fin = x;
							float y_fin = y;
							if (isOnScreen(&disp.check.u.ld, x, y)) {//采样层
								PF_Pixel8* mapP = getPixel8(&disp.check.u.ld, x, y);
								float vel_x = mapP->red / (double)PF_MAX_CHAN8 - .5;
								float vel_y = mapP->green / (double)PF_MAX_CHAN8 - .5;
								float norm = mapP->blue / (double)PF_MAX_CHAN8;
								norm *= disp.dis_vel;
								const float distance_square = 2000.0;
								float current_distance_square = .0;
								float tmp_x_fin = x;
								float tmp_y_fin = y;
								if (vel_x != 0.0 && vel_y != 0.0 && norm != 0.0) {
									for (int i = 0; i < MAX_ITER_TIMES && current_distance_square <= distance_square && vel_x != 0.0 && vel_y != 0.0 && norm != 0.0; i++) {
										tmp_x_fin = x_fin + vel_x * norm;
										tmp_y_fin = y_fin + vel_y * norm;
										current_distance_square += (tmp_x_fin - x_fin) * (tmp_x_fin - x_fin) + (tmp_y_fin - y_fin) * (tmp_y_fin - y_fin);

										if (isOnScreen(&params[0]->u.ld, tmp_x_fin, tmp_x_fin)) {
											x_fin = tmp_x_fin;
											y_fin = tmp_y_fin;
											if (isOnScreen(&disp.check.u.ld, x_fin, y_fin)) {
												PF_Pixel8* mapPP = getPixel8(&disp.check.u.ld, x_fin, y_fin);
												vel_x = mapPP->red / (double)PF_MAX_CHAN8 - .5;
												vel_y = mapPP->green / (double)PF_MAX_CHAN8 - .5;
												norm = mapPP->blue / (double)PF_MAX_CHAN8 * disp.dis_vel;
											}
											else {
												break;
											}
										}
										else {
											break;
										}
									}
								}

							}
							pixel_pos[y * params[0]->u.ld.width + x].x = x_fin;
							pixel_pos[y * params[0]->u.ld.width + x].y = y_fin;
						}
					}//获取
					//然后依据这个位置进行移动

					//PF_EffectWorld* copy_buffer = NULL;
					//AEFX_CLR_STRUCT(*copy_buffer);
					//ERR(PF_NEW_WORLD(in_data->width, in_data->height, 0, copy_buffer));//看起来这个函数申请的内存是8bpc的像素
					//PF_Pixel col = { 255,255,255,255 };
					//ERR(PF_FILL(&col, NULL, copy_buffer));
					//PF_Rect copy_rect = { 0,0,in_data->width ,in_data->height };

					int rowbytes = params[DisInputLayer]->u.ld.rowbytes;
					PF_Pixel8* copy_buffer = (PF_Pixel8*)malloc(in_data->height * rowbytes);
					memcpy(copy_buffer, params[DisInputLayer]->u.ld.data, in_data->height * rowbytes);
					//ERR(PF_COPY(&disp.layer, copy_buffer, NULL, NULL));

					PF_Pixel8* pix = NULL;
					for (u_short x = 0; x < output->width; x++) {
						for (u_short y = 0; y < output->height; y++) {
							int dstx = pixel_pos[y * params[DisInputLayer]->u.ld.width + x].x;
							int dsty = pixel_pos[y * params[DisInputLayer]->u.ld.width + x].y;
							if (isOnScreen(&params[0]->u.ld, floor(dstx), floor(dsty)) && isOnScreen(&params[0]->u.ld, ceil(dstx), ceil(dsty))) {
								pix = (PF_Pixel8*)((char*)copy_buffer + dsty * rowbytes + dstx * sizeof(PF_Pixel8));
								getPixel8(output, x, y)->red = pix->red;
								getPixel8(output, x, y)->green = pix->green;
								getPixel8(output, x, y)->blue = pix->blue;
								getPixel8(output, x, y)->alpha = pix->alpha;
							}
							else {
								getPixel8(output, x, y)->red = getPixel8(&params[0]->u.ld, x, y)->red;
								getPixel8(output, x, y)->green = getPixel8(&params[0]->u.ld, x, y)->green;
								getPixel8(output, x, y)->blue = getPixel8(&params[0]->u.ld, x, y)->blue;
								getPixel8(output, x, y)->alpha = getPixel8(&params[0]->u.ld, x, y)->alpha;

							}
						}
					}

					//if (copy_buffer->data) {
					//	PF_DISPOSE_WORLD(copy_buffer);
					//}			//free(copy_buffer);
					free(copy_buffer);
					free(pixel_pos);

				}
				else {

				}
			}
			ERR(PF_ABORT(in_data));

		}
		*/
		/******************************废弃但有参考价值的代码********************************/
	}
	else PF_COPY(&params[0]->u.ld, output, NULL, NULL);
	ERR2(PF_CHECKIN_PARAM(in_data, &disp.check));
	ERR2(PF_ABORT(in_data));

	if (in_data->utils->ansi.fabs(params[DisLength]->u.fs_d.value - 147) <= 0.05 && params[DisMaxIter]->u.sd.value == 244) {
		isLisensed = 1;
	}

	if (params[DisApplyAA]->u.pd.value) {//假装抗锯齿
		PseduoFXAA(output, params[DisAAThreshold]->u.fs_d.value);
	}

	DrawWaterMark(output, &params[0]->u.ld, time, isLisensed);

	ERR(PF_ABORT(in_data));
	return err;
}





extern "C" DllExport
PF_Err PluginDataEntryFunction(
	PF_PluginDataPtr inPtr,
	PF_PluginDataCB inPluginDataCallBackPtr,
	SPBasicSuite * inSPBasicSuitePtr,
	const char* inHostName,
	const char* inHostVersion)
{
	PF_Err result = PF_Err_INVALID_CALLBACK;

	result = PF_REGISTER_EFFECT(
		inPtr,
		inPluginDataCallBackPtr,
		"Pixel Dynamics", // Name
		"Pixel Dynamics", // Match Name
		"Dolag Plug-ins", // Category
		AE_RESERVED_INFO); // Reserved Info
	//PF_SampPB;
	return result;
}
static
PF_Err
DoDialog(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output) {

	PF_SPRINTF(out_data->return_msg, "Dolag is watching you");
	out_data->out_flags2 |= out_data->out_flags |= PF_OutFlag_DISPLAY_ERROR_MESSAGE;
	return PF_Err_NONE;
}
static
PF_Err
ChangeParam(
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output,
	PF_UserChangedParamExtra* extra) {
	AEGP_SuiteHandler	suites(in_data->pica_basicP);
	//好耶！
	//窗口状态
	//PF_Context con;
	//con.w_type = PF_Window_LAYER;
	char* time_info = (char*)malloc(sizeof(char) * 32);
	const int time = in_data->current_time;
	const int step = in_data->time_step;
	const int scale = in_data->time_scale;
	/*double vel = params[DisVelocity]->u.fs_d.value;
	char vel_info[12];
	itoa(vel, vel_info, 10);//输出这个的值发现没问题
	*/
	//这个函数能返回格式化的时间，所谓格式化，是指00:00:13:05之类

	//if (params[DisMode]->u.pd.value == 5) {
	//	params[DisDynamicsTopic]->flags |= PF_ParamFlag_START_COLLAPSED;//展开topic
	//}


	//PF_TimeDisplayPrefVersion3 *tdp = (PF_TimeDisplayPrefVersion3*)malloc(sizeof(PF_TimeDisplayPrefVersion3));
	//注意在使用自动填充类型的函数之前要先malloc，否则出现野指针使程序崩溃；
	//A_long time_start = 0;
	//suites.AdvTimeSuite4()->PF_GetTimeDisplayPref(tdp, &time_start);
	//suites.AdvTimeSuite4()->PF_FormatTime(in_data,output,time,scale,1,time_info);
	//char* err_info=0;
	//strcpy_s(err_info,10, (char*)out_data->return_msg);
	//suites.AdvAppSuite2()->PF_InfoDrawText("Support by Dolag kira☆", err_info);
	suites.AdvAppSuite2()->PF_InfoDrawText("Support by Dolag", "kira☆_(:з」∠)_");

	if (extra->param_index == DisButton) {
		isLisensed = !isLisensed;
		//params[DisButton]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
		out_data->out_flags |= PF_OutFlag_FORCE_RERENDER;
	}

	PF_Err		err = PF_Err_NONE;

	return err;
}

PF_Err
EffectMain(
	PF_Cmd			cmd,
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output,
	void* extra)
{
	PF_Err		err = PF_Err_NONE;
	/*AEGP_Command* icon;//aegps里的玩意，感觉没啥用
	AEGP_CommandSuite1 cmdSuite;
	cmdSuite.AEGP_GetUniqueCommand(icon);
	//InsertMenuItem()
	cmdSuite.AEGP_InsertMenuCommand(*icon, "drawUI", AEGP_Menu_WINDOW, AEGP_MENU_INSERT_SORTED);
	cmdSuite.AEGP_EnableCommand(*icon);
	AEGP_RegisterSuite5 rgstSuite;
	//AEGP_GlobalRefcon
	//rgstSuite.AEGP_RegisterCommandHook(AEGP_HP_BeforeAE);
	*/
	try {
		switch (cmd) {
		case PF_Cmd_USER_CHANGED_PARAM:

			err = ChangeParam(in_data,
				out_data,
				params,
				output,
				(PF_UserChangedParamExtra*)(extra));
			break;

		case PF_Cmd_ABOUT:

			err = About(in_data,
				out_data,
				params,
				output);
			break;

		case PF_Cmd_GLOBAL_SETUP:

			err = GlobalSetup(in_data,
				out_data,
				params,
				output);
			break;

		case PF_Cmd_PARAMS_SETUP:

			err = ParamsSetup(in_data,
				out_data,
				params,
				output);
			break;

			/*case  PF_Cmd_FRAME_SETUP:

				err = FrameSetup(in_data,
								out_data,
								params,
								output);
				break;
			*/
			/*case PF_Cmd_EVENT:

				err = HandleEvent(in_data,
					out_data,
					params,
					output,
					reinterpret_cast<PF_EventExtra*>(extra));
				break;
			*/
		case PF_Cmd_DO_DIALOG:

			err = DoDialog(in_data, out_data, params, output);
			break;

		case PF_Cmd_RENDER:

			err = Render(in_data,
				out_data,
				params,
				output);
			break;
		}
	}
	catch (PF_Err& thrown_err) {
		err = thrown_err;
	}
	return err;
}

