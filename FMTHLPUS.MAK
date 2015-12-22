# Created by IBM WorkFrame/2 MakeMake at 01:32:56 on 03/31/94
#
# This makefile should be run in the following directory:
#   d:\work\c\pmformat
#
# The actions included in this makefile are:
#   COMPILE::IPF Compile

.all: \
  .\pmfmtus.inf

.SUFFIXES:

.SUFFIXES: .ipf

.ipf.inf:
      @echo WF::COMPILE::IPF Compile
      ipfc.exe %s


!include FMTHLPUS.Dep