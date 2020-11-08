#include "DolagAEClass.h"
A_u_short DolagAEParams::AddFloatSliderX(const char* param_name, const float value_max, const float value_min, const float slider_max, const float slider_min, const float default, const A_long precision, bool displace_flag, const A_long flags, const A_u_short diskID) {
	param_index++;
	AEFX_CLR_STRUCT(def);
	diskID == -1 ? param_index : diskID;
	PF_ADD_FLOAT_SLIDERX(param_name, value_min, value_max, slider_min, slider_max, default, precision, displace_flag, flags,diskID);
	return diskID;
}

A_u_short DolagAEParams::AddButton(const char* param_name, const char* button_message, const A_long PUI_control, const A_long flags, const A_u_short diskID) {
	param_index++;
	diskID == -1 ? param_index : diskID;
	AEFX_CLR_STRUCT(def);
	PF_ADD_BUTTON(param_name, button_message, PUI_control, flags, diskID);
	return diskID;
}

A_u_short DolagAEParams::AddLayer(const char* param_name, const A_u_short diskID, const A_long default_layer) {
	param_index++;
	diskID == -1 ? param_index : diskID;
	AEFX_CLR_STRUCT(def);
	PF_ADD_LAYER(param_name, default_layer, diskID);
	return diskID;
}

A_u_short DolagAEParams::AddCheckboxX(const char* param_name, const bool default, const A_long flags, const A_u_short diskID) {
	param_index++;
	diskID == -1 ? param_index : diskID;
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOXX(param_name, default, flags, diskID);
	return diskID;
}

A_u_short DolagAEParams::AddPOPUPX(const char* param_name, const A_u_short pop_num, const A_u_short default, const char* choices, const A_long flags, const A_u_short diskID){
	param_index++;
	diskID == -1 ? param_index : diskID;
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUPX(param_name, pop_num, default, choices, flags, diskID);
	return diskID;
}

A_u_short DolagAEParams::AddAngle(const char* param_name, const float default, const A_u_short diskID) {
	param_index++;
	diskID == -1 ? param_index : diskID;
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(param_name, default, diskID);
	return diskID;
}

A_u_short DolagAEParams::AddTopicBegin(const char* param_name,const A_long flags, const A_u_short diskID) {
	param_index++;
	diskID == -1 ? param_index : diskID;
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPICX(param_name, flags, diskID);
	return diskID;
}

A_u_short DolagAEParams::AddTopicEnd(const A_u_short diskID) {
	param_index++;
	diskID == -1 ? param_index : diskID;
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(diskID);
	return param_index;
}

A_u_short DolagAEParams::GetParamsNum() const{
	return param_index;
}