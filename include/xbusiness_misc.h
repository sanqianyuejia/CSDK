#ifndef _MISC_H_
#define _MISC_H_

#ifdef __cplusplus 
extern "C" {
#endif

#if defined(WIN32) && !defined(GNUWINCE)
#else
#include <sys/types.h>
#endif 


/*
	用于获取WEB响应数据的结构体
	*/
struct curl_return_str{
	char *str;		// 返回的字符串
	size_t len;		// 返回字符串的长度
	size_t size;	// 允许的最大字符串长度
};


/**
 *	返回WEB响应的回调函数，注意：
 *	1. 用户通过 curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, _recive_data_from_http_api);来设置回调函数；
 *	2. 在WIN32平台上，如果设置了CURLOPT_WRITEDATA参数，那么必须相应地设置CURLOPT_WRITEFUNCTION参数，否则会崩溃
 */
size_t _recive_data_from_http_api(void *buffer, size_t size, size_t nmemb, void *user_p);

#ifdef __cplusplus 
} 
#endif

#endif	// _MISC_H_
