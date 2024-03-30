# Microsoft Visual C++ generated build script - Do not modify

PROJ = LOADCEPC
DEBUG = 0
PROGTYPE = 6
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = d:\WINCE\PLATFORM\CEPC\LOADCEPC\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = MAIN.C      
FIRSTCPP =             
RC = rc
CFLAGS_D_DEXE = /I . /nologo /G2 /W4 /Z7 /Od /D "_DEBUG" /D "_DOS" /Fc /FR 
CFLAGS_R_DEXE = /I . /nologo /Gs /G2 /W4 /Z7 /Ox /D "NDEBUG" /D "_DOS" /Fc /FR 
LFLAGS_D_DEXE = /NOLOGO /NOI /STACK:5120 /ONERROR:NOEXE /CO /MAP /LINE 
LFLAGS_R_DEXE = /NOLOGO /NOI /STACK:5120 /ONERROR:NOEXE /CO /MAP /LINE 
LIBS_D_DEXE = slibce  oldnames
LIBS_R_DEXE = slibce  oldnames
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
OBJS_EXT = TRANSFER.OBJ 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_DEXE)
LFLAGS = $(LFLAGS_D_DEXE)
LIBS = $(LIBS_D_DEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_DEXE)
LFLAGS = $(LFLAGS_R_DEXE)
LIBS = $(LIBS_R_DEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = MAIN.SBR \
		XMSAPI.SBR \
		VIDEO.SBR \
	PPFS.SBR


TRANSFER_DEP = 

MAIN_DEP = d:\wince\platform\cepc\loadcepc\loadcepc.h \
	d:\wince\platform\cepc\loadcepc\xmsapi.h

XMSAPI_DEP = d:\wince\platform\cepc\loadcepc\xmsapi.h


VIDEO_DEP = d:\wince\platform\cepc\loadcepc\loadcepc.h \
	d:\wince\platform\cepc\loadcepc\video.h

MDPPFS_DEP = d:\wince\platform\cepc\ppfstool\wdm.h \
	d:\wince\platform\cepc\ppfstool\pc.h


PPFS_DEP = d:\wince\platform\cepc\ppfstool\kernel.h


PPFSTOOL_DEP = d:\wince\platform\cepc\ppfstool\ppfs.h

ETHMAIN_DEP = d:\wince\platform\cepc\loadcepc\loadcepc.h \
	d:\wince\platform\cepc\loadcepc\smchw.h

all:    $(PROJ).EXE $(PROJ).BSC

MAIN.OBJ:       MAIN.C $(MAIN_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c MAIN.C

XMSAPI.OBJ:     XMSAPI.C $(XMSAPI_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c XMSAPI.C

VIDEO.OBJ:      VIDEO.C $(VIDEO_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c VIDEO.C

MDPPFS.OBJ:     MDPPFS.C $(MDPPFS_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c MDPPFS.C

PPFS.OBJ:       PPFS.C $(PPFS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c PPFS.C

DEBUG.OBJ:       DEBUG.C $(PPFS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c DEBUG.C

PPFSTOOL.OBJ:   PPFSTOOL.C $(PPFSTOOL_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c PPFSTOOL.C

ETHMAIN.OBJ:   ETHMAIN.C $(ETHMAIN_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ETHMAIN.C

$(PROJ).EXE::   MAIN.OBJ XMSAPI.OBJ VIDEO.OBJ MDPPFS.OBJ DEBUG.OBJ PPFS.OBJ PPFSTOOL.OBJ ETHMAIN.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
MAIN.OBJ +
XMSAPI.OBJ +
VIDEO.OBJ +
MDPPFS.OBJ +
PPFS.OBJ +
PPFSTOOL.OBJ +
ETHMAIN.OBJ +
DEBUG.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSc: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
