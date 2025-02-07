<div align="center">
  <img alt="LOGO" src="https://raw.githubusercontent.com/WindowsNT351/CE-Collections-2.0/main/WNT/menu2.exe/res/about.bmp" /><br />
  <h1>CE Collections -2.01-</h1>
  <p>不止一个合集</p>
  <p>
    English version: <a href="https://github.com/WindowsNT351/CE-Collections/blob/main/README_EN.md">https://github.com/WindowsNT351/CE-Collections/blob/main/README_EN.md</a>
  </p>
</div>

---

### ONEW CE Collections

## ❓ 什么是 CE Collections
CE Collections 集合了几乎所有基于 CE 内核的官方版本（例如 Windows CE Core,Windows Mobile）的镜像和模拟器。内容包括：
- 使用 Loadcepc 引导的 x86Bin 镜像
- 使用 DeviceEmulator 引导的 ArmBin 镜像
- 适用于 Windows NT 的 Shell 模拟器

## 📥 如何使用
**本镜像不仅可以在虚拟机中启动使用 CEPC x86 镜像，还可以在 Windows 中直接运行 MENU2.EXE，从而启动其他形式的镜像/模拟器。两种平台（直接引导及在 Windows 中启动）所支持的 Windows CE 版本各不相同！**

WinNT平台模拟器使用方法：
1. 将 ISO 文件加载至虚拟光驱或虚拟机中（推荐使用 Windows XP SP3，因为 Windows PPC 2000 模拟器无法在 NT6.x 上运行）。
2. 打开 ISO 根目录下的 WNT\MENU2.exe。
3. 选择想要启动的 Windows CE 版本，并点击 Run 打开对应的模拟器。

CEPC平台使用方法：
1. 将ISO文件加载至虚拟机或模拟器中，推荐使用86Box。
2. 打开虚拟机，引导光盘。
3. 选择想要启动的 Windows CE 版本。
4. 若提示需要选择分辨率，请依照您使用的模拟器、虚拟机或实体硬件选择。

注意：
- 在引导较早版本的 CE 时，菜单可能会询问是否使用高分辨率模式。这是因为现代虚拟机无法正常模拟当时的显卡，导致早期 CE 系统只能以低分辨率模式运行。若希望使用高分辨率模式，请使用 PCEM 或 86Box 等能够模拟当时硬件的模拟器（推荐使用 86Box 中的 S3 ViRGE）。

## ❓ QA
Q: 从光盘引导后，选择 Windows CE 2.x 到 Windows 3.x 黑屏  
A: 分辨率可能选错了。在 VMware 等虚拟机中，需要使用低分辨率模式；另外，VMware 新版本在模拟老系统时较为卡顿，也有可能导致黑屏。

Q: 我已经使用了 86Box，但依然无法使用高分辨率  
A: 在 PCem 中，需要选择 S3 ViRGE 显卡，并搭配性能大于等于 486/33 的 CPU，才能使用高分辨率。

Q: 在 86Box 中无法引导光盘  
A: 请使用 7Zip 等压缩软件打开 ISO 文件，解压 [boot] 文件夹中的 img 文件，然后从软盘引导。

Q: 关机功能为何无法正常使用？  
A: 真就用不了。

Q: 在 CEPC 引导器中选择完系统后显示 “Error image section doesn't fit in allocated block”  
A: 内存可能设定过小，请适当调大内存。

Q: Windows EC 2013 引导后黑屏  
A: 创建虚拟机时请不要使用 VMware 的 DOS，而应使用 Windows XP。

Q: Windows Mobile 分辨率选项让人感觉莫名其妙  
A: 请尝试其他分辨率选项。

Q: Shell Emulator 无法同时启动多个实例  
A: 请不要同时启动多个 Shell Emulator 实例；若不小心同时启动了，请注销或重启，然后运行 “[CDROM]:\WNT\200HPC\ceu.reg”。

Q: Windows PPC 2000 无法在 Windows Vista 及以上系统上使用  
A: 由于 Windows API 不兼容，所以最佳的 Shell Emulator 体验平台是 Windows XP。

Q: 为什么整体内容是英文的？  
A: 因为英文镜像资源较全，构建也较完善，所以选择了英文版镜像；菜单等内容也相应采用英文格式。

Q: 我的 Windows CE/Mobile 实体机无法启动，能帮忙解决吗？ / 能帮我安装一下这个WinCE软件吗？  
A: 无法回答。

Q: 你能把其他 Windows 系统集成进来吗？  
A: 请看项目名称——答案自明。


## ❗ 本项目集成的 CE 镜像和模拟器

以下颜色标识对应不同状态：
- 🟢绿色：已集成
- 🔵蓝色：已集成（但存在一些问题）
- 🟡黄色：有资源，可集成，但尚未集成
- 🔴红色：无资源

<img src="https://raw.githubusercontent.com/WindowsNT351/CE-Collections/main/cetl.png"/>


## ℹ 关于
本项目由 ONEW 成员共同开发

<table>
  <tr>
    <td align="center">
      <a href="https://space.bilibili.com/484165196">
        <img src="https://github.com/Inter1006/Extensions/blob/main/1720663857759dcbe7c89c6455282b29bc8695211ad7924a0.jpg" width="150px;" alt=""/>
        <br /><sub><b>351<br />351Workshop</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://space.bilibili.com/1756824708">
        <img src="https://github.com/Inter1006/PenPointOS_Vbox/blob/Readme_Files/b_fa517952f054ca8c99a234cc1b50b50b.jpg" width="150px;" alt=""/>
        <br /><sub><b>Inter1006<br />INTER_INIT</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://space.bilibili.com/410046866">
        <img src="https://github.com/Inter1006/Extensions/blob/main/1720663903084330ee6855a3795b453f2ab6ded4863c9b08a.jpg" width="150px;" alt=""/>
        <br /><sub><b>GoldApple<br />不务正业的金苹果</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://space.bilibili.com/648710692">
        <img src="https://github.com/Inter1006/Extensions/blob/main/1720663907859cac6209da2520c4e1c83c6a99fb65ae1bcb4.jpg" width="150px;" alt=""/>
        <br /><sub><b>WinPad(御坂)<br />とある科学の手机副屏</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://space.bilibili.com/2057331843">
        <img src="https://github.com/Inter1006/Extensions/blob/main/17206639278647b179c13f807cbc2bf27b899725d34fc5c79.jpg" width="150px;" alt=""/>
        <br /><sub><b>DZY<br />DZY20070614</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://space.bilibili.com/437201853">
        <img src="https://github.com/Inter1006/Extensions/blob/main/1720663947047a3c221d7c72c685e35b27b3fe6d41b6f8f93.jpg" width="150px;" alt=""/>
        <br /><sub><b>Zesa<br />LinuxMEMZ</b></sub>
      </a>
    </td>
  </tr>
</table>

<p align="center">
  ONEW Studio QQ 群：981893945 —— 欢迎加入！
</p>


本项目采用了以下项目的代码/资源：

CE：
- 大部分镜像由我们使用 Platform Builder 编译而成。
- 少部分模拟器/镜像为示例版本或直接从 M$ SDK 中提取。
- x86CEPC Bootloader (LOADCEPC) 及工具为 M$ 官方版 6.0 修改版。

其他：
- DOS-LOGO: http://retro.timb.us/Documents/Software/DOS-LOGO.html

## ❗SDK及其他相关工具下载
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
[Microsoft Windows Mobile 6 Professional SDK](https://archive.org/download/WMSDK/Windows%20Mobile%206%20Professional%20SDK%20Refresh.msi)<br />
[Microsoft Windows Mobile 6 Standard SDK](https://archive.org/download/WMSDK/Windows%20Mobile%206%20Standard%20SDK%20Refresh.msi)<br />
[Microsoft Windows Mobile 6.1 Standard Images](https://web.archive.org/web/20130330025109/http://download.microsoft.com/download/d/c/7/dc73c8e7-f9c3-49de-821f-5588eeefcac4/Windows%20Mobile%206.1%20Standard%20Images%20(USA).msi)<br />
[Microsoft Windows Mobile 6.1 Professional Images](https://web.archive.org/web/20130330025531/http://download.microsoft.com/download/d/c/7/dc73c8e7-f9c3-49de-821f-5588eeefcac4/Windows%20Mobile%206.1%20Professional%20Images%20(USA).msi)<br />
[Microsoft Windows Mobile 6.1.4 Professional Images](https://archive.org/download/WMSDK/Windows%20Mobile%206.1.4%20Professional%20Images%20%28USA%29.msi)<br />
[Microsoft Windows Mobile 6.1.4 Standard Images](https://archive.org/download/WMSDK/Windows%20Mobile%206.1.4%20Standard%20Images%20%28USA%29.msi)<br />
[Microsoft Windows Mobile 6.5 Professional DKT](https://archive.org/download/WMSDK/Windows%20Mobile%206.5%20Professional%20Developer%20Tool%20Kit%20%28USA%29.msi)<br />
[Microsoft Windows Mobile 6.5 Standard DKT](https://archive.org/download/WMSDK/Windows%20Mobile%206.5%20Standard%20Developer%20Tool%20Kit%20%28USA%29.msi)<br />
[Microsoft Windows Mobile 6.5.3 Professional DKT](https://archive.org/download/WMSDK/Windows%20Mobile%206.5.3%20Professional%20DTK.msi)<br />
[Microsoft Windows Mobile 6.5.3 Standard DKT](https://archive.org/download/WMSDK/Windows%20Mobile%206.5.3%20Standard%20DTK.msi)<br />
<br />
### Windows CE 6.x
#### Windows CE 6.00
[Microsoft Windows Embedded CE 6.0 Platform Builder](https://archive.org/details/en_windows_embedded_ce_6.0_dvd)<br />
[Microsoft Windows Embedded CE 6.0 Platform Builder R2](https://archive.org/details/windows-embedded-ce-6.0-r2)<br />
[Microsoft Windows Embedded CE 6.0 Platform Builder R3](https://archive.org/details/en_windows_embedded_ce_6.0_r3_dvd__x16-28730)<br />
<br />
### Windows CE 7.x
#### Windows CE 7.00
[Microsoft Windows Embedded Compact 7.0 Platform Builder](https://archive.org/details/WindowsEmbeddedCompact7)<br />
[Microsoft Windows Embedded Automotive 7.0 Platform Builder](https://archive.org/details/windows-embedded-automotive-7)<br />
<br />
### Windows CE 8.x
#### Windows CE 8.00
[Microsoft Windows Embedded Compact 2013 Platform Builder](https://archive.org/details/en_windows_embedded_compact_2013_x86_x64_dvd_2247108)

## 🤝友情链接
[WinPad的CEShellCollection](https://github.com/WinPad1993/Windows_CE_Win32_Shell_Emulator_Collection_in_a_WindowsPE)<br />
👆点击戳一下WinPad<br />
[Inter的PenPointOS模拟器](https://github.com/Inter1006/PenPointOS_Vbox/tree/main)<br />
👆点击戳一下Inter<br />

<br /><br /><br /><br />
Copyright 351Workshop 2022-2024


