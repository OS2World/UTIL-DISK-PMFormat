.list
.lall
.sfcond
.286
page 62,132
title BOOT - Bootsektor fr PMFORMAT

comment "
    PMFORMAT.C -- Ein Formatierprogramm fr Disketten

    Modul 6: Bootsektor

    Dieser Bootsektor entspricht dem des Formatierprogrammes der
    OS/2 - Version 2.0; er liefert jedoch eine Versionsnummer von
    OS/2 - Version 2.1.

    Version 1.1  - 15.4.1991
    Version 1.2  - 9.6.1991   > Žnderung der Version in 10.3;
                                sprachabh„ngiger Schalter fr Fehlermeldungen
    Version 1.3  - 9.11.1991
    Version 2.0  - 20.9.1992  > vereinfachte Segmentdirektiven wurden durch segment ersetzt,
                                um fr OS/2 2.0 eine eigene Gruppe fr 16-Bit Segmente zu
                                erzeugen.
    Version 2.1  - 2.4.1994   > Anpassung damit auch OS/2 2.1 gebootet werden kann.
    Version 2.14 - 18.11.1995 > Code„nderung wegen Virus-Scanner.
"

SEG_0     segment at 0
          org 0078h
DISK_POINTER LABEL WORD
          org 0413h
MEMORY_SIZE  LABEL WORD
          org 7C00h
BOOT_LOCN label far
SEG_0     ends

SEG_800   segment at 800h
          org 0121h
CONTINUE  label far
SEG_800   ends


DGROUP16  group     BOOT

BOOT      segment   para 'FAR_DATA'     ; Align: para, Combine: private, Class: FAR_DATA

          assume    CS:DGROUP16, DS:SEG_0, SS:NOTHING
          public    BiosBPB
          public    BootSect
          public    Serial
          public    EndOfBPB

BootSect  label     byte
          JMP       near ptr BOOTUP     ;NOP
LBL0003   DB        "PMFORMAT"

BiosBPB   label     byte
SECSIZ    DW        512                 ;0Bh | USHORT usBytesPerSector
CLUSTSIZ  DB        2                   ;0Dh | BYTE   bSectorsPerCluster
RESSECT   DW        1                   ;0Eh | USHORT usReservedSectors
NUMFATS   DB        2                   ;10h | BYTE   cFATs
DIRENTR   DW        112                 ;11h | USHORT cRootEntries
NUMSECT   DW        720                 ;13h | USHORT cSectors
MEDIAFLG  DB        0FDH                ;15H | BYTE   bMedia
FATSIZ    DW        2                   ;16H | USHORT usSectorsPerFAT
SECTPTR   DW        9                   ;18H | USHORT usSectorsPerTrack
NUMHEADS  DW        2                   ;1AH | USHORT cHeads
PARTOFFS  DW        0,0                 ;1CH | ULONG  cHiddenSectors
RESERVD1  DB        4 dup (0)           ;20H | ULONG  cLargeSectors
EndOfBPB  label     byte
RDDRIVE   DB        0                   ;24H
RDHEAD    DB        0                   ;25H
RESERVD2  DB        029H                ;26H
SERIAL    DB        0, 0, 0, 0          ;27H
RESERVD3  DB        "NO NAME    FAT     "         ;2BH

BOOTUP    proc far
FIRSTSEC  label WORD
          CLI
          MOV    SP, 7BFFh
RDANFSEC  label WORD
          XOR    BX, BX
RDNUMSEC  label WORD
          MOV    SS, BX
          MOV    DS, BX
          DEC    word ptr [MEMORY_SIZE] ; Memory Size (40:13)
          STI
          INT    12h                    ; AX = RAM size in KB
          SHL    AX, 6                  ; AX = RAM size in Paragraphs
          MOV    ES, AX                 ; ES = End of Real Mode RAM
          XOR    DI, DI

          LDS    SI, dword ptr [DISK_POINTER] ; DS:SI = DiskPointer
          CLD
          MOV    CX, 11                 ; kopiere 11 Byte
     REP  MOVSB

          MOV    AX, 07C0h
          MOV    DS, AX

          assume    CS:DGROUP16, DS:DGROUP16, SS:NOTHING

          MOV    AX, word ptr [SECTPTR]
          MOV    ES:[4],AL

          assume    CS:DGROUP16, DS:SEG_0, SS:NOTHING

          PUSH   DS
          MOV    DS, BX
          MOV    [DISK_POINTER], BX
          MOV    [DISK_POINTER+2], ES
          POP    DS

          assume    CS:DGROUP16, DS:DGROUP16, SS:NOTHING

          MOV    DL, byte ptr [RDDRIVE]
          INT    13h          ; RESET

          MOV    AL,byte ptr [NUMFATS]
          CBW
          MUL    word ptr [FATSIZ]
          ADD    AX, [RESSECT]
          PUSH   AX
          XCHG   AX, CX
          MOV    AX, 32
          MUL    word ptr [DIRENTR]
          MOV    BX, [SECSIZ]
          ADD    AX, BX
          DEC    AX
          DIV    BX
          PUSH   AX
          ADD    AX, CX
          MOV    [FIRSTSEC], AX
          MOV    AX, 1000h
          MOV    [FIRSTSEC+2], AX
          MOV    ES, AX
          XOR    DI, DI
          POP    CX
          MOV    [RDNUMSEC], CX
          POP    AX
          MOV    [RDANFSEC], AX
          XOR    DX, DX
          CALL   ReadSect

          XOR    BX, BX
          MOV    CX, word ptr [DIRENTR]
COMPDIRS: MOV    DI, BX
          PUSH   CX
          MOV    CX, 11
          MOV    SI, offset [OS2BOOT]
     REP  CMPSB
          POP    CX
          JZ     DIRFOUND
          ADD    BX, 32
          LOOP   COMPDIRS
DIRFOUND: JCXZ   SYS1475
          MOV    AX, ES:[BX+1Ch]
          MOV    DX, ES:[BX+1Eh]
          DIV    word ptr [SECSIZ]
          INC    AL
          MOV    CL, AL
          MOV    DX, ES:[BX+1Ah]
          DEC    DX
          DEC    DX
          MOV    AL, byte ptr [CLUSTSIZ]
          XOR    AH,AH
          MUL    DX
          ADD    AX, [FIRSTSEC]
          ADC    DX, 0
          MOV    BX, 800h
          MOV    ES, BX
          XOR    DI, DI
          PUSH   ES
          PUSH   DI
          CALL   ReadSect
          LEA    SI, [BiosBPB]
          RET
BOOTUP    endp

SYS1475:  MOV    SI, offset [KEINLDR]
          CALL   WrtStr

BOOTHD:   PUSH   DS
          POP    ES
          MOV    CX, 0100h
          MOV    SI, 0
          MOV    DI, 0400h
     REP  MOVSW
          JMP    CONTINUE

CONT1:    MOV    AX, 0201h              ; Read 1 Sector
          XOR    BX, BX                 ; to 7C0:0000H
          MOV    DX, 0080h              ; from HD #0 / Head 0
          MOV    CX, 0001h              ; Sector 1, Track 0
          INT    13h
          JC     SYS2025A
          JMP    BOOT_LOCN

SYS2025A: MOV    AX, 800h
          MOV    DS, AX
SYS2025:  MOV    SI, offset [LESEFEHL]
SYSOUT:   CALL   WrtStr

          MOV    AH, 0                  ; Get KeyCode in AH
          INT    16h
          INT    19h

WrtBegin: MOV    AH, 14
          MOV    BX, 7
          INT    10h
WrtStr:   LODSB
          OR     AL, AL
          JNZ    WrtBegin
          RET

ReadSect: PUSH   AX
          PUSH   DX
          PUSH   CX
          ADD    AX, word ptr [PARTOFFS]
          ADC    DX, word ptr [PARTOFFS+2]
          DIV    word ptr [SECTPTR]
          INC    DL
          MOV    BL, DL
          XOR    DX, DX
          DIV    word ptr [NUMHEADS]
          MOV    BH, DL
          MOV    DX, AX
          MOV    AX, word ptr [SECTPTR]
          SUB    AL, BL
          INC    AX
          PUSH   AX
          MOV    AH, 2
          MOV    CL, 6
          SHL    DH, CL
          OR     DH, BL
          MOV    CX, DX
          XCHG   CH, CL
          MOV    DL, byte ptr [RDDRIVE]
          MOV    DH, BH
          MOV    BX, DI
          INT    13H
          JC     SYS2025
          POP    BX
          POP    CX
          MOV    AX, BX
          MUL    word ptr [SECSIZ]
          ADD    DI, AX
          POP    DX
          POP    AX
          ADD    AX, BX
          ADC    DX, 0
          SUB    CL, BL
          JG     ReadSect
          RET

KEINLDR   DB        0Dh, 0Ah
ifdef     GERMAN
          DB        "OS2BOOT nicht gefunden", 0DH, 0AH
          DB        "Boote von Festplatte...", 0DH, 0AH, 0
else
          DB        "File OS2BOOT not found", 0DH, 0AH
          DB        "Booting from harddisc...", 0DH, 0AH, 0
endif

ifdef     GERMAN
LESEFEHL  DB     "Lesefehler Datentr„ger", 0DH, 0AH, 0
else
LESEFEHL  DB     "A disk error occurred", 0DH, 0AH, 0
endif

OS2BOOT   DB     "OS2BOOT    ",0

          org 01FEh
          DB        055h, 0AAh
BOOT      ends

          end
