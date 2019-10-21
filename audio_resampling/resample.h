#ifndef __ENC_AUD_SWR_H__
#define __ENC_AUD_SWR_H__

extern "C" {
#include <libavutil/opt.h>
#include <libavutil/channel_layout.h>
#include <libavutil/samplefmt.h>
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

class EncAudSwr
{
public:
	EncAudSwr(int src_rate, int dst_rate,
		AVSampleFormat src_sample_fmt, AVSampleFormat dst_sample_fmt);
	~EncAudSwr();
	int process(uint8_t **data);
protected:
	AVSampleFormat m_src_sample_fmt;
	AVSampleFormat m_dst_sample_fmt;
	int m_src_rate;
	int m_dst_rate;
	int m_src_nb_channels;
	int m_dst_nb_channels;
	int m_src_linesize;
	int m_dst_linesize;
	int m_src_nb_samples;
	int m_dst_nb_samples;
	int m_max_dst_nb_samples;
	SwrContext *m_swr_ctx;
	uint8_t **m_dst_data;
};


#endif
