/*----------------------------------------------------------
   PMFORMAT.C -- Ein Formatierprogramm fÅr Disketten

   Modul 3: Formatier-Thread

   Version 1.1 - 15.4.1991
   Version 1.2 - 9.4.1991   > Korrektur von 'Terminate';
                              Einbau von 'rsWaitThread'
   Version 1.3 - 9.11.1991  > zusÑtzlicher RÅckgabewert in fsError;
                              ID_DISKSTAT nicht anzeigen, falls Diskette
                              bereits formatiert und nicht neu formatiert
                              werden soll.
   Version 1.31- 9.2.1992   > Bugfix in WriteBuffer
   Version 2.0 - 20.9.1992  > OS/2 2.0-Version fÅr C Set/2
                              Verarbeiten der recommended BPBs
                              Umbau WriteBuffer => DiskIO
   Version 2.11- 1.4.1994   > Bugfix fÅr allokierte FAT-Grî·e
   Version 2.16 - 4.5.2000  > DosSetCurrentDir wegen Help-Datei
  ----------------------------------------------------------*/
#define  INCL_DOSMEMMGR
#define  INCL_DOSDEVICES
#define  INCL_DOSDEVIOCTL
#define  INCL_DOSSEMAPHORES
#define  INCL_DOSPROCESS
#define  INCL_DOSMISC
#define  INCL_ERRORS
#define  INCL_WINMESSAGEMGR
#define  INCL_WINDIALOGS
#include <os2.h>
#include <string.h>
#include "format.h"

#pragma pack(1)
typedef struct _FORMATPRM
    {
    BYTE    bCommand;
    USHORT  usHead;
    USHORT  usCylinder;
    USHORT  uscTrack;
    USHORT  uscSect;
    struct  {
            BYTE bCylinder;
            BYTE bHead;
            BYTE bSectorID;
            BYTE bBytesPerSector;
            } TrackTable[1];
    } FORMATPRM, *PFORMATPRM;

#pragma pack()
extern HWND         hwndFrame;
extern ULONG        Serial;
extern SFILEINFO    FSInfoBuf;
extern QFILEINFO    FSInfo;
extern DISKINFO     FSDiskInfo;
extern USHORT       usRetMsgBox;
extern ULONG        ulFmtMode;
extern USHORT       usWrnMode;
extern LONG         flDrvStat;

extern ULONG        ulBootDrive;
extern PCHAR        pszDevName;
extern HEV          rsFormatBlocks;
extern ULONG        ulFBPostCount;
extern HMTX         rsWaitThread;

/* Bootsektor */
extern BYTE               BootSect;
extern BIOSPARAMETERBLOCK BiosBPB;
extern BYTE               EndOfBPB;

/* Thread-lokale Daten */
static BIOSPARAMETERBLOCK BPBData;
static HFILE              hFile;
static ULONG              ulAction;
static BYTE               bBuffer[512];
static USHORT             uscReadPrm, uscFormatPrm;
static BYTE               bDataPacket, bParmPacket;
static ULONG              ulcDataP, ulcParmP;
static PTRACKLAYOUT       pReadPrm;
static PFORMATPRM         pFormatPrm;
static PBYTE              pbFAT;

/* BPBs fÅr alle Diskettenformate, die auf Laufwerkstypen mit */
/* grî·erer DatenkapazitÑt zusÑtzlich unterstÅtzt werden.     */
/* Index 0 ist fÅr den recommended BPB vorgesehen             */
static BIOSPARAMETERBLOCK astrucBPB[4] =
    {
    {  0, 0, 0, 0,   0,    0,    0, 0,  0, 0, 0, 0, {0,0,0,0,0,0},  0, 0, 0},   /* ----- */
    {512, 2, 1, 2, 112,  720, 0xFD, 2,  9, 2, 0, 0, {0,0,0,0,0,0}, 40, 0, 0},   /* 5¨"DD */
    {512, 2, 1, 2, 112, 1440, 0xF9, 3,  9, 2, 0, 0, {0,0,0,0,0,0}, 80, 2, 0},   /* 3´"DD */
    {512, 1, 1, 2, 224, 2880, 0xF0, 9, 18, 2, 0, 0, {0,0,0,0,0,0}, 80, 7, 0}    /* 3´"HD */
    };

/********************************************************************
    Berechnung der Disk-ID
 ********************************************************************/
ULONG CalcCRC (BYTE *pString, USHORT usLen)
    {
    ULONG ulCRC;

    ulCRC = 0;
    while (usLen-- > 0)
        {
        ulCRC += (LONG) *pString++;
        ulCRC = (ulCRC >> 2) + (ulCRC << 30);
        }
    return ulCRC;
    }

/********************************************************************
    Schreiben/Lesen eines Puffers auf Diskette. Track-öbergreifendes
    Schreiben/Lesen wird unterstÅtzt.
    Eingang: pBuffer   : Zeiger auf den Puffer
             uscSectors: Zahl der Sektoren
             usStart   : Startsektor
             ulFunction: Funktion DSK_WRITETRACK od. DSK_READTRACK
    Ausgang: DOS-Fehlercode
 ********************************************************************/
APIRET DiskIO (BYTE * pBuffer, USHORT uscSectors, USHORT usStart, ULONG ulFunction)
    {
    APIRET ulReturn;
    ULONG  ulcData, ulcParm;

    do
        {
        pReadPrm->bCommand      = 0;
        pReadPrm->usCylinder    = (USHORT) (usStart / BPBData.usSectorsPerTrack / BPBData.cHeads);
        pReadPrm->usHead        = (USHORT) ((usStart / BPBData.usSectorsPerTrack) % BPBData.cHeads);
        pReadPrm->usFirstSector = (USHORT) (usStart % BPBData.usSectorsPerTrack);
        pReadPrm->cSectors      = (USHORT) ((uscSectors + pReadPrm->usFirstSector >
            BPBData.usSectorsPerTrack) ?
            (BPBData.usSectorsPerTrack - pReadPrm->usFirstSector) :
            uscSectors);

        ulcData = (ULONG) (pReadPrm->cSectors*BPBData.usBytesPerSector);
        ulcParm = uscReadPrm;
        ulReturn = DosDevIOCtl (hFile, IOCTL_DISK, ulFunction,
            pReadPrm, ulcParm, &ulcParm,
            pBuffer, ulcData, &ulcData);

        if (ulReturn)
            break;
        usStart    += pReadPrm->cSectors;
        uscSectors -= pReadPrm->cSectors;
        pBuffer    += pReadPrm->cSectors * BPBData.usBytesPerSector;
        } while (uscSectors > 0);

    return ulReturn;
    }

/********************************************************************
    F o r m a t i e r t h r e a d
 ********************************************************************/
void _System FormatThread (TARG *parThreadArg)
    {
    USHORT  uscTrack, uscRecord, uscHead, usNumTrack;
    USHORT  usFATLen, usCluster, usSizeCode, usSectSizeTmp;
    USHORT  fsError;
    USHORT  usFmt;
    PUSHORT pusFAT;
    BOOL    usTrackOK;
    ULONG   ulSerial;
    LONG    lBPBNum;
    APIRET  ulReturn;

    fsError = RTYPE_SYSERR;
    ulReturn = 0;

    /* untersuchen, ob dem physikalischen Laufwerk mehrere logische Laufwerke zugeordnet sind */
    bBuffer[0] = (CHAR) ('A' + ulBootDrive - 1);
    bBuffer[1] = ':';
    bBuffer[2] = '\0';
    DosRequestMutexSem (rsWaitThread, (ULONG) SEM_INDEFINITE_WAIT);
    if ((ulReturn = DosOpen (bBuffer, &hFile, &ulAction, 0L,
            FILE_NORMAL, OPEN_ACTION_OPEN_IF_EXISTS,
            OPEN_FLAGS_DASD | OPEN_SHARE_DENYNONE | OPEN_ACCESS_READONLY,
            NULL)) == 0)
        flDrvStat |= OPEN;
    DosReleaseMutexSem (rsWaitThread);
    if (ulReturn != 0)
        DosExit (EXIT_PROCESS, 1L); /* fatal Error: Fehler beim ôffnen der Bootplatte */

    bParmPacket = 0;
    bDataPacket = (BYTE) (pszDevName[0] - 'A' + 1);
    ulcParmP = 1;                   /* 1 Byte Parameter */
    ulcDataP = 1;                   /* keine Daten */
    DosDevIOCtl (hFile, IOCTL_DISK, DSK_GETLOGICALMAP,
        &bParmPacket, 1, &ulcParmP,
        &bDataPacket, 1, &ulcDataP);
    DosRequestMutexSem (rsWaitThread, (ULONG) SEM_INDEFINITE_WAIT);
    DosClose (hFile);
    flDrvStat &= ~OPEN;
    DosReleaseMutexSem (rsWaitThread);

    /* dem Laufwerk ist nur 1 logischer Laufwerksbuchstabe zugeordnet */
    if (bDataPacket == 0)
        bDataPacket = (BYTE) (pszDevName[0] - 'A' + 1);

    /* Falls bDataPacket != ulDevice existieren mehrere log. Laufwerke fÅr das physikalische */
    /* Laufwerk und es wurde zuletzt unter einer anderen Laufwerksnummer angesprochen        */
    /* => Umschalten auf die neue Laufwerksnummer                                            */
    if ((BYTE) (pszDevName[0] - 'A' + 1) != bDataPacket)
        {
        bBuffer[0] = (BYTE) (bDataPacket + 'A' - 1);
            DosRequestMutexSem (rsWaitThread, (ULONG) SEM_INDEFINITE_WAIT);
            if ((ulReturn = DosOpen (bBuffer, &hFile, &ulAction, 0L,
                    FILE_NORMAL, OPEN_ACTION_OPEN_IF_EXISTS,
                    OPEN_FLAGS_DASD | OPEN_SHARE_DENYNONE | OPEN_ACCESS_READWRITE,
                    NULL)) == 0)
                flDrvStat |= OPEN;
            DosReleaseMutexSem (rsWaitThread);

        /* Falls keine Diskette eingelegt ist, Thread beenden  */
        if ((flDrvStat & OPEN) == 0)
            goto ERR_OPEN;

        bParmPacket = 0;
        bDataPacket = (BYTE) (pszDevName[0] - 'A' + 1);
        ulcParmP = ulcDataP = 1;
        DosDevIOCtl (hFile, IOCTL_DISK, DSK_SETLOGICALMAP,
            &bParmPacket, 1, &ulcParmP,
            &bDataPacket, 1, &ulcDataP);

        DosRequestMutexSem (rsWaitThread, (ULONG) SEM_INDEFINITE_WAIT);
        DosClose (hFile);
        flDrvStat &= ~OPEN;
        DosReleaseMutexSem (rsWaitThread);

        DosRequestMutexSem (rsWaitThread, (ULONG) SEM_INDEFINITE_WAIT);
        if ((ulReturn = DosOpen (pszDevName, &hFile, &ulAction, 0L,
                FILE_NORMAL, OPEN_ACTION_OPEN_IF_EXISTS,
                OPEN_FLAGS_DASD | OPEN_SHARE_DENYNONE | OPEN_ACCESS_READWRITE,
                NULL)) == 0)
            flDrvStat |= OPEN;
        DosReleaseMutexSem (rsWaitThread);

        if ((flDrvStat & OPEN) == 0)
            goto ERR_OPEN;
        }

    /* Sonst mu· die Einheit geîffnet werden */
    else
        {
        DosRequestMutexSem (rsWaitThread, (ULONG) SEM_INDEFINITE_WAIT);
        if ((ulReturn = DosOpen (pszDevName, &hFile, &ulAction, 0L,
                FILE_NORMAL, OPEN_ACTION_OPEN_IF_EXISTS,
                OPEN_FLAGS_DASD | OPEN_SHARE_DENYREADWRITE | OPEN_ACCESS_READWRITE,
                NULL)) == 0)
            flDrvStat |= OPEN;
        DosReleaseMutexSem (rsWaitThread);

        /* Falls keine Diskette eingelegt ist, Thread beenden  */
        if ((flDrvStat & OPEN) == 0)
            goto ERR_OPEN;
        }

    /* BPB fÅr die neue Diskette bestimmen */
    lBPBNum = GetBPB (parThreadArg->pFSDriveInfo,
                      parThreadArg->ulDiskType,
                      parThreadArg->pulBtnID);

    switch (lBPBNum)
        {
        /* Fehler in GetBPB: korrekter BPB konnte nicht bestimmt werden */
        case GBPB_ERROR:
            ulReturn = ERROR_NOT_READY;
            break;

        /* falls hîchste Datendichte des Laufwerkes angefordert */
        /* wurde: recommended BPB des Laufwerkes anfordern      */
        case GBPB_RECOMMENDED:
            bParmPacket = 0;                /* recommended BPB fÅr Laufwerk */
            ulcParmP = 1;                   /* 1 Byte Parameter */
            ulcDataP = 0;                   /* keine Daten */
            ulReturn = DosDevIOCtl (hFile, IOCTL_DISK, DSK_GETDEVICEPARAMS,
                &bParmPacket, 1, &ulcParmP,
                &astrucBPB[GBPB_RECOMMENDED], sizeof (BIOSPARAMETERBLOCK), &ulcDataP);
            break;
        }
        if (ulReturn != 0)
            goto ERR_THREAD;

    /* PrÅfen, ob Diskette bereits formatiert ist */
    ulFmtMode = MBID_OK;                            /* Default: normal formatieren */
    if (DosQueryFSInfo((ULONG)(pszDevName[0]-'A'+1), FSIL_VOLSER, &FSInfo,
            sizeof (QFILEINFO)) == 0)
        {
        /* Warnmodus | Format || Meldung                    */
        /*   ein     |   ok.  || "formatiert"               */
        /*   aus     |   ok.  || keine                      */
        /*   ./.     | n.ok.  || "falsches Format"          */
        /*   ./.     |  leer  || keine                      */
        MPARAM PostPrm;

        bDataPacket = 1;                            /* Media BPB */
        ulcParmP = sizeof (BYTE);
        ulcDataP = sizeof (BIOSPARAMETERBLOCK);
        usFmt = FMT_EMPTY;
        if (DosDevIOCtl (hFile, IOCTL_DISK, DSK_GETDEVICEPARAMS,
                &bDataPacket, ulcParmP, &ulcParmP,
                &BPBData, ulcDataP, &ulcDataP) == 0)
            {
            if (memcmp (&BPBData, &astrucBPB[lBPBNum],
                    (size_t)&BPBData.abReserved - (size_t)&BPBData) == 0)
                usFmt = FMT_OK;
            else
                usFmt = FMT_NOT_OK;
            }
        PostPrm = 0;
        switch (usFmt)
            {
            case FMT_OK:
                if (usWrnMode==TRUE)
                    PostPrm = MPFROM2SHORT (BLCK_FORMAT, 0);
                break;
            case FMT_NOT_OK:
                PostPrm = MPFROM2SHORT (BLCK_WRONGFMT, 0);
                break;
            }

        /* Meldung ausgeben, falls erforderlich */
        if (PostPrm != 0)
            {
            DosResetEventSem (rsFormatBlocks, &ulFBPostCount);
            WinPostMsg (hwndFrame, WM_FMTBLOCK, PostPrm, (MPARAM) &FSInfo);
            DosWaitEventSem (rsFormatBlocks, (ULONG) SEM_INDEFINITE_WAIT);
            switch (usRetMsgBox)
                {
                case MBID_CANCEL:
                    fsError  = RTYPE_WARNING;
                    ulReturn = usRetMsgBox;
                    goto ERR_THREAD;

                case MBID_QUICK:
                    ulFmtMode = MBID_QUICK;
                    break;

                case MBID_BOOTS:
                    ulFmtMode = MBID_BOOTS;
                    break;
                }
            }

        /* Alte Seriennummer erhalten */
        /* Fehler in OS/2-Doku: Filesystem-ID nur mit Infolevel 2 */
        memcpy ((PBYTE) &Serial, &FSInfo.ulVolSerial, 4);
        }
    else
        FSInfo.ulVolSerial = 0L;

    /* Den ausgewÑhlten BPB in die Arbeitsstruktur kopieren */
    memcpy (&BPBData, &astrucBPB[lBPBNum], sizeof (BIOSPARAMETERBLOCK));

    if (FSInfo.ulVolSerial == 0L)
        {
        /* Neue Seriennummer berechnen (wie in FORMAT.COM) */
        DosGetDateTime ((PDATETIME) bBuffer);
        ulSerial = CalcCRC(bBuffer, sizeof (DATETIME));

        memcpy ((PBYTE) &Serial, &ulSerial, 4);
        }

    /* File-System fÅr Formatiervorgang vorbereiten */
    bParmPacket= bDataPacket= 0;
    ulcParmP   = ulcDataP   = 0;
    DosRequestMutexSem (rsWaitThread, (ULONG) SEM_INDEFINITE_WAIT);
    if ((ulReturn = DosDevIOCtl (hFile, IOCTL_DISK, DSK_LOCKDRIVE,
            &bParmPacket, 1, &ulcParmP,
            &bDataPacket, 1, &ulcDataP)) != 0)
        goto FMT_END;
    flDrvStat |= LOCKED;
    DosReleaseMutexSem (rsWaitThread);

    if (ulReturn != 0)
        goto ERR_THREAD;

    /* Setzen der Laufwerksparameter auf den angeforderten BPB */
    bParmPacket = REPLACE_BPB_FOR_MEDIUM;
    ulcParmP = 1;
    ulcDataP = sizeof (BIOSPARAMETERBLOCK);
    DosRequestMutexSem (rsWaitThread, (ULONG) SEM_INDEFINITE_WAIT);
    ulReturn = DosDevIOCtl (hFile, IOCTL_DISK, DSK_SETDEVICEPARAMS,
        &bParmPacket, 1, &ulcParmP,
        &BPBData, sizeof (BIOSPARAMETERBLOCK), &ulcDataP);
    flDrvStat |= PARMS;
    DosReleaseMutexSem (rsWaitThread);

    if (ulReturn != 0)
        goto ERR_THREAD;

    /* AusfÅllen der DISKINFO-Struktur */
    FSDiskInfo.uscSectorsize   = BPBData.usBytesPerSector;
    FSDiskInfo.bcClustersize   = BPBData.bSectorsPerCluster;
    FSDiskInfo.uscBootsectors  = BPBData.usReservedSectors;
    FSDiskInfo.uscUnitsize     = BPBData.cSectors;
    FSDiskInfo.uscDirEntries   = BPBData.cRootEntries;
    FSDiskInfo.bcFATCount      = BPBData.cFATs;
    FSDiskInfo.uscFATSize      = BPBData.usSectorsPerFAT;
    FSDiskInfo.uscUsedClusters = 0;
    FSDiskInfo.uscDefectsize   = 0;
    FSDiskInfo.ulDiskSerial    = Serial;
    FSDiskInfo.pszVolName      = FSInfoBuf.szLabel;

    /* Allokieren eines Puffers fÅr FAT und der Puffer fÅr READ/WRITE/FORMAT */
    /* Bis 4085Cluster(!) (32MByte) werden nur 12bit-FAT-EintrÑge verwendet  */
    usFATLen = BPBData.usSectorsPerFAT*BPBData.usBytesPerSector;
    usFATLen += usFATLen<<1;
    uscFormatPrm = sizeof (FORMATPRM) + (BPBData.usSectorsPerTrack-1)*sizeof (pFormatPrm->TrackTable);
    DosRequestMutexSem (rsWaitThread, (ULONG) SEM_INDEFINITE_WAIT);
    uscReadPrm   = sizeof (TRACKLAYOUT) + (BPBData.usSectorsPerTrack-1)*sizeof (pReadPrm->TrackTable);
    DosAllocMem ((PVOID)&pbFAT, usFATLen, PAG_COMMIT | PAG_READ | PAG_WRITE);
    DosAllocMem ((PVOID)&pFormatPrm, uscFormatPrm, PAG_COMMIT | PAG_READ | PAG_WRITE);
    DosAllocMem ((PVOID)&pReadPrm, uscReadPrm, PAG_COMMIT | PAG_READ | PAG_WRITE);
    flDrvStat |= ALLOC;
    DosReleaseMutexSem (rsWaitThread);
    memset (pbFAT, '\0', usFATLen);

    /* AusfÅllen der Track Layout Table fÅr Read/Write/Verify */
    for (uscRecord = 0; uscRecord < BPBData.usSectorsPerTrack; uscRecord++)
        {
        pReadPrm->TrackTable[uscRecord].usSectorNumber = uscRecord + 1;
        pReadPrm->TrackTable[uscRecord].usSectorSize   = BPBData.usBytesPerSector;
        }

    /* Verzweigung: normaler / schneller Formatiervorgang oder Bootsektor lîschen? */
    if (ulFmtMode == MBID_BOOTS)
        {
        /* Bootsektor lîschen */

        /* Wenn kein Label eingegeben wurde, wird das alte Åbernommen */
        if (FSInfoBuf.ccLenLabel == 0)
            {
            FSInfoBuf.ccLenLabel = FSInfo.ccLenLabel;
            strcpy (FSInfoBuf.szLabel, FSInfo.szLabel);
            }

        /* 1. FAT lesen */
        if ((ulReturn = DiskIO (pbFAT, BPBData.usSectorsPerFAT,
                BPBData.usReservedSectors, DSK_READTRACK)) != 0)
            goto FMT_END;

        /* ZÑhlen der belegten EintrÑge und bad sectors */
        for (usCluster=2; usCluster<BPBData.usSectorsPerFAT*BPBData.usBytesPerSector;
                usCluster += 2)
            {
            pusFAT = (USHORT *) (pbFAT+((usCluster/2)*3));
            if ((*pusFAT & 0x0FFF) == 0xFF7)
                FSDiskInfo.uscDefectsize++;
            else if ((*pusFAT & 0xFFF) != 0)
                FSDiskInfo.uscUsedClusters++;
            pusFAT = (USHORT *) (pbFAT+((usCluster/2)*3+1));
            if ((*pusFAT & 0xFFF0) == 0xFF70)
                FSDiskInfo.uscDefectsize++;
            else if ((*pusFAT & 0xFFF0) != 0)
                FSDiskInfo.uscUsedClusters++;
            }
        }
    else if (ulFmtMode == MBID_QUICK)
        {
        /* schnell formatieren */

        /* 1. FAT lesen */
        if ((ulReturn = DiskIO (pbFAT, BPBData.usSectorsPerFAT,
                BPBData.usReservedSectors, DSK_READTRACK)) != 0)
            goto FMT_END;

        /* Leeren der FAT bis auf die ersten 3 EintrÑge und bad sectors */
        for (usCluster=2; usCluster<BPBData.usSectorsPerFAT*BPBData.usBytesPerSector;
                usCluster += 2)
            {
            pusFAT = (USHORT *) (pbFAT+((usCluster/2)*3));
            if ((*pusFAT & 0x0FFF) != 0xFF7)
                *pusFAT &= 0xF000;
            else
                FSDiskInfo.uscDefectsize++;
            pusFAT = (USHORT *) (pbFAT+((usCluster/2)*3+1));
            if ((*pusFAT & 0xFFF0) != 0xFF70)
                *pusFAT &=0xF;
            else
                FSDiskInfo.uscDefectsize++;
            }
        }
    else if (ulFmtMode == MBID_OK)
        {
        /* normal formatieren */

        /* Setzen der ersten 2 EintrÑge der FAT */
        pbFAT[0] = BPBData.bMedia;
        pbFAT[1] = 0xFF;
        pbFAT[2] = 0xFF;

        /* Beginn der Formatierung */
        bDataPacket = 0;
        ulcDataP = ulcParmP = 1;
        DosRequestMutexSem (rsWaitThread, (ULONG) SEM_INDEFINITE_WAIT);
        if ((ulReturn = DosDevIOCtl (hFile, IOCTL_DISK, DSK_BEGINFORMAT,
                "", 1, &ulcParmP,
                &bDataPacket, 1, &ulcDataP)) != 0)
            goto FMT_END;
        flDrvStat |= FMT_TRACK;             /* REDETERMINE MEDIA nîtig */
        DosReleaseMutexSem (rsWaitThread);

        pFormatPrm->bCommand = 1;           /* Formatieren aufeinanderfolgender Tracks */
        pFormatPrm->uscTrack = 0;           /* Single Track Format */
        pFormatPrm->uscSect  = BPBData.usSectorsPerTrack;

        /* Zahl der Tracks des DatentrÑgers */
        usNumTrack = BPBData.cSectors /
                     BPBData.usSectorsPerTrack /
                     BPBData.cHeads;

        /***** F o r m a t i e r s c h l e i f e *****/
        for (uscTrack = 0; uscTrack < usNumTrack; uscTrack++)
            {
            usTrackOK = TRUE;
            for (uscHead = 0; uscHead < BPBData.cHeads; uscHead++)
                {
                for (uscRecord = 0; uscRecord < pFormatPrm->uscSect; uscRecord++)
                    {
                    pFormatPrm->TrackTable[uscRecord].bCylinder       = (BYTE)uscTrack;
                    pFormatPrm->TrackTable[uscRecord].bHead           = (BYTE)uscHead;
                    pFormatPrm->TrackTable[uscRecord].bSectorID       = (BYTE)(uscRecord + 1);
                    usSectSizeTmp = BPBData.usBytesPerSector >> 7;
                    for (usSizeCode = 0; usSizeCode < 3; usSizeCode++, usSectSizeTmp>>=1)
                        if (usSectSizeTmp & 1 == 1)
                            break;
                    pFormatPrm->TrackTable[uscRecord].bBytesPerSector = (BYTE)usSizeCode;
                    }
                pFormatPrm->usHead = uscHead;
                pFormatPrm->usCylinder = uscTrack;

                ulcParmP = uscFormatPrm;
                ulcDataP = 1;
                bDataPacket = 0;
                ulReturn = DosDevIOCtl (hFile, IOCTL_DISK, DSK_FORMATVERIFY,
                    pFormatPrm, uscFormatPrm, &ulcParmP,
                    &bDataPacket, 1, &ulcDataP);
                if (ulReturn == ERROR_CRC || ulReturn == ERROR_SECTOR_NOT_FOUND)
                    {
                    usTrackOK = FALSE;
                    pReadPrm->bCommand   = 0;
                    pReadPrm->usHead     = uscHead;
                    pReadPrm->usCylinder = uscTrack;
                    pReadPrm->cSectors   = 1;
                    for (uscRecord = 0; uscRecord < pFormatPrm->uscSect; uscRecord++)
                        {
                        pReadPrm->usFirstSector = uscRecord;
                        ulcParmP = uscReadPrm;
                        ulcDataP = 0;
                        ulReturn = DosDevIOCtl (hFile, IOCTL_DISK, DSK_READTRACK,
                                pReadPrm, uscReadPrm, &ulcParmP,
                                bBuffer, sizeof (bBuffer), &ulcDataP);
                        if (ulReturn == ERROR_NOT_READY)
                            goto FMT_END;
                        else if (ulReturn)
                            {
                            usCluster = ((uscTrack*BPBData.cHeads) + uscHead) *
                                BPBData.usSectorsPerTrack + uscRecord;
                            usCluster -= BPBData.usReservedSectors +
                                         BPBData.cFATs * BPBData.usSectorsPerFAT +
                                         (BPBData.cRootEntries*16-15)/BPBData.usBytesPerSector + 1;
                            if ((SHORT)usCluster < 0)
                                {
                                fsError  = RTYPE_USRERR;
                                ulReturn = ERROR_UNUSABLE;
                                goto FMT_END;               /* Diskette nicht verwendbar */
                                }
                            FSDiskInfo.uscDefectsize++;
                            usCluster = (usCluster / BPBData.bSectorsPerCluster) + 2;
                            if (usCluster % 2)
                                {
                                pusFAT = (USHORT *) (pbFAT+((usCluster/2)*3+1));
                                *pusFAT &= 0xF;
                                *pusFAT |= 0xFF70;
                                }
                            else
                                {
                                pusFAT = (USHORT *) (pbFAT+((usCluster/2)*3));
                                *pusFAT &= 0xF000;
                                *pusFAT |= 0xFF7;
                                }
                            }
                        }
                    }
                else if (ulReturn)
                    {
                    usTrackOK = FALSE;
                    goto FMT_END;
                    }
                }

            WinPostMsg (hwndFrame, WM_TRACK,
                MPFROM2SHORT (uscTrack, usNumTrack),
                MPFROMSHORT  (usTrackOK));
            }
        }

    /* Bootsektor schreiben */
    memcpy (&BiosBPB, &BPBData, (size_t)((BYTE)&EndOfBPB - (BYTE)&BiosBPB));
    if ((ulReturn = DiskIO (&BootSect, 1, 0, DSK_WRITETRACK)) != 0)
        goto FMT_END;

    if (ulFmtMode != MBID_BOOTS)
        {
        /* 1. FAT schreiben */
        if ((ulReturn = DiskIO (pbFAT, BPBData.usSectorsPerFAT,
                BPBData.usReservedSectors, DSK_WRITETRACK)) != 0)
            goto FMT_END;

        /* 2. FAT schreiben */
        if ((ulReturn = DiskIO (pbFAT, BPBData.usSectorsPerFAT,
                (USHORT) (BPBData.usReservedSectors + BPBData.usSectorsPerFAT),
                DSK_WRITETRACK)) != 0)
            goto FMT_END;

        /* Inhaltsverzeichnis schreiben */
        memset (bBuffer, '\0', BPBData.usBytesPerSector);
        usCluster = (USHORT) (BPBData.usReservedSectors +
                    BPBData.cFATs * BPBData.usSectorsPerFAT);
        for (uscRecord = 0;
                uscRecord < 1 + (BPBData.cRootEntries*16-15)/BPBData.usBytesPerSector; uscRecord++)
            if ((ulReturn = DiskIO (bBuffer, 1, (USHORT) (uscRecord+usCluster),
                    DSK_WRITETRACK)) != 0)
                goto FMT_END;
        }

FMT_END:
ERR_THREAD:
    Terminate (TERMTHREAD);

    if (!((fsError+ulReturn) || ((ulFmtMode==MBID_BOOTS) && !(FSInfoBuf.ccLenLabel))))
        DosSetFSInfo ((ULONG) (pszDevName[0]-'A'+1), 2, (PBYTE) &FSInfoBuf,
            sizeof (SFILEINFO));

ERR_OPEN:
    WinPostMsg (hwndFrame, WM_FMTEXIT,
        MPFROM2SHORT (fsError, ulReturn),
        MPFROMP (&FSDiskInfo));

    DosExit (0, 0);
    }

/********************************************************************
    UP fÅr DosExitList und zum direkten Aufruf
    Dieses UP ist verantwortlich fÅr:
    - LOCK auf DatentrÑger aufheben
    - CLOSE auf DatentrÑger
    falls nîtig.
    Falls das Programm wÑhrend Terminate beendet wird und der neue
    Zustand von 'flDrvStat' noch nicht gÅltig ist, kann der
    entsprechende Aufruf (REDETERMINE, UNLOCK, CLOSE) evtl. 2 mal
    auftreten, was jedoch nicht stîrt und deshalb nicht behandelt
    wird.
 ********************************************************************/
void APIENTRY Terminate (USHORT usReason)
    {
    if ((flDrvStat & FMT_TRACK) != 0)
        {
        bDataPacket = 0;
        bParmPacket = 0;
        ulcParmP = ulcDataP = 1;
        DosRequestMutexSem (rsWaitThread, (ULONG) SEM_INDEFINITE_WAIT);
        DosDevIOCtl (hFile, IOCTL_DISK, DSK_REDETERMINEMEDIA,
            &bParmPacket, 1, &ulcParmP,
            &bDataPacket, 1, &ulcDataP);
        flDrvStat &= ~FMT_TRACK;
        DosReleaseMutexSem (rsWaitThread);
        }

    if ((flDrvStat & ALLOC) != 0)
        {
        DosRequestMutexSem (rsWaitThread, (ULONG) SEM_INDEFINITE_WAIT);
        DosFreeMem (pReadPrm);
        DosFreeMem (pFormatPrm);
        DosFreeMem (pbFAT);
        flDrvStat &= ~ALLOC;
        DosReleaseMutexSem (rsWaitThread);
        }

    if ((flDrvStat & PARMS) != 0)
        {
        bParmPacket = BUILD_BPB_FROM_MEDIUM;    /* Originalwerte restaurieren */
        ulcParmP    = 1;
        ulcDataP    = sizeof (BPBData);
        DosRequestMutexSem (rsWaitThread, (ULONG) SEM_INDEFINITE_WAIT);
        DosDevIOCtl (hFile, IOCTL_DISK, DSK_SETDEVICEPARAMS,
            &bParmPacket, 1, &ulcParmP,
            &BPBData, sizeof (BPBData), &ulcDataP);
        flDrvStat &= ~PARMS;
        DosReleaseMutexSem (rsWaitThread);
        }

    if ((flDrvStat & LOCKED) != 0)
        {
        bParmPacket = bDataPacket = 0;
        ulcParmP = ulcDataP = 1;
        DosRequestMutexSem (rsWaitThread, (ULONG) SEM_INDEFINITE_WAIT);
        DosDevIOCtl (hFile, IOCTL_DISK, DSK_UNLOCKDRIVE,
            &bParmPacket, 1, &ulcParmP,
            &bDataPacket, 1, &ulcDataP);
        flDrvStat &= ~LOCKED;
        DosReleaseMutexSem (rsWaitThread);
        }

    if ((flDrvStat & OPEN) != 0)
        {
        DosRequestMutexSem (rsWaitThread, (ULONG) SEM_INDEFINITE_WAIT);
        DosClose (hFile);
        flDrvStat &= ~OPEN;
        DosReleaseMutexSem (rsWaitThread);
        }

    if (usReason != TERMTHREAD)
        DosExitList (EXLST_EXIT, NULL);
    }
