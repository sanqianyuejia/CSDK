# 声纹识别开发平台C SDK
作为一种核心的语音技术，声纹识别技术在通信技术、语音技术得到飞速发展的今天有着巨大的应用潜力，相关产品已经应用于公共安全、社保身份认证、社区矫正，金融投资服务领域的身份辨认，智能手机移动商务和行业用户满意度调查等不同领域。

声纹识别开发平台由快商通声纹(www.shengwenyun.com)研发，平台提供一个分布式声纹识别解决方案，可根据应用规模适当增减服务器，灵活性非常强大，在以下领域有成功案例：

- 司法社区矫正
- 远程教育身份认证
- 高校教学实验平台

![](voiceplusplus.png)

## 平台特征介绍

- 文本相关，方便快捷。快商通声纹识别系统基于文本相关声纹识别，用户只需要一句简短口令，即可实时验证身份。
- 领先算法，精准识别。采用独特的声纹识别处理算法，声纹验证准确率高达99.5%，支持防录音攻击、环境降噪、动态数字口令。
- 分布式部署，扩展性强。分布式部署声纹识别服务器，支持上百万的声纹容量，易容扩展，而且可以根据应用规模适当调整硬件配置。
- 简约API，快速集成。提供一组RestFul风格API，支持多终端、多平台集成调用。可针对特定语言定制SDK开发包。

## 快速上手
请参考以下步骤进行开发

1. 申请api key和secret（请发邮件至lixm@shengwenyun.com，邮件标题置为“快商通声纹识别开发平台：API调用申请”）
2. 参照example目录中的例子，编写应用程序
3. 上线运营。上线之前，请联系管理员（lixm@shengwenyun.com）

## 开发示例——注册说话人声纹

 
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
	} else {
		printf("Remove speaker information success.\n");
	}
		
	// 创建说话人
	ret = xvpr_create_person(handle/* 声纹引擎句柄 */, name/* 说话人用户名 */);
	if (XVPR_CODE_SUCCESS != ret) {
		printf("Create speaker information failed. RET = %d\n", ret);
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


## 错误代码对照表
<table cellpadding="0" cellspacing="1" border="0" style="width:100%" class="tableborder">
<tbody><tr>
<th>错误代码</th>
<th>错误信息</th>
<th>详细描述</th>
</tr>

<tr>
<td class="td"><strong>0000</strong></td>
<td class="td">call succeed</td>
<td class="td">调用成功</td>
</tr>

<tr>
<td class="td"><strong>0001</strong></td>
<td class="td">call fail</td>
<td class="td">调用失败</td>
</tr>

<tr>
<td class="td"><strong>0002</strong></td>
<td class="td">connect error</td>
<td class="td">链接错误</td>
</tr>

<tr>
<td class="td"><strong>0003</strong></td>
<td class="td">parameters error</td>
<td class="td">参数设置错误</td>
</tr>

<tr>
<td class="td"><strong>1001</strong></td>
<td class="td">Auth fail</td>
<td class="td">Key或Secret错误</td>
</tr>

<tr>
<td class="td"><strong>1002</strong></td>
<td class="td">network error</td>
<td class="td">网络错误</td>
</tr>

<tr>
<td class="td"><strong>1003</strong></td>
<td class="td">internal error</td>
<td class="td">未知错误</td>
</tr>

<tr>
<td class="td"><strong>1004</strong></td>
<td class="td">argument error</td>
<td class="td">参数错误</td>
</tr>

<tr>
<td class="td"><strong>1005</strong></td>
<td class="td">database error</td>
<td class="td">数据库错误</td>
</tr>

<tr>
<td class="td"><strong>1006</strong></td>
<td class="td">system file lost</td>
<td class="td">系统文件丢失</td>
</tr>

<tr>
<td class="td"><strong>2001</strong></td>
<td class="td">person already exist</td>
<td class="td">说话人已存在</td>
</tr>

<tr>
<td class="td"><strong>2002</strong></td>
<td class="td">person not exist</td>
<td class="td">说话人不存在</td>
</tr>

<tr>
<td class="td"><strong>2003</strong></td>
<td class="td">has no persons</td>
<td class="td">没有任何说话人</td>
</tr>

<tr>
<td class="td"><strong>2004</strong></td>
<td class="td">person has no speeches</td>
<td class="td">该说话人没有登记语音</td>
</tr>

<tr>
<td class="td"><strong>3001</strong></td>
<td class="td">voiceprint state error</td>
<td class="td">声纹训练状态错误</td>
</tr>

<tr>
<td class="td"><strong>3002</strong></td>
<td class="td">voiceprint already registered</td>
<td class="td">说话人声纹已注册</td>
</tr>

<tr>
<td class="td"><strong>3003</strong></td>
<td class="td">voiceprint has no speeches</td>
<td class="td">该说话人不存在登记语音</td>
</tr>

<tr>
<td class="td"><strong>3004</strong></td>
<td class="td">verify error</td>
<td class="td">验证异常</td>
</tr>

<tr>
<td class="td"><strong>3005</strong></td>
<td class="td">voiceprint not trained</td>
<td class="td">没有登记声纹</td>
</tr>

<tr>
<td class="td"><strong>3006</strong></td>
<td class="td">identify error</td>
<td class="td">声纹辨认发生错误</td>
</tr>

<tr>
<td class="td"><strong>4001</strong></td>
<td class="td">speech too short</td>
<td class="td">语音太短</td>
</tr>

<tr>
<td class="td"><strong>4002</strong></td>
<td class="td">speech too long</td>
<td class="td">语音太长</td>
</tr>

<tr>
<td class="td"><strong>4003</strong></td>
<td class="td">speech sample rate error</td>
<td class="td">语音采样率错误，目前仅支持8k</td>
</tr>

<tr>
<td class="td"><strong>4004</strong></td>
<td class="td">speech already exist</td>
<td class="td">语音已存在</td>
</tr>

<tr>
<td class="td"><strong>4005</strong></td>
<td class="td">speech not exist</td>
<td class="td">语音不存在</td>
</tr>

<tr>
<td class="td"><strong>4006</strong></td>
<td class="td">speech process error</td>
<td class="td">语音处理错误</td>

<tr>
<td class="td"><strong>5001</strong></td>
<td class="td">asr recognize error</td>
<td class="td">语音识别错误</td>
</tr>

<tr>
<td class="td"><strong>5002</strong></td>
<td class="td">asr not matched</td>
<td class="td">语音内容不匹配</td>

</tr>

</tbody></table>
	

## 下载

### 从 release 版本下载

下载地址：https://github.com/sanqianyuejia/CSDK/releases

## 许可证

Copyright (c) 2013 快商通信息技术有限公司

基于 MIT 协议发布:

* [www.opensource.org/licenses/MIT](http://www.opensource.org/licenses/MIT)
