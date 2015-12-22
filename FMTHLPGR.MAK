# Created by IBM WorkFrame/2 MakeMake at 01:33:15 on 03/31/94
#
# This makefile should be run in the following directory:
#   d:\work\c\pmformat
#
# The actions included in this makefile are:
#   COMPILE::IPF Compile

.all: \
  .\PMFMTGR.inf

.SUFFIXES:

.SUFFIXES: .IPF

.IPF.inf:
      @echo WF::COMPILE::IPF Compile
      ipfc.exe %s


!include FMTHLPGR.Dep