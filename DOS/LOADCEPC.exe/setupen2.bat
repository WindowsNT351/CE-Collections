@REM
@REM Copyright (c) Microsoft Corporation.  All rights reserved.
@REM
@REM
@REM Use of this sample source code is subject to the terms of the Microsoft
@REM license agreement under which you licensed this sample source code. If
@REM you did not accept the terms of the license agreement, you are not
@REM authorized to use this sample source code. For the terms of the license,
@REM please see the license agreement between you and Microsoft or, if applicable,
@REM see the LICENSE.RTF on your install media or the root of your tools installation.
@REM THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES.
@REM

@ECHO OFF

@REM Note: Please read the README.TXT in this directory for build instructions
@REM for loadcepc. 
@REM
@REM The environment variable WINCEROOT must be set. The assembler
@REM "ml" used for TRANSFER.ASM requires this program. 

if "%_WINCEROOT%"=="" goto ERROR2

PROMPT *** 16-bit dev mode - type exit to return to normal ***$_%prompt%

@REM Locate the 16 bit dev tools. Try local drive, then Windows CE tree
@REM and finally, the \\boneyard share.

if not exist %HOMEDRIVE%\msvc goto ENLIST_PATH
set __MSVCDIR=%HOMEDRIVE%\msvc
goto PATH_DONE

:ENLIST_PATH
if not exist %_WINCEROOT%\sdk\vc150.win goto SHARE_PATH
set __MSVCDIR=%_WINCEROOT%\sdk\vc150.win
goto PATH_DONE

:SHARE_PATH
if not exist \\boneyard\rellang\vc150.win\msvc goto ERROR
set __MSVCDIR=\\boneyard\rellang\vc150.win\msvc

@REM Setup PATH, DevDir, LIB, and INCLUDE locations.

:PATH_DONE
set PATH=%__MSVCDIR%\bin;%PATH%;%_WINCEROOT%\sdk\bin\i386;C:\MASM\BIN
set MSDevDir=%__MSVCDIR%
set LIB=%__MSVCDIR%\lib
set INCLUDE=%__MSVCDIR%\include
goto DONE

:ERROR
echo ERROR: vc150.win path NOT found.
goto DONE

:ERROR2
echo ERROR: WINCEROOT must be set.

:DONE
command