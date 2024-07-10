<div align="center">
  
  <img alt="LOGO" src="https://raw.githubusercontent.com/WindowsNT351/CE-Collections-2.0/main/WNT/menu2.exe/res/about.bmp" /><br />
  # CE Collections -2.01-
  Windows CE 大合集<br />

</div>

### ONEW CE Collections
## ❓什么是CE Collections<br />
CE Collections集合了几乎所有的CE（CE内核基础的官方版本，如Windows Mobile）镜像和模拟器，包括使用Loadcepc引导的x86Bin镜像、使用DeviceEmulator引导的ArmBin镜像和适用于Windows NT的Shell模拟器。<br />

## 📥如何使用
**本镜像不止能在虚拟机里启动使用CEPCx86镜像，还能在Windows中打开MENU2.EXE,使用其他形式镜像/模拟器，两个平台（直接引导和Windows启动)能启动的Windows CE版本不同!!!!!** <br /><br />
将iso载入虚拟光驱或虚拟机里（推荐Windows XP SP3，因为Windows PPC 2000模拟器无法在NT6.x上运行）打开根目录下的WNT\MENU2.exe，选择想要开启的Windows CE版本，单击Run即可打开模拟器<br />
在引导早期的CE时，菜单可能会询问是否使用高分辨率模式，这是因为现代虚拟机无法正常模拟当时的显卡，导致早期CE只能用低分辨率模式，如果想使用高分辨率模式，请使用PCEM或者86Box等可以模拟当时的硬件的模拟器，推荐使用PCEM中的S3 ViRGE<br />

## ❓QA:
Q:从光盘引导后，选择Windows CE 2.x到Windows 3.x黑屏  A:分辨率可能选错了，在VMware等虚拟机中，需要使用低分辨率模式，另外，VMware新版本在模拟老系统时非常卡，也有可能导致黑屏。<br />
Q:我已经使用了PCem，但是依旧用不了高分辨率  A:在PCem中，需要选择S3 ViRGE显卡搭配性能大于等于486/33的CPU才能使用高分辨率。<br />
Q:PCem中，无法引导光盘  A:使用7Zip等压缩软件打开ISO文件，解压[boot]下的img文件，并从软盘引导。<br />
Q:关机怎么用不了  A:真就用不了。<br />
Q:在CEPC引导器中，选择完系统后显示Error image section doesn't fit in allocated block  A:内存太小了，调大内存。<br />
Q:Windows EC 2013引导后黑屏  A:创建虚拟机时不要使用VMware的DOS，换用Windows XP。<br />
Q:Windows Mobile分辨率太抽象了  A:换另一个分辨率选项试试。<br />
Q:Shell Emulator无法多个一起启动  A:不要多个Shell Emulator一起启动，若不小心一起启动，请注销或重启，并运行[CDROM]:\WNT\200HPC\ceu.reg。<br />
Q:Windows PPC 2000没法在Windows Vista及以上系统工作  A:没办法，Windows API不兼容，所以最好的体验Shell Emulator的系统是Windows XP。<br />
Q:为什么整体是英文的？  A:英文的镜像比较全，好Build，所以选了英文版镜像，菜单啥的适配镜像就全是英文的了<br />
Q:我的Windows CE/Mobile实体机不起动了，你能帮帮我吗  A:6<br />
Q:你能把其他Windows集成进来吗？  A:看看项目名啊啊啊啊啊<br />


## ❗以下是本项目集成的CE镜像和模拟器
绿色:支持  黄色：有资源，可集成，但未集成  浅黄色：有资源，技术不可集成  红色：无资源<br />
<img src="https://raw.githubusercontent.com/WindowsNT351/CE-Collections-2.0/main/cetl.png"/>

## ℹ关于
本项目由ONEW成员共同开发<br /><br />

开发团队：<br />
名字             Bilibili ID<br />
351              351Workshop<br />
Inter            -INTER_INIT-<br />
GoldApple        不务正业的金苹果<br />
WinPad           とある科学の手机副屏<br />
DZY              DZY20070614<br />
Zesa             LinuxMEMZ<br />
ONEW Studio QQ交流群:**981893945** 欢迎来玩<br /><br />

本项目使用了以下项目的代码/资源:<br />
CE：<br />
大多数镜像为自己用Platform Builder编译的<br />
少部分模拟器/镜像为HPCF版本或是M$的SDK提取<br />
还有少部分是HPCF版本<br />
x86CEPC Bootloader(LOADCEPC)/工具 为M$官方版6.0修改版<br /><br />

其他：<br />
DOS-LOGO: http://retro.timb.us/Documents/Software/DOS-LOGO.html<br />


## 🤝友情链接
[Inter的PenPointOS模拟器](https://github.com/Inter1006/PenPointOS_Vbox/tree/main)<br />
👆点击戳一下Inter<br />
[樱之谷-MC服务器](www.sakuravalley.xyz)<br />
👆点击戳一下Axium. 钰<br />
[宣传一下我自己的MC服务器（？](http://qm.qq.com/cgi-bin/qm/qr?_wv=1027&k=V8a6CPxKB1QwMpykpz9PVsMMDedITCqr&authKey=HMJy269CbqbjLatvD4QQG9Z3XrGGqEKUqD7LKciNWPvP1lVUX7LxXvIQWxX7yqYX&noverify=0&group_code=377904406)<br />
👆点击戳一下351(?<br />

<br /><br /><br /><br />
Copyright 351Workshop 2022-2024


