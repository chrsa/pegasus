#include "VoiceDataBase.h"


VoiceDataBase::VoiceDataBase()
{
	voices_["English"]["Male"]["Chris"] = "en-US-ChristopherNeural";
	voices_["English"]["Male"]["Jacob"] = "en-US-JacobNeural";

	voices_["English"]["Female"]["Amber"] = "en-US-AmberNeural";
	voices_["English"]["Female"]["Ana"] = "en-US-AnaNeural";


}

const VoiceDataBase::MapType &VoiceDataBase::Voices()
{
	return voices_;
}
