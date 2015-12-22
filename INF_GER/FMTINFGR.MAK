# Created by IBM WorkFrame/2 MakeMake at 01:26:53 on 04/02/94
#
# This makefile should be run in the following directory:
#   d:\work\c\pmformat\inf_ger
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


!include FMTINFGR.Dep