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
Q: Windows Mobile resolution is too abstract. A: Try changing to another resolution option<br />
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

<br /><br /><br /><br />
Copyright 351Workshop 2022-2024
