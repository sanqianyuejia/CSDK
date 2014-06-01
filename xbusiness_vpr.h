/***************************************************************************
 *                    __    ____       __ ______  ______
 *  Project           \ \  / /\ \     / /|  __  \|  __  \	
					   \ \/ /  \ \   / / | | _| || |__| |
					   / /\ \   \ \_/ /  | ||___|| |  \ \
					  /_/  \_\   \___/   |_|     |_|   \_\

 * Copyright (C) 2013 - 2014, Chimy, <chimy@shengwenyun.cn>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

#ifndef _xbusiness_vpr_h_
#define _xbusiness_vpr_h_
#include <memory.h>

#ifdef XVPR_LIB_EXPORTS
#define DLLAPI	_declspec(dllexport)
#else
#define DLLAPI	_declspec(dllimport)
#endif

#ifdef __cplusplus 
extern "C" {
#endif 

/* ����ʶ�������� */
typedef void XVPR;

/* �˵��������� */
typedef void XVAD;

/* ��������ֵö������ */
enum XVPR_CODE {
	XVPR_CODE_SUCCESS,			/* ���óɹ� */
	XVPR_CODE_FAIL,				/* ����ʧ�� */
	XVPR_CODE_CONNECT_ERR,		/* �������Ӵ��� */
	XVPR_CODE_PARAM_ERR,		/* �������� */
	XVPR_CODE_NOT_FOUND			/* ����δ���� */
};

/* ����˵������Ϣ */
struct xvpr_person_info {
	char collection[32];	/* ˵����Ⱥ�� */
	char name[32];			/* ˵�������� */
	char tag[32];			/* ˵���˱�ǩ */
	bool flag;				/* �Ǽ�״̬ */

	xvpr_person_info() {
		memset(this->collection, 0, sizeof(char)*32); 
		memset(this->name, 0, sizeof(char) * 32);
		memset(this->tag, 0, sizeof(char) * 32);
		this->flag = false;
	}
};

/* ����ʶ���� */
struct xvpr_result {
	char collection[32];	/* ˵����Ⱥ�� */
	char name[32];		/* ˵�������� */
	float similarity;	/* ���ƶ� */
	char gender;	/* ˵�����Ա� */

	xvpr_result() {
		memset(this->collection, 0, sizeof(char)*32); 
		memset(this->name, 0, sizeof(char) * 32);
		this->similarity = 0.0f;
		this->gender = 'M';
	}
};


/*************************************** ������Ľӿ� ************************************/

/**
 * �����ʼ����ֻ�ܵ���һ�Σ�
 * @return �ɹ�����XVPR_CODE_SUCCESS��ʧ�ܷ���XVPR_CODE_FAIL
 */
DLLAPI XVPR_CODE xvpr_global_init();

/**
 * ��ȡ����汾��Ϣ
 * @return ���ذ汾��Ϣ
 */
DLLAPI const char *xvpr_version();

/**
 * �ͷ�������Դ���ڳ����˳���ʱ����ã�
 */
DLLAPI void xvpr_global_release();

/**
 * �����������
 * @param key �����ؼ���
 * @param param ����ֵ
 * @return ����״ֵ̬
 */
DLLAPI XVPR_CODE xvpr_global_setparam(const char *key, const char *param);

/**
 * ��ʼ���������󣨶��߳�֮�䲻�ܹ���ýӿڷ��صľ����
 * @param collection Ⱥ��ID�����Դ�����ͬ�ĵ�������
 * �Ա����ò�ͬ��Ⱥ��ID
 * @return ����������
 */
DLLAPI XVPR *xvpr_client_init(const char *collection);

/**
 * ����˵����
 * @param handle ��������������
 * @param name ˵�����û���
 * @return ����״ֵ̬
 */
DLLAPI XVPR_CODE xvpr_create_person(XVPR *handle, const char *name);

/**
 * ɾ��˵������Ϣ
 * @param handle ��������������
 * @param name ˵�����û���
 * @return ����״ֵ̬
 */
DLLAPI XVPR_CODE xvpr_remove_person(XVPR *handle, const char *name);

/**
 * ��ȡ˵������Ϣ
 * @param handle ��������������
 * @param name ˵�����û���
 * @param info ����˵������Ϣ
 * @return ����״ֵ̬
 */
DLLAPI XVPR_CODE xvpr_get_info(XVPR *handle, const char *name, xvpr_person_info &info);

/**
 * Ϊ˵�����������
 * @param handle �������������� 
 * @param name ˵�����û���
 * @param paramlist ���������б�
 * @param stream ������
 * @param buf_len ����������
 * @param checksum ��������MD5ֵ
 * @return ����״ֵ̬
 */
DLLAPI XVPR_CODE xvpr_add_speech(XVPR *handle, const char *name, const char *paramlist, 
						  short *stream, size_t buf_len, char *checksum);


/**
 * ɾ��������Ϣ
 * @param handle �������������� 
 * @param checksum ����MD5ֵ
 * @return ����״ֵ̬
 */
DLLAPI XVPR_CODE xvpr_remove_speech(XVPR *handle, const char *checksum);

/**
 * �����������ԣ�ɾ����������
 * @param handle �������������� 
 * @param name ˵�����û���
 * @return ����״ֵ̬
 */
DLLAPI XVPR_CODE xvpr_reserve_speeches(XVPR *handle, const char *name);


/**
 * ע��˵����������Ϣ
 * @param handle �������������� 
 * @param name ˵�����û���
 * @return ����״ֵ̬
 */
DLLAPI XVPR_CODE xvpr_register_person(XVPR *handle, const char *name);


/**
 * ����˵��������ģ����Ϣ
 * @param handle �������������� 
 * @param name ˵�����û���
 * @return ����״ֵ̬
 */
DLLAPI XVPR_CODE xvpr_update_person(XVPR *handle, const char *name);

/**
 * ˵����������֤
 * @param handle �������������� 
 * @param name ˵�����û���
 * @param paramlist ���������б�
 * @param stream ������
 * @param buf_len ����������
 * @param res ��������ȷ�Ͻ��
 * @return ����״ֵ̬
 */
DLLAPI XVPR_CODE xvpr_verify_person(XVPR *handle, const char *name, const char *paramlist, short *straem, size_t buf_len, xvpr_result &res);
/**
 * ˵�������Ʊ���
 * @param handle ��������������
 * @param paramlist ���������б�
 * @param stream ������
 * @param buf_len ����������
 * @param res ��������ȷ�Ͻ��
 * @return ����״ֵ̬
 */
DLLAPI XVPR_CODE xvpr_identify_person(XVPR *handle, const char *paramlist, short *stream, size_t buf_len, xvpr_result &res);




/********************************* �˵���������Ľӿ� ******************************************/


/**
 * ����ʵʱ�˵�������
 * @return ������
 */
DLLAPI XVAD *xvpr_vad_create_vad(size_t sr);

/**
 * ȥ������
 * @param sr ������
 * @param input ����������
 * @param length �������������������
 * @param output ���������
 * @return ������Ч�������������
 */
DLLAPI size_t xvpr_vad_detect_valid_speech(XVAD *&handle, short *input, size_t length, short *&output);

/**
 * ʵʱ�˵���
 * @param handle �˵���������
 * @param buffer ʵʱ������
 * @param buf_len ʵʱ��������������
 * @return 0-����˵����1-ֹͣ˵��
 */
DLLAPI int xvpr_vad_is_speaking(XVAD *&handle, short *stream, size_t buf_len);

/** 
 * ����VAD������
 */
DLLAPI void xvpr_vad_reset_vad(XVAD *&handle);


/********************************* ���渨���ӿ� ******************************************/

/**
 * �������ļ���������
 * @param path �����ļ�·��
 * @param skip_bits ��Ҫ����ͷ�����ֽ���
 * @param buffer �����������ݣ��ڴ��ɽӿ��ڲ����룬������Ҫ�����ߵ��á��ͷ��ڴ桿�Ľӿڽ����ͷţ�
 * @return �����������������
 */
DLLAPI size_t xvpr_aid_read_buffer(const char *path, size_t skip_bits/* ����ͷ�������ֽ� */, short *&buffer);

/** 
 * ����������д���ļ�
 * @param path �����ļ�·��
 * @param buffer �������ݻ���
 * @param buf_len �������������
 */
DLLAPI int xvpr_aid_write_buffer(const char *path, short *buffer/* �������� */, size_t buf_len);

/**
 * �ͷ��ڴ�
 * @param buffer ���ͷŵ��ڴ�ָ��
 */
DLLAPI void xvpr_aid_release_buffer(short *&buffer);

#ifdef __cplusplus 
} 
#endif

#endif	// _xbusiness_vpr_h_