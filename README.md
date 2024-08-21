<div align="center">
  
  <img alt="LOGO" src="https://raw.githubusercontent.com/WindowsNT351/CE-Collections-2.0/main/WNT/menu2.exe/res/about.bmp" /><br />
  # CE Collections -2.01-
  不止一个合集<br /><br />

  English version: https://github.com/WindowsNT351/CE-Collections/blob/main/README_EN.md
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
绿色:集成  蓝色：已集成，但有点问题  黄色：有资源，可集成，但未集成  浅黄色：有资源，技术不可集成  红色：无资源<br />
<img src="https://raw.githubusercontent.com/WindowsNT351/CE-Collections-2.0/main/cetl.png"/>

## ℹ关于
本项目由ONEW成员共同开发<br /><br />

<table>
  <tr>
    <td align="center"><a href="https://space.bilibili.com/484165196"><img src="https://github.com/Inter1006/Extensions/blob/main/1720663857759dcbe7c89c6455282b29bc8695211ad7924a0.jpg" width="150px;" alt=""/><br /><sub><b>351<br />351Workshop</b></sub></a><br /></td>
    <td align="center"><a href="https://space.bilibili.com/1756824708"><img src="https://github.com/Inter1006/PenPointOS_Vbox/blob/Readme_Files/b_fa517952f054ca8c99a234cc1b50b50b.jpg" width="150px;" alt=""/><br /><sub><b>Inter1006<br />INTER_INIT</b></sub></a><br /></td>
    <td align="center"><a href="https://space.bilibili.com/410046866"><img src="https://github.com/Inter1006/Extensions/blob/main/1720663903084330ee6855a3795b453f2ab6ded4863c9b08a.jpg" width="150px;" alt=""/><br /><sub><b>GoldApple<br />不务正业的金苹果</b></sub></a><br /></td>
    <td align="center"><a href="https://space.bilibili.com/648710692"><img src="https://github.com/Inter1006/Extensions/blob/main/1720663907859cac6209da2520c4e1c83c6a99fb65ae1bcb4.jpg" width="150px;" alt=""/><br /><sub><b>WinPad(御坂)<br />とある科学の手机副屏</b></sub></a><br /></td>
    <td align="center"><a href="https://space.bilibili.com/2057331843"><img src="https://github.com/Inter1006/Extensions/blob/main/17206639278647b179c13f807cbc2bf27b899725d34fc5c79.jpg" width="150px;" alt=""/><br /><sub><b>DZY<br />DZY20070614</b></sub></a><br /></td>
    <td align="center"><a href="https://space.bilibili.com/437201853"><img src="https://github.com/Inter1006/Extensions/blob/main/1720663947047a3c221d7c72c685e35b27b3fe6d41b6f8f93.jpg" width="150px;" alt=""/><br /><sub><b>Zesa<br />LinuxMEMZ</b></sub></a><br /></td>
  </tr>
  
</table>
<br />
ONEW Studio QQ交流群：981893945 欢迎来玩<br /><br />

本项目使用了以下项目的代码/资源:<br />
CE：<br />
大多数镜像为自己用Platform Builder编译的<br />
少部分模拟器/镜像为示例镜像版本或是M$的SDK提取<br />
x86CEPC Bootloader(LOADCEPC)/工具 为M$官方版6.0修改版<br />

其他：<br />
DOS-LOGO: http://retro.timb.us/Documents/Software/DOS-LOGO.html<br />

## ❗SDK及其他相关工具下载 [更新中]
### Windows CE 1.x
#### Windows CE 1.0x
[Microsoft Visual C++ For Windows CE Version 1.0](https://archive.org/details/msvcceu.100)<br />
<br />
### Windows CE 2.x
#### Windows CE 2.00
[Microsoft Handheld PC 2.0 Platform SDK](https://archive.org/details/wincesdk)<br />
#### Windows CE 2.01
Microsoft Palm-size PC 1.0 Platform SDK: [Programming Windows CE Disc](https://archive.org/details/mspress_programmingwindowsce_0970002166)<br />
Microsoft Auto PC 1.0 Platform SDK: [Windows CE Developers Conference DevCon 99 Conference CD](https://archive.org/details/windowscedevcon99conferencecd)<br />
#### Windows CE 2.10
[Microsoft Windows CE 2.10 Enhancement Pack for Windows CE Embedded Toolkit](https://www.betaarchive.com/database/view_release.php?uuid=0bef77bc-078b-468d-9ed0-18c51508948d)<br />
#### Windows CE 2.11
[Microsoft Windows CE 2.11 Platform Builder](https://archive.org/details/MicrosoftWindowsCEPlatformBuilder2.11Disc1.iso.7z)<br />
[Microsoft Palm-size PC 1.1 Platform SDK](https://web.archive.org/web/20051218093617/http://download.microsoft.com/download/wincepalm/pdasdk/1.1/WCE/CN/PPC_SDK(CHS).zip)<br />
Microsoft Palm-size PC 1.2 Platform SDK: [Windows CE Developers Conference DevCon 99 Conference CD](https://archive.org/details/windowscedevcon99conferencecd)<br />
Microsoft Handheld PC 3.0(HPC Pro) Platform SDK: [Windows CE Developers Conference DevCon 99 Conference CD](https://archive.org/details/windowscedevcon99conferencecd)<br />
#### Windows CE 2.12
[Microsoft Windows CE 2.12 Platform Builder](https://www.betaarchive.com/database/view_release.php?uuid=e73feaeb-c4dc-4915-9b16-1692c269d3b5)<br />
<br />
### Windows CE 3.x
#### Windows CE 3.00
[Microsoft Windows CE 3.0 Platform Builder(Part 1)](https://archive.org/details/ms-wince-pb30)<br />
[Microsoft Windows CE 3.0 Platform Builder(Part 2)](https://archive.org/details/ms-wince-pb30_5-11)<br />
[Microsoft Pocket PC 2000(Palm-size PC 3.0) Platform SDK(Backup)](https://pan.baidu.com/s/14ykCvhJfTmiYzloPs6ApRg) 提取码6zjx<br />
[Microsoft Handheld PC 2000 SDK](https://archive.org/details/hpc2ksdkusa)<br />
[Microsoft Smart Phone 2002 SDK](https://archive.org/download/windows-mobile-sdks-2023/smartphone_2002_sdk.exe)<br />
[Microsoft Pocket PC 2002 SDK](https://archive.org/download/windows-mobile-sdks-2023/ppc2002_sdk.exe)<br />
<br />
### Windows CE 4.x
#### Windows CE 4.00
[Microsoft Windows CE.net 4.0 Platform Builder Emulation Edition(Backup)](https://www.123pan.com/s/KDyhTd-zTPN3) 提取码:eGCt<br />
[Microsoft Windows CE.net 4.0 Platform Builder](https://www.betaarchive.com/database/view_release.php?uuid=51000b20-9a15-4403-8b56-c3759aa682b7)<br />
#### Windows CE 4.10
[Microsoft Windows CE.net 4.1 Platform Builder Emulation Edition](https://archive.org/details/winceemul41)<br />
[Microsoft Windows CE.net 4.1 Platform Builder](https://www.betaarchive.com/database/view_release.php?uuid=fc6d66f5-e047-4303-9f0e-82e7f1de6645)<br />
#### Windows CE 4.20
[Microsoft Windows CE.net 4.2 Platform Builder Emulation Edition](https://archive.org/details/winceemul42)<br />
[Microsoft Windows CE.net 4.2 Platform Builder](https://www.betaarchive.com/database/view_release.php?uuid=2b657ca7-2b53-4aac-8b1e-446b5669fda9)<br />
[Microsoft Windows Mobile 2003 SDK](https://archive.org/download/WMSDK/Microsoft%20Pocket%20PC%202003%20SDK.msi)<br />
#### Windows CE 4.21
[Microsoft Windows Mobile 2003 SE Images](https://archive.org/download/WMSDK/WM2003SEimages.msi)<br />
<br />
### Windows CE 5.x
#### Windows CE 5.00
[Microsoft Windows CE.net 5.0 Platform Builder](https://archive.org/details/en_win_ce_net_cd1)<br />
#### Windows CE 5.10
[Microsoft Windows Mobile 5.0 SDK](https://archive.org/download/WMSDK/Windows%20Mobile%205.0%20Pocket%20PC%20SDK.msi)<br />
#### Windows CE 5.20
[Microsoft Windows Mobile 5.0 Pocket PC SDK](https://archive.org/download/WMSDK/Windows%20Mobile%205.0%20Pocket%20PC%20SDK.msi)<br />
[Microsoft Windows Mobile 6 Professional SDK](https://archive.org/download/WMSDK/Windows%20Mobile%206%20Professional%20SDK%20Refresh.msi)<br />
[Microsoft Windows Mobile 6 Standard SDK](https://archive.org/download/WMSDK/Windows%20Mobile%206%20Standard%20SDK%20Refresh.msi)<br />
[Microsoft Windows Mobile 6.1.4 Professional Images](https://archive.org/download/WMSDK/Windows%20Mobile%206.1.4%20Professional%20Images%20%28USA%29.msi)<br />
[Microsoft Windows Mobile 6.1.4 Standard Images](https://archive.org/download/WMSDK/Windows%20Mobile%206.1.4%20Standard%20Images%20%28USA%29.msi)<br />
[Microsoft Windows Mobile 6.5 Professional DKT](https://archive.org/download/WMSDK/Windows%20Mobile%206.5%20Professional%20Developer%20Tool%20Kit%20%28USA%29.msi)<br />
[Microsoft Windows Mobile 6.5 Standard DKT](https://archive.org/download/WMSDK/Windows%20Mobile%206.5%20Standard%20Developer%20Tool%20Kit%20%28USA%29.msi)<br />
[Microsoft Windows Mobile 6.5.3 Professional DKT](https://archive.org/download/WMSDK/Windows%20Mobile%206.5.3%20Professional%20DTK.msi)<br />
[Microsoft Windows Mobile 6.5.3 Standard DKT](https://archive.org/download/WMSDK/Windows%20Mobile%206.5.3%20Standard%20DTK.msi)<br />
<br />


## 🤝友情链接
[Inter的PenPointOS模拟器](https://github.com/Inter1006/PenPointOS_Vbox/tree/main)<br />
👆点击戳一下Inter<br />
[樱之谷-MC服务器](www.sakuravalley.xyz)<br />
👆点击戳一下Axium. 钰<br />

<br /><br /><br /><br />
Copyright 351Workshop 2022-2024


