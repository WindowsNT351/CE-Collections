//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//

Build instructions for loadcepc.exe
-----------------------------------

The build process for "loadcepc" requires a 16 bit compiler. Because 
of this, its not built in a Windows CE build window, but rather using 
a standard cmd.exe window. The location of the compiler is determined 
by the batch file setupen2.bat. It searches in different locations for the 
appropriate version. The assembler (ml) for the single asm file is accessed 
from the _WINCEROOT\sdk\bin\i386 directory. If the compiler cannot be 
located or _WINCEROOT environment variable is not set, an error is displayed 
when setting up the environment.

The procedure to build loadcepc.exe is as follows:

        - Launch a basic cmd window (cmd.exe). 
          
        Execute following commands in this window:

        - Set the _WINCEROOT environment variable:

                X:>set _WINCEROOT=<your wince root here>

        - Execute the batch file setupenv.bat:

                X:>setupenv.bat
        
        - Execute the make file using nmake:

                x:>nmake /F loadcepc.mak 

This will build a loadcepc.exe in this directory. The warning 
messages may be safely ignored.

To remove files used in the build process the "clean" target 
is available, as follows:

                X:>nmake /F loadcepc.mak clean
