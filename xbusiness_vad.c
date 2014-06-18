#include "xbusiness_vpr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct xvpr_vad_info {
	size_t sample_rate;
	size_t total_sil_buffers;
	size_t total_voice_buffers;
	size_t time_out;	/* ��ʱ���� */

	xvpr_vad_info() {
		this->sample_rate = 8000;
		this->total_sil_buffers = 0;
		this->total_voice_buffers = 0;
		this->time_out = 3;
	}
};


XVAD *xvpr_vad_create_vad(size_t sr) 
{
	xvpr_vad_info vad;
	vad.sample_rate = sr;

	return (XVAD *)(&vad);
}


size_t xvpr_vad_detect_valid_speech(XVAD *handle, short *input, size_t length, short **output)
{
	static const int VOC_BLOCK_LEN = 800;
	static const int MIN_VOC_ENG = 350;

	int j=0, p=0;
	short old1, old2, old3, cur_sample;
	size_t out_length = 0;
	short *cur_ptr = NULL;

	short win_buf[VOC_BLOCK_LEN+1];
	size_t num_of_win, num_of_mod, i, k, eng;

	*output = (short *) malloc(sizeof(short) * length);
	cur_ptr = *output;

	num_of_win = length/VOC_BLOCK_LEN;
	num_of_mod = length%VOC_BLOCK_LEN;

	for (i=0; i<num_of_win; i++)
	{
		eng = 0;
		for (k=0; k<VOC_BLOCK_LEN; k++)
		{
			eng += abs(input[VOC_BLOCK_LEN*i+k]);
		}

		if (eng > MIN_VOC_ENG*VOC_BLOCK_LEN)
		{
			j = p = 0;
			old1 = old2 = old3 = 0;
			for(k=0;k<VOC_BLOCK_LEN;k++)
			{
				cur_sample = input[VOC_BLOCK_LEN*i+k];
				if(cur_sample==old1 && old1==old2 && old2==old3)
				{
					if(p>=0) j=p;
				}
				else
				{
					win_buf[j++]=cur_sample;
					p=j-3;
				}

				old3=old2;
				old2=old1;
				old1=cur_sample;
			}

			memcpy(cur_ptr,win_buf,sizeof(short)*j);
			cur_ptr += j;
			out_length += j;
		}
	}

	eng = 0;
	for (i=0; i<num_of_mod; i++)
	{
		eng += abs(input[VOC_BLOCK_LEN*num_of_win+i]);
	}
	if (eng > MIN_VOC_ENG*num_of_mod)
	{
		j = p = 0;
		old1 = old2 = old3 = 0;
		for(i=0;i<num_of_mod;i++)
		{
			cur_sample = input[VOC_BLOCK_LEN*num_of_win+i];
			if(cur_sample==old1 && old1==old2 && old2==old3)
			{
				if(p>=0) j=p;
			}
			else
			{
				win_buf[j++]=cur_sample;
				p=j-3;
			}

			old3=old2;
			old2=old1;
			old1=cur_sample;
		}

		memcpy(cur_ptr,win_buf,sizeof(short)*j);
		out_length += j;
	}

	return out_length;

}

static short get_buffer_mean(short *buffer, size_t buf_len) 
{
	float sum = 0.0f;

	if (buffer == NULL || buf_len <= 0) {
		return 0;
	}

	for (int i=0; i < buf_len; i++) {
		sum += abs(buffer[i]);
	}

	return (short) (sum/buf_len);
}

static const short _voice_threshold = 350;
/* �ж�˵��ֹͣ�������������֡�� */
static const int _num_of_sil_buffers = 3;
/* �ж�˵��ֹͣ����Ҫ�����������������֡����������6֡ */
static const int _num_of_total_buffers = 6;
int xvpr_vad_is_speaking(XVAD *&handle, short *stream, size_t buf_len)
{
	short mean = get_buffer_mean(stream, buf_len);	
	
	if (mean > _voice_threshold) {
		((xvpr_vad_info *) handle)->total_voice_buffers += 1;
		((xvpr_vad_info *) handle)->total_sil_buffers = 0;
	} else {
		((xvpr_vad_info *) handle)->total_sil_buffers += 1;
	}

	if (((xvpr_vad_info *) handle)->total_sil_buffers > _num_of_sil_buffers 
		&& (((xvpr_vad_info *) handle)->total_sil_buffers+((xvpr_vad_info *) handle)->total_voice_buffers) > _num_of_total_buffers
		&& ((xvpr_vad_info *) handle)->total_voice_buffers > 0) {
		((xvpr_vad_info *) handle)->total_sil_buffers = 0;
		((xvpr_vad_info *) handle)->total_voice_buffers = 0;

		return false;
	} 

	return true;
}

/**
 * ���ö˵�������
 * @param handle ������
 */
void xvpr_vad_reset_vad(XVAD *&handle)
{
	((xvpr_vad_info *) handle)->total_sil_buffers = 0;
	((xvpr_vad_info *) handle)->total_voice_buffers = 0;
}