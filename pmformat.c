/*----------------------------------------------------------
   PMFORMAT.C -- Ein Formatierprogramm fÅr Disketten

   Modul 1: PM-OberflÑche

   Version 1.1 - 15.4.1991
   Version 1.2 - 9.6.1991   > Einbau von rsWaitThread;
                              Editbox behÑlt Inhalt nach Fehler
   Version 1.3 - 9.11.1991  > Einbau von WinMapDlgPoints:
                              HardwareunabhÑngige Koordinaten;
                              Box mit Diskettendaten nach Formatierproze·;
                              Fehler in ErrBox korrigiert;
   Version 1.31- 9.2.1992
   Version 2.0 - 20.9.1992  > OS/2 2.0-Version fÅr C Set/2
                              Einbau STOP-Button
                              erweiterter Modus mit Spin-Button
                              Lesen der Kommandozeilenargumente
   Version 2.11- 8.3.1994   > Korrigierte Versionsabfrage
   Version 2.12- 21.1.1995  > Warp-Fix in DRVINFO
   Version 2.13- 28.7.1995  > Ausbau der Shareware-EinschrÑnkung
   Version 2.14- 13.2.1997  > VollstÑndiger Ausbau der Shareware-
                              EinschrÑnkung
   Version 2.16 - 4.5.2000  > DosSetCurrentDir wegen Help-Datei
  ----------------------------------------------------------*/
#define  INCL_WIN
#define  INCL_DOSERRORS
#define  INCL_DOSPROCESS
#define  INCL_DOSDEVIOCTL
#define  INCL_DOSMEMMGR
#define  INCL_DOSMODULEMGR
#define  INCL_DOSFILEMGR
#define  INCL_DOSNLS
#define  INCL_DOSMISC
#define  INCL_DOSSEMAPHORES
#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include "format.h"
#include "pmfmthlp.h"
#include <crypt.h>

#define STACKSIZE  16384                /* Stackgrî·e des Formatier-Threads */
#define BUFFLEN     1024                /* LÑnge aller ASCIIZ-Puffer */
#define ARGLEN        12                /* LÑnge des Argumentes von main() */
#define TITLELEN      80                /* LÑnge des Titels des Hilfefensters */
#define LIBNAMELEN    13                /* LÑnge des Namens der Help-Library */
#define APP_NAME    "PMFORMAT"
#define MSGFILE     "OSO001.MSG"
#define INV_CHAR    "\t\r\n.*?"         /* Zeichen, die in Vol-Labels nicht */
                                        /*   auftreten dÅrfen               */

#define DI2UL(mp) (ULONG)((PDISKINFO)mp)

SHORT   sTestMode;                      /* Disktest beim Laufwerkswechsel: Ein=True*/
LONG    flDrvStat;                      /* Zustand des DISK-DD (0|OPEN|LOCKED|...) */
USHORT  usAppMode;                      /* Modus des Programmes: Standard/Extended */
USHORT  usWrnMode = TRUE;               /* Warnmodus: Ein=TRUE                     */
ULONG   ulFmtMode = MBID_OK;            /* Formatiermodus: MBID_* von wpWarning    */

BOOL    bIsHelp = FALSE;                /* TRUE: Help verfÅgbar */

USHORT  uscDrives;                      /* Rechnerkonf.: Laufwerkszahl */
USHORT  uscDiskettes;                   /* Zahl der Diskettenlaufwerke */
PCHAR   pszDevName;                     /* Laufwerksname (ASCIIZ) */
ULONG   ulDevice;                       /* ausgewÑhltes Laufwerk */
PCHAR   pszDrives;                      /* Stringarray fÅr Spinbutton */
PCHAR   *ppszDrives;                    /* Zeiger auf Stringarray fÅr Spinbutton */
ULONG   ulBootDrive;                    /* Boot-Laufwerk 1='A', ... */

USHORT  usRetMsgBox;                    /* Formatieren abbrechen? MBID_YES|NO|CANCEL */

HEV     rsFormatBlocks;                 /* Semaphoren */
ULONG   ulFBPostCount;
HMTX    rsWaitThread;

SFILEINFO  FSInfoBuf;                   /* Puffer fÅr DosSetFSInfo */
QFILEINFO  FSInfo;                      /* Puffer fÅr DosQueryFSInfo */
DISKINFO   FSDiskInfo;                  /* Daten der formatierten Diskette */
PDRIVEINFO pFSDriveInfo;                /* Zeiger auf DriveInfo-Segment */

HWND    hwndFrame;
HWND    hwndTrack = 0L;
HWND    hwndLabel = 0L;
HWND    hwndStatic = 0L;
HWND    hwndHelp = 0L;
HAB     hab;

HELPINIT hini;

TID     idFThread;                      /* Formatierthread */
TARG    arFThreadArg;                   /*   Argumentstruktur */
TID     idTThread;                      /* DiskType-Thread */
SDTARG  arTThreadArg;                   /*   Argumentstruktur */

static CHAR szBuffer[BUFFLEN];          /* Puffer fÅr div. Anwendungen */
static CHAR szArgs[ARGLEN+1];           /* Puffer fÅr Argument des Programmes */
static CHAR szWindowTitle[TITLELEN];    /* Puffer fÅr Titel des Hilfefensters */
static CHAR szLibName[LIBNAMELEN];      /* Puffer fÅr Name der Help-Library */

static POINTL arPoints[6] = {{14,82},{117,8},   /* DlgPts: Editbox           */
                             {15,93},{110,8},   /*         Text Åber Editbox */
                             {3,75},{137,20}};  /*         Trackfenster      */

/***********************************
    H a u p t p r o g r a m m
 **********************************/
int main (int argc, char *argv[])
    {
    HMQ      hmq;
    QMSG     qmsg;
    SWCNTRL  switchCntrl;
    COUNTRYCODE strucCtry;
    PCHAR    pszDrvArr;
    USHORT   i;
    ULONG    ulVersion[2];
    PTIB     ptib = NULL;
    PPIB     ppib = NULL;

    flDrvStat = 0;                              /* flDrvStat initialisieren */

    DosError (FERR_DISABLEHARDERR);                     /* keine Harderror-Popups */
    DosExitList (EXLST_ADD, (PFNEXITLIST) Terminate);   /* UP fÅr DosExitList festlegen */

    DosQuerySysInfo (QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, &ulBootDrive, sizeof (ulBootDrive));
    uscDrives = FSDriveData (&pFSDriveInfo);    /* DriveInfo-Struktur allokieren und auffÅllen */

    /* Laufwerksarray fÅr Spinbutton erzeugen und fÅllen */
    DosAllocMem ((PVOID) &pszDrives,  (ULONG) (uscDrives * 3),
        PAG_READ | PAG_WRITE | PAG_COMMIT);
    DosAllocMem ((PVOID) &ppszDrives, (ULONG) (uscDrives*sizeof (PVOID)),
        PAG_READ | PAG_WRITE | PAG_COMMIT);
    for (uscDiskettes = i = 0, pszDrvArr = pszDrives; i < uscDrives; i++)
        {
        if (IsDiskette ((USHORT) (i+1)))
            {
            ppszDrives[uscDiskettes] = pszDrvArr;
            *pszDrvArr++ = (CHAR) ('A' + i);
            *pszDrvArr++ = ':';
            *pszDrvArr++ = '\0';
            uscDiskettes++;
            }
        }

    /* selektiertes Laufwerk bestimmen ("aktuelles Laufwerk") */
    strucCtry.country  = 0L;
    strucCtry.codepage = 0L;
    ulDevice = 1;                               /* mit 'A' vorbelegen, falls kein Argument */
    if (argc > 1)
        {
        strncpy (szArgs, argv[1], ARGLEN);
        DosMapCase (1, &strucCtry, szArgs);
        ulDevice = (ULONG)(*szArgs - (USHORT)('A')+1);
        if (!IsDiskette ((USHORT) (ulDevice)))
            return 1;
        }

    /* Semaphoren anlegen */
    DosCreateEventSem (NULL, &rsFormatBlocks, 0L, TRUE);
    DosCreateMutexSem (NULL, &rsWaitThread,   0L, FALSE);

    /* aktuelles Verzeichnis setzen, falls Help-Datei nicht im HELP-Pfad ist */
    if (DosGetInfoBlocks (&ptib, &ppib) == NO_ERROR)
        {
        if (ppib != NULL)
            {
            CHAR    szDirectory[CCHMAXPATH];
            HMODULE hMod = ppib->pib_hmte;

            if (DosQueryModuleName (hMod, CCHMAXPATH, szDirectory) == NO_ERROR)
                {
                PSZ psz = strrchr (szDirectory, '\\');
                if (psz)
                    {
                    *psz = '\0';
                    DosSetCurrentDir (szDirectory);
                    }
                }
            }
        }

    /* PM Initialisierung und Hauptschleife */
    hab = WinInitialize (0);
    hmq = WinCreateMsgQueue (hab, 0);

    DosQuerySysInfo (QSV_VERSION_MAJOR, QSV_VERSION_MINOR, ulVersion, sizeof (ulVersion));
        if (((ulVersion[0] << 16) | (ulVersion[1] & 0xFFFF)) <
            ((VER_MAJOR << 16) | (VER_MINOR & 0xFFFF)))
            {
            ErrBox (HWND_DESKTOP, MPFROM2SHORT (RTYPE_USRERR, IDS_VERSION));
            return 1;
            }

    WinRegisterClass (hab, "Tracks", (PFNWP) wpTrackWnd, 0L, 0);

    hwndFrame = WinLoadDlg (HWND_DESKTOP,   /* Dialogbox laden */
                            HWND_DESKTOP,
                            (PFNWP) DialogWndProc,
                            0,
                            ID_FORMAT,
                            NULL);

    switchCntrl.hwnd          = hwndFrame;
    switchCntrl.hwndIcon      = 0;
    switchCntrl.hprog         = 0L;
    switchCntrl.idProcess     = 0 ;
    switchCntrl.idSession     = 0;
    switchCntrl.uchVisibility = SWL_VISIBLE;
    switchCntrl.fbJump        = SWL_JUMPABLE;
    WinLoadString (hab, 0, IDS_SWENTRY, MAXNAMEL + 1,
        switchCntrl.szSwtitle);
    WinCreateSwitchEntry (hab, &switchCntrl);

    WinSendMsg (hwndFrame,                  /* Icon laden */
                WM_SETICON,
                (MPARAM) WinLoadPointer (HWND_DESKTOP, 0, ID_ICON),
                0L);

    /*  Message-Schleife */
    while (WinGetMsg (hab, &qmsg, 0L, 0L, 0L))
         WinDispatchMsg (hab, &qmsg);

    /*  Programmende */
    if (bIsHelp) WinDestroyHelpInstance (hwndHelp);
    WinDestroyWindow (hwndFrame);
    WinDestroyMsgQueue (hmq);
    WinTerminate (hab);
    DosFreeMem (pszDrives);
    DosFreeMem (ppszDrives);
    return 0;
    }

/***********************************************************************
    PrÅft, ob ASCIIZ-String ungÅltiges Zeichen enthÑlt. Die Zeichen,
    die als ungÅltig gelten, stammen aus dem String INV_CHAR.
    Eingang: szString : Zeiger auf ASCIIZ-String
    Return: FFFF : kein ungÅltiges Zeichen
            sonst: Index des ersten ungÅltigen Zeichens
 ***********************************************************************/
USHORT IsInvalidChar (CHAR *szString)
    {
    CHAR *szStrPtr;

    szStrPtr = szString;
    while (*szStrPtr != '\0')
        if (strchr (INV_CHAR, *szStrPtr++))
            return (USHORT)(szStrPtr-szString-1);
    return 0xFFFF;
    }

/***********************************************************************
    Erzeugt eine Editbox fÅr den Disklabel in der Dialogbox.
    Gleichzeitig wird die PUSHBUTTON-Beschriftung auf 'formatieren' gesetzt.
    Eingang: hwnd : Owner/Parent window handle
 ***********************************************************************/
void CreateEdit (HWND hwnd)
    {
    if (hwndTrack)
        {
        WinDestroyWindow (hwndTrack);
        hwndTrack = 0L;
        }
    WinLoadString (hab, 0, IDS_LABEL, BUFFLEN, szBuffer);

    hwndLabel = WinCreateWindow (hwnd, WC_ENTRYFIELD, FSInfoBuf.szLabel,
        WS_VISIBLE | WS_TABSTOP | WS_GROUP | ES_MARGIN,
        (SHORT) arPoints[0].x, (SHORT) arPoints[0].y,
        (SHORT) arPoints[1].x, (SHORT) arPoints[1].y,
        hwnd, HWND_TOP, DID_ENTRY, NULL, NULL);
    hwndStatic = WinCreateWindow (hwnd, WC_STATIC, szBuffer,
        WS_VISIBLE | SS_TEXT | DT_TOP | DT_LEFT,
        (SHORT) arPoints[2].x, (SHORT) arPoints[2].y,
        (SHORT) arPoints[3].x, (SHORT) arPoints[3].y,
        hwnd, HWND_TOP, 0, NULL, NULL);
    WinSendMsg (hwndLabel, EM_SETTEXTLIMIT, MPFROMSHORT (11), 0L);
    WinSendMsg (hwndLabel, EM_SETSEL, MPFROM2SHORT (0, 11), 0L);

    /* Button wieder auf 'formatieren' setzen */
    WinLoadString (hab, 0, IDS_BTN_FMT, BUFFLEN, szBuffer);
    WinSetDlgItemText (hwnd, DID_FORMAT, szBuffer);

    /* Titelzeile: Disklabel wieder entfernen */
    WinLoadString (hab, 0, IDS_TITLE_STD, BUFFLEN, szBuffer);
    WinSetDlgItemText (hwndFrame, FID_TITLEBAR, szBuffer);
    }

/***********************************************************************
    Erzeugt die Text- und Grafikausgabe fÅr den Track-ZÑhler.
    Gleichzeitig wird die PUSHBUTTON-Beschriftung auf 'STOP' gesetzt.
    Eingang: hwnd : Owner/Parent window handle
 ***********************************************************************/
void CreateTrackCtr (HWND hwnd)
    {
    if (hwndLabel)
        {
        WinDestroyWindow (hwndLabel);
        WinDestroyWindow (hwndStatic);
        hwndStatic = hwndLabel = 0;
        }
    hwndTrack = WinCreateWindow (hwnd, "Tracks", NULL, WS_VISIBLE,
        (SHORT) arPoints[4].x, (SHORT) arPoints[4].y,
        (SHORT) arPoints[5].x, (SHORT) arPoints[5].y,
        hwnd, HWND_TOP, DID_TRACKS, NULL, NULL);

    /* Button: Text 'STOP' laden */
    WinLoadString (hab, 0, IDS_BTN_STOP, BUFFLEN, szBuffer);
    WinSetDlgItemText (hwnd, DID_FORMAT, szBuffer);

    /* Falls Editbox Text enthÑlt: in Titelzeile kopieren */
    if (FSInfoBuf.ccLenLabel)
        {
        WinLoadString (hab, 0, IDS_TITLE_EXT, BUFFLEN, szBuffer);
        strcat (szBuffer, FSInfoBuf.szLabel);
        WinSetDlgItemText (hwndFrame, FID_TITLEBAR, szBuffer);
        }
    }

/***********************************************************************
    Erzeugt eine Message-Box mit einer Fehlermeldung.
    Eingang: hwnd : Owner window handle
             mpErrCode : SHORT1 : RTYPE_SYSERR => SHORT2 : DOS-Error
                         SHORT1 : RTYPE_USRERR => SHORT2 : Fehlerstring aus .RES
 ***********************************************************************/
void ErrBox (HWND hwnd, MPARAM mpErrCode)
    {
    ULONG ulMsgLen;
    CHAR *pszSrcPtr, *pszDstPtr, *pszBuffer;

    DosAllocMem ((PVOID)&pszBuffer, BUFFLEN, PAG_COMMIT | PAG_WRITE | PAG_READ);
    pszDstPtr = pszSrcPtr = pszBuffer;

    if (SHORT1FROMMP (mpErrCode) != RTYPE_SYSERR)
        WinLoadString (hab, 0, SHORT2FROMMP (mpErrCode), BUFFLEN,
            pszSrcPtr);
    else
        {
        /* Message aus OS/2-Fehlerdatei laden */
        DosGetMessage (NULL, 0L, pszSrcPtr, BUFFLEN,
            SHORT2FROMMP (mpErrCode), MSGFILE, &ulMsgLen);

        /* CR-LF-Sequenzen entfernen */
        do
            {
            if (*pszSrcPtr == '\n' || *pszSrcPtr == '\r')
                {
                if (pszDstPtr != pszBuffer &&
                    *(pszDstPtr-1) != ' ')
                    *pszDstPtr++ = ' ';
                }
            else
                *pszDstPtr++ = *pszSrcPtr;
            pszSrcPtr++;
            } while (--ulMsgLen != 0);
        *pszDstPtr = '\0';
        }

    /* Ausgabe der Message */
    WinMessageBox (HWND_DESKTOP, hwnd, pszBuffer, 0L, 0, MB_OK | MB_ERROR);

    DosFreeMem (pszBuffer);
    }

/***********************************************************************
    D i a l o g  -  W i n d o w   P r o c e d u r e
 ***********************************************************************/
MRESULT EXPENTRY DialogWndProc(HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2)
    {
    static  HWND     hwndCtls[LAST_DID - FIRST_DID + 1];
    static  PULONG   pulBtnID;
    static  HPOINTER hptrWait, hptrArrow;
    static  BOOL     bIsTThread;
    USHORT  i, usCharPos;
    ULONG   ulBytes;
    BOOL    bool;
    CHAR    szID1[5], szID2[5];
    SWP     swpFrame;
    CHAR    szInteger [6];

    /* Variable fÅr System-MenÅ-EintrÑge */
    static MENUITEM mi[4] =
        {
        {MIT_END, MIS_SEPARATOR, 0L, 0L,         0L, 0L},
        {MIT_END, MIS_TEXT,      0L, IDM_ABOUT,  0L, 0L},
        {MIT_END, MIS_TEXT,      0L, IDM_OPTION, 0L, 0L},
        {MIT_END, MIS_TEXT,      0L, IDM_HELP,   0L, 0L}
        };
    HWND        hwndSysMenu, hwndSysSubMenu;
    MENUITEM    miSysMenu;
    USHORT      sItem, idSysMenu;

    switch (msg)
        {
/*----- Window Message WM_INITDLG          */
/*      Radiobuttons richtig voreinstellen */
        case WM_INITDLG:
            /*  Hilfe initialisieren */
            hini.cb = sizeof (HELPINIT);
            hini.ulReturnCode = 0;
            hini.pszTutorialName = NULL;
            hini.phtHelpTable = (PHELPTABLE)MAKEULONG (IDH_MAIN, 0xFFFF);
            hini.hmodHelpTableModule = NULLHANDLE;
            hini.hmodAccelActionBarModule = NULLHANDLE;
            hini.idAccelTable = 0;
            hini.idActionBar = 0;
            hini.fShowPanelId = CMIC_HIDE_PANEL_ID;

            WinLoadString(hab, NULLHANDLE, IDS_HELPWINDOWTITLE, TITLELEN, szWindowTitle);
            hini.pszHelpWindowTitle = (PSZ)szWindowTitle;

            WinLoadString(hab, NULLHANDLE, IDS_HELPLIBRARYNAME, LIBNAMELEN, szLibName);
            hini.pszHelpLibraryName = (PSZ)szLibName;

            hwndHelp = WinCreateHelpInstance (hab, &hini);
            if (!hwndHelp)
                ErrBox (hwnd, MPFROM2SHORT (RTYPE_USRERR, IDS_NOHELP));
            else
                {
                WinAssociateHelpInstance (hwndHelp, hwnd);
                bIsHelp = TRUE;
                }

            /* Umrechnen der Dlg-Points in Pixel fÅr die Positionierung */
            /* der Editbox und der Track-Anzeige */
            WinMapDlgPoints (hwnd, arPoints,
                sizeof (arPoints) / sizeof (POINTL), TRUE);

            /* Eintragen von "ABOUT", "OPTION" und "HELP" in das System-MenÅ */
            mi[3].hwndSubMenu = WinLoadMenu (hwnd, NULLHANDLE, IDM_HELPMENU);

            hwndSysMenu = WinWindowFromID (hwnd, FID_SYSMENU);

            idSysMenu = SHORT1FROMMR (WinSendMsg (hwndSysMenu,
                MM_ITEMIDFROMPOSITION, NULL, NULL));

            WinSendMsg (hwndSysMenu, MM_QUERYITEM,
                MPFROM2SHORT (idSysMenu, FALSE),
                MPFROMP (&miSysMenu));

            hwndSysSubMenu = miSysMenu.hwndSubMenu;

            for (sItem = 0; sItem < 4; sItem++)
                {
                ULONG ulStringID;

                switch (sItem)
                    {
                    case 1:
                        ulStringID = IDS_ABOUT;
                        break;
                    case 2:
                        ulStringID = IDS_OPTION;
                        break;
                    case 3:
                        ulStringID = IDS_HELP;
                        break;
                    }
                WinLoadString (hab, 0, ulStringID, BUFFLEN, szBuffer);
                WinSendMsg (hwndSysSubMenu, MM_INSERTITEM,
                    MPFROMP (mi + sItem),
                    MPFROMP (szBuffer));
                }

            /* Initialisierungen */
            bIsTThread = FALSE;             /* SetDiskType-Thread nicht gestartet */
            hptrWait  = WinQuerySysPointer (HWND_DESKTOP, SPTR_WAIT, FALSE);
            hptrArrow = WinQuerySysPointer (HWND_DESKTOP, SPTR_ARROW, FALSE);

            *FSInfoBuf.szLabel = '\0';
            FSInfoBuf.ccLenLabel = 0;
            CreateEdit (hwnd);
            WinSetDlgItemText (hwnd, DID_ENTRY, szArgs+1);

            WinFocusChange (HWND_DESKTOP,
                WinWindowFromID (hwnd, DID_FORMAT), 0);

            WinMultWindowFromIDs (hwnd, hwndCtls, FIRST_DID, LAST_DID);

            /* Fensterposition festlegen */
            WinSetWindowPos(hwnd, 0L,
                PrfQueryProfileInt (HINI_USERPROFILE, APP_NAME, "WindowPos.x", 8),
                PrfQueryProfileInt (HINI_USERPROFILE, APP_NAME, "WindowPos.y", 8),
                0L, 0L, SWP_MOVE);

            /* Initialisierung Groupbox 'Laufwerke' */
            if (ulDevice > 2)
                usAppMode = MODE_EXT;
            else
                usAppMode = (USHORT) PrfQueryProfileInt (HINI_USERPROFILE, APP_NAME,
                    "AppMode", MODE_STD);
            WinSendDlgItemMsg (hwnd, DID_LW_SPIN, SPBM_SETTEXTLIMIT, MPFROMSHORT (3), 0L);
            WinSendDlgItemMsg (hwnd, DID_LW_SPIN, SPBM_SETARRAY, ppszDrives, MPFROMSHORT (uscDiskettes));
            WinSendMsg (hwnd, WM_REDRAW, 0L, 0L);

            /* Åbrige Profiledaten rÅcklesen */
            sTestMode = (SHORT) PrfQueryProfileInt (HINI_USERPROFILE, APP_NAME,
                "TestMode", TEST_STD);

            /* Initialisierung Groupbox 'Typ' */
            WinSendMsg (hwnd, WM_QDISKTYPE_B, 0L, 0L);

            /* Initialisierung Formatierbutton (DID_FORMAT) */
            WinSetWindowULong (hwndCtls[DID_FORMAT-FIRST_DID], QWL_USER, IS_FORMAT);

            return 0;

/*----- Window Message WM_COMMAND */
/*      WinDefDlgProc darf nicht aufgerufen werden, da diese WinDismissDlg  */
/*      aufruft. Damit wird die Dialog-Box beendet, die Message-Schleife im */
/*      Hauptprogramm lÑuft jedoch endlos weiter und wartet auf WM_QUIT.    */
        case WM_COMMAND:
            switch (COMMANDMSG(&msg)->cmd)
                {
                case DID_FORMAT:
                    switch (WinQueryWindowULong (hwndCtls[DID_FORMAT-FIRST_DID], QWL_USER))
                        {
                        case IS_FORMAT:
                            WinSetWindowULong (hwndCtls[DID_FORMAT-FIRST_DID], QWL_USER, IS_IGNORE);

                            /* Disk-Label auslesen und verifizieren */
                            FSInfoBuf.ccLenLabel = (BYTE) WinQueryDlgItemText (hwnd,
                                DID_ENTRY, 12, FSInfoBuf.szLabel);
                            usCharPos = IsInvalidChar (FSInfoBuf.szLabel);
                            if (usCharPos != 0xFFFF)
                                {
                                WinSendMsg (hwndLabel, EM_SETSEL,
                                    MPFROM2SHORT (usCharPos, usCharPos+1), 0L);
                                WinLoadString (hab, 0, IDS_INVALID_CHARACTER,
                                    BUFFLEN, szBuffer);
                                WinMessageBox (HWND_DESKTOP, hwnd,
                                    szBuffer, NULL, 0, MB_OK | MB_ICONEXCLAMATION);
                                WinFocusChange (HWND_DESKTOP, hwndLabel, 0);
                                WinSetWindowULong (hwndCtls[DID_FORMAT-FIRST_DID],
                                    QWL_USER, IS_FORMAT);
                                break;
                                }

                            /* Formatier-Thread erzeugen */
                            arFThreadArg.ulDiskType = (ULONG) WinSendDlgItemMsg (hwnd, DID_DD,
                                BM_QUERYCHECKINDEX, 0L, 0L);
                            arFThreadArg.pFSDriveInfo = &pFSDriveInfo[ulDevice-1];
                            arFThreadArg.pulBtnID = pulBtnID;
                            DosCreateThread (&idFThread,
                                (PFNTHREAD) FormatThread, (ULONG) &arFThreadArg, 0L, STACKSIZE);
                            CreateTrackCtr (hwnd);
                            WinSetWindowULong (hwndCtls[DID_FORMAT-FIRST_DID], QWL_USER, IS_STOP);
                            break;

                        case IS_STOP:
                            WinSetWindowULong (hwndCtls[DID_FORMAT-FIRST_DID], QWL_USER, IS_IGNORE);

                            /* Formatier-Thread terminieren, wenn */
                            /* rsWaitThread-Semaphore frei ist.   */
                            DosRequestMutexSem (rsWaitThread, (ULONG) SEM_INDEFINITE_WAIT);
                            DosKillThread (idFThread);
                            DosReleaseMutexSem (rsWaitThread);
                            Terminate (TERMTHREAD);
                            WinSendMsg (hwnd, WM_FMTEXIT, MPFROM2SHORT (RTYPE_INTERRUPT, 0), 0L);
                            WinSetWindowULong (hwndCtls[DID_FORMAT-FIRST_DID], QWL_USER, IS_FORMAT);
                            break;

                        case IS_IGNORE:
                            return 0;   /* WinInvalidateRect wird nicht benîtigt */
                        }
                    WinInvalidateRect (hwnd, NULL, TRUE);
                    return 0;

                case IDM_ABOUT:
                    /* About Box anzeigen */
                    WinDlgBox (HWND_DESKTOP, hwnd, (PFNWP) wpAbout, 0,
                        ID_ABOUT, NULL);
                    return 0;

                case IDM_OPTION:
                    /* Options Dialogbox anzeigen */
                    WinDlgBox (HWND_DESKTOP, hwnd, (PFNWP) wpOptions, 0,
                        ID_OPTIONS, NULL);
                    return 0;
                }
            return 0;

/*----- Window Message WM_CONTROL */
/*      wird bei jedem Laufwerkswechsel benutzt, um die */
/*      erlaubten Diskettenformate zu bestimmen         */
        case WM_CONTROL:
            if (usAppMode == MODE_EXT && mp1 == MPFROM2SHORT (DID_LW_SPIN, SPBN_ENDSPIN))
                {
                /* erweiterter Modus: Laufwerksbuchstabe von Spinbutton */
                WinSendDlgItemMsg (hwnd, DID_LW_SPIN, SPBM_QUERYVALUE,
                    &ulDevice, MPFROM2SHORT (0, SPBQ_DONOTUPDATE));
                ulDevice++;
                }
            else if (usAppMode == MODE_STD &&
                    (mp1 == MPFROM2SHORT (DID_LW_A, BN_CLICKED) ||
                     mp1 == MPFROM2SHORT (DID_LW_B, BN_CLICKED)))
                /* Standardmodus: Laufwerksbuchstabe von Radiobutton */
                ulDevice = (ULONG) WinSendDlgItemMsg (hwnd, DID_LW_A,
                    BM_QUERYCHECKINDEX, 0L, 0L);
            else
                /* sonstiger Button der Applikation */
                break;

            WinSendMsg (hwnd, WM_QDISKTYPE_B, 0L, 0L);
            return 0;

/*----- Window Message WM_HELP */
        case WM_HELP:
            if (bIsHelp && (SHORT1FROMMP (mp2)==CMDSRC_MENU)
                    && (SHORT1FROMMP (mp1)==IDM_HFH))
                {
                WinSendMsg (hwndHelp, HM_DISPLAY_HELP,
                    MPFROM2SHORT (PANEL_HFH, 0), MPFROMSHORT (HM_RESOURCEID));
                return 0;
                }
            break;

/*----- Window Message WM_SYSCOMMAND */
        case WM_SYSCOMMAND:
            if ((SHORT1FROMMP (mp2)==CMDSRC_MENU) && (SHORT1FROMMP (mp1)==SC_HELPEXTENDED))
                {
                WinSendMsg (hwndHelp, HM_EXT_HELP, 0L, 0L);
                return 0;
                }
            break;

/*----- Window Message WM_WINDOWPOSCHANGED */
/*      FÅr den Fall von SWP.fs == SWP_MINIMIZE oder SWP_RESTORE mu· das          */
/*      Frame-Window wieder sichtbar gemacht werden, da in der vorangegangenen    */
/*      WM_MINMAXFRAME-Message das Fenster unsichtbar gesetzt wurde, um die       */
/*      AktivitÑten (MINIMIZE, Icon anzeigen) zu verstecken.                      */
/*      SWP_MAXIMIZE kann nicht auftreten, da diese Funktion nicht aktiviert ist. */
        case WM_WINDOWPOSCHANGED:
            if (((PSWP) (mp1))->fl & SWP_MINIMIZE ||
                ((PSWP) (mp1))->fl & SWP_RESTORE)
                WinShowWindow (hwnd, TRUE);
            break;

/*----- Window Message WM_MINMAXFRAME */
/*      Tritt SWP_MINIMIZE auf, wird erst das Frame-Window                     */
/*      unsichtbar gemacht, anschlie·end werden sÑmtliche Controls der Dialog- */
/*      Box unsichtbar gemacht, damit diese spÑter das Icon nicht Åberdecken.  */
/*      Bei SWP_RESTORE wird der Vorgang rÅckgÑngig gemacht.                   */
        case WM_MINMAXFRAME:
            bool = ((PSWP) (mp1))->fl & SWP_MINIMIZE ? (BOOL) FALSE : (BOOL) TRUE;
            WinShowWindow (hwnd, FALSE);        /* Hide Frame Window */

            for (i=0; i< (LAST_DID - FIRST_DID + 1); i++)
                if (hwndCtls[i])
                    WinShowWindow (hwndCtls[i], bool);
            return FALSE;                       /* Perform Default Actions */

/*----- Window Message WM_SAVEAPPLICATION */
/*      Tritt WM_SAVEAPPLICATION auf, so wird die augenblickliche Window-      */
/*      Position im Profile OS2.INI gespeichert. Dazu werden die Daten in      */
/*      eine SWP-Struktur geholt und Åber ein WinWriteProfileString            */
/*      eingetragen.                                                           */
        case WM_SAVEAPPLICATION:
            WinQueryWindowPos(hwndFrame, &swpFrame);
            PrfWriteProfileString(HINI_USERPROFILE, APP_NAME, "WindowPos.x",
                _itoa(swpFrame.x, szInteger, 10));
            PrfWriteProfileString(HINI_USERPROFILE, APP_NAME, "WindowPos.y",
                _itoa(swpFrame.y, szInteger, 10));
            break;

/*----- Window Message WM_FMTEXIT: User defined */
        case WM_FMTEXIT:
            if (mp1 == 0)
                {
                WinAlarm (HWND_DESKTOP, WA_NOTE);
                WinDlgBox (HWND_DESKTOP, hwnd, (PFNWP) wpDiskData, 0,
                    ID_DISKSTAT, &FSDiskInfo);
                *FSInfoBuf.szLabel = '\0';
                FSInfoBuf.ccLenLabel = 0;
                }
            else if (SHORT1FROMMP (mp1) == RTYPE_SYSERR ||
                     SHORT1FROMMP (mp1) == RTYPE_USRERR)
                {
                WinAlarm (HWND_DESKTOP, WA_ERROR);
                ErrBox (hwnd, mp1);
                }
            WinSetWindowULong (hwndCtls[DID_FORMAT-FIRST_DID], QWL_USER, IS_FORMAT);
            flDrvStat = 0;                      /* flDrvStat initialisieren */
            CreateEdit (hwnd);
            WinInvalidateRect (hwnd, NULL, TRUE);
            return 0;

/*----- Window Message WM_FMTBLOCK: User defined */
        case WM_FMTBLOCK:
            {
            static WRN_CREATE strucCreate;
            PCHAR   lvTable [3];
            ULONG   ulcTable, ulStringID;

            /* Zeiger auf Diskettennamen in lvTable[0], falls vorhanden */
            ulcTable = 3;
            lvTable[0] = (PCHAR)(mp2) + 5;
            lvTable[1] = lvTable[2] = "";
            /* Text aus Resource laden */
            if (*((PBYTE)mp2+4)==0)
                ulStringID = IDS_NONAME_FORMATTED;
            else
                ulStringID = IDS_ALREADY_FORMATTED;
            ulBytes = (ULONG)(USHORT)WinLoadString (hab, 0, ulStringID, BUFFLEN, szBuffer);
            /* Volume ID vorhanden? ja => Text an vorhergehenden anhÑngen */
            if (*((PULONG)(mp2)) != 0L)
                {
                ulBytes += WinLoadString (hab, 0, IDS_DISK_ID,
                    (LONG) (BUFFLEN - ulBytes), szBuffer + ulBytes);
                lvTable[2] = _itoa (*((PUSHORT)(mp2)),
                    szID2, 16);
                lvTable[1] = _itoa (*((PUSHORT)(mp2)+1),
                    szID1, 16);
                }

            switch (SHORT1FROMMP (mp1))
                {
                /* Disk bereits formatiert; Trotzdem formatieren? */
                case BLCK_FORMAT:
                    /* Frage, ob formatieren anhÑngen */
                    ulBytes += WinLoadString (hab, 0, IDS_QST_FORMAT,
                        (LONG) (BUFFLEN - ulBytes), szBuffer + ulBytes);
                    strucCreate.flMsgStyle = MBW_OK | MBW_CANCEL | MBW_QUICK |
                        MBW_BOOTS | MB_WARNING | MB_DEFBUTTON2;
                    break;

                /* Disk bereits formatiert, jedoch anderes Format */
                case BLCK_WRONGFMT:
                    ulBytes += WinLoadString (hab, 0, IDS_WRONGFMT,
                        (LONG) (BUFFLEN - ulBytes), szBuffer + ulBytes);
                    ulBytes += WinLoadString (hab, 0, IDS_QST_FORMAT,
                        (LONG) (BUFFLEN - ulBytes), szBuffer + ulBytes);
                    strucCreate.flMsgStyle = MBW_OK | MBW_CANCEL |
                        MB_WARNING | MB_DEFBUTTON2;
                    break;
                }

            /* Speicher allokieren; Message ausgeben; Speicher freigeben */
            DosAllocMem ((PVOID) &strucCreate.pszText, BUFFLEN, PAG_COMMIT | PAG_READ | PAG_WRITE);
            DosInsertMessage (lvTable, ulcTable, szBuffer, ulBytes,
                strucCreate.pszText, BUFFLEN, &ulBytes);

            *(strucCreate.pszText+ulBytes) = '\0';
            WinAlarm (HWND_DESKTOP, WA_WARNING);
            usRetMsgBox = (USHORT) WinDlgBox (HWND_DESKTOP, hwnd,
                (PFNWP) wpWarning, NULLHANDLE, ID_WARNING, (PVOID) &strucCreate);
            DosFreeMem (strucCreate.pszText);

            /* Thread entblocken; Ergebnis in usRetMsgBox */
            DosPostEventSem (rsFormatBlocks);
            return 0;
            }

/*----- Window Message WM_TRACK: User defined */
        case WM_TRACK:
            WinSendMsg (hwndTrack, WM_TRACK, mp1, mp2);
            return 0;

/*----- Window Message WM_REDRAW: User defined */
        case WM_REDRAW:
            hwndCtls[DID_LW_A -  FIRST_DID] = WinWindowFromID (hwnd, DID_LW_A);
            hwndCtls[DID_LW_B -  FIRST_DID] = WinWindowFromID (hwnd, DID_LW_B);
            hwndCtls[DID_LW_SPIN-FIRST_DID] = WinWindowFromID (hwnd, DID_LW_SPIN);
            switch (usAppMode)
                {
                /* Erweiterter Modus: Spin-Button fÅr alle Laufwerke */
                case MODE_EXT:
                    WinShowWindow (hwndCtls[DID_LW_A-FIRST_DID],    FALSE);
                    WinShowWindow (hwndCtls[DID_LW_B-FIRST_DID],    FALSE);
                    WinShowWindow (hwndCtls[DID_LW_SPIN-FIRST_DID], TRUE);
                    /* Bestimmung des aktuellen Laufwerkes; ulDevice: A=>1, B=>2 etc. */
                    for (i = 0; *ppszDrives[i] != ulDevice + 'A' - 1; i++);
                    WinSendMsg (hwndCtls[DID_LW_SPIN-FIRST_DID], SPBM_SETCURRENTVALUE,
                        MPFROMSHORT (i), 0L);
                    hwndCtls[DID_LW_A - FIRST_DID] = 0L;
                    hwndCtls[DID_LW_B - FIRST_DID] = 0L;
                    break;

                /* Standard-Modus fÅr A/B-Button */
                default:
                    WinShowWindow (hwndCtls[DID_LW_A-FIRST_DID],    TRUE);
                    WinShowWindow (hwndCtls[DID_LW_B-FIRST_DID],    TRUE);
                    WinShowWindow (hwndCtls[DID_LW_SPIN-FIRST_DID], FALSE);
                    WinSetWindowBits(hwndCtls[DID_LW_A-FIRST_DID],
                        QWL_STYLE, WS_DISABLED, IsDiskette (1) ? 0 : (ULONG) WS_DISABLED);
                    WinSetWindowBits(hwndCtls[DID_LW_B-FIRST_DID],
                        QWL_STYLE, WS_DISABLED, IsDiskette (2) ? 0 : (ULONG) WS_DISABLED);
                    WinSendDlgItemMsg (hwnd, ulDevice+DID_GR_DISK, BM_SETCHECK,
                        MPFROMSHORT (1), 0L);
                    hwndCtls[DID_LW_SPIN-FIRST_DID] = 0L;
                }
            return 0;

/*----- Window Message WM_QDISKTYPE_B: User defined */
        case WM_QDISKTYPE_B:
            bIsTThread = TRUE;
            if (sTestMode)
                {
                WinSendMsg (hwnd, WM_CONTROLPOINTER, (MPARAM) ID_FORMAT, (MPARAM) hptrArrow);
                WinSetPointer (HWND_DESKTOP, hptrWait);

                /* FÅr die Zeit der Bestimmung des Diskettentyps: Eingaben unterdrÅcken */
                {
                SHORT arID[] =
                    {
                    DID_LW_A-FIRST_DID,     DID_LW_B-FIRST_DID,
                    DID_LW_SPIN-FIRST_DID,  DID_FORMAT-FIRST_DID
                    };

                for (i = 0; i < sizeof (arID) / sizeof (SHORT); i++)
                    if (hwndCtls[arID[i]] != 0)
                        WinEnableWindow (hwndCtls[arID[i]], FALSE);
                }
                }

            pszDevName = ppszDrives[ulDevice-1];
            arTThreadArg.ppulBtnID = &pulBtnID;
            arTThreadArg.hwnd = hwnd;
            DosCreateThread (&idTThread,
                (PFNTHREAD) SetDiskType, (ULONG) &arTThreadArg, 0L, STACKSIZE);
            return 0;

/*----- Window Message WM_QDISKTYPE_E: User defined */
        case WM_QDISKTYPE_E:
            {
            ULONG ulBtnID;

            for (i = 0; i < NUMFBUTTON; i++)
                {
                ulBtnID = (ULONG) (DID_GR_TYPE + i + 1);
                WinEnableWindow (WinWindowFromID (hwnd, ulBtnID),
                    pulBtnID[i]);
                }

            WinSendDlgItemMsg (hwnd, (ULONG) mp1,
                BM_SETCHECK, MPFROMSHORT (1), 0L);

            if (sTestMode)
                {
                /* Buttons wieder aktivieren */
                {
                SHORT arID[] =
                    {
                    DID_LW_A-FIRST_DID,     DID_LW_B-FIRST_DID,
                    DID_LW_SPIN-FIRST_DID,  DID_FORMAT-FIRST_DID
                    };

                for (i = 0; i < 4; i++)
                    if (hwndCtls[arID[i]] != 0)
                        WinEnableWindow (hwndCtls[arID[i]], TRUE);
                }

                WinSendMsg (hwnd, WM_CONTROLPOINTER, (MPARAM) ID_FORMAT, (MPARAM) hptrArrow);
                WinSetPointer (HWND_DESKTOP, hptrArrow);
                }
            bIsTThread = FALSE;
            return 0;
            }

        case WM_CONTROLPOINTER:
            if (sTestMode)
                return bIsTThread ? (MRESULT) hptrWait : (MRESULT) mp2;
            else
                break;

/*----- Window Message WM_CLOSE */
/*      Die WinDefDlgProc sendet auf WM_CLOSE nicht von sich aus eine */
/*      WM_QUIT-Message sondern ruft WinDismissDlg auf.               */
        case WM_CLOSE:
            /* auf rsWaitThread warten und FormatThread beenden */
            DosRequestMutexSem (rsWaitThread, (ULONG) SEM_INDEFINITE_WAIT);
            DosKillThread (idFThread);
            WinPostMsg (hwnd, WM_QUIT, 0L, 0L);
            return 0;
        }
    return WinDefDlgProc (hwnd, msg, mp1, mp2);
    }

/*******************************************************************
   Dialog-Procedure fÅr ABOUT-Box
 *******************************************************************/
MRESULT EXPENTRY wpAbout (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2)
    {
    switch (msg)
        {
        case WM_INITDLG:
            return (MRESULT) FALSE;

        case WM_COMMAND:
            switch (COMMANDMSG(&msg)->cmd)
                {
                case DID_OK:
                case DID_CANCEL:
                    WinSendMsg (hwnd, WM_CLOSE, 0, 0);
                    return 0;
                }
            break;

        case WM_CLOSE:
            WinDismissDlg (hwnd, 0);
            return 0;
        }

    return WinDefDlgProc (hwnd, msg, mp1, mp2);
    }

/*******************************************************************
   Dialog-Procedure fÅr die Daten der formatierten Diskette
 *******************************************************************/
MRESULT EXPENTRY wpDiskData (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2)
    {
    ULONG ulSize, ulSects;
    CHAR szNumber[10];                   /* grî·te Zahl: Disk-ID */

    switch (msg)
        {
        case WM_INITDLG:
            if (DI2UL(mp2)->pszVolName[0] == '\0')
                WinLoadString (hab, 0, IDS_NONAME, BUFFLEN, szBuffer);
            else
                {
                /* BUFFLEN ist gro· genug gewÑhlt! */
                WinLoadString (hab, 0, IDS_NAME, BUFFLEN, szBuffer);
                WinUpper (hab, 0, 0, ((PDISKINFO)mp2)->pszVolName);
                strcat (szBuffer, ((PDISKINFO)mp2)->pszVolName);
                }
            WinSetDlgItemText (hwnd, DID_VOLN, szBuffer);
            _ultoa (DI2UL(mp2)->ulDiskSerial >> 16, szNumber, 16);
            szNumber[4] = ':';
            _ultoa (DI2UL(mp2)->ulDiskSerial & 0xFFFF, szNumber +5, 16);
            WinSetDlgItemText (hwnd, DID_NUMBER, szNumber);

            ulSects = DI2UL(mp2)->uscUnitsize -
                      DI2UL(mp2)->uscBootsectors -
                      DI2UL(mp2)->bcFATCount *
                      DI2UL(mp2)->uscFATSize -
                     (DI2UL(mp2)->uscDirEntries * 32 +
                      DI2UL(mp2)->uscSectorsize - 1) /
                      DI2UL(mp2)->uscSectorsize;
            WinSetDlgItemText (hwnd, DID_SUM_UNIT,
                _ultoa (ulSects / DI2UL(mp2)->bcClustersize, szNumber, 10));

            ulSize  = ulSects * DI2UL(mp2)->uscSectorsize;
            WinSetDlgItemText (hwnd, DID_SUM_BYTE,
                _ultoa (ulSize, szNumber, 10));

            ulSects -= DI2UL(mp2)->uscDefectsize;
            WinSetDlgItemText (hwnd, DID_AVL_UNIT,
                _ultoa (ulSects / DI2UL(mp2)->bcClustersize -
                        DI2UL(mp2)->uscUsedClusters, szNumber, 10));

            ulSize = (ulSects-DI2UL(mp2)->uscUsedClusters*DI2UL(mp2)->bcClustersize)*
                    DI2UL(mp2)->uscSectorsize;
            WinSetDlgItemText (hwnd, DID_AVL_BYTE,
                _ultoa (ulSize, szNumber, 10));

            ulSize = DI2UL(mp2)->bcClustersize * DI2UL(mp2)->uscSectorsize;
            WinSetDlgItemText (hwnd, DID_BPC,
                _ultoa (ulSize, szNumber, 10));

            ulSize = DI2UL(mp2)->uscDefectsize * DI2UL(mp2)->uscSectorsize;
            if (ulSize)
                {
                WinSetDlgItemText (hwnd, DID_ERR_BYTE,
                    _ultoa (ulSize, szNumber, 10));
                WinShowWindow (WinWindowFromID (hwnd, DID_ERR_BYTE), TRUE);
                WinShowWindow (WinWindowFromID (hwnd, DID_ERR_TEXT), TRUE);
                }
            return 0;
        }
    return WinDefDlgProc (hwnd, msg, mp1, mp2);
    }

/*******************************************************************
   Dialog-Procedure fÅr die Optionen
 *******************************************************************/
MRESULT EXPENTRY wpOptions (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2)
    {
    static USHORT usTmpMode;
    static SHORT  sTmpTest;
    CHAR    szInteger [6];

    switch (msg)
        {
        case WM_INITDLG:
            WinSendDlgItemMsg (hwnd,
                (usAppMode==MODE_EXT) ? (ULONG) DID_EXT : (ULONG) DID_STD,
                BM_SETCHECK,
                MPFROMSHORT (1),
                0L);
            usTmpMode = usAppMode;
            WinSendDlgItemMsg (hwnd, DID_CHECK, BM_SETCHECK,
                MPFROMSHORT (sTestMode), 0L);
            sTmpTest = sTestMode;

            return 0;

        case WM_CONTROL:
            if (SHORT2FROMMP (mp1) == BN_CLICKED)
                switch (SHORT1FROMMP (mp1))
                    {
                    /* Im Standard-Modus MODE_STD werden nur die zwei Laufwerke */
                    /* A: und B: durch 2 Radiobuttons dargestellt               */
                    case DID_STD:
                        usTmpMode = MODE_STD;
                        return 0;

                    /* Im erweiterten Modus MODE_EXT werden alle Laufwerke mit  */
                    /* austauschbarem Medium in einem Spinbutton dargestellt    */
                    case DID_EXT:
                        usTmpMode = MODE_EXT;
                        return 0;

                    /* Ist der Testmodus ein, wird bei jedem Klick auf einen    */
                    /* Laufwerksbuchstaben der Typ der eingelegten Diskette ge- */
                    /* prÅft und der entsprechende Type-Button als Vorein-      */
                    /* stellung aktiviert                                       */
                    case DID_CHECK:
                        sTmpTest = (SHORT) !sTmpTest;
                        return 0;
                    }
            break;

        case WM_COMMAND:
            if (SHORT1FROMMP (mp2) == CMDSRC_PUSHBUTTON)
                switch (SHORT1FROMMP (mp1))
                    {
                    case DID_OK:
                        PrfWriteProfileString (HINI_USERPROFILE, APP_NAME, "AppMode",
                            _itoa ((USHORT) usTmpMode, szInteger, 10));
                        PrfWriteProfileString (HINI_USERPROFILE, APP_NAME, "TestMode",
                            _itoa ((SHORT) sTmpTest, szInteger, 10));
                        usAppMode  = usTmpMode;
                        sTestMode = sTmpTest;

                    case DID_CANCEL:
                        WinSendMsg (hwndFrame, WM_REDRAW, 0L, 0L);
                        WinDismissDlg (hwnd, ID_OPTIONS);
                        return 0;
                    }
            break;
        }
    return WinDefDlgProc (hwnd, msg, mp1, mp2);
    }

/*******************************************************************
   Dialog-Procedure fÅr die Warnmeldung
 *******************************************************************/
MRESULT EXPENTRY wpWarning (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2)
    {
    LONG lIcon;
    ULONG ulIcon, ulRet;
    CHAR szIcon[3];
    static USHORT usId;

    switch (msg)
        {
        case WM_INITDLG:
            WinSetDlgItemText (hwnd, DID_TEXT, ((WRN_CREATE *)mp2)->pszText);
            ulIcon = ((WRN_CREATE *)mp2)->flMsgStyle & 0xF0;
            switch (ulIcon)
                {
                case MB_ICONQUESTION:    lIcon = SPTR_ICONQUESTION;    break;
                case MB_WARNING:         lIcon = SPTR_ICONWARNING;     break;
                case MB_ICONASTERISK:    lIcon = SPTR_ICONINFORMATION; break;
                case MB_ERROR:           lIcon = SPTR_ICONERROR;       break;
                default:                 lIcon = 0;
                }
            if (lIcon)
                {
                szIcon[0] = 0xFF;
                szIcon[1] = (CHAR) lIcon;
                szIcon[2] = (CHAR) (lIcon>>8);
                WinSetDlgItemText (hwnd, DID_ICON, szIcon);
                }

            WinEnableControl (hwnd, DID_OK,
                (ULONG)((((WRN_CREATE *)mp2)->flMsgStyle & MBW_OK) ? TRUE : FALSE));
            WinEnableControl (hwnd, DID_CANCEL,
                (ULONG)((((WRN_CREATE *)mp2)->flMsgStyle & MBW_CANCEL) ? TRUE : FALSE));
            WinEnableControl (hwnd, DID_WQUICK,
                (ULONG)((((WRN_CREATE *)mp2)->flMsgStyle & MBW_QUICK) ? TRUE : FALSE));
            WinEnableControl (hwnd, DID_WBOOTS,
                (ULONG)((((WRN_CREATE *)mp2)->flMsgStyle & MBW_BOOTS) ? TRUE : FALSE));
            usId = (((WRN_CREATE *)mp2)->flMsgStyle & MBW_QUICK) ?
                DID_WQUICK : DID_WFORMAT;
            WinCheckButton (hwnd, DID_WARN, usWrnMode);

            /* Damit der Checkzustand der Autoradio-Buttons hier gesetzt werden */
            /* kann, dÅrfen sie keinen WS_TABSTOP-Style im Resource-File haben! */
            WinCheckButton (hwnd, usId, TRUE);
            return 0;

        case WM_COMMAND:
            if (SHORT1FROMMP (mp2) == CMDSRC_PUSHBUTTON)
                switch (SHORT1FROMMP (mp1))
                    {
                    case DID_OK:
                        switch (usId)
                            {
                            case DID_WFORMAT:
                                ulRet = MBID_OK;
                                break;

                            case DID_WQUICK:
                                ulRet = MBID_QUICK;
                                break;

                            case DID_WBOOTS:
                                ulRet = MBID_BOOTS;
                                break;
                            }
                        WinDismissDlg (hwnd, ulRet);
                        return 0;

                    case DID_CANCEL:
                        WinDismissDlg (hwnd, MBID_CANCEL);
                        return 0;
                    }
            break;

        case WM_CONTROL:
            if (SHORT2FROMMP (mp1) == BN_CLICKED)
                {
                switch (SHORT1FROMMP (mp1))
                    {
                    case DID_WARN:
                        usWrnMode = (USHORT) WinSendDlgItemMsg (hwnd, DID_WARN,
                            BM_QUERYCHECK, 0L, 0L);
                        return 0;

                    case DID_WFORMAT:
                    case DID_WQUICK:
                    case DID_WBOOTS:
                        usId = SHORT1FROMMP (mp1);
                        return 0;
                    }
                }
            break;
        }
    return WinDefDlgProc (hwnd, msg, mp1, mp2);
    }
