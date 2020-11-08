#pragma once
#include "AEConfig.h"


#ifdef AE_OS_WIN
typedef unsigned short PixelType;
#include <Windows.h>
#endif
#define PF_TABLE_BITS	12
#define PF_TABLE_SZ_16	4096
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
//#include "DolagDisp_Strings.h"
//#include "dolag_ae_func.h"*/
class DolagAEParams//生命周期限定在params_setup内
{
public:
	DolagAEParams(PF_InData*& in_data, PF_OutData*& out_data)
		:in_data(in_data)
	{
		AEFX_CLR_STRUCT(def);
	};
	~DolagAEParams() {
		AEFX_CLR_STRUCT(def);
	}
	inline A_u_short AddFloatSliderX(const char* param_name,const float value_max,const float value_min,const float slider_max,const float slider_min,const float default,const A_long precision,bool displace_flag,const A_long flags,const A_u_short diskID = -1);
	inline A_u_short AddButton(const char* param_name, const char* button_message,const A_long PUI_control,const A_long flags,const A_u_short diskID = -1);
	inline A_u_short AddLayer(const char* param_name, const A_u_short diskID = -1, const A_long default_layer = PF_LayerDefault_MYSELF);
	inline A_u_short AddCheckboxX(const char* param_name,const bool default,const A_long flags,const A_u_short diskID = -1);
	inline A_u_short AddAngle(const char* param_name,const float default,const A_u_short diskID = -1);
	inline A_u_short AddTopicBegin(const char* param_name, const A_long flags,const A_u_short diskID = -1);
	inline A_u_short AddTopicEnd(const A_u_short diskID = -1);//注意这里的diskID = -1要和TopicBegin的一致
	inline A_u_short AddPOPUPX(const char* param_name,const A_u_short pop_num,const A_u_short default,const char* choices,const A_long flags,const A_u_short diskID = -1);
	A_u_short GetParamsNum() const;
private:
	PF_InData *in_data;
	PF_ParamDef def;
	A_u_long param_index = 0;
};

