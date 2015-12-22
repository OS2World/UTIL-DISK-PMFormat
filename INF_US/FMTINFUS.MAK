# Created by IBM WorkFrame/2 MakeMake at 22:37:38 on 04/03/94
#
# This makefile should be run in the following directory:
#   d:\work\c\pmformat\inf_us
#
# The actions included in this makefile are:
#   COMPILE::IPF Compile

.all: \
  .\PMFORMAT.inf

.SUFFIXES:

.SUFFIXES: .IPF

.IPF.inf:
      @echo WF::COMPILE::IPF Compile
      ipfc.exe /inf %s


!include FMTINFUS.Dep