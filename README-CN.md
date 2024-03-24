### CE Collections 2.0
## 什么是CE Collections 2.0
CE Collections 2.0集合了几乎所有的CE镜像和模拟器，包括使用Loadcepc引导的x86Bin镜像、使用DeviceEmulator引导的ArmBin镜像和适用于Windows NT的Shell模拟器。
## 如何使用
将iso载入虚拟光驱或虚拟机里（推荐Windows XP SP3，因为Windows PPC 2000模拟器无法在NT6.x上运行）打开根目录下的WNT\MENU.exe，选择想要开启的Windows CE版本，单击Run即可打开模拟器
注意，在这里只能打开以DeviceEmulator或ShellEmulator为基础的模拟器，若想引导x86Loadcepc，请在虚拟机内引导iso，进入到引导菜单后，选择想要的版本（用鼠标单击或使用键盘）。
在引导早期的CE时，菜单可能会询问是否使用高分辨率模式，这是因为现代虚拟机无法正常模拟当时的显卡，导致早期CE只能用低分辨率模式，如果想使用高分辨率模式，请使用PCEM或者86Box等可以模拟当时的硬件的模拟器，推荐使用PCEM中的S3 ViRGE

以下是本项目继承的CE镜像和模拟器
CEPC:
? Windows CE 2.11
√ Windows CE 2.12
√ Windows CE 3.0
√ Windows CE 4.0(Thin Client)
√ Windows CE 4.1(Thin Client)
√ Windows CE 4.2(Thin Client)
√ Windows CE 5(Thin Client,IP Phone)
√ Windows CE 6 R3(Thin Client,IP Phone)
√ Windows EC 7(Thin Client,Silverlight)
√ Windows EC 2013(Thin Client,Silverlight)
√ Windows HPC 2000
√ Windows Mobile 2002 PPC - CN
√ Windows Mobile 2002 SP
√ Windows Mobile 2003 PPC
√ Windows Mobile 2003 SP - CN
√ Windows Mobile 2003 PPC SE
√ Windows Mobile 2003 SP SE - CN

ARM Emulator:
√ Windows Mobile 5 PPC - CN
√ Windows Mobile 6.0 SP
√ Windows Mobile 6.1 PPC
√ Windows Mobile 6.5 PPC - CN

Shell Emulator:
√ Windows CE 1.0 Beta 2
√ Windows CE 1.0 Beta 3
√ Windows CE 1.0 RTM
√ Windows CE 2.00
√ Windows CE 2.11 Plam
√ Windows CE HPC 3
√ Windows PPC 2000
