#ifndef BiquadFilter_H
#define BiquadFilter_H

#include "biquad.h"
#include "public.sdk/source/vst2.x/audioeffectx.h"

enum
{
	// Global
	kNumPrograms = 1,

	// Parameters Tags
	kType = 0,
	kFrequency,
	kResonance,
	kGain,

	kNumParams
};

class BiquadFilter : public AudioEffectX
{
public:
	BiquadFilter (audioMasterCallback audioMaster);
	~BiquadFilter ();
	virtual void setParameter (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterDisplay (VstInt32 index, char* text);
	virtual void getParameterLabel (VstInt32 index, char* label);
	virtual void getParameterName (VstInt32 index, char* text);
	virtual void setProgramName (char* name);
	virtual void getProgramName (char* name);
	virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);
	virtual bool getEffectName (char* name);
	virtual bool getProductString (char* text);
	virtual bool getVendorString (char* text);
	virtual VstInt32 getVendorVersion ();
	virtual VstPlugCategory getPlugCategory ();
	virtual void setSampleRate (float sampleRate);
	virtual void setBlockSize (VstInt32 blockSize);
	virtual void resume ();
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
private:
	float Type;
	float Frequency;
	float Resonance;
	float Gain;
	char ProgramName[32];
	sf_biquad_state_st bq_state;
	sf_sample_st *buf;
	int rate;
	int bufsize;
};

#endif
