# Created by IBM WorkFrame/2 MakeMake at 10:49:13 on 06/26/94
#
# This makefile should be run in the following directory:
#   d:\work\c\pmformat
#
# The actions included in this makefile are:
#   BIND::Resource Bind
#   COMPILE::CLC C
#   COMPILE::Resource Compile
#   LINK::LINK386: Link

.all: \
  .\PMFMTGR.EXE

.SUFFIXES:

.SUFFIXES: .c .RC

.RC.res:
      @echo WF::COMPILE::Resource Compile
      rc.exe -r %s %|fF.RES

.c.obj:
      @echo WF::COMPILE::CLC C
      icc.exe /Ss /Q /Wcnviniparprorearet /Ti /Gm /G4 /C   %s

.asm.obj:
      @echo WF::COMPILE::MASM ASM
      masm %s, bootsgr /Dgerman /Zi

.\PMFMTGR.EXE: \
    .\bootsgr.obj \
    .\PMTRACK.obj \
    .\DISKTHRD.obj \
    .\DRVINFO.obj \
    .\FORMAT.obj \
    .\pmformat.obj \
    .\PMFMTGR.res \
    .\pmformat.def \
    PMFMTGR.MAK
      @echo WF::LINK::Link
      icc.exe @<<
        /B"/IG /PM:PM /de /nologo"
        /Fepmfmtgr.exe
        .\bootsgr.obj
        .\pmformat.obj
        .\pmtrack.obj
        .\diskthrd.obj
        .\drvinfo.obj
        .\format.obj
        cutil.lib
        pmformat.def
<<
      @echo WF::BIND::Resource Bind
      rc.exe .\PMFMTGR.res PMFMTGR.EXE


!include PMFMTGR.Dep
