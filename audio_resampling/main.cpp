
#include "resample.h"

int main()
{
	EncAudSwr *p_swr = new EncAudSwr(48000, 44100, AV_SAMPLE_FMT_S16, AV_SAMPLE_FMT_S16);
	printf("%s\n", avcodec_configuration());
	system("pause");
	return 0;
}