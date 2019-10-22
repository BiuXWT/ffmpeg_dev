extern "C" {
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
#include "libavcodec/avcodec.h"
#include "libavutil/avutil.h"
#include "libavutil/imgutils.h"
#include "libavutil/opt.h"
#include "libavutil/audio_fifo.h"
#include "libswscale/swscale.h"
#include "libavfilter/buffersink.h"
#include "libswresample/swresample.h"
#include "libavfilter/buffersrc.h"
	}
int main()
{
	//av_register_all();
	printf("%s\n", avcodec_configuration());
	AVFrame *frame = NULL;
	getchar();
	return 0;
}