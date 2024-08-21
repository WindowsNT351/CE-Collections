<div align="center">
  
  <img alt="LOGO" src="https://raw.githubusercontent.com/WindowsNT351/CE-Collections-2.0/main/WNT/menu2.exe/res/about.bmp" /><br />
  # CE Collections -2.01-
  More than one<br />

</div>

### ONEW CE Collections
## ❓What is CE Collections<br />
CE Collections integrates almost all CEs(official versions of CEs, such as Windows Mobile) images and emulators, including x86 Bin images booted by Loadcepc, Arm Bin images booted by Device Emulator, and Shell emulators for Windows NT.<br />

## 📥How to use
**This image can not only boot and use the CEPCx86 image in a virtual machine,but also,you can use MENU2.EXE in Windows and use other forms of images/emulators.The two platforms (direct boot and MENU2 in Windows) can boot different versions of Windows CE!!!!!** <br /><br />
Load ISO into a virtual optical drive or virtual machine (Windows XP SP3 is recommended as the Windows PPC 2000 emulator cannot run on NT6. x).Open [CDROM]:\WNT\MENU2.exe,select the Windows CE version you want to open,and click Run to open the emulator.Or just boot it directly and choose the system you want to boot.<br />
When booting early CEs in CEPC Boot Loader,the menu may ask whether to use high-resolution mode,because modern virtual machines cannot emulate graphics cards properly,resulting in early CE only being able to use low resolution mode.If you want to use high-resolution mode,please use emulators such as PCEM or 86Box that can emulate hardware at that old time. It is recommended to use S3 ViRGE in PCEM<br />

## ❓QA:
Q: After booting from the CD, select Windows CE version from 2.x to Windows 3.x and I got a black screen!   A: The resolution may be selected incorrectly. In virtual machines such as VMware, low resolution mode is required. In addition, the new version of VMware is very slow when simulating old systems, which may also cause black screens<br />
Q: I have already used PCem, but still cannot use high-resolution A: In PCem, you need to choose an S3 ViRGE graphics card with a CPU greater than or equal to 486/33 to use high-resolution<br />
Q: In PCem, CD cannot be booted. A: Use compression software such as 7Zip to open the ISO file, extract the img file under [boot], and boot from the floppy disk<br />
Q: Why can't I use the shutdown in the CEPC? A: You really can't use it<br />
Q: In the CEPC bootloader, after selecting the system, the Error image section does not fit in allocated block showed A: The memory is too small, increase the memory<br />
Q: Windows EC 2013 black screen after booting A: Do not use VMware [DOS] option when creating a virtual machine, switch to [Windows XP] option<br />
Q: Windows Mobile resolution is too strange. A: Try changing to another resolution option<br />
Q: Shell Simulators cannot be started together A: Do not start multiple Shell Simulators together. If you accidentally start them together, please log off or restart and run [CDROM]:\WNT\200HPC\CEU.reg<br />
Q: Windows PPC 2000 Shell Emulator cannot work on Windows Vista and above systems. A: No way, the Windows API is not compatible, so the best system to experience Shell emulator is Windows XP<br />
Q: My Windows CE or Mobile machine is not starting, can you help me? A: Well....<br />
Q: Can you integrate other Windows into it? A: Look at the project name!!!!!!<br />


## ❗Here is the systems that CEC can emulated
Green: Supports<br />
Blue: Supports,but has problem<br />
Yellow: Has resources, can be integrated, but not integrated<br />
Light yellow: Has resources, cannot be integrated<br />
Red: No resources<br />
<img src="https://raw.githubusercontent.com/WindowsNT351/CE-Collections-2.0/main/cetl.png"/>

## ℹAbout
This project is developed by ONEW members:<br /><br />

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

This project utilized the code and resources of the following projects:<br />
CE:<br />
Most images are compiled by myself using Platform Builder.<br />
A small number of emulators and images are sample image versions or official SDK extracts.<br />
x86CEPC Bootloader(LOADCEPC)/utilities are moded from the code comes from official PB 6.0.<br />
Others:<br />
DOS-LOGO: http://retro.timb.us/Documents/Software/DOS-LOGO.html<br />


## 📥SDK and others download
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
[Microsoft Pocket PC 2000(Palm-size PC 3.0) Platform SDK(Backup)](https://pan.baidu.com/s/14ykCvhJfTmiYzloPs6ApRg) Key:6zjx<br />
[Microsoft Handheld PC 2000 SDK](https://archive.org/details/hpc2ksdkusa)<br />
[Microsoft Smart Phone 2002 SDK](https://archive.org/download/windows-mobile-sdks-2023/smartphone_2002_sdk.exe)<br />
[Microsoft Pocket PC 2002 SDK](https://archive.org/download/windows-mobile-sdks-2023/ppc2002_sdk.exe)<br />
<br />
### Windows CE 4.x
#### Windows CE 4.00
[Microsoft Windows CE.net 4.0 Platform Builder Emulation Edition(Backup)](https://www.123pan.com/s/KDyhTd-zTPN3) Key:eGCt<br />
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
[Microsoft Windows Mobile 6.1.4 Professional Images](https://archive.org/download/WMSDK/Windows%20Mobile%206.1.4%20Professional%20Images%20%28USA%29.msi)<br />
[Microsoft Windows Mobile 6.1.4 Standard Images](https://archive.org/download/WMSDK/Windows%20Mobile%206.1.4%20Standard%20Images%20%28USA%29.msi)<br />
[Microsoft Windows Mobile 6.5 Professional DKT](https://archive.org/download/WMSDK/Windows%20Mobile%206.5%20Professional%20Developer%20Tool%20Kit%20%28USA%29.msi)<br />
[Microsoft Windows Mobile 6.5 Standard DKT](https://archive.org/download/WMSDK/Windows%20Mobile%206.5%20Standard%20Developer%20Tool%20Kit%20%28USA%29.msi)<br />
[Microsoft Windows Mobile 6.5.3 Professional DKT](https://archive.org/download/WMSDK/Windows%20Mobile%206.5.3%20Professional%20DTK.msi)<br />
[Microsoft Windows Mobile 6.5.3 Standard DKT](https://archive.org/download/WMSDK/Windows%20Mobile%206.5.3%20Standard%20DTK.msi)<br />
<br />
### Windows CE 6.x
#### Windows CE 6.0
[Microsoft Windows Embedded CE 6.0 Platform Builder](https://archive.org/details/en_windows_embedded_ce_6.0_dvd)<br />
[Microsoft Windows Embedded CE 6.0 Platform Builder R2](https://archive.org/details/windows-embedded-ce-6.0-r2)<br />
[Microsoft Windows Embedded CE 6.0 Platform Builder R3](https://archive.org/details/en_windows_embedded_ce_6.0_r3_dvd__x16-28730)<br />
<br />
### Windows CE 7.x
#### Windows CE 7.0
[Microsoft Windows Embedded Compact 7.0 Platform Builder](https://archive.org/details/WindowsEmbeddedCompact7)<br />
[Microsoft Windows Embedded Automotive 7.0 Platform Builder](https://archive.org/details/windows-embedded-automotive-7)<br />
<br />
### Windows CE 8.x
#### Windows CE 8.0
[Microsoft Windows Embedded Compact 2013 Platform Builder](https://archive.org/details/en_windows_embedded_compact_2013_x86_x64_dvd_2247108)

<br /><br /><br /><br />
Copyright 351Workshop 2022-2024
