#include "include/config.h"
#include "include/xbusiness_vpr.h"


int main(int argc, char* argv[])
{
	XVPR_CODE ret;			// 返回值
	XVPR *handle;			// 引擎句柄
	xvpr_result res;		// 识别结果
	
	char checksum[64] = "";
	xvpr_person_info person;	

	short *buffer = NULL;	
	short *valid_buffer = NULL;
	size_t buf_len = 0;

	XVAD *vad_handle = NULL;
	
	static char *identifies = "4621d373cade4e83";	// 声纹库群组号，可为不同类型的任务分配不同的群组号
	static char *name = "test";	// 说话人用户名

	// 版本号
	printf("VER: %s\n", xvpr_version());

	// 全局初始化（全局只需调一次）
	ret = xvpr_global_init();
	if (XVPR_CODE_SUCCESS != ret) {
		printf("Init xbusiness-vpr failed. RET = %d\n", ret);
		return -1;
	}
	printf("Init xbusiness-vpr success.\n");
		
	// 设置接口参数
	xvpr_global_setparam("accesskey", "84cc0e5c525dc728e1769ad6663341c8");	// AK
	xvpr_global_setparam("secretkey", "84cc0e5c525dc728e1769ad6663341c8");	// SK
	xvpr_global_setparam("host", "127.0.0.1");		// 服务地址
	xvpr_global_setparam("port", "81");					// 服务端口
	xvpr_global_setparam("version", "1");				// 服务端引擎声纹版本号

	// 获取声纹引擎句柄	
	// 该句柄不能多线程共享，必须在每个线程独立获取
	handle = xvpr_client_init(identifies/* 声纹库群组号 */);	
	if (NULL == handle) {
		printf("Create VPR handle failed. RET = %d\n", ret);
		return -1;
	}
	printf("Create VPR handle success.\n");

	// 删除说话人
	ret = xvpr_remove_person(handle, name);	
	if (XVPR_CODE_SUCCESS != ret) {
		printf("Remove speaker information failed. RET = %d\n", ret);
		// return -1;
	} else {
		printf("Remove speaker information success.\n");
	}
		
	// 创建说话人
	ret = xvpr_create_person(handle/* 声纹引擎句柄 */, name/* 说话人用户名 */);
	if (XVPR_CODE_SUCCESS != ret) {
		printf("Create speaker information failed. RET = %d\n", ret);
//		return -1;
	} else {
		printf("Create speaker information success.\n");
	}
	
	// 获取说话人信息
	// 能得到该说话人是否已成功注册声纹
	ret = xvpr_get_info(handle, name, &person);
	if (XVPR_CODE_SUCCESS != ret) {
		printf("Get speaker information failed. RET = %d\n", ret);
		return -1;
	}
	printf("Get speaker information success.\n");

	// 读取语音信息
	buf_len = xvpr_aid_read_buffer(argv[1], 50, &buffer);	// 语音参数
	if (buf_len <= 0) {
		printf("Read speech buffer failed. RET = %d\n", buf_len);
		return -1;
	}
	printf("Read speech buffer success.\n");

	// 创建端点检测实例
	vad_handle = xvpr_vad_create_vad(8000);
	if (NULL == vad_handle) {
		printf("Create VAD handle failed. RET = %d\n", vad_handle);
		return -1;
	}
	printf("Create VAD handle success.\n");

	// 进行有效语音检测
	buf_len = xvpr_vad_detect_valid_speech(vad_handle, buffer, buf_len, &valid_buffer);
	if (buf_len <= 0) {
		printf("No speech detected. RET = %d\n", buf_len);
		return -1;
	}
	printf("Detected valid speech success.\n");

	// 为说话人添加语音（注意：需要添加3条或以上语音）	
	ret = xvpr_add_speech(handle, name, "codec:pcm/raw;sr:8000;verify:true;rule:*", buffer, buf_len, checksum);
	if (XVPR_CODE_SUCCESS != ret) {
		printf("Add speech failed. RET = %d\n", ret);
		return -1;
	}
	printf("Add speech success.\n");

	// 注册声纹
	ret = xvpr_register_person(handle, name);
	if (XVPR_CODE_SUCCESS != ret) {
		printf("Register speaker failed. RET = %d\n", ret);
		return -1;
	}
	printf("Register speaker success.\n");

	// 声纹确认
	ret = xvpr_verify_person(handle, name, "codec:pcm/raw;sr:8000;verify:true;rule:*", buffer, buf_len, &res);
	if (XVPR_CODE_SUCCESS != ret) {
		printf("Verify speaker failed. RET = %d\n", ret);
		return -1;
	}
	printf("Verify speaker success. name=%s, similarity=%2f\n", res.name, res.similarity);

	// 声纹辨认
	ret = xvpr_identify_person(handle, "codec:pcm/raw;sr:8000;verify:true;rule:*", buffer, buf_len, &res);
	if (XVPR_CODE_SUCCESS != ret) {
		printf("Identify speaker failed. RET = %d\n", ret);
		return -1;
	}
	printf("Identify speaker success. name=%s, similarity=%2f\n", res.name, res.similarity);

	// 释放内存
	xvpr_aid_release_buffer(&buffer);
	xvpr_aid_release_buffer(&valid_buffer);
	xvpr_global_release();
	
	#if defined(WIN32) && !defined(GNUWINCE)
	getch();
	#else
	getchar();	
	#endif
	return 0;
}

