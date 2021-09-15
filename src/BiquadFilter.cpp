#include "BiquadFilter.h"

BiquadFilter::BiquadFilter (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{
	// init
	setNumInputs (2);	// stereo input
	setNumOutputs (2);	// stereo output
	setUniqueID ('BQFL');	// identify
	Type = 0;
	Frequency = 440;
	Resonance = 0;
	Gain = 0;
	rate = (int)sampleRate;
	bufsize = blockSize;
	buf = NULL;
	buf = new sf_sample_st[bufsize];
	if (buf)
	{
		memset(buf, 0, 8*bufsize);
	}
	strcpy (ProgramName, "Default");
	canProcessReplacing ();
	resume ();
}

BiquadFilter::~BiquadFilter ()
{
	if (buf)
	{
		delete[] buf;
		buf = NULL;
	}
}

void BiquadFilter::setParameter (VstInt32 index, float value)
{
	switch(index)
	{
	case kType:
		Type = value;
		break;
	case kFrequency:
		Frequency = value*(rate/2);
		break;
	case kResonance:
		Resonance = value*10;
		break;
	case kGain:
		Gain = value*10;
		break;
	}
	resume ();
}

float BiquadFilter::getParameter (VstInt32 index)
{
	float value = 0;
	switch(index)
	{
	case kType:
		value = Type;
		break;
	case kFrequency:
		value = Frequency/(rate/2);
		break;
	case kResonance:
		value = Resonance/10;
		break;
	case kGain:
		value = Gain/10;
		break;
	}
	return value;
}

void BiquadFilter::getParameterDisplay (VstInt32 index, char* text)
{
	switch(index)
	{
	case kType:
		if (Type >= 0.0 && Type < 0.125)	
		{
			strcpy (text, "LOWPASS");					
		}
		else if (Type >= 0.125 && Type < 0.25)	
		{
			strcpy (text, "HIGHPASS");					
		}
		else if (Type >= 0.25 && Type < 0.375)	
		{
			strcpy (text, "BANDPASS");					
		}
		else if (Type >= 0.375 && Type < 0.5)	
		{
			strcpy (text, "ALLPASS");					
		}
		else if (Type >= 0.5 && Type < 0.625)	
		{
			strcpy (text, "NOTCH");					
		}
		else if (Type >= 0.625 && Type < 0.75)	
		{
			strcpy (text, "PEAKING");					
		}
		else if (Type >= 0.75 && Type < 0.875)	
		{
			strcpy (text, "LOWSHELF");					
		}
		else if (Type >= 0.875 && Type <= 1.0)	
		{
			strcpy (text, "HIGHSHELF");					
		}
		else
		{
			strcpy (text, "NONE");					
		}
		break;
	case kFrequency:
		float2string (Frequency, text, kVstMaxParamStrLen);
		break;
	case kResonance:
		float2string (Resonance, text, kVstMaxParamStrLen);
		break;
	case kGain:
		float2string (Gain, text, kVstMaxParamStrLen);
		break;
	}
}

void BiquadFilter::getParameterLabel (VstInt32 index, char* label)
{
	switch(index)
	{
	case kFrequency:
		strcpy (label, "hZ");
		break;
	case kResonance:
		strcpy (label, "dB");
		break;
	case kGain:
		strcpy (label, "dB");
		break;
	}
}

void BiquadFilter::getParameterName (VstInt32 index, char* text)
{
	switch(index)
	{
	case kType:
		strcpy (text, "Type");
		break;
	case kFrequency:
		strcpy (text, "Frequency");
		break;
	case kResonance:
		strcpy (text, "Resonance");
		break;
	case kGain:
		strcpy (text, "Gain");
		break;
	}
}

void BiquadFilter::setProgramName (char *name)
{
	strcpy (ProgramName, name);
}

void BiquadFilter::getProgramName (char *name)
{
	strcpy (name, ProgramName);
}

bool BiquadFilter::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
	if (index == 0)
	{
		strcpy (text, ProgramName);
		return true;
	}
	return false;
}

bool BiquadFilter::getEffectName (char* name)
{
	strcpy (name, "BiquadFilter");
	return true;
}

bool BiquadFilter::getProductString (char* text)
{
	strcpy (text, "BiquadFilter");
	return true;
}

bool BiquadFilter::getVendorString (char* text)
{
	strcpy (text, "Datajake");
	return true;
}

VstInt32 BiquadFilter::getVendorVersion ()
{
	return 1000;
}

VstPlugCategory BiquadFilter::getPlugCategory ()
{
	return kPlugCategEffect;
}

void BiquadFilter::setSampleRate (float sampleRate)
{
	rate = (int)sampleRate;
}

void BiquadFilter::setBlockSize (VstInt32 blockSize)
{
	if (buf)
	{
		delete[] buf;
		buf = NULL;
	}
	bufsize = blockSize;
	buf = new sf_sample_st[bufsize];
	if (buf)
	{
		memset(buf, 0, 8*bufsize);
	}
}

void BiquadFilter::resume ()
{
	if (Type >= 0.0 && Type < 0.125)	
	{
		sf_lowpass(&bq_state, rate, Frequency/2, Resonance);
	}
	else if (Type >= 0.125 && Type < 0.25)	
	{
		sf_highpass(&bq_state, rate, Frequency/2, Resonance);
	}
	else if (Type >= 0.25 && Type < 0.375)	
	{
		sf_bandpass(&bq_state, rate, Frequency/2, Resonance);
	}
	else if (Type >= 0.375 && Type < 0.5)	
	{
		sf_allpass(&bq_state, rate, Frequency/2, Resonance);
	}
	else if (Type >= 0.5 && Type < 0.625)	
	{
		sf_notch(&bq_state, rate, Frequency/2, Resonance);
	}
	else if (Type >= 0.625 && Type < 0.75)	
	{
		sf_peaking(&bq_state, rate, Frequency/2, Resonance, Gain);
	}
	else if (Type >= 0.75 && Type < 0.875)	
	{
		sf_lowshelf(&bq_state, rate, Frequency/2, Resonance, Gain);
	}
	else if (Type >= 0.875 && Type <= 1.0)	
	{
		sf_highshelf(&bq_state, rate, Frequency/2, Resonance, Gain);
	}
	else
	{
		sf_highpass(&bq_state, rate, 0, 0);
	}
}

void BiquadFilter::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	float* in1 = inputs[0];
	float* in2 = inputs[1];
	float* out1 = outputs[0];
	float* out2 = outputs[1];
	int i;
	if (buf == NULL)
	{
		return;
	}
	if (sampleFrames > bufsize)
	{
		sampleFrames = bufsize;
	}
	for (i=0; i<sampleFrames; i++)
	{
		buf[i].L = *in1;
		buf[i].R = *in2;
		*in1++;
		*in2++;
	}
	sf_biquad_process(&bq_state, sampleFrames, buf, buf);
	for (i=0; i<sampleFrames; i++)
	{
		*out1 = buf[i].L;
		*out2 = buf[i].R;
		*out1++;
		*out2++;
	}
}
