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

#ifdef __cplusplus 
extern "C" {
#endif 

/* 声纹识别引擎句柄 */
typedef void XVPR;

/* 端点检测引擎句柄 */
typedef void XVAD;

/* 函数返回值枚举类型 */
enum XVPR_CODE {
	XVPR_CODE_SUCCESS,			/* 调用成功 */
	XVPR_CODE_FAIL,				/* 调用失败 */
	XVPR_CODE_CONNECT_ERR,		/* 网络连接错误 */
	XVPR_CODE_PARAM_ERR,		/* 参数错误 */
	XVPR_CODE_NOT_FOUND			/* 对象未发现 */
};

/* 返回说话人信息 */
struct xvpr_person_info {
	char collection[32];	/* 说话人群组 */
	char name[32];			/* 说话人名字 */
	char tag[32];			/* 说话人标签 */
	bool flag;				/* 登记状态 */

	xvpr_person_info() {
		memset(this->collection, 0, sizeof(char)*32); 
		memset(this->name, 0, sizeof(char) * 32);
		memset(this->tag, 0, sizeof(char) * 32);
		this->flag = false;
	}
};

/* 返回识别结果 */
struct xvpr_result {
	char collection[32];	/* 说话人群组 */
	char name[32];		/* 说话人名字 */
	char gender;	/* 说话人性别 */
	float similarity;	/* 相似度 */

	xvpr_result() {
		memset(this->collection, 0, sizeof(char)*32); 
		memset(this->name, 0, sizeof(char) * 32);
		this->gender = 'M';
		this->similarity = 0.0f;
	}
};


/*************************************** 引擎核心接口 ************************************/

/**
 * 引擎初始化（只能调用一次）
 * @return 成功返回XVPR_CODE_SUCCESS，失败返回XVPR_CODE_FAIL
 */
XVPR_CODE xvpr_global_init();

/**
 * 获取引擎版本信息
 * @return 返回版本信息
 */
const char *xvpr_version();

/**
 * 释放引擎资源（在程序退出的时候调用）
 */
void xvpr_global_release();

/**
 * 设置引擎参数
 * @param key 参数关键字
 * @param param 参数值
 * @return 返回状态值
 */
XVPR_CODE xvpr_global_setparam(const char *key, const char *param);

/**
 * 初始化调用请求（多线程之间不能共享该接口返回的句柄）
 * @param collection 群组ID，可以创建不同的调用请求，
 * 以便设置不同的群组ID
 * @return 返回请求句柄
 */
XVPR *xvpr_client_init(const char *collection);

/**
 * 创建说话人
 * @param handle 声纹引擎请求句柄
 * @param name 说话人用户名
 * @return 返回状态值
 */
XVPR_CODE xvpr_create_person(XVPR *handle, const char *name);

/**
 * 删除说话人信息
 * @param handle 声纹引擎请求句柄
 * @param name 说话人用户名
 * @return 返回状态值
 */
XVPR_CODE xvpr_remove_person(XVPR *handle, const char *name);

/**
 * 获取说话人信息
 * @param handle 声纹引擎请求句柄
 * @param name 说话人用户名
 * @param info 返回说话人信息
 * @return 返回状态值
 */
XVPR_CODE xvpr_get_info(XVPR *handle, const char *name, xvpr_person_info &info);

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
XVPR_CODE xvpr_add_speech(XVPR *handle, const char *name, const char *paramlist, 
						  short *stream, size_t buf_len, char *checksum);


/**
 * 删除语音信息
 * @param handle 声纹引擎请求句柄 
 * @param checksum 语音MD5值
 * @return 返回状态值
 */
XVPR_CODE xvpr_remove_speech(XVPR *handle, const char *checksum);

/**
 * 保留最新语言，删除多余语音
 * @param handle 声纹引擎请求句柄 
 * @param name 说话人用户名
 * @return 返回状态值
 */
XVPR_CODE xvpr_reserve_speeches(XVPR *handle, const char *name);


/**
 * 注册说话人声纹信息
 * @param handle 声纹引擎请求句柄 
 * @param name 说话人用户名
 * @return 返回状态值
 */
XVPR_CODE xvpr_register_person(XVPR *handle, const char *name);

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
XVPR_CODE xvpr_verify_person(XVPR *handle, const char *name, const char *paramlist, short *buffer, size_t buf_len, xvpr_result &res);
/**
 * 说话人声纹辨认
 * @param handle 声纹引擎请求句柄
 * @param paramlist 语音参数列表
 * @param stream 语音流
 * @param buf_len 语音流长度
 * @param res 返回声纹确认结果
 * @return 返回状态值
 */
XVPR_CODE xvpr_identify_person(XVPR *handle, const char *paramlist, short *buffer, size_t buf_len, xvpr_result &res);




/********************************* 端点检测引擎核心接口 ******************************************/


/**
 * 创建实时端点检测引擎
 * @return 引擎句柄
 */
XVAD *xvpr_vad_create_vad(size_t sr);

/**
 * 去除静音
 * @param sr 采样率
 * @param input 输入语音流
 * @param length 输入语音流采样点个数
 * @param output 输出语音流
 * @return 返回有效语音采样点个数
 */
size_t xvpr_vad_detect_valid_speech(XVAD *&handle, short *input, size_t length, short *&output);

/**
 * 实时端点检测
 * @param handle 端点检测引擎句柄
 * @param buffer 实时语音包
 * @param buf_len 实时语音包采样点数
 * @return 0-还在说话，1-停止说话
 */
int xvpr_vad_is_speaking(XVAD *&handle, short *buffer, size_t buf_len);

/** 
 * 重置VAD引擎句柄
 */
void xvpr_vad_reset_vad(XVAD *&handle);


/********************************* 引擎辅助接口 ******************************************/

/**
 * 从语音文件读出数据
 * @param path 语音文件路径
 * @param skip_bits 需要跳过头部的字节数
 * @param buffer 返回语音数据（内存由接口内部申请，并且需要开发者调用【释放内存】的接口进行释放）
 * @return 返回语音采样点个数
 */
size_t xvpr_aid_read_buffer(const char *path, size_t skip_bits/* 跳过头部多少字节 */, short *&buffer);

/** 
 * 将语音数据写入文件
 * @param path 语音文件路径
 * @param buffer 语音数据缓冲
 * @param buf_len 语音采样点个数
 */
void xvpr_aid_write_buffer(const char *path, short *buffer/* 语音数据 */, size_t buf_len);

/**
 * 释放内存
 * @param buffer 待释放的内存指针
 */
void xvpr_aid_release_buffer(short *&buffer);

#ifdef __cplusplus 
} 
#endif

#endif	// _xbusiness_vpr_h_