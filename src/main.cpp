#include "BiquadFilter.h"

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new BiquadFilter (audioMaster);
}
