VS2022 C++ + GoogleTest 

# 资料
> [googletest/googletest/samples at main · google/googletest](https://github.com/google/googletest/tree/main/googletest/samples) 
> [一文掌握谷歌 C++ 单元测试框架 GoogleTest](https://github.com/0voice/cpp_backend_awsome_blog/blob/main/【NO.195】一文掌握谷歌%20C%2B%2B%20单元测试框架%20GoogleTest.md) 

# 环境
VS2022，x64，GTest内置模板，C++运行库 MD / MDd
添加单元测试

# 框架
Core（静态库 .lib，存放业务模块，无main）
MainApp（应用程序 .exe，业务入口，自带main函数）
UnitTests（GTest测试程序 .exe，GTest自带main，不需要手写main）
编译产物：不使用bin目录，输出到 `$(SolutionDir)$(Platform)\$(Configuration)\`

```
SampleTest01/
├─ SampleTest01.sln
├─ Core/                      # 静态库项目
│  ├─ module_math/            # 物理文件夹（不是VS筛选器）
│  │  ├─ math.h
│  │  └─ math.cpp
│  └─ module_user/            # 物理文件夹（不是VS筛选器）
│     ├─ user.h
│     └─ user.cpp
├─ MainApp/
│  └─ main.cpp
├─ UnitTests/                 # GTest测试项目，最终生成exe
├─ x64/                       # 编译输出目录，git忽略
└─ .gitignore
```

# 新建空白解决方案

VS2022 → 创建新项目 → 空白解决方案，名称 `SampleTest01`

注意 **空白解决方案** 不是**空项目**

# 创建Core项目（静态库）

1. 右键解决方案 → 添加 → 新建项目 → C++空项目，名称`Core`
2. 解决方案资源管理器顶部：**打开【显示所有文件夹】**，可以直接操作磁盘真实目录
3. 右键Core → 添加 → 新建文件夹，创建物理文件夹：`module_math`、`module_user`> 
> 👉 这里添加的是**磁盘物理文件夹**，不是VS虚拟筛选器，目录在磁盘真实存在
4. 在文件夹内添加头文件和源文件
5. 右键文件夹 -> 包括在项目中
6. 右键Core → 属性，【所有配置】、【所有平台】

- module_math/math.h

```cpp
#pragma once
int Add(int a, int b);
```

- module_math/math.cpp

```cpp
#include "math.h"
int Add(int a, int b)
{
    return a + b;
}
```

- module_user/user.h

```cpp
#pragma once
bool CheckAdult(int age);
```

- module_user/user.cpp

```cpp
#include "user.h"
bool CheckAdult(int age)
{
    return age >= 18;
}
```

## 配置属性
右键 Core 项目 -> 属性
- 常规 → 配置类型：**静态库(.lib)**
- 常规 → 输出目录：`$(SolutionDir)$(Platform)\$(Configuration)\`
- C/C++ → 代码生成 → 运行库
  - Debug：多线程调试DLL (/MDd)  (左上方选择 debug 模式)
  - Release：多线程DLL (/MD)  (左上方选择 release 模式)

# 创建MainApp项目（业务exe）

1. 右键解决方案 → 添加 → 新建项目 → C++空项目，名称`MainApp`
2. 添加 `main.cpp`
3. 右键MainApp → 属性，【所有配置】、【所有平台】

```cpp
#include <iostream>
#include "module_math/math.h"
#include "module_user/user.h"

int main()
{
    std::cout << "Add(10,20) = " << Add(10, 20) << std::endl;
    std::cout << "CheckAdult(22) = " << CheckAdult(22) << std::endl;
    return 0;
}
```

## 配置属性
- 常规 → 配置类型：**应用程序(.exe)**
- 常规 → 输出目录：`$(SolutionDir)$(Platform)\$(Configuration)\`
- C/C++ → 常规 → 附加包含目录：`$(SolutionDir)Core`
- C/C++ → 代码生成 → 运行库
  - Debug：多线程调试DLL (/MDd)  (左上方选择 debug 模式)
  - Release：多线程DLL (/MD)  (左上方选择 release 模式)
- 链接器 → 常规 → 附加库目录：`$(SolutionDir)$(Platform)\$(Configuration)\`
- 链接器 → 输入 → 附加依赖项：`Core.lib`

## 设置项目依赖（Core与MainApp）

右键解决方案 → **项目依赖项**
项目下拉框选 `MainApp` → 勾选 `Core`

作用：生成解决方案时，VS保证Core优先编译，再编译MainApp

# 运行业务程序 MainApp

1. 右键 `MainApp` → **设为启动项目**（项目名称变成粗体）
2. 顶部工具栏：Debug，x64
3. 快捷键 `Ctrl+Shift+B`（生成解决方案），确认编译成功
4. 运行：
   - `Ctrl + F5`：不调试启动，控制台运行完不会一闪消失（推荐单纯看输出）
   - `F5`：带调试器启动，可以断点调试
   输出结果：

```cpp
Add(10,20) = 30
CheckAdult(22) = 1
```

1代表bool true。到这里业务代码已经跑通。

# 创建 GoogleTest 单元测试项目

1. 右键解决方案 → 添加 → 新建项目 → 搜索 `Google Test`
项目名称：`UnitTests`，位置保持在解决方案目录下

> 
> 弹出【测试项目配置】弹窗，**关键设置**
> 
> 
> - 选择要测试的项目：`<无项目>` ✅ 不要选Core/MainApp（手动配置头文件和lib引用，避免自动引用冲突）
> - 将Google Test用作：**静态库(.lib)**
> - C++运行时库：**动态链接(推荐)**（MD/MDd，和Core、MainApp保持一致，防止LNK2038）
> 点击OK创建项目
> 
> 
> ✅ 注意：UnitTests默认配置类型是**应用程序(.exe)**，**不需要修改**。GTest测试工程本质就是exe，GTest自带main函数，不用自己写main。

2. UnitTests项目属性配置（【所有配置】、【所有平台】）
3. 编写测试代码
删除模板自带的`test.cpp`，新建两个文件：`test_math.cpp`、`test_user.cpp`

test_math.cpp

```cpp
#include "pch.h"
#include <gtest/gtest.h>
#include "module_math/math.h"

TEST(MathSuite, AddTest)
{
    EXPECT_EQ(Add(1, 2), 3);
    EXPECT_EQ(Add(-5, 3), -2);
}
```

test_user.cpp

```cpp
#include "pch.h"
#include <gtest/gtest.h>
#include "module_user/user.h"

TEST(UserSuite, AdultCheckTest)
{
    EXPECT_TRUE(CheckAdult(20));
    EXPECT_FALSE(CheckAdult(16));
}
```

## 配置属性
右键UnitTests → 属性

- 常规 → 输出目录：`$(SolutionDir)$(Platform)\$(Configuration)\`
- C/C++ → 常规 → 附加包含目录：`$(SolutionDir)Core`
- C/C++ → 代码生成 → 运行库
  - Debug：多线程调试DLL (/MDd)  (左上方选择 debug 模式)
  - Release：多线程DLL (/MD)  (左上方选择 release 模式)
- 链接器 → 常规 → 附加库目录：`$(SolutionDir)$(Platform)\$(Configuration)\`
- 链接器 → 输入 → 附加依赖项：`Core.lib`

## 设置项目依赖（UnitTests依赖Core）
右键解决方案 → **项目依赖项**
项目下拉框选 `UnitTests` → 勾选 `Core`

> 
> 依赖关系说明：
> Core 没有依赖；MainApp依赖Core；UnitTests依赖Core
> 生成解决方案时：Core最先编译；MainApp和UnitTests互相无依赖，可以并行编译，不是固定Core→MainApp→UnitTests。
> 
> 
> - 单独右键某个项目点【生成】，只会编译当前这一个项目，**不会自动编译依赖项目**
> - 顶部菜单【生成 → 生成解决方案(Ctrl+Shift+B)】，才会按项目依赖构建全部项目

# 运行原始业务代码 MainApp

1. 右键 `MainApp` → **设为启动项目**（粗体）
2. `Ctrl+Shift+B` 生成解决方案
3. `Ctrl+F5` 运行业务控制台程序，输出业务打印信息

# 执行单元测试

## 测试资源管理器
图形化看用例结果
> 
> ✅ **不需要修改启动项目，不用把UnitTests设为启动项**

1. 顶部菜单：**测试 → 测试资源管理器**，调出窗口
2. 点击【全部运行】
3. 窗口直接展示：每个测试套件、测试用例，绿色=成功，红色=失败，可以单独运行某一个用例，支持给测试代码打断点调试

## 把UnitTests设为启动项目，控制台运行GTest

1. 右键 `UnitTests` → **设为启动项目**
2. `Ctrl+F5`，直接运行UnitTests.exe> 
> 控制台打印GTest全部测试日志，显示成功、失败数量；缺点：不方便单独挑选用例执行。

# .gitignore

```
# VS编译产物
x64/
x86/
*.suo
*.user
*.sdf
*.ipch
*.obj
*.pdb
*.exe
*.lib
*.log
.vs/
```
