# Created by IBM WorkFrame/2 MakeMake at 13:27:27 on 04/02/94
#
# This makefile should be run in the following directory:
#   d:\work\c\pmformat
#
# The actions included in this makefile are:
#   BIND::Resource Bind
#   COMPILE::CLC C
#   COMPILE::Resource Compile
#   LINK::Link

.all: \
  .\pmfmtus.exe

.SUFFIXES:

.SUFFIXES: .c .RC

.RC.res:
      @echo WF::COMPILE::Resource Compile
      rc.exe -r %s %|fF.RES

.c.obj:
      @echo WF::COMPILE::CLC C
      icc.exe /Id:\work\c\cutil /Ss /Q /Wcnviniparprorearet /Ti /Gm /G4 /C %s

.\pmfmtus.exe: \
    d:\work\c\pmformat\bootsus.obj \
    .\pmformat.obj \
    .\PMTRACK.obj \
    .\DISKTHRD.obj \
    .\DRVINFO.obj \
    .\FORMAT.obj \
    .\PMFMTUS.res \
    .\pmformat.def \
    PMFMTUS.MAK
      @echo WF::LINK::Link
      icc.exe @<<
        /B"/IG /PM:PM /DE /NOLOGO"
        /Fepmfmtus.exe
        .\bootsus.obj
        .\pmformat.obj
        .\PMTRACK.obj
        .\DISKTHRD.obj
        .\DRVINFO.obj
        .\FORMAT.obj
        cutil.lib
        pmformat.def
<<
      @echo WF::BIND::Resource Bind
      rc.exe .\PMFMTUS.res pmfmtus.exe


!include PMFMTUS.Dep
