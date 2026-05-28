# BartenderCLR 常见问题排查指南

## demo文件夹说明

包含了基本使用，可参考进行使用并进行后续开发

## 目录

1. [问题1：StartEngine() 失败 - FileLoadException](#问题1-startengine-失败---fileloadexception)
   - [问题描述](#问题描述)
   - [错误信息](#错误信息)
   - [根本原因](#根本原因)
   - [解决方案](#解决方案)
   - [配置文件示例](#配置文件示例)

2. [问题2：创建 API 实例失败](#问题2-创建-api-实例失败)
   - [问题描述](#问题描述-1)
   - [可能原因](#可能原因)
   - [解决方案](#解决方案-1)

3. [问题3：打印时提示找不到打印机](#问题3-打印时提示找不到打印机)
   - [问题描述](#问题描述-2)
   - [可能原因](#可能原因-1)
   - [解决方案](#解决方案-2)

4. [部署清单](#部署清单)

5. [最佳实践](#最佳实践)

---

## 问题1：StartEngine() 失败 - FileLoadException

### 问题描述

调用 `StartEngine(handle)` 时返回 0（失败），错误信息包含：

```
System.IO.FileLoadException: ...v2.0.50727...4.0...
```

### 错误信息示例

```
启动Bartender引擎失败
System.IO.FileLoadException: ???????????????????v2.0.50727????????????????????????????????????????￡?????? 4.0 ??????м???ó?????
    ?? System.Signature.GetSignature(Void* pCorSig, Int32 cCorSig, RuntimeFieldHandleInternal fieldHandle, IRuntimeMethodInfo methodHandle, RuntimeType declaringType)
    ?? System.Reflection.RuntimeMethodInfo.FetchNonReturnParameters()
    ?? System.Reflection.RuntimeMethodInfo.GetParameters()
    ...
```

### 根本原因

**.NET Framework 版本隔离问题**：

| 组件                                        | .NET 版本          |
| ------------------------------------------- | ------------------ |
| Bartender SDK (Seagull.BarTender.Print.dll) | .NET Framework 2.0 |
| BartenderCLR.dll (C++/CLI)                  | .NET Framework 4.x |

- .NET 2.0 和 .NET 4.x 使用**不同的 CLR (Common Language Runtime)**
- 默认情况下，一个进程只能加载**一个版本**的 CLR
- 当 .NET 4.x 运行时尝试加载 .NET 2.0 程序集时，会遇到兼容性限制

### 解决方案

在你的应用程序目录创建或修改 `YourApp.exe.config` 文件，添加运行时配置：

### 配置文件示例

```xml
<?xml version="1.0" encoding="utf-8"?>
<configuration>
  <startup useLegacyV2RuntimeActivationPolicy="true">
    <supportedRuntime version="v4.0" sku=".NETFramework,Version=v4.7.2"/>
    <supportedRuntime version="v2.0.50727"/>
  </startup>
  <runtime>
    <generatePublisherEvidence enabled="false"/>
  </runtime>
</configuration>
```

**关键设置说明**：

| 设置                                        | 作用                                                         |
| ------------------------------------------- | ------------------------------------------------------------ |
| `useLegacyV2RuntimeActivationPolicy="true"` | 启用旧版运行时激活策略，允许 .NET 4.x 正确加载 .NET 2.0 程序集 |
| `generatePublisherEvidence="false"`         | 禁用发布者证据检查，避免权限问题                             |
| 同时声明 v4.0 和 v2.0.50727                 | 告诉 CLR 同时支持两个版本                                    |

---

## 问题2：创建 API 实例失败

### 问题描述

调用 `CreateBartenderAPI()` 返回 `nullptr`。

### 可能原因

1. **缺少 .NET 依赖**：`Seagull.BarTender.Print.dll` 等文件未复制到程序目录
2. **平台不匹配**：32位/64位不兼容
3. **权限不足**：程序没有足够权限

### 解决方案

1. **检查依赖文件**：确保以下文件在程序目录：
   - `BartenderCLR.dll`
   - `Seagull.BarTender.Print.dll`
   - `Seagull.Services.PrintScheduler.dll`
   - `Seagull.Services.PrintScheduler.Contracts.dll`

2. **确认平台匹配**：
   - 如果 Bartender 是 32 位，程序必须编译为 32 位
   - 如果 Bartender 是 64 位，程序必须编译为 64 位

3. **以管理员身份运行**：右键程序 → 以管理员身份运行

---

## 问题3：打印时提示找不到打印机

### 问题描述

调用 `PrintLabel()` 失败，提示找不到指定的打印机。

### 可能原因

1. **打印机名称错误**：传入的打印机名称与系统中的打印机名称不完全匹配
2. **打印机未安装**：目标打印机未在系统中安装
3. **权限不足**：程序没有访问打印机的权限

### 解决方案

1. **使用 `GetPrinterNames()` 获取正确的打印机名称**：

```cpp
char* printerNames[100];
for (int i = 0; i < 100; i++) {
    printerNames[i] = new char[256];
}
int count = GetPrinterNames(handle, printerNames, 100);
for (int i = 0; i < count; i++) {
    printf("Printer[%d]: %s\n", i, printerNames[i]);
}
FreePrinterNames(printerNames, count);
```

2. **确认打印机已正确安装**：在控制面板 → 设备和打印机中检查

3. **确保打印机名称完全匹配**（区分大小写）

---

## 部署清单

将以下文件复制到应用程序目录：

```
YourApp/
├── YourApp.exe                    ← 你的主程序
├── YourApp.exe.config             ← 运行时配置文件
├── BartenderCLR.dll               ← 编译生成的 C++/CLI DLL
├── Seagull.BarTender.Print.dll    ← Bartender SDK 核心库
├── Seagull.Services.PrintScheduler.dll       ← 调度服务
└── Seagull.Services.PrintScheduler.Contracts.dll  ← 调度服务合约
```

**Bartender SDK DLL 来源**：

```
C:\Program Files (x86)\Seagull\BarTender Suite\SDK\Assemblies\
```

---

## 最佳实践

### 1. 错误处理

始终检查 API 返回值，并使用 `GetLastErrorMessage()` 获取详细错误信息：

```cpp
int res = StartEngine(handle);
if (res == 0) {
    qDebug() << "启动失败:" << GetLastErrorMessage(handle);
}
```

### 2. 资源清理

使用完成后务必释放资源：

```cpp
CloseLabelFormatDocument(handle);
StopEngine(handle);
DeleteBartenderAPI(handle);
```

### 3. 平台一致性

确保所有组件的平台一致：

- BartenderCLR.dll 编译平台
- 主程序编译平台  
- Bartender 安装版本

### 4. 配置文件

始终随程序发布 `YourApp.exe.config` 文件，避免运行时版本冲突。

---

## 错误码说明

| 返回值 | 含义 |
| ------ | ---- |
| `1`    | 成功 |
| `0`    | 失败 |
| `-1`   | 超时 |

---

**文档版本**：v1.0  
**更新日期**：2026年5月  
**适用范围**：BartenderCLR API

