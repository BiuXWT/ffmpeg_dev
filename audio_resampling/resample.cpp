#include "resample.h"

EncAudSwr::EncAudSwr(int src_rate, int dst_rate, AVSampleFormat src_sample_fmt, AVSampleFormat dst_sample_fmt)
{
	int ret = 0;
	m_src_rate = src_rate;
	m_dst_rate = dst_rate;
	//m_src_nb_samples = src_nb_samples;
	//m_dst_nb_samples = dst_nb_samples;
	m_src_sample_fmt = src_sample_fmt;
	m_dst_sample_fmt = dst_sample_fmt;

	m_swr_ctx = swr_alloc();
	if (!m_swr_ctx)
	{
		printf("Could not allocate resampler context\n");
		exit(1);
	}

	av_opt_set_int(m_swr_ctx, "in_channel_layout", AV_CH_LAYOUT_STEREO, 0);
	av_opt_set_int(m_swr_ctx, "in_sample_rate", src_rate, 0);
	av_opt_set_sample_fmt(m_swr_ctx, "in_sample_fmt", m_src_sample_fmt, 0);

	av_opt_set_int(m_swr_ctx, "out_channel_layout", AV_CH_LAYOUT_STEREO, 0);
	av_opt_set_int(m_swr_ctx, "out_sample_rate", dst_rate, 0);
	av_opt_set_sample_fmt(m_swr_ctx, "out_sample_fmt", m_dst_sample_fmt, 0);

	if ((ret = swr_init(m_swr_ctx)) < 0)
	{
		printf("init resampling context failed\n");
		exit(1);
	}

	m_max_dst_nb_samples = m_dst_nb_samples = av_rescale_rnd(m_src_nb_samples, dst_rate, src_rate, AV_ROUND_UP);
	m_dst_nb_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
	ret = av_samples_alloc_array_and_samples(&m_dst_data, &m_dst_linesize, m_dst_nb_channels,
		m_dst_nb_samples, m_dst_sample_fmt, 0);
	if (ret < 0) {
		char msg[128];
		av_make_error_string(msg, 128, ret);
		printf("Could not allocate destination samples %d %s\n", ret, msg);
		exit(1);
	}
	printf("m_dst_linesize %d,\n \
			m_dst_nb_channels %d,\n \
			m_dst_nb_samples %d,\n \
			m_dst_rate %d,\n \
			m_dst_sample_fmt %d,\n \
			m_max_dst_nb_samples %d,\n \
			m_src_nb_samples %d,\n \
			m_src_rate %d\n",
		m_dst_linesize,
		m_dst_nb_channels,
		m_dst_nb_samples,
		m_dst_rate,
		m_dst_sample_fmt,
		m_max_dst_nb_samples,
		m_src_nb_samples,
		m_src_rate);
}

int EncAudSwr::process(uint8_t **data)
{
	int ret;
	int dst_bufsize;

	m_dst_nb_samples = av_rescale_rnd(swr_get_delay(m_swr_ctx, m_src_rate) + m_src_nb_samples, m_dst_rate, m_src_rate, AV_ROUND_UP);
	if (m_dst_nb_samples > m_max_dst_nb_samples) {
		av_freep(&m_dst_data[0]);
		ret = av_samples_alloc(m_dst_data, &m_dst_linesize, m_dst_nb_channels, m_dst_nb_samples, m_dst_sample_fmt, 1);
		if (ret < 0)
		{
			char msg[128];
			av_make_error_string(msg, 128, ret);
			printf("av_samples_alloc %d %s\n", ret, msg);
			return ret;
		}
		m_max_dst_nb_samples = m_dst_nb_samples;
	}

	ret = swr_convert(m_swr_ctx, m_dst_data, m_dst_nb_samples, (const uint8_t **)data, m_src_nb_samples);
	if (ret < 0) {
		char msg[128];
		av_make_error_string(msg, 128, ret);
		printf("Error while converting %d %s\n", ret, msg);
		return ret;
	}
	dst_bufsize = av_samples_get_buffer_size(&m_dst_linesize, m_dst_nb_channels, ret, m_dst_sample_fmt, 1);
	if (dst_bufsize < 0) {
		printf("Could not get sample buffer size\n");
		return -1;
	}
	printf("dst buf size is %d\n", dst_bufsize);
	return 0;
}

EncAudSwr::~EncAudSwr()
{
	if (m_dst_data)
		av_freep(&m_dst_data[0]);
	av_freep(&m_dst_data);
	swr_free(&m_swr_ctx);
}
