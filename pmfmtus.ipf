.*----------------------------------------------------------
.* PMFORMAT.C -- Ein Formatierprogramm fÅr Disketten
.*
.* IPF Help File
.*
.* Version 2.14 - 28.7.1995  >
.* Version 2.16 - 4.5.2000  > DosSetCurrentDir wegen Help-Datei
.*----------------------------------------------------------
:userdoc.

.*----------------------------------------------------------
.*  Hilfe HauptmenÅ
.*      res = PANEL_MAIN
.*----------------------------------------------------------
:h1 res=1000 id=MAIN name=PANEL_MAIN.PMFORMAT
:i1 id=MAIN.PMFORMAT
:p.PMFORMAT is capable to format all common diskette formats with every
usual (and unusual) capacity.
The program offers the following features&colon.
:ul.
:li.Simple :link reftype=hd refid=DRIVE.selection out of 2 drives:elink.
in standard mode.
:li.:link reftype=hd refid=DRIVE.Selection out of up to 26 drives:elink.
in enhanced mode.
:li.Support of all :link reftype=hd refid=TYPE.standard diskette formats:elink.
(DD, HD, ED) as well as special formats for logical EXTDSKDD-drives.
:li.:link reftype=hd refid=FMTTYPE.Quick format function:elink.. It deletes all
system areas of a diskette, but takes all bad sectors into consideration.
:li.:link reftype=hd refid=FMTTYPE.Exchange boot sector:elink.. This function allows
to exchange the boot sector of the inserted diskette against a new one, which will
allow you to start your system off the hard disk without removing the diskette, if
this diskette doesn't contain an operating system.
:li.:link reftype=hd refid=WARN.Safety warning:elink. for non-empty diskettes.
This warning may be switched off.
:eul.

.*----------------------------------------------------------
.*  Hilfe Optionen
.*      res = PANEL_OPTIONS
.*----------------------------------------------------------
:h1 res=1010 id=OPTIONS name=PANEL_OPTIONS.Options
:i1 id=OPTION.Options
:p.In the options dialog window selections can be made, that will
be saved in the OS/2 user profile (OS2.INI).
:p.
These selections are accepted on pressing the :hp1.OK:ehp1.-button, whereas
:hp1.Cancel:ehp1. will ignore all changes.
:p.
Related Information&colon.
:ul compact.
:li.:link reftype=hd refid=MODE.drive selection mode:elink.
:li.:link reftype=hd refid=CHECK.Check format:elink.
:eul.

.*----------------------------------------------------------
.*  Hilfe Warnungen
.*      res = PANEL_WARNING
.*----------------------------------------------------------
:h1  res=1020 id=WARNING name=PANEL_WARNING.Warnings
:i1 id=WARNING.Warnings
:p.In this dialog window problems occuring during a format operation
will be notified to the user.
These are&colon.
:ul.
:li.:hp2.Diskette already formatted. :ehp2.:hp1.(suppressable):ehp1.
The user gets the choice to either format the diskette nevertheless (:hp1.OK:ehp1.),
or to cancel the operation (:hp1.Cancel:ehp1.).
You may select one of three :link reftype=hd refid=FMTTYPE.format modes:elink..
If :link reftype=hd refid=WARN.Warnings:elink. will be switched off, a warning
of this kind will never be popped up until ending the program.
:li.:hp2.Format of diskette differs from presumed one. :ehp2.
This message will appear, if the diskette in the drive has already been formatted
with a format, that differs from the one, the user has selected.
:hp1.OK:ehp1. tries to format the diskette with the new format, whereas :hp1.Cancel:ehp1.
ends the format operation.
The only format mode available is :hp1.format:ehp1..
:eul.
:p.
Related information&colon.
:ul compact.
:li.:link reftype=hd refid=FMTTYPE.format modes:elink.
:li.:link reftype=hd refid=WARN.Warnings:elink.
:eul.

.*----------------------------------------------------------
.*  Hilfe Keys
.*      res = PANEL_HFH
.*----------------------------------------------------------
:h1  res=1040 name=PANEL_HFH.Use of Help
:i1.Use of Help
:p.The help function may be called as follows&colon.
:ul.
:li.Select Help in the menu of an object.
:li.Select Help in a notebook.
:li.Press F1 key in a window, which supports help in the menu bar.
:li.Select Help in the menu of the titlebar.
:li.Press the Help button.
:eul.
:p.The help text shown depends on the function, that has been active when
activating help.
:p.If you use the help function for example when a selection is active in
the menu bar, a specific help text for this selection will be shown.

.*----------------------------------------------------------
.*  Hilfe Laufwerke
.*      res = PANEL_DRIVE
.*----------------------------------------------------------
:h1  res=1050 id=DRIVE name=PANEL_DRIVE.Selection of drives
:i2 refid=MAIN.Selection of drives
:p.The selection of a drive is done in the group :hp1.disk:ehp1..
The kind, how the selection group appears, depends on the
:link reftype=hd refid=MODE.drive selection mode:elink., chosen in the
:link reftype=hd refid=OPTIONS.options:elink. menu.
In this group only installed diskette drives will be shown.
:p.
Related information&colon.
:ul compact.
:li.:link reftype=hd refid=MODE.Drive selection mode:elink.
:eul.

.*----------------------------------------------------------
.*  Hilfe Diskettentyp
.*      res = PANEL_TYPE
.*----------------------------------------------------------
:h1  res=1060 id=TYPE name=PANEL_TYPE.Selection of diskette format
:i2 refid=MAIN.Selection of diskette format
:p.In this group the diskette format can be selected.
The following selections are possible&colon.
:ul compact.
:li.:hp2.DD:ehp2. Normal density diskettes.
:li.:hp2.HD:ehp2. High density diskettes.
:li.:hp2.ED:ehp2. Extremly high density diskettes (2,88 MByte diskettes).
:li.:hp2.Special:ehp2. This button will be active, if a logical drive has been defined by
EXTDSKDD with a format, that differs from the standard.
:eul.

.*----------------------------------------------------------
.*  Hilfe Formatieren
.*      res = PANEL_FORMAT
.*----------------------------------------------------------
:h1  res=1070 name=PANEL_FORMAT.Start of the format operation
:i2 refid=MAIN.Start of the format operation
:p.After pressing this button, the inserted diskette will be formatted with the
preselected parameters.

.*----------------------------------------------------------
.*  Hilfe Label
.*      res = PANEL_LABEL
.*----------------------------------------------------------
:h1  res=1080 name=PANEL_LABEL.Volume name
:i2 refid=MAIN.Volume name
:p.In this field, an optional diskette name (with at most 11 characters)
may be entered. It is useful for a better identification of a diskette.

.*----------------------------------------------------------
.*  Hilfe Format mode
.*      res = PANEL_FMTTYPE
.*----------------------------------------------------------
:h1  res=1090 id=FMTTYPE name=PANEL_FMTTYPE.Format mode
:i2 refid=WARNING.Format mode
:p.:hp4.PMFORMAT:ehp4. offers three different format modes.
:ul.
:li.:hp2.format:ehp2. formats diskettes as known from programs like
FORMAT.COM shipped with OS/2 and DOS.
:li.:hp2.erase:ehp2. simply erases the inserted diskette. For that purpose
it writes a new boot sector, an empty directory as well as two empty FAT's
onto the diskette.
Possibly present bad sector entries in the old FAT's will be retained in
the new FAT's.
:p.Diskettes formatted by use of this method cannot be restored by means of
an "unerase utility". Also a possibly present boot sector virus will be removed.
An additional advantage compared to a "DEL *.*" is the much higher speed.

:li.:hp2.write boot sector:ehp2.&colon. This selection simply exchanges the
boot sector of the diskette with a new one. Nothing else will be altered.
If you entered a volume label in the main panel, this name will also be written
to the diskette.
:p.The new boot sector differs from the one supplied by FORMAT.COM in the way,
that it starts your system off the hard disk, if the diskette doesn't contain
a boot sector. The bothering restarting of the system isn't necessary any more.
This function may also be used to remove a boot sector virus.
:p.If you want to use this new boot sector on a diskette with an operating system on it,
please note, that :hp4.PMFORMAT:ehp4. only supports booting OS/2 version 2.1 and newer!
:eul.
:p.You cannot select these options, if you select
:link reftype=hd refid=WARN.switch off warnings:elink.. A normal formatting operation
will be performed as the default.

.*----------------------------------------------------------
.*  Hilfe Warnung ausschalten
.*      res = PANEL_WARNOFF
.*----------------------------------------------------------
:h1  res=1100 id=WARN name=PANEL_WARNOFF.Switch off warnings
:i2 refid=WARNING.Switch off warnings
:p.If you try to format a diskette, that is already formatted, you normally
will be notified.
(see :link reftype=hd refid=WARNING.warnings:elink.).
This warning may be suppressed with the button :hp1.warnings:ehp1..
This is then valid for the entire session! Also you are not able
to select a :link reftype=hd refid=FMTTYPE.format mode:elink. afterwards.
Only normal formatting operations will be performed.

.*----------------------------------------------------------
.*  Hilfe Auswahlmodus Laufwerke
.*      res = PANEL_BTNTYPE
.*----------------------------------------------------------
:h1  res=1110 id=MODE name=PANEL_BTNTYPE.Drive selection mode
:i2 refid=OPTION.Drive selection mode
:p.To select the diskette drive, two methods are at your disposal&colon.
:ul.
:li.:hp1.Standard mode&colon.:ehp1. In this mode radio buttons for maximum two drives
("A&colon." und "B&colon.") will be shown. This enables a much quicker grip for an
intended drive.
:li.:hp1.Enhanced mode&colon.:ehp1. In this mode, the selection is done by means of a
spin button. This enables you to select out of more than two drives.
:eul.

.*----------------------------------------------------------
.*  Hilfe Diskettencheck
.*      res = PANEL_CHECK
.*----------------------------------------------------------
:h1  res=1120 id=CHECK name=PANEL_CHECK.Diskette check
:i2 refid=OPTION.Diskette check
:p.If the button :hp1.check format:ehp1. is active, at each program startup
as well as at each drive change the type of the inserted will be checked.
In the panel :link reftype=hd refid=TYPE.Type:elink. the determined type will be selected
as the default value, if the diskette is already formatted.

.*----------------------------------------------------------
.*  Hilfe Diskettendaten
.*      res = PANEL_DSKDATA
.*----------------------------------------------------------
:h1  res=1130 name=PANEL_DSKDATA.Diskette data
:i1.Diskette data
:p.This panel shows the data of the presently formatted diskette.
You will see the following entries&colon.
:ol compact.
:li.:hp2.bytes total disk space:ehp2.&colon. Total number of bytes on disk.
:li.:hp2.bytes available on disk:ehp2.&colon. Number of available bytes
remaining on the diskette (total disk space - bad areas - used bytes).
:li.:hp2.bytes in bad sectors:ehp2.&colon. Number of bytes in bad sectors.
:li.:hp2.bytes in each allocation unit:ehp2.&colon. Number of bytes in each
allocation unit (sector).
:li.:hp2.allocation units:ehp2.&colon. Total number of allocation units
(sectors).
:li.:hp2.available allocation units:ehp2.&colon. Number of available allocation
units remaining on the diskette (total number - bad sectors - used sectors).
:eol.

:euserdoc.
