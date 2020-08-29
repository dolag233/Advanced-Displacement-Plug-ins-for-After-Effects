#include "AEConfig.h"
#include "AE_EffectVers.h"

#ifndef AE_OS_WIN
	#include <AE_General.r>
#endif
	
resource 'PiPL' (16000) {
	{	/* array properties: 12 elements */
		/* [1] */
		Kind {
			AEEffect
		},
		/* [2] */
		Name {
			"Pixel Dynamics"
		},
		/* [3] */
		Category {
			"Dolag Plug-ins"
		},
#ifdef AE_OS_WIN
	#ifdef AE_PROC_INTELx64
		CodeWin64X86 {"EffectMain"},//这里可以编辑入口函数的名称
	#else
		CodeWin32X86 {"EntryPointFunc"},//32位
	#endif
#else
	#ifdef AE_OS_MAC
		CodeMacIntel64 {"EffectMain"},//mac的
	#endif
#endif
		/* [6] */
		AE_PiPL_Version {
			2,
			0
		},
		/* [7] */
		AE_Effect_Spec_Version {
			PF_PLUG_IN_VERSION,
			PF_PLUG_IN_SUBVERS
		},
		/* [8] */
		AE_Effect_Version {
			524289	/* 1.0 */
		},
		/* [9] */
		AE_Effect_Info_Flags {
			0
		},
		/* [10] */
		AE_Effect_Global_OutFlags {
		//0x02000006 //50332160//这里的flag要和parameter setup时的flag对应
		0x02000226

		},
		AE_Effect_Global_OutFlags_2 {
		1048584
		},
		/* [11] */
		AE_Effect_Match_Name {
			"Pixel Dynamics"
		},
		/* [12] */
		AE_Reserved_Info {
			0
		}
	}
};

