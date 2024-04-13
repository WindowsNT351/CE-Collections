### CE Collections 2.0
## 什么是CE Collections 2.0<br />
CE Collections 2.0集合了几乎所有的CE镜像和模拟器，包括使用Loadcepc引导的x86Bin镜像、使用DeviceEmulator引导的ArmBin镜像和适用于Windows NT的Shell模拟器。<br />
## 如何使用
将iso载入虚拟光驱或虚拟机里（推荐Windows XP SP3，因为Windows PPC 2000模拟器无法在NT6.x上运行）打开根目录下的WNT\MENU.exe，选择想要开启的Windows CE版本，单击Run即可打开模拟器<br />
注意，在这里只能打开以DeviceEmulator或ShellEmulator为基础的模拟器，若想引导x86Loadcepc，请在虚拟机内引导iso，进入到引导菜单后，选择想要的版本（用鼠标单击或使用键盘）。<br />
在引导早期的CE时，菜单可能会询问是否使用高分辨率模式，这是因为现代虚拟机无法正常模拟当时的显卡，导致早期CE只能用低分辨率模式，如果想使用高分辨率模式，请使用PCEM或者86Box等可以模拟当时的硬件的模拟器，推荐使用PCEM中的S3 ViRGE<br />

## 以下是本项目集成的CE镜像和模拟器
CEPC:<br />
? Windows CE 2.11<br />
√ Windows CE 2.12<br />
√ Windows CE 3.0<br />
√ Windows CE 4.0(Thin Client)<br />
√ Windows CE 4.1(Thin Client)<br />
√ Windows CE 4.2(Thin Client)<br />
√ Windows CE 5(Thin Client,IP Phone)<br />
√ Windows CE 6 [R3](Thin Client,IP Phone)<br />
√ Windows EC 7(Thin Client,Silverlight)<br />
√ Windows EC 2013(Thin Client,Silverlight)<br />
√ Windows HPC 2000<br />
√ Windows Mobile 2002 PPC<br />
√ Windows Mobile 2002 SP<br />
√ Windows Mobile 2003 PPC<br />
√ Windows Mobile 2003 SP<br />
√ Windows Mobile 2003 PPC SE<br />
√ Windows Mobile 2003 SP SE<br />
<br />
ARM Emulator:<br />
√ Windows Mobile 5.0 PPC<br />
√ Windows Mobile 5.0 SP<br />
√ Windows Mobile 6.0 PPC<br />
√ Windows Mobile 6.0 SP<br />
√ Windows Mobile 6.1 PPC<br />
√ Windows Mobile 6.1 SP<br />
√ Windows Mobile 6.5 PPC<br />
√ Windows Mobile 6.5 SP<br />
<br />
Shell Emulator:<br />
√ Windows CE 1.0 Beta 2<br />
√ Windows CE 1.0 Beta 3<br />
√ Windows CE 1.0 RTM<br />
√ Windows CE 2.00<br />
√ Windows CE 2.11 Palm<br />
√ Windows CE HPC 3<br />
√ Windows PPC 2000<br />
