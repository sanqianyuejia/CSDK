#ifndef _MISC_H_
#define _MISC_H_

#ifdef __cplusplus 
extern "C" {
#endif 


/*
	���ڻ�ȡWEB��Ӧ���ݵĽṹ��
	*/
struct curl_return_str{
	char *str;		// ���ص��ַ���
	size_t len;		// �����ַ����ĳ���
	size_t size;	// ���������ַ�������
};


/**
 *	����WEB��Ӧ�Ļص�������ע�⣺
 *	1. �û�ͨ�� curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, _recive_data_from_http_api);�����ûص�������
 *	2. ��WIN32ƽ̨�ϣ����������CURLOPT_WRITEDATA��������ô������Ӧ������CURLOPT_WRITEFUNCTION��������������
 */
size_t _recive_data_from_http_api(void *buffer, size_t size, size_t nmemb, void *user_p);

#ifdef __cplusplus 
} 
#endif

#endif	// _MISC_H_