:userdoc.
:title.PMFORMAT
:docprof toc='*3'.
:h1.PMFORMAT
:lines align=center.

:elines.
:cgraphic.
:color fc=red.
€ﬂﬂﬂﬂﬂﬂﬂ€ﬂﬂﬂﬂﬂﬂﬂ€ﬂﬂﬂﬂﬂﬂﬂ€ﬂﬂﬂﬂﬂﬂﬂ€ﬂﬂﬂﬂﬂﬂ€€ﬂﬂﬂﬂﬂﬂﬂ€€ﬂﬂﬂﬂﬂ€€ﬂﬂﬂﬂﬂﬂﬂ€
€ €€€€€ € €€ €€ € €€€€€€€ €€€‹  € €€€€ €€ €€ €€ €€ €€€ €€€€€ €€€€
€  ‹‹‹‹‹€  € €€ €  ‹‹‹‹‹€ €€€€€ €  ‹‹‹‹ €  € €€ €  ‹‹‹‹ €€€€  €€€
€  €€€€€€  € €€ €  €€€€€€ €€€€€ €  €€€€ €  € €€ €  €€€€ €€€€  €€€
€‹‹€€€€€€‹‹€‹€€‹€‹‹€€€€€€‹‹‹‹‹‹‹€‹‹€€€€‹€‹‹€‹€€‹€‹‹€€€€‹€€€€‹‹€€€
:color fc=blue.

:hp2.the PM - Diskette - Formatter                 Version 2.16


        (C) 1992 ... 2001 noller &amp. breining software

    * * * * * * * * * * * * * * * * * * * * * * * * *:ehp2.
:ecgraphic.

:h1.About PMFORMAT

:artwork align=center name='PMFORMAT.BMP'.

:lines align=center.
PMFORMAT  version 2.16

(C) 1992 ... 2001, noller &amp. breining software

e-mail: klaus.breining@t-online.de
Telephone&colon. (49)7191-960345
:elines.

:h1.Introduction

:p.This program serves for the formatting of diskettes under
PM (Presentation Manager) of OS/2. The user interaction is
handled completely in a small window and is very clearly
arranged therefore. The big amount of command line switches
of the original format program is obsolete for this reason.

:p.The program generates the volume id automatically for disks
formatted first time; it is preserved on disks which were
formatted before. In addition to that the user will receive
a warning, if the diskette in the drive already contains
data. 
In this case additional options, the :hp2.format modes:ehp2.
will be available.

:p.We, Klaus Breining and Markus Noller, distribute common
written software under the name noller &amp. breining software.
Although it is our attempt, to release only full functional and thoroughly
tested programs to the public, a bug may always creep in.
If you find any errors, please report them to us.

:h1.NOTE

:p.:hp8.NOTE:ehp8.

:p.:hp8.PMFORMAT is not supplied with any sort of guarantee. You use
PMFORMAT at your own risk.:ehp8.


:p.The following names are trademarks of companies used only
for reference purpose in this document.
:ul.
:li.IBM und OS/2 - IBM Corporation
:li.MSDOS - Microsoft Corporation
:eul.

:h1.Start

:p.The program is started by simply typing :hp2.PMFORMAT:ehp2. on the
command line. Parameters are neither necessary nor recognized.

:cgraphic.
&bxh.&bxh.&bxh.&bxh.&bxh.&bxde.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxde.&bxh.&bxh.&bxh.&bxh.&bxh.&bxde.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxde.&bxh.&bxh.&bxh.:link
reftype=launch object='pmformat.exe'. PMFORMAT :elink.&bxh.&bxh.&bxde.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxde.&bxde.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxde.&bxri.
     &bxll.&bxh.:link refid=drive reftype=fn. Laufwerk :elink.&bxh.&bxlr.     &bxll.&bxh.:link refid=drive reftype=fn. Pfad :elink.&bxh.&bxlr.               &bxll.&bxh.:link
reftype=fn refid=ddrive. D&colon. :elink.&bxh.&bxlr.&bxll.&bxh.:link reftype=fn refid=label. label :elink.&bxh.&bxlr.
:ecgraphic.

:fn id=drive.
:lines align=center.
 External command - Parameter: drive, path
 
:elines.
Drive and path for the command to execute. External commands
are stored in form of program files on your disk. If you give
an external command the system looks first in the actual
directory of the standard drive for this command. If the command
is not found there, the system continues to search along the
directories noted in the PATH statement.
:efn.

:fn id=ddrive.
:lines align=center.
Drive
:elines.
:p.Defines the drive that contains the unformatted diskette.
:efn.

:fn id=label.
:lines align=center.
Volume label
:elines.
:p.Here you can enter a diskette name with up to 11 characters.
:efn.

:p.The program can be inserted very easily into a folder of the
workplace shell. It can be invoked from there with a
single "double-click".

:h1.Usage

:p.The usage of this program is rather simple. You only have to
enter the name of the new diskette and to select the drive
and the kind of diskette.
The following types are available:
:ul.
:li.DD - Double density diskettes (3´" and 5¨").
:li.HD - High density diskettes (3´" and 5¨").
:li.ED - Extended density diskettes (3´" only).
:li.Special - special formats for the driver EXTDSKDD (3´" und 5¨").
:eul.
:p.This could be done in any sequence; the entry of a
diskette label is optional and not necassary. Pushing to the
FORMAT-button starts the formatting.

:p.During formatting the button at the lower end of the window
serves as a stop button to break the formatting action.
When the format is complete this is signalled with a bell
tone and a popup window in which some data about the diskette
will be displayed.

:p.If the diskette is already formatted, this is shown to the
user. An additional window is opened; it shows the volume-id and
the diskette name if present. You can decide now to proceed
with  your format or abort it. The popup of this window also
generates a bell tone. This is for the case you
changed to a full screen session right after pushing the
button and don't have to wait for the end of the format a
very long time.
In addition you can see three :hp2.format mode:ehp2. buttons
in this window.
:ul compact.
:li.:hp1.format:ehp1. With this button a normal format operation
is be selected.
:li.:hp1.erase:ehp1. This selections serves for erasing
of formatted and used diskettes. This is done by Writing
of a new boot sector, an empty directory as well as two
empty FAT's.
Only the bad sector-entries are copied from the old FAT's.
:li.:hp1.write boot sector:ehp1. This action simply exchanges
the boot sector of the diskette against a new one.
:p.:hp8.The boot sector of:ehp8. :hp4.PMFORMAT:ehp4.:hp8.
is compatible to the one of OS/2 starting with version 2.1. 
It is therefore able to boot this operating system from diskette.
But in contrast to the boot sector of FORMAT.COM delivered
with OS/2, it boots from hard disk, if the inserted diskette
doesn't contain an operating system.:ehp8.
:eul.

:p.The appearance of this window may be suppressed by
deactivating of the button :hp2.warnings:ehp2. until you
end the program.

:p.If the selected diskette format is not corresponding to the
format of the already formatted, inserted diskette, the 
:hp2.format modes:ehp2. may not be used.
This warning may not be suppressed by means of the button
:hp2.warnings:ehp2.!

:p.If there occurres a system error during format, the program
will show it in an additional popup window too. The error number
supplied with the text could be used for reference in the
HELP command on the command line. An error on your diskette
is shown by a red segment on the progress display bar.

:p.If the format is aborted by a system error or by the user,
there will be no problems according the drive
(Locking or something like that),  but the diskette is
inoperable and should be formatted again before use as a
storage medium.

:p.The program saves its current window position on termination
in conformity to the IBM CUA standard.

:h1.Extensions to version 2.16
:p.:hp2.Drive Check:ehp2.
:p.At startup time the program begins to search all diskette drives
on a computer and determines all possible diskette formats.
If a diskette is inserted into the drive, it additionally tries to determine
the type of this diskette in case of the activated option :hp2.check format:ehp2. in the
options menu.

:p.:hp2.Command line arguments:ehp2.
:p.PMFORMAT 2.12 now recognizes command line arguments that are compatible
with those of the PMFORMAT program supplied with OS/2 starting with version 2.1.
It uses the following syntax:
:p.:font facename=Courier size=10x8.
PMFORMAT [drive[disklabel]]
:font facename=default.
:p.Here :font facename=Courier size=10x8.drive:font facename=default. stands
for the drive letter, not including the colon '&colon.'.

:p.:hp2.Standard and Extended Mode:ehp2.
:p.On most PC's 2 diskette drives are installed.
In this case, the 'radio button' is the most convenient and rapid way
to select a drive.
However, if more than two physical or logical drives are installed (e. g. by
means of the device driver EXTDSKDD), these drives can be selected in the
extended mode of the program.
Here, selection is performed by a 'spin button', that offers the possibility
to select as much as 26 drive while wasting only a minimal screen area.

:p.:hp2.ED and Special diskette formats:ehp2.
:p.Since several time, 3´" diskette drives with a capacity of 2.88 MByte are
available. PMFORMAT 2.12 now makes it possible to format these diskettes using
the ED-type button.

:p.OS/2 includes a device driver EXTDSKDD.SYS on shipment.
This driver makes it possible, to access a physical drive by additional logical
drive letters.
It also allows diskette formats, that are no OS/2 or MSDOS standard.
For these types, the 4th button "Special" has been assigned. Here the program
gets the diskette type from the device driver and formats the diskette in the drive
with it.

:p.:hp2.Format modes:ehp2.
:p.Very often a diskette format is used only to erase a complete diskette.
Here PMFORMAT now offers an erase mode. It deletes the directory
entries and FAT's and writes a new OS/2 boot-sector.
Only the bad sector entries of the old FAT's are copied to the new ones.
:p.The mode :hp1.Write boot sector:ehp1. offers a possibility to exchange
the boot sector of the diskette against a new, slightly modified one.
It is also a quite simple means of removing a boot sector virus.

:h1.Registration

:p.This program is released as :hp2.SHAREWARE.:ehp2. 

:p.Of course, we are wanting only :hp2.VERY LITTLE MONEY:ehp2. (not worth
talking about). So you may get the registered version at a prize of&colon.

:p.:hp2.$ 10.-- (in words: ten):ehp2.

:p.as bank note in your letter. You get the actual version of
:hp4.PMFORMAT:ehp4. in german and english on a
diskette of your choice 5 1/4 inch or 3 1/2 inch and a note,
if there is an upgrade because of
a bug (we don't hope so) or because of user requests.

:hp8.
:nt.Please do NOT send any cheques. Send bank notes, please !
There is a rather high fee for discharging foreign cheques at
German banks (about $ 8 - for a $ 5 cheque). So letters
containing cheques go directly to /DEV/NULL (if the cheque is
not $ 15 or above <grin> )
:ent.:ehp8.

:h2.Registration form
:fig.
                                         _____________,  ___ __th __




      noller &amp. breining software
      Lauffener Str. 4
      D - 71522 Backnang
      Germany



      According: Registration PM-Format


      Yes, I want to register PMFORMAT. I know that this program is
      released without any kind of guarantee  what is caused by its
      concept  (shareware) and its  very low  prize.  I enclose the
      registration fee in form of a $10 bill to this letter.
      (Again: No cheques please ! Thank you)

      I would like to receive the software on a

      o   5 1/4 inch diskette
      o   3 1/2 inch diskette.

      Please send it to the following address:


      Name, First Name:  _______________________________

      Street, No. :      _______________________________

      Postal Code, Town: _______________________________

      Country:           _______________________________


      __________________________________
      (signature)
:efig.

:h1.Versions
:cgraphic.:hp2.
   2.16 Help file is also found in directory of executable file

   2.15 Removal of registration dialog.

   2.14 Change to bootsector code to support virus checking.

   2.13 Shareware restrictions removed.

   2.12 Adaption to different return data of DosDevIOCtl under
        OS/2 Warp 3.0.

   2.11 Correction of version test in order to support OS/2 2.11
        (Service pack XR06200). Introduction of three format modes
        and a new boot sector.

   2.1  OS/2 2.1 version. Uses a much simpler method to
        determine the drive types, because OS2DASD.DMD
        is now complete after all.
        Offers a quick format mode.

   2.0  OS/2 2.0 version. Supports more than 2 drives,
        additional diskette formats as well as 
        command line arguments. (This version was never
        released to the public due to OS/2 2.0 problems)

   1.3  Device independant calculation of positions.
        Output of diskette data after formatting.

   1.2  Improvement in thread handling.
        Edit box keeps diskette name in case of error.

   1.1  Storage of window position in OS2.INI when
        "SAVE" is selected in system menu or window.

   1.01 Correction in handling of "invalid Characters"
        entered in "Label" edit box.

   1.0  First public version:ehp2.
:ecgraphic.
:euserdoc.
