/*----------------------------------------------------------
   PMFORMAT.C -- Ein Formatierprogramm fÅr Disketten

   Modul 4: Bestimmen der Laufwerksdaten

   Version 2.0 - 20.9.1992  > OS/2 2.0-Version fÅr C Set/2
   Version 2.16 - 4.5.2000  > DosSetCurrentDir wegen Help-Datei
  ----------------------------------------------------------*/
#define  INCL_DOSMEMMGR
#define  INCL_DOSDEVICES
#define  INCL_DOSDEVIOCTL
#define  INCL_WINCOUNTRY
#define  INCL_ERRORS
#include <os2.h>
#include "format.h"

extern HAB hab;

extern SHORT sTestMode;                 /* Diskettentype-PrÅfung ein/aus */

extern PDRIVEINFO pFSDriveInfo;         /* Zeiger auf DriveInfo-Segment */

/* Informationen Åber vorhandene Laufwerke */
ULONG  ulLogDrvMap;                     /* Drive-Map: nur Disketten-Laufwerke */
ULONG  ulCurDisk;                       /* aktuelles Laufwerk */
USHORT uscDrives;                       /* Zahl der Laufwerke */

/* Array zulÑssiger Diskettentypen fÅr Laufwerkstyp */
BPBDATA arBPBData[NUMDRVTYPE] =
    {{720,  9,  {DID_DD, 0,      0,      0}},   /* DEVTYPE_48TPI   */
     {2400, 15, {DID_DD, DID_HD, 0,      0}},   /* DEVTYPE_96TPI   */
     {1440, 9,  {DID_DD, 0,      0,      0}},   /* DEVTYPE_35      */
     {2880, 18, {DID_DD, DID_HD, 0,      0}},   /* DEVTYPE_UNKNOWN */
     {5760, 36, {DID_DD, DID_HD, DID_ED, 0}}};  /* Typ 9 (2,88MB)  */

ULONG arButton[NUMFBUTTON];             /* Array fÅr erlaubte Format-Buttons */

/******************************************************************
    Initialisiert den Vektor der PDRIVEINFO-Struktur
    Eingang: pFSDrvInfo: Zeiger auf Struktur
    Ausgang: return    : Zahl der Laufwerke
 ******************************************************************/
USHORT FSDriveData (PDRIVEINFO *pFSDrvInfo)
    {
    USHORT usc;
    ULONG  ulDriveMap, ulParmLen, ulDataLen;
    BYTE   pParm[2];
    APIRET ret;

    DosQCurDisk (&ulCurDisk, &ulLogDrvMap);

    /* Bestimmung der Zahl der Laufwerke */
    for (uscDrives = 0, ulDriveMap = 1L; ulDriveMap < (1L << 26); ulDriveMap <<= 1)
        if (ulDriveMap & ulLogDrvMap)
            uscDrives++;

    /* Allokieren des DRIVEINFO-Segmentes */
    DosAllocMem ((PVOID) pFSDrvInfo, (ULONG) uscDrives * sizeof (DRIVEINFO),
        PAG_COMMIT | PAG_READ | PAG_WRITE);

    /* Initialisieren von DRIVEINFO */
    for (usc = 0; usc < uscDrives; usc++)
        {
        pParm[0] = 0;
        pParm[1] = (BYTE) usc;
        ulParmLen = 2;
        ulDataLen = sizeof (BIOSPARAMETERBLOCK);
        ret = DosDevIOCtl ((HFILE) -1, IOCTL_DISK, DSK_GETDEVICEPARAMS,
            pParm, 2, &ulParmLen,
            (PVOID)&(*pFSDrvInfo+usc)->strucBPB, sizeof (BIOSPARAMETERBLOCK), &ulDataLen);
        (*pFSDrvInfo+usc)->cDrive = (UCHAR) ((ret) ? 0 : 'A'+usc);
        }

    return uscDrives;
    }

/***********************************************************************
    PrÅft, ob usDrvNum ein Diskettenlaufwerk ist.
    Eingang: usDrvNum: Laufwerksnummer (1based)
    Ausgang: RETURN:   TRUE:  Laufwerksnummer ist Diskettenlaufwerk
                       FALSE: Laufwerk existiert nicht oder ist kein
                              Diskettenlaufwerk
 ***********************************************************************/
BOOL IsDiskette (USHORT usDrvNum)
    {
    USHORT i;
    PDRIVEINFO pDrive;

    for (i = 0, pDrive = pFSDriveInfo; i < uscDrives; i++, pDrive++)
        {
        if (WinUpperChar (hab, 0, 0, pDrive->cDrive) - 'A' + 1 == usDrvNum)
            {
            if (pDrive->strucBPB.fsDeviceAttr & 2)
                switch (pDrive->strucBPB.bDeviceType)
                    {
                    case DEVTYPE_48TPI:     /* 48 TPI low-density diskette drive */
                    case DEVTYPE_96TPI:     /* 96 TPI high-density diskette drive */
                    case DEVTYPE_35:        /* 3.5-inch 720KB diskette drive */
                    case DEVTYPE_8SD:       /* 8-inch single density diskette drive */
                    case DEVTYPE_8DD:       /* 8-inch double density diskette drive */
                    case 9:                 /* 3.5-inch 4.0MB diskette drive (2.88MB formatted) */
                        return TRUE;

                    case DEVTYPE_UNKNOWN:   /* Other (includes 1.44MB 3.5-inch diskette drive) */
                        if (pDrive->strucBPB.cCylinders == 80)
                            return TRUE;
                        break;
                    }
            break;                          /* Laufwerk ist kein Diskettenlaufwerk */
            }
        }
    return FALSE;
    }

/******************************************************************
    Untersucht die Diskettentypen, die fÅr ein Laufwerk erlaubt sind
    Eingang: pFSDriveInfo: Laufwerkstruktur
    Ausgang: return:       ID-Vektor der zu aktivierenden Buttons
 ******************************************************************/
PULONG GetDiskTypeBtn (PDRIVEINFO pFSDriveInfo)
    {
    USHORT i, j;

    for (i = 0; i < NUMDRVTYPE; i++)
        if (pFSDriveInfo->strucBPB.cSectors == arBPBData[i].cSectors &&
            pFSDriveInfo->strucBPB.usSectorsPerTrack == arBPBData[i].usSectorsPerTrack)
            break;
    /* kein Format gefunden => EXTDSKDD-Laufwerk */
    if (i == NUMDRVTYPE)
        {
        arButton[0] = 0;
        arButton[1] = 0;
        arButton[2] = 0;
        arButton[3] = DID_SP;
        }
    /* Format gefunden => Button IDs kopieren */
    else
        {
        for (j = 0; j < NUMFBUTTON; j++)
            arButton[j] = arBPBData[i].idButton[j];
        }

    return arButton;
    }

/******************************************************************
    Untersucht den Typ der eingelegten Diskette
    Eingang: pszDevName  : Zeiger auf Laufwerksnamen
             pulBtnID    : Vektor der aktivierten Buttons
    Ausgang: return:   Button-ID fÅr Diskettentyp
 ******************************************************************/
ULONG CheckDisk (PCHAR pszDevName, PULONG pulBtnID)
    {
    SHORT i, j;

    ULONG ulAction, ulRet, ulcCmd, ulcBPB;
    HFILE hFile;
    BYTE  bCmd[2];
    BIOSPARAMETERBLOCK arBPB;

    /* kennt das Laufwerk evt. nur einen Diskettentyp? */
    /* ja => Button ID fÅr diesen Type zurÅckgeben */
    for (i = j = 0; i < NUMFBUTTON; i++)
        if (*(pulBtnID+i) != 0)
            {
            j++;
            ulRet = *(pulBtnID+i);
            }

    /* j enthÑlt die Zahl der Formate */
    if (j == 1)
        return ulRet;                   /* Ausstieg: Laufwerk kennt nur 1 Diskettentyp */

    /* Der nachfolgende Test des Diskettentyps wird nur gemacht, */
    /* falls der Testmodus mit sTestMode==TRUE aktiviert ist     */
    if (sTestMode)
        {
        /* Laufwerk kennt mehrere Diskettentypen; Falls eine formatierte */
        /* Diskette eingelegt ist soll deren Type Åbergeben werden       */
        if (DosOpen (pszDevName, &hFile, &ulAction, 0L,
                FILE_NORMAL, FILE_OPEN,
                OPEN_FLAGS_DASD | OPEN_SHARE_DENYREADWRITE | OPEN_ACCESS_READWRITE, NULL) == 0)
            {
            bCmd[0]= 1;                     /* Media BPB */
            bCmd[1]= 0;
            ulcCmd = sizeof (bCmd);
            ulcBPB = sizeof (BIOSPARAMETERBLOCK);
            if (DosDevIOCtl (hFile, IOCTL_DISK, DSK_GETDEVICEPARAMS,
                    &bCmd, sizeof (bCmd), &ulcCmd,
                    &arBPB, sizeof (BIOSPARAMETERBLOCK), &ulcBPB) == 0)
                {
                for (i = 0; i < NUMDRVTYPE; i++)
                    if (arBPB.cSectors == arBPBData[i].cSectors &&
                        arBPB.usSectorsPerTrack == arBPBData[i].usSectorsPerTrack)
                        break;
                /* Format der Diskette als Standardtype erkannt */
                /* Dieser Typ ist in der arBPBData-Struktur als */
                /* hîchste Button-ID verzeichnet                */
                if (i != NUMDRVTYPE)
                    {
                    for (j = NUMFBUTTON-2; j >= 0; j--)
                        if ((ulRet = arBPBData[i].idButton[j]) > 0)
                            break;
                    }                       /* Ausstieg mit Diskettentyp (j<NUMFBUTTON-1) */
                }
            DosClose (hFile);
            }
        }

    /* Es ist keine Diskette eingelegt, oder sTestMode ist FALSE            */
    /* j enthÑlt noch die Zahl der Formate                                  */
    /* Falls der Laufwerkstype noch nicht bestimmt wurde (j=NUMFBUTTON),    */
    /* wird der HD-Button aktiviert                                         */
    if (j == NUMFBUTTON)
        ulRet = DID_HD;

    return ulRet;
    }

/******************************************************************
    Bestimmt aus dem Button-ID fÅr den Diskettentyp und dem
    Laufwerkstyp den BPB, der verwendet werden soll.
    - maximale Datendichte des Laufwerkes: recommended BPB verwenden
    - sonst: der Index eines BPB in astrucBPB wird zurÅckgegeben.
    Eingang: pFSDrvInfo: Laufwerkstruktur
             ulDiskType: Index des Buttons fÅr Diskettentyp
             pulBtnID  : Vektor der aktivierten Buttons
    Ausgang: Index des BPB in astrucBPB
 ******************************************************************/
LONG GetBPB (PDRIVEINFO pFSDriveInfo, ULONG ulDiskType, PULONG pulBtnID)
    {
    SHORT i;

    /* Ist es ein Laufwerk mit 'Spezial-BPB'? */
    /* ja => recommended BPB verwenden        */
    if (pulBtnID[NUMFBUTTON-1] != 0)
        return GBPB_RECOMMENDED;

    /* Ist es der Formattyp mit der hîchsten Datendichte? */
    /* ja => recommended BPB verwenden                    */
    for (i = NUMFBUTTON-1; i >= 0; i--)
        if (pulBtnID[i] != 0)
            break;
    if (ulDiskType == pulBtnID[i]-DID_GR_TYPE)
        return GBPB_RECOMMENDED;

    /* sonst: BPB aus Array bestimmen und Index Åbergeben */
    switch (pFSDriveInfo->strucBPB.bDeviceType)
        {
        case DEVTYPE_96TPI:     /* 96 TPI high-density diskette drive */
            return GBPB_DD5;

        case DEVTYPE_UNKNOWN:   /* Other (includes 1.44MB 3.5-inch diskette drive) */
            return GBPB_DD3;

        case 9:                 /* 3.5-inch 4.0MB diskette drive (2.88MB formatted) */
            return (ulDiskType == DID_DD-DID_GR_TYPE) ? GBPB_DD3 : GBPB_HD3;
        }

    /* Hier darf das Programm nie vorbeikommen! */
    return GBPB_ERROR;
    }

