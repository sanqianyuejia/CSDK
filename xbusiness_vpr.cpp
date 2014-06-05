#include "xbusiness_vpr.h"
#include "xbusiness_misc.h"
#include "json.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <time.h>


static INT16 g_init_state = 0x0000;	// ȫ�ֳ�ʼ��״̬��־λ
static char xvpr_param_api_key[64] = "1ee0d9b01e8d92a155597785e0b7074e";
static char xvpr_param_api_secret[64] = "1ee0d9b01e8d92a155597785e0b7074e";
static char xvpr_param_host_address[64] = "openapi.shengwenyun.com";
static char xvpr_param_port[8] = "80";
static char xvpr_param_version[2] = "1";
static INT8 xvpr_param_type = 0x00;

// person related api
const static char *URL_PERSON_CREATE = "/person/create.json";
const static char *URL_PERSON_DELETE = "/person/delete.json";
const static char *URL_PERSON_GETINFO = "/person/find.json";
const static char *URL_PERSON_FIND_ALL = "/person/find_all.json";

// speech related api
const static char *URL_SPEECH_ADD = "/speech/add.json";
const static char *URL_SPEECH_FIND = "/speech/find.json";
const static char *URL_SPEECH_FIND_PERSON = "/speech/find_person.json";
const static char *URL_SPEECH_DELETE = "/speech/delete.json";
const static char *URL_SPEECH_DELETE_PERSON = "/speech/delete_person.json";
const static char *URL_SPEECH_RESERVE_SPEECHES = "/speech/reserve.json";

// model related api
const static char *URL_MODEL_REGISTER = "/model/train.json";
const static char *URL_MODEL_VERIFY = "/model/verify.json";
const static char *URL_MODEL_IDENTIFY = "/model/identify_2.json";


/*
	ȫ�ֳ�ʼ�������������һ�Σ��ұ������
	*/
XVPR_CODE xvpr_global_init() 
{
	if (0x0000 == g_init_state)	{
		mkdir("./xbusiness-vpr/");
		CURLcode ret = curl_global_init(CURL_GLOBAL_WIN32);
		if (CURLE_OK != ret) {		
			return XVPR_CODE_FAIL;
		} else {
			g_init_state = 0x0001;	
			return XVPR_CODE_SUCCESS;
		}
	}

	return XVPR_CODE_SUCCESS;
}



/*
	��������汾
	*/
const char *xvpr_version() { return "xbusiness-vpr-1.1"; }



/*
	���ռ�õ�ϵͳ��Դ
	*/
void xvpr_global_release() 
{
	if (0x0001 == g_init_state) {
		curl_global_cleanup();
	}
}

// key: parameter name
// param: value
// "key:1ee0d9b01e8d92a155597785e0b7074e;host:openapi.shengwenyun.com;port:80;version:1;type:0";
XVPR_CODE xvpr_global_setparam(const char *key,			/* ������ǩ */
							   const char *param)		/* ����ֵ */
{
	if (0 == strcmp(key, "accesskey")) 
	{
		strncpy(xvpr_param_api_key, param, 64);
	} 
	else if (0 == strcmp(key, "secretkey")) 
	{
		strncpy(xvpr_param_api_secret, param, 64);
	} 
	else if (0 == strcmp(key, "host")) 
	{
		strncpy(xvpr_param_host_address, param, 64);
	} 
	else if (0 == strcmp(key, "port")) {
		strncpy(xvpr_param_port, param, 8);
	} 
	else if (0 == strcmp(key, "version")) {
		strncpy(xvpr_param_version, param, 2);
	} 
	else if (0 == strcmp(key, "type")) {
		xvpr_param_type = atoi(param);
	} else {
		return XVPR_CODE_PARAM_ERR;
	}

	return XVPR_CODE_SUCCESS;
}

XVPR *xvpr_client_init(const char *collection)	/* �û�Ⱥ���ʶ */
{
	return (XVPR *) collection;
}


XVPR_CODE xvpr_create_person(XVPR *handle,	/* ������ */
							 const char *name)		/* �û��� */
{
	// ��ʼ����������
	struct curl_return_str curl_result_t;
	curl_result_t.len = 0;
	curl_result_t.size = 1024;
	curl_result_t.str = (char *)calloc(1, 1024);
	if (curl_result_t.str == NULL) { return XVPR_CODE_FAIL; }

	CURL *easyhandle = curl_easy_init();

	// �������
	char data[256] = "";
	sprintf(data, "api_key=%s&api_secret=%s&id=%s&name=%s&tag=C", xvpr_param_api_key, xvpr_param_api_secret, (char *)handle, name);
	curl_easy_setopt(easyhandle, CURLOPT_POSTFIELDS, data);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, _recive_data_from_http_api);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &curl_result_t);

	// ����URL
	char url[256] = "";
	sprintf(url, "http://%s:%s/%s%s", xvpr_param_host_address, xvpr_param_port, xvpr_param_version, URL_PERSON_CREATE);
	curl_easy_setopt(easyhandle, CURLOPT_URL, url);
	
	// POST
	CURLcode ret = curl_easy_perform(easyhandle);
	if (CURLE_OK != ret) {
		return XVPR_CODE_CONNECT_ERR;
	}

	// �������
	json_object *result_json;
	result_json = json_tokener_parse(curl_result_t.str);
	if (!json_object_get_boolean(json_object_object_get(result_json, "success"))) {
		return XVPR_CODE_PARAM_ERR;
	}

	return XVPR_CODE_SUCCESS;
}


XVPR_CODE xvpr_remove_person(XVPR *handle,	/* ������ */
							 const char *name)		/* �û��� */
{
	// ��ʼ����������
	struct curl_return_str curl_result_t;
	curl_result_t.len = 0;
	curl_result_t.size = 1024;
	curl_result_t.str = (char *)calloc(1, 1024);
	if (curl_result_t.str == NULL) { return XVPR_CODE_FAIL; }

	CURL *easyhandle = curl_easy_init();

	// �������
	char data[256] = "";
	sprintf(data, "api_key=%s&api_secret=%s&id=%s&name=%s", xvpr_param_api_key, xvpr_param_api_secret, (char *)handle, name);
	curl_easy_setopt(easyhandle, CURLOPT_POSTFIELDS, data);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, _recive_data_from_http_api);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &curl_result_t);

	// ����URL
	char url[256] = "";
	sprintf(url, "http://%s:%s/%s%s", xvpr_param_host_address, xvpr_param_port, xvpr_param_version, URL_PERSON_DELETE);
	curl_easy_setopt(easyhandle, CURLOPT_URL, url);

	// POST		
	CURLcode ret = curl_easy_perform(easyhandle);
	if (CURLE_OK != ret) {
		return XVPR_CODE_CONNECT_ERR;
	}

	// �������
	json_object *result_json;
	result_json = json_tokener_parse(curl_result_t.str);
	if (!json_object_get_boolean(json_object_object_get(result_json, "success"))) {
		return XVPR_CODE_PARAM_ERR;
	}

	return XVPR_CODE_SUCCESS;
}


XVPR_CODE xvpr_get_info(XVPR *handle,	/* ������ */
						const char *name,		/* �û��� */
						xvpr_person_info &info)		/* ����˵������Ϣ */
{
	// ��ʼ����������
	struct curl_return_str curl_result_t;
	curl_result_t.len = 0;
	curl_result_t.size = 1024;
	curl_result_t.str = (char *)calloc(1, 1024);
	if (curl_result_t.str == NULL) { return XVPR_CODE_FAIL; }

	// ��ȡCURL���
	CURL *easyhandle = curl_easy_init();

	// �������
	char data[256] = "";
	sprintf(data, "api_key=%s&api_secret=%s&id=%s&name=%s", xvpr_param_api_key, xvpr_param_api_secret, (char *)handle, name);
	curl_easy_setopt(easyhandle, CURLOPT_POSTFIELDS, data);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, _recive_data_from_http_api);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &curl_result_t);

	// ����URL
	char url[256] = "";
	sprintf(url, "http://%s:%s/%s%s", xvpr_param_host_address, xvpr_param_port, xvpr_param_version, URL_PERSON_GETINFO);
	curl_easy_setopt(easyhandle, CURLOPT_URL, url);

	// POST
	CURLcode ret = curl_easy_perform(easyhandle);
	if (CURLE_OK != ret) {
		return XVPR_CODE_CONNECT_ERR;
	}

	// �������
	json_object *result_json, *tmp_json;
	result_json = json_tokener_parse(curl_result_t.str);
	if (!json_object_get_boolean(json_object_object_get(result_json, "success"))) {
		return XVPR_CODE_PARAM_ERR;
	}

	// ����ֵ��ֵ
	tmp_json = json_object_object_get(result_json, "person");
	array_list *persons = json_object_get_array(tmp_json);
	if (sizeof(persons) <= 0) { return XVPR_CODE_NOT_FOUND; }

	strncpy(info.collection, (char *)handle, 32);
	strncpy(info.name, name, 32);
	strncpy(info.tag, "G", 32);
	info.flag = json_object_get_boolean(json_object_object_get((json_object *)persons->array[0], "flag"));


	return XVPR_CODE_SUCCESS;
}

XVPR_CODE xvpr_add_speech(XVPR *handle,			/* ������ */
						  const char *name,				/* �û��� */
						  const char *paramlist,		/* �����б� */
						  short *buffer,					/* ���������� */
						  size_t buf_len,		/* �������ݳ��� */
						  char *checksum)			/* ���ظ���������������MD5ֵ */
{
	// ��ʼ����������
	struct curl_return_str curl_result_t;
	curl_result_t.len = 0;
	curl_result_t.size = 1024;
	curl_result_t.str = (char *)calloc(1, 1024);
	if (curl_result_t.str == NULL) { return XVPR_CODE_FAIL; }

	char tmppath[256] = "";

	// ��ȡCURL���
	CURL *easyhandle = curl_easy_init();

	// ��������������ʱ�ļ�
	sprintf(tmppath, "./xbusiness-vpr/register_%s_%d.pcm", name, time(NULL));
	xvpr_aid_write_buffer(tmppath, buffer, buf_len);

	// �������
	struct curl_httppost *post = NULL;
	struct curl_httppost *last = NULL;
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "api_key", CURLFORM_COPYCONTENTS, xvpr_param_api_key, CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "api_secret", CURLFORM_COPYCONTENTS, xvpr_param_api_secret, CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "id", CURLFORM_COPYCONTENTS, (char *)handle, CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "name", CURLFORM_COPYCONTENTS, name, CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "codec", CURLFORM_COPYCONTENTS, "pcm/raw", CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "sr", CURLFORM_COPYCONTENTS, "8000", CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "rule", CURLFORM_COPYCONTENTS, "*", CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "verify", CURLFORM_COPYCONTENTS, "false", CURLFORM_END);
	curl_formadd(&post, &last,   CURLFORM_COPYNAME, "file",   CURLFORM_FILE, tmppath, CURLFORM_END);
	curl_easy_setopt(easyhandle, CURLOPT_HTTPPOST, post);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, _recive_data_from_http_api);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &curl_result_t);

	// ����URL
	char url[256] = "";
	sprintf(url, "http://%s:%s/%s%s", xvpr_param_host_address, xvpr_param_port, xvpr_param_version, URL_SPEECH_ADD);
	curl_easy_setopt(easyhandle, CURLOPT_URL, url);

	// POST
	CURLcode ret = curl_easy_perform(easyhandle);
	curl_formfree(post);
	if (CURLE_OK != ret) {
		return XVPR_CODE_CONNECT_ERR;
	}

	// �������
	json_object *result_json;
	result_json = json_tokener_parse(curl_result_t.str);
	if (!json_object_get_boolean(json_object_object_get(result_json, "success"))) {
		return XVPR_CODE_PARAM_ERR;
	}
	strncpy(checksum, json_object_get_string(json_object_object_get(result_json, "md5")), 32);

	return XVPR_CODE_SUCCESS;
}


XVPR_CODE xvpr_remove_speech(XVPR *handle, 
							 const char *checksum)
{
	// ��ʼ����������
	struct curl_return_str curl_result_t;
	curl_result_t.len = 0;
	curl_result_t.size = 1024;
	curl_result_t.str = (char *)calloc(1, 1024);
	if (curl_result_t.str == NULL) { return XVPR_CODE_FAIL; }

	// ��ȡCURL���
	CURL *easyhandle = curl_easy_init();

	// �������
	char data[256] = "";
	sprintf(data, "api_key=%s&api_secret=%s&md5=%s", xvpr_param_api_key, xvpr_param_api_secret, checksum);
	curl_easy_setopt(easyhandle, CURLOPT_POSTFIELDS, data);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, _recive_data_from_http_api);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &curl_result_t);

	// ����URL
	char url[256] = "";
	sprintf(url, "http://%s:%s/%s%s", xvpr_param_host_address, xvpr_param_port, xvpr_param_version, URL_SPEECH_DELETE);
	curl_easy_setopt(easyhandle, CURLOPT_URL, url);

	// POST
	CURLcode ret = curl_easy_perform(easyhandle);
	if (CURLE_OK != ret) {
		return XVPR_CODE_CONNECT_ERR;
	}

	// �������
	json_object *result_json;
	result_json = json_tokener_parse(curl_result_t.str);
	if (!json_object_get_boolean(json_object_object_get(result_json, "success"))) {
		return XVPR_CODE_PARAM_ERR;
	}

	return XVPR_CODE_SUCCESS;

}

XVPR_CODE xvpr_reserve_speeches(XVPR *handle, 
								const char *name)
{
	// ��ʼ����������
	struct curl_return_str curl_result_t;
	curl_result_t.len = 0;
	curl_result_t.size = 1024;
	curl_result_t.str = (char *)calloc(1, 1024);
	if (curl_result_t.str == NULL) { return XVPR_CODE_FAIL; }

	// ��ȡCURL���
	CURL *easyhandle = curl_easy_init();

	// �������
	char data[256] = "";
	sprintf(data, "api_key=%s&api_secret=%s&id=%s&name=%s&number=3", xvpr_param_api_key, xvpr_param_api_secret, (char *)handle, name);
	curl_easy_setopt(easyhandle, CURLOPT_POSTFIELDS, data);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, _recive_data_from_http_api);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &curl_result_t);

	// ����URL
	char url[256] = "";
	sprintf(url, "http://%s:%s/%s%s", xvpr_param_host_address, xvpr_param_port, xvpr_param_version, URL_SPEECH_RESERVE_SPEECHES);
	curl_easy_setopt(easyhandle, CURLOPT_URL, url);

	// POST
	CURLcode ret = curl_easy_perform(easyhandle);
	if (CURLE_OK != ret) {
		return XVPR_CODE_CONNECT_ERR;
	}

	// �������
	json_object *result_json;
	result_json = json_tokener_parse(curl_result_t.str);
	if (!json_object_get_boolean(json_object_object_get(result_json, "success"))) {
		return XVPR_CODE_PARAM_ERR;
	}

	return XVPR_CODE_SUCCESS;
}


XVPR_CODE xvpr_register_person(XVPR *handle, 
							   const char *name)
{
	// ��ʼ����������
	struct curl_return_str curl_result_t;
	curl_result_t.len = 0;
	curl_result_t.size = 1024;
	curl_result_t.str = (char *)calloc(1, 1024);
	if (curl_result_t.str == NULL) { return XVPR_CODE_FAIL; }

	// ��ȡCURL���
	CURL *easyhandle = curl_easy_init();

	// �������
	char data[256] = "";
	sprintf(data, "api_key=%s&api_secret=%s&id=%s&name=%s&update=true", xvpr_param_api_key, xvpr_param_api_secret, (char *)handle, name);
	curl_easy_setopt(easyhandle, CURLOPT_POSTFIELDS, data);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, _recive_data_from_http_api);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &curl_result_t);

	// ����URL
	char url[256] = "";
	sprintf(url, "http://%s:%s/%s%s", xvpr_param_host_address, xvpr_param_port, xvpr_param_version, URL_MODEL_REGISTER);
	curl_easy_setopt(easyhandle, CURLOPT_URL, url);

	// POST
	CURLcode ret = curl_easy_perform(easyhandle);
	if (CURLE_OK != ret) {
		return XVPR_CODE_CONNECT_ERR;
	}

	// �������
	json_object *result_json;
	result_json = json_tokener_parse(curl_result_t.str);
	if (!json_object_get_boolean(json_object_object_get(result_json, "success"))) {
		return XVPR_CODE_PARAM_ERR;
	}

	return XVPR_CODE_SUCCESS;
}

XVPR_CODE xvpr_update_person(XVPR *handle, 
									const char *name)
{
	XVPR_CODE ret = xvpr_reserve_speeches(handle, name);
	if (XVPR_CODE_SUCCESS != ret) {
		return XVPR_CODE_FAIL;
	}

	ret = xvpr_register_person(handle, name);
	if (XVPR_CODE_SUCCESS != ret) {
		return XVPR_CODE_FAIL;
	}

	return XVPR_CODE_SUCCESS;
}

XVPR_CODE xvpr_verify_person(XVPR *handle, 
							 const char *name,
							 const char *paramlist,		/* �����б� */
							 short *stream,					/* ���������� */
							 size_t buf_len, 		/* �������ݳ��� */							 
							 xvpr_result &res)				/* ���ƶ� */
{
	// ��ʼ����������
	struct curl_return_str curl_result_t;
	curl_result_t.len = 0;
	curl_result_t.size = 1024;
	curl_result_t.str = (char *)calloc(1, 1024);
	if (curl_result_t.str == NULL) { return XVPR_CODE_FAIL; }

	char tmppath[256] = "";

	// ��ȡCURL���
	CURL *easyhandle = curl_easy_init();

	// ��������������ʱ�ļ�
	sprintf(tmppath, "./xbusiness-vpr/verify_%s_%d.pcm", name, time(NULL));
	xvpr_aid_write_buffer(tmppath, stream, buf_len);

	// �������
	struct curl_httppost *post = NULL;
	struct curl_httppost *last = NULL;
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "api_key", CURLFORM_COPYCONTENTS, xvpr_param_api_key, CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "api_secret", CURLFORM_COPYCONTENTS, xvpr_param_api_secret, CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "id", CURLFORM_COPYCONTENTS, (char *)handle, CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "name", CURLFORM_COPYCONTENTS, name, CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "codec", CURLFORM_COPYCONTENTS, "pcm/raw", CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "sr", CURLFORM_COPYCONTENTS, "8000", CURLFORM_END);
	curl_formadd(&post, &last,   CURLFORM_COPYNAME, "file",   CURLFORM_FILE, tmppath, CURLFORM_END);
	curl_easy_setopt(easyhandle, CURLOPT_HTTPPOST, post);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, _recive_data_from_http_api);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &curl_result_t);

	// ����URL
	char url[256] = "";
	sprintf(url, "http://%s:%s/%s%s", xvpr_param_host_address, xvpr_param_port, xvpr_param_version, URL_MODEL_VERIFY);
	curl_easy_setopt(easyhandle, CURLOPT_URL, url);

	// POST
	CURLcode ret = curl_easy_perform(easyhandle);
	curl_formfree(post);
	if (CURLE_OK != ret) {
		return XVPR_CODE_CONNECT_ERR;
	}

	// �������
	json_object *result_json;
	result_json = json_tokener_parse(curl_result_t.str);
	if (!json_object_get_boolean(json_object_object_get(result_json, "success"))) {
		return XVPR_CODE_PARAM_ERR;
	}

	res.gender = 'M';	// to-do
	res.similarity = json_object_get_double(json_object_object_get(result_json, "similarity"));		// ��ȡ���ƶ�
	strncpy(res.collection, (char *)handle, 32);			// ��ȡȺ���
	strncpy(res.name, name, 32);		// ��ȡ�û���

	return XVPR_CODE_SUCCESS;
}

XVPR_CODE xvpr_identify_person(XVPR *handle, 
							   const char *paramlist,
							   short *stream,
							   size_t buf_len,
							   xvpr_result &res)
{
	// ��ʼ����������
	struct curl_return_str curl_result_t;
	curl_result_t.len = 0;
	curl_result_t.size = 1024;
	curl_result_t.str = (char *)calloc(1, 1024);
	if (curl_result_t.str == NULL) { return XVPR_CODE_FAIL; }

	char tmppath[256] = "";

	// ��ȡCURL���
	CURL *easyhandle = curl_easy_init();

	// ��������������ʱ�ļ�
	sprintf(tmppath, "./xbusiness-vpr/identify_%s_%d.pcm", (char *)handle, time(NULL));
	xvpr_aid_write_buffer(tmppath, stream, buf_len);

	// �������
	struct curl_httppost *post = NULL;
	struct curl_httppost *last = NULL;
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "api_key", CURLFORM_COPYCONTENTS, xvpr_param_api_key, CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "api_secret", CURLFORM_COPYCONTENTS, xvpr_param_api_secret, CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "id", CURLFORM_COPYCONTENTS, (char *)handle, CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "codec", CURLFORM_COPYCONTENTS, "pcm/raw", CURLFORM_END);
	curl_formadd(&post, &last, CURLFORM_COPYNAME, "sr", CURLFORM_COPYCONTENTS, "8000", CURLFORM_END);
	curl_formadd(&post, &last,   CURLFORM_COPYNAME, "file",   CURLFORM_FILE, tmppath, CURLFORM_END);
	curl_easy_setopt(easyhandle, CURLOPT_HTTPPOST, post);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, _recive_data_from_http_api);
	curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &curl_result_t);

	// ����URL
	char url[256] = "";
	sprintf(url, "http://%s:%s/%s%s", xvpr_param_host_address, xvpr_param_port, xvpr_param_version, URL_MODEL_IDENTIFY);
	curl_easy_setopt(easyhandle, CURLOPT_URL, url);

	// POST
	CURLcode ret = curl_easy_perform(easyhandle);
	curl_formfree(post);
	if (CURLE_OK != ret) {
		return XVPR_CODE_CONNECT_ERR;
	}

	// �������
	json_object *result_json;
	result_json = json_tokener_parse(curl_result_t.str);
	if (!json_object_get_boolean(json_object_object_get(result_json, "success"))) {
		return XVPR_CODE_PARAM_ERR;
	}

	res.gender = 'M';	// to-do
	res.similarity = json_object_get_double(json_object_object_get(result_json, "similarity"));		// ��ȡ���ƶ�
	strncpy(res.collection, json_object_get_string(json_object_object_get(result_json, "id")), 32);			// ��ȡȺ���
	strncpy(res.name, json_object_get_string(json_object_object_get(result_json, "name")), 32);		// ��ȡ�û���A

	return XVPR_CODE_SUCCESS;
}