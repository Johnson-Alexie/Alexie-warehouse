# Week2D3

## 完成事项

1.完成了gitclone、add、commit、push 的完整流程

2.成功在本地用CMake工具运行了C++和opencv识图程序

## 问题

1.源文件名称不匹配报错

- CMakeLists.txt里写的源码名是opencv_test.cpp，但项目里实际文件是opencv1.cpp，CMake找不到源文件直接终止编译，通过统一文件名解决

2.程序运行后图片读取失败

- 不理解OpenCV的imread路径规则，程序在build目录运行时默认在当前工作目录找图片，找不到根目录下的test.jpg，通过切换运行路径/移动图片/写绝对路径三种方案解决

3.快速生成测试图片

- 本地没有现成JPG测试图，通过ImageMagick的convert命令一行生成标准测试图片，不用手动截图

## 知识点

1.CMake基础工作流

- 标准的out-of-source编译：新建build目录，在里面执行cmake ..生成Makefile，再执行make编译，不会污染项目根目录
- CMakeLists.txt核心三要素：指定C++版本、查找第三方依赖、把源码编译成可执行文件并链接依赖库

- CMakeLists.txt里写的源码名需要与运行程序一致

2.OpenCV基础API规则
- imread的路径逻辑：读取终端的当前工作目录，和源码、可执行文件的存放位置无关
- 图片读取失败的通用排查思路：优先检查路径是否正确，而不是怀疑库本身安装错误

3.实用工具技巧
- 用convert命令在无图形界面的终端快速生成测试图片
- 理解Markdown基础语法和Typora的使用逻辑，能快速整理技术笔记

4.git 网络上传

- git push 在配置SSH密钥后，HTTPS通道和SSH通道都能用于上传，但HTTPS式和SSH式的仓库地址形式不一样

