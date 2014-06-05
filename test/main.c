#include "../xbusiness_vpr.h"
#include <stdio.h>
#include <conio.h>


int main(int argc, char* argv[])
{
	XVPR_CODE ret;			// ����ֵ
	XVPR *handle;			// ������
	xvpr_result res;		// ʶ����
	
	char checksum[64] = "";
	xvpr_person_info person;	

	short *buffer = NULL;	
	short *valid_buffer = NULL;
	size_t buf_len = 0;

	XVAD *vad_handle = NULL;
	
	static char *identifies = "1e24cf708a14ce81";	// ���ƿ�Ⱥ��ţ���Ϊ��ͬ���͵�������䲻ͬ��Ⱥ���
	static char *name = "test";	// ˵�����û���

	// �汾��
	printf("VER: %s\n", xvpr_version());

	// ȫ�ֳ�ʼ����ȫ��ֻ���һ�Σ�
	ret = xvpr_global_init();
	if (XVPR_CODE_SUCCESS != ret) {
		printf("Init xbusiness-vpr failed. RET = %d\n", ret);
		return -1;
	}
	printf("Init xbusiness-vpr success.\n");
		
	// ���ýӿڲ���
	xvpr_global_setparam("accesskey", "db06c78d1e24cf708a14ce81c9b617ec");	// AK
	xvpr_global_setparam("secretkey", "db06c78d1e24cf708a14ce81c9b617ec");	// SK
	xvpr_global_setparam("host", "114.215.103.99");		// �����ַ
	xvpr_global_setparam("port", "81");					// ����˿�
	xvpr_global_setparam("version", "1");				// ������������ư汾��

	// ��ȡ����������	
	// �þ�����ܶ��̹߳���������ÿ���̶߳�����ȡ
	handle = xvpr_client_init(identifies/* ���ƿ�Ⱥ��� */);	
	if (NULL == handle) {
		printf("Create VPR handle failed. RET = %d\n", ret);
		return -1;
	}
	printf("Create VPR handle success.\n");
		
	// ����˵����
	ret = xvpr_create_person(handle/* ���������� */, name/* ˵�����û��� */);
	if (XVPR_CODE_SUCCESS != ret) {
		printf("Create speaker information failed. RET = %d\n", ret);
		return -1;
	}
	printf("Create speaker information success.\n");
	
// 	// ɾ��˵����
// 	ret = xvpr_remove_person(handle, name);	
// 	if (XVPR_CODE_SUCCESS != ret) {
// 		printf("Remove speaker information failed. RET = %d\n", ret);
// 		return -1;
// 	}
//	printf("Remove speaker information success.\n");
	
// 	// ��ȡ˵������Ϣ
// 	// �ܵõ���˵�����Ƿ��ѳɹ�ע������
// 	ret = xvpr_get_info(handle, name, person);
// 	if (XVPR_CODE_SUCCESS != ret) {
// 		printf("Get speaker information failed. RET = %d\n", ret);
// 		return -1;
// 	}
//	printf("Get speaker information success.\n");

	// ��ȡ������Ϣ
	buf_len = xvpr_aid_read_buffer("../audio/0-9.1.wav", 50, buffer);
	if (buf_len <= 0) {
		printf("Read speech buffer failed. RET = %d\n", buf_len);
		return -1;
	}
	printf("Read speech buffer success.\n");

	// �����˵���ʵ��
	vad_handle = xvpr_vad_create_vad(8000);
	if (NULL == vad_handle) {
		printf("Create VAD handle failed. RET = %d\n", vad_handle);
		return -1;
	}
	printf("Create VAD handle success.\n");

	// ������Ч�������
	buf_len = xvpr_vad_detect_valid_speech(vad_handle, buffer, buf_len, valid_buffer);
	if (buf_len <= 0) {
		printf("No speech detected. RET = %d\n", buf_len);
		return -1;
	}
	printf("Detected valid speech success.\n");

	// Ϊ˵�������������ע�⣺��Ҫ���3��������������	
	ret = xvpr_add_speech(handle, name, "codec:pcm/raw;sr:8000;verify:true;rule:*", buffer, buf_len, checksum);
	if (XVPR_CODE_SUCCESS != ret) {
		printf("Add speech failed. RET = %d\n", ret);
		return -1;
	}
	printf("Add speech success.\n");

	// ע������
	ret = xvpr_register_person(handle, name);
	if (XVPR_CODE_SUCCESS != ret) {
		printf("Register speaker failed. RET = %d\n", ret);
		return -1;
	}
	printf("Register speaker success.\n");

	// ����ȷ��
	ret = xvpr_verify_person(handle, name, "codec:pcm/raw;sr:8000;verify:true;rule:*", buffer, buf_len, res);
	if (XVPR_CODE_SUCCESS != ret) {
		printf("Verify speaker failed. RET = %d\n", ret);
		return -1;
	}
	printf("Verify speaker success. name=%s, similarity=%2f\n", res.name, res.similarity);

	// ���Ʊ���
	ret = xvpr_identify_person(handle, "codec:pcm/raw;sr:8000;verify:true;rule:*", buffer, buf_len, res);
	if (XVPR_CODE_SUCCESS != ret) {
		printf("Identify speaker failed. RET = %d\n", ret);
		return -1;
	}
	printf("Identify speaker success. name=%s, similarity=%2f\n", res.name, res.similarity);

	// �ͷ��ڴ�
	xvpr_aid_release_buffer(buffer);
	xvpr_aid_release_buffer(valid_buffer);
	xvpr_global_release();

	getch();	
	return 0;
}

