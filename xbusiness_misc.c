#include "xbusiness_misc.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

size_t _recive_data_from_http_api(void *buffer, 
								  size_t size, 
								  size_t nmemb, 
								  void *user_p) 
{
	struct curl_return_str *result_t = (struct curl_return_str *)user_p;

	if (result_t->size < ((size * nmemb) + 1)) {
		result_t->str = (char *)realloc(result_t->str, (size * nmemb) + 1);
		if (result_t->str == NULL) {
			return 0;
		}
		result_t->size = (size * nmemb) + 1;
	}

	result_t->len = size * nmemb;
	memcpy(result_t->str, buffer, result_t->len);
	result_t->str[result_t->len] = '\0';

	return result_t->len;
}