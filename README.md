# 基于CH552T的数码时钟

### 介绍
 - 8051单片机驱动OLED12864显示
 - 使用主控芯片为CH552T，基于开发板CH552T Super Mini
 - OLED采用SSD1306显示驱动
 - I2C总线协议(软件I2C)
### 使用说明
1. 下载工程（也可以下载release的压缩包后解压），使用keil打开KeilFile目录下的OLED.uvproj。
2. 编译此工程。
3. 下载好WCHISPStudio官方软件。
4. 打开软件，勾选“连接后自动下载”，定位“目标程序文件1”到编译好的KeilFile/Objects/OLED.hex文件处。
5. 按住开发板上的P31按钮（或将P31引脚短接到地），不要松手以进入下载模式。
6. 插入USB线连接到电脑，提示正在下载程序时，可以松手。
7. 等待程序烧录完毕，会自动运行。
### 有什么问题可以留言，第一次用github不太会用
