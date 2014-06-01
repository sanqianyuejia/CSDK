#include "../xbusiness_vpr.h"
#include <stdio.h>
#include <conio.h>


int main(int argc, char* argv[])
{
	XVPR_CODE ret;
	XVPR *handle;
	xvpr_result res;
	static char *identifies = "123456";
	static char *name = "12345671234567";

	ret = xvpr_global_init();
	if (XVPR_CODE_SUCCESS != ret) {
		printf("Init xbusiness-vpr failed.\n");
		return -1;
	}
	
	printf("VER: %s\n", xvpr_version());
	xvpr_global_setparam("accesskey", "1ee0d9b01e8d92a155597785e0b7074e");
	xvpr_global_setparam("secretkey", "1ee0d9b01e8d92a155597785e0b7074e");
	// xvpr_global_setparam("host", "114.215.103.99");
	xvpr_global_setparam("host", "192.168.1.245");
	xvpr_global_setparam("port", "82");
	xvpr_global_setparam("version", "1");

	handle = xvpr_client_init(identifies);
		
	// xvpr_remove_person(handle, name);
	xvpr_create_person(handle, name);
	
	//
	// xvpr_person_info person;
	// xvpr_get_info(handle, name, person);

	char checksum[64] = "";
	short *buffer = NULL;
	short *valid_buffer = NULL;
	size_t buf_len = 0;
	buf_len = xvpr_aid_read_buffer("../audio/0-9.4.wav", 50, buffer);

	// 创建端点检测实例
	XVAD *vad_handle = xvpr_vad_create_vad(8000);
	buf_len = xvpr_vad_detect_valid_speech(vad_handle, buffer, buf_len, valid_buffer);
	// xvpr_aid_write_buffer("./xbusiness-vpr/valid.pcm", valid_buffer, buf_len);
	// xvpr_add_speech(handle, name, "codec:pcm/alaw;sr:8000;verify:true;rule:*", buffer, buf_len, checksum);
	// xvpr_register_person(handle, name);
	xvpr_verify_person(handle, name, "codec:pcm/alaw;sr:8000;verify:true;rule:*", buffer, buf_len, res);
	xvpr_aid_release_buffer(buffer);
	xvpr_aid_release_buffer(valid_buffer);

	getch();
	xvpr_global_release();
	return 0;
}

