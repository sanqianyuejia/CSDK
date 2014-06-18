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

#ifdef XVPR_LIB_EXPORTS
#define DLLAPI	_declspec(dllexport)
#else
#define DLLAPI	_declspec(dllimport)
#endif

#ifdef __cplusplus 
extern "C" {
#endif 

#define bool int
#define false 0
#define true 1

#ifndef NULL
#define NULL 0
#endif

/* 声纹识别引擎句柄 */
typedef void XVPR;

/* 端点检测引擎句柄 */
typedef void XVAD;

/* 返回值类型 */
typedef int XVPR_CODE;

/* 函数返回值及说明 */
XVPR_CODE static XVPR_CODE_SUCCESS = 0x0000;		// 调用成功
XVPR_CODE static XVPR_CODE_FAIL = 0x0001;			// 调用失败
XVPR_CODE static XVPR_CODE_CONNECT_ERR = 0x0002;	// 网络连接错误
XVPR_CODE static XVPR_CODE_PARAM_ERR = 0x0003;		// 参数设置错误

// ---------------------------------------------------------------------
// 错误代码	|	错误信息						 |	详细描述
// ---------|--------------------------------|--------------------------
// 0000		|	call succeed				 |	调用成功
// 0001		|	call fail					 |	调用失败
// 0002		|	connect error				 |	链接错误
// 0003		|	parameters error			 |	参数设置错误
// 1001		|	auth fail					 |	Key或Secret错误
// 1002		|	network error				 |	网络错误
// 1003		|	internal error				 |	未知错误
// 1004		|	argument error				 |	参数错误
// 1005		|	database error				 |	数据库错误
// 1006		|	system file lost			 |	系统文件丢失
// 2001		|	person already exist		 |	说话人已存在
// 2002		|	person not exist			 |	说话人不存在
// 2003		|	has no persons				 |	没有任何说话人
// 2004		|	person has no speeches		 |	该说话人没有登记语音
// 3001		|	voiceprint state error		 |	声纹训练状态错误
// 3002		|	voiceprint already registered|	说话人声纹已注册
// 3003		|	voiceprint has no speeches	 |	该说话人不存在登记语音
// 3004		|	verify error				 |	验证异常
// 3005		|	voiceprint not trained		 |	没有登记声纹
// 3006		|	identify error				 |	声纹辨认发生错误
// 4001		|	speech too short			 |	语音太短
// 4002		|	speech too long				 |	语音太长
// 4003		|	speech sample rate error	 |	语音采样率错误，目前仅支持8k
// 4004		|	speech already exist		 |	语音已存在
// 4005		|	speech not exist			 |	语音不存在
// 4006		|	speech process error		 |	语音处理错误
// 5001		|	asr recognize error			 |	语音识别错误
// 5002		|	asr not matched				 |	语音内容不匹配
//-------------------------------------------------------------------------

/* 返回说话人信息 */
typedef struct xvpr_person_info_s {
	char collection[32];	/* 说话人群组 */
	char name[32];			/* 说话人名字 */
	bool flag;				/* 登记状态 */
	char tag[32];			/* 说话人标签 */
} xvpr_person_info;

/* 返回识别结果 */
typedef struct xvpr_result_s {
	char collection[32];	/* 说话人群组 */
	char name[32];		/* 说话人名字 */
	float similarity;	/* 相似度 */
	char gender;	/* 说话人性别 */
}xvpr_result;


/*************************************** 引擎核心接口 ************************************/

/**
 * 引擎初始化（只能调用一次）
 * @return 成功返回XVPR_CODE_SUCCESS，失败返回XVPR_CODE_FAIL
 */
DLLAPI XVPR_CODE xvpr_global_init();

/**
 * 获取引擎版本信息
 * @return 返回版本信息
 */
DLLAPI const char *xvpr_version();

/**
 * 释放引擎资源（在程序退出的时候调用）
 */
DLLAPI void xvpr_global_release();

/**
 * 设置引擎参数
 * @param key 参数关键字
 * @param param 参数值
 * @return 返回状态值
 */
DLLAPI XVPR_CODE xvpr_global_setparam(const char *key, const char *param);

/**
 * 初始化调用请求（多线程之间不能共享该接口返回的句柄）
 * @param collection 群组ID，可以创建不同的调用请求，
 * 以便设置不同的群组ID
 * @return 返回请求句柄
 */
DLLAPI XVPR *xvpr_client_init(const char *collection);

/**
 * 创建说话人
 * @param handle 声纹引擎请求句柄
 * @param name 说话人用户名
 * @return 返回状态值
 */
DLLAPI XVPR_CODE xvpr_create_person(XVPR *handle, const char *name);

/**
 * 删除说话人信息
 * @param handle 声纹引擎请求句柄
 * @param name 说话人用户名
 * @return 返回状态值
 */
DLLAPI XVPR_CODE xvpr_remove_person(XVPR *handle, const char *name);

/**
 * 获取说话人信息
 * @param handle 声纹引擎请求句柄
 * @param name 说话人用户名
 * @param info 返回说话人信息
 * @return 返回状态值
 */
DLLAPI XVPR_CODE xvpr_get_info(XVPR *handle, const char *name, xvpr_person_info *info);

/**
 * 为说话人添加语音
 * @param handle 声纹引擎请求句柄 
 * @param name 说话人用户名
 * @param paramlist 语音参数列表
 * @param stream 语音流
 * @param buf_len 语音流长度
 * @param checksum 返回语音MD5值
 * @return 返回状态值
 */
DLLAPI XVPR_CODE xvpr_add_speech(XVPR *handle, const char *name, const char *paramlist, 
						  short *stream, size_t buf_len, char *checksum);


/**
 * 删除语音信息
 * @param handle 声纹引擎请求句柄 
 * @param checksum 语音MD5值
 * @return 返回状态值
 */
DLLAPI XVPR_CODE xvpr_remove_speech(XVPR *handle, const char *checksum);

/**
 * 保留最新语言，删除多余语音
 * @param handle 声纹引擎请求句柄 
 * @param name 说话人用户名
 * @return 返回状态值
 */
DLLAPI XVPR_CODE xvpr_reserve_speeches(XVPR *handle, const char *name);


/**
 * 注册说话人声纹信息
 * @param handle 声纹引擎请求句柄 
 * @param name 说话人用户名
 * @return 返回状态值
 */
DLLAPI XVPR_CODE xvpr_register_person(XVPR *handle, const char *name);


/**
 * 更新说话人声纹模型信息
 * @param handle 声纹引擎请求句柄 
 * @param name 说话人用户名
 * @return 返回状态值
 */
DLLAPI XVPR_CODE xvpr_update_person(XVPR *handle, const char *name);

/**
 * 说话人声纹验证
 * @param handle 声纹引擎请求句柄 
 * @param name 说话人用户名
 * @param paramlist 语音参数列表
 * @param stream 语音流
 * @param buf_len 语音流长度
 * @param res 返回声纹确认结果
 * @return 返回状态值
 */
DLLAPI XVPR_CODE xvpr_verify_person(XVPR *handle, const char *name, const char *paramlist, short *straem, size_t buf_len, xvpr_result *res);
/**
 * 说话人声纹辨认
 * @param handle 声纹引擎请求句柄
 * @param paramlist 语音参数列表
 * @param stream 语音流
 * @param buf_len 语音流长度
 * @param res 返回声纹确认结果
 * @return 返回状态值
 */
DLLAPI XVPR_CODE xvpr_identify_person(XVPR *handle, const char *paramlist, short *stream, size_t buf_len, xvpr_result *res);




/********************************* 端点检测引擎核心接口 ******************************************/


/**
 * 创建实时端点检测引擎
 * @return 引擎句柄
 */
DLLAPI XVAD *xvpr_vad_create_vad(size_t sr);

/**
 * 去除静音
 * @param sr 采样率
 * @param input 输入语音流
 * @param length 输入语音流采样点个数
 * @param output 输出语音流
 * @return 返回有效语音采样点个数
 */
DLLAPI size_t xvpr_vad_detect_valid_speech(XVAD *handle, short *input, size_t length, short **output);

/**
 * 实时端点检测
 * @param handle 端点检测引擎句柄
 * @param buffer 实时语音包
 * @param buf_len 实时语音包采样点数
 * @return 0-还在说话，1-停止说话
 */
DLLAPI int xvpr_vad_is_speaking(XVAD *handle, short *stream, size_t buf_len);

/** 
 * 重置VAD引擎句柄
 */
DLLAPI void xvpr_vad_reset_vad(XVAD *handle);


/********************************* 引擎辅助接口 ******************************************/

/**
 * 从语音文件读出数据
 * @param path 语音文件路径
 * @param skip_bits 需要跳过头部的字节数
 * @param buffer 返回语音数据（内存由接口内部申请，并且需要开发者调用【释放内存】的接口进行释放）
 * @return 返回语音采样点个数
 */
DLLAPI size_t xvpr_aid_read_buffer(const char *path, size_t skip_bits/* 跳过头部多少字节 */, short **buffer);

/** 
 * 将语音数据写入文件
 * @param path 语音文件路径
 * @param buffer 语音数据缓冲
 * @param buf_len 语音采样点个数
 */
DLLAPI int xvpr_aid_write_buffer(const char *path, short *buffer/* 语音数据 */, size_t buf_len);

/**
 * 释放内存
 * @param buffer 待释放的内存指针
 */
DLLAPI void xvpr_aid_release_buffer(short **buffer);

#ifdef __cplusplus 
} 
#endif

#endif	// _xbusiness_vpr_h_