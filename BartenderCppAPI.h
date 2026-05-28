#pragma once

#ifndef BARTENDER_EXPORTS
#define BARTENDER_EXPORTS
#endif

#ifdef BARTENDER_EXPORTS
#define BARTENDER_API __declspec(dllexport)
#else
#define BARTENDER_API __declspec(dllimport)
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// BartenderCLR API - C++直接调用接口
// 版本: 1.0
// 平台: Windows (x86/x64)
// 依赖: Bartender SDK / Bartender 软件安装
// 
// 功能说明:
//   此API提供C++程序直接调用Bartender打印功能的接口，通过C++/CLI封装层
//   调用Bartender的.NET SDK，实现标签打印功能。
// 
// 使用流程:
//   1. CreateBartenderAPI()      - 创建API实例
//   2. StartEngine()              - 启动Bartender引擎
//   3. OpenLabelFormatDocument()  - 打开标签文件(.btw)
//   4. PrintLabel()               - 执行打印
//   5. CloseLabelFormatDocument() - 关闭标签文件
//   6. StopEngine()               - 停止引擎
//   7. DeleteBartenderAPI()       - 释放API实例
// 
// 部署要求:
//   - 目标机器必须安装Bartender软件或Bartender SDK
//   - 需要将以下DLL放在程序目录:
//     * BartenderCLR.dll
//     * Seagull.BarTender.Print.dll
//     * Seagull.Services.PrintScheduler.dll
//     * Seagull.Services.PrintScheduler.Contracts.dll
// =============================================================================

/**
 * @brief 创建Bartender API实例
 * 
 * @return 返回API实例句柄，失败返回NULL
 * 
 * @note 此函数仅分配内存，不会启动引擎
 */
BARTENDER_API void* CreateBartenderAPI();

/**
 * @brief 释放Bartender API实例
 * 
 * @param handle API实例句柄（由CreateBartenderAPI返回）
 * 
 * @note 调用此函数前应先调用StopEngine停止引擎
 */
BARTENDER_API void DeleteBartenderAPI(void* handle);

/**
 * @brief 启动Bartender引擎
 * 
 * @param handle API实例句柄
 * 
 * @return 成功返回1，失败返回0
 * 
 * @note 失败时可通过GetLastErrorMessage获取详细错误信息
 */
BARTENDER_API int StartEngine(void* handle);

/**
 * @brief 停止Bartender引擎
 * 
 * @param handle API实例句柄
 * 
 * @note 此函数会自动关闭已打开的标签文档
 */
BARTENDER_API void StopEngine(void* handle);

/**
 * @brief 获取最后一次错误的描述信息
 * 
 * @param handle API实例句柄
 * 
 * @return 返回错误信息字符串指针，调用者无需释放内存
 * 
 * @note 错误信息在获取后会被清空
 */
BARTENDER_API const char* GetLastErrorMessage(void* handle);

/**
 * @brief 获取系统中可用的打印机名称列表
 * 
 * @param handle API实例句柄
 * @param names 存储打印机名称的字符串数组（可为NULL用于获取数量）
 * @param maxCount 最大获取数量
 * 
 * @return 成功返回打印机数量，失败返回0或负数
 * 
 * @note 1. 当names为NULL时，仅返回打印机数量
 *       2. 使用后需调用FreePrinterNames释放内存
 */
BARTENDER_API int GetPrinterNames(void* handle, char** names, int maxCount);

/**
 * @brief 释放打印机名称数组内存
 * 
 * @param names GetPrinterNames返回的名称数组
 * @param count 打印机数量
 */
BARTENDER_API void FreePrinterNames(char** names, int count);

/**
 * @brief 打开标签格式文档（.btw文件）
 * 
 * @param handle API实例句柄
 * @param absFilePath 标签文件的绝对路径（UTF-8编码）
 * 
 * @return 成功返回1，失败返回0
 * 
 * @note 1. 必须先调用StartEngine启动引擎
 *       2. 失败时可通过GetLastErrorMessage获取详细错误信息
 *       3. 路径支持中文和空格，需以UTF-8编码传入
 */
BARTENDER_API int OpenLabelFormatDocument(void* handle, const char* absFilePath);

/**
 * @brief 关闭已打开的标签文档
 * 
 * @param handle API实例句柄
 * 
 * @note 此函数不保存对文档的修改
 */
BARTENDER_API void CloseLabelFormatDocument(void* handle);

/**
 * @brief 设置标签中的文本变量值
 * 
 * @param handle API实例句柄
 * @param name 变量名称（在Bartender中定义的SubString名称，UTF-8编码）
 * @param value 要设置的变量值（UTF-8编码）
 * 
 * @return 成功返回1，失败返回0
 * 
 * @note 1. 必须先调用OpenLabelFormatDocument打开标签
 *       2. 变量名称区分大小写
 *       3. 失败时可通过GetLastErrorMessage获取详细错误信息
 *       4. 支持中文变量名和值，需以UTF-8编码传入
 */
BARTENDER_API int SetSubStringValue(void* handle, const char* name, const char* value);

/**
 * @brief 获取标签中的文本变量值
 * 
 * @param handle API实例句柄
 * @param name 变量名称
 * @param buffer 用于存储变量值的缓冲区
 * @param bufferSize 缓冲区大小
 * 
 * @return 成功返回变量值长度，失败返回0或负数
 * 
 * @note 必须先调用OpenLabelFormatDocument打开标签
 */
BARTENDER_API int GetSubStringValue(void* handle, const char* name, char* buffer, int bufferSize);

/**
 * @brief 获取标签中的所有变量名称
 * 
 * @param handle API实例句柄
 * @param names 存储变量名称的字符串数组（可为NULL用于获取数量）
 * @param maxCount 最大获取数量
 * 
 * @return 成功返回变量数量，失败返回0或负数
 * 
 * @note 1. 必须先调用OpenLabelFormatDocument打开标签
 *       2. 当names为NULL时，仅返回变量数量
 *       3. 使用后需调用FreeSubStrings释放内存
 */
BARTENDER_API int GetDocumentSubStrings(void* handle, char** names, int maxCount);

/**
 * @brief 释放变量名称数组内存
 * 
 * @param names GetDocumentSubStrings返回的名称数组
 * @param count 变量数量
 */
BARTENDER_API void FreeSubStrings(char** names, int count);

/**
 * @brief 执行标签打印
 * 
 * @param handle API实例句柄
 * @param printerName 打印机名称（NULL或空字符串使用默认打印机）
 * @param waitMs 打印超时时间（毫秒），建议设置为30000（30秒）
 * @param msgs 用于接收打印消息的数组（调用者需预分配内存）
 * @param maxMsgCount 最大消息数量
 * 
 * @return 返回值说明：
 *         1  - 打印成功
 *         0  - 打印失败
 *        -1  - 打印超时
 * 
 * @note 1. 必须先调用OpenLabelFormatDocument打开标签
 *       2. 使用后需调用FreePrintMessages释放msgs内存
 */
BARTENDER_API int PrintLabel(void* handle, const char* printerName, int waitMs, char** msgs, int maxMsgCount);

/**
 * @brief 释放打印消息数组内存
 * 
 * @param msgs PrintLabel返回的消息数组
 * @param count 消息数量（即调用PrintLabel时传入的maxMsgCount）
 */
BARTENDER_API void FreePrintMessages(char** msgs, int count);

#ifdef __cplusplus
}
#endif