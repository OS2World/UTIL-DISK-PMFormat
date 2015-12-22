/*----------------------------------------------------------
   PMFORMAT.C -- Ein Formatierprogramm fÅr Disketten

   Include-Datei

   Version 1.1 - 15.4.1991
   Version 1.2 - 9.6.1991
   Version 1.3 - 9.11.1991
   Version 1.31- 9.2.1992
   Version 2.0 - 20.9.1992
   Version 2.11- 8.4.1994
   Version 2.16 - 4.5.2000
  ----------------------------------------------------------*/
#include "pmformat.h"

#define CLIENTCLASS     "Format"

/***********************************************************************/
/*      User Messages                                                  */
/***********************************************************************/
/* mp1: SHORT1 : Fehlertype (RTYPE) SHORT2 : Fehlernummer */
/* mp2: Zeiger auf Diskdaten */
#define WM_FMTEXIT      WM_USER

/* mp1: SHORT1 : Grund fÅr Block: BLCK_*-Wert */
/* mp2: Zeiger auf DosQueryFSInfo-Struktur */
#define WM_FMTBLOCK     WM_USER+1

/* mp1: SHORT1 : Track-Nummer       SHORT2 : Trackzahl */
/* mp2: SHORT1 : FALSE=>bad sector */
#define WM_TRACK        WM_USER+2

/* mp1: reserved */
/* mp2: reserved */
#define WM_REDRAW       WM_USER+3

/* mp1: reserved */
/* mp2: reserved */
#define WM_QDISKTYPE_B  WM_USER+4

/* mp1: BtnID fÅr BM_SETCHECK */
/* mp2: reserved */
#define WM_QDISKTYPE_E  WM_USER+5

/* Typ des RÅckgabewertes vom FORMAT-Thread */
#define RTYPE_SYSERR        0   /* Fehler ist OS/2 Fehlercode */
#define RTYPE_USRERR        1   /* Fehler; Fehlercode siehe unten */
#define RTYPE_WARNING       2   /* RÅckgabewert einer Message-Box */
#define RTYPE_INTERRUPT     3   /* Abbruch durch Benutzer; RÅckgabewert ignorieren */

/* Grund fÅr eine Blockierung von FORMAT */
#define BLCK_FORMAT         1   /* Diskette bereits formatiert */
#define BLCK_WRONGFMT       2   /* falsches Format */

/* Modus des Formatierprogrammes (INI-Eintrag 'AppMode') */
#define MODE_STD    1                   /* Standard-Modus: A/B-Laufwerke */
#define MODE_EXT    2                   /* Erweiterter Modus: alle Diskettenlaufwerke */

/* Default-Modi */
#define TEST_STD    TRUE                /* Diskettentype prÅfen 'ein' */
#define FMT_STD     FALSE               /* Diskette komplett formatieren */
#define WRN_STD     TRUE                /* Warnungen ein */

/* Nicht-PM-Definitionen */
#define OPEN        0x00000001          /* Diskettenhandle geîffnet */
#define LOCKED      0x00000002          /* Laufwerk gelocked */
#define PARMS       0x00000004          /* SETPARM mit CMD=00 ist nîtig */
#define ALLOC       0x00000008          /* Puffer wurden allokiert */
#define FMT_TRACK   0x00000010          /* 1 Spur bereits formatiert -> Redetermine! */

#define NUMDRVTYPE  5                   /* es werden 5 verschiedene Laufwerkstypen unterstÅtzt */
#define NUMFBUTTON  4                   /* es gibt 4 Formatknîpfe */

#define TERMTHREAD  0x100               /* User Term. code fÅr TERMINATE */

/* OS/2 Mindestversion */
#define VER_MAJOR   20
#define VER_MINOR   10

/* Konstanten fÅr die Bestimmung des BPBs (in GetBPB) */
#define GBPB_ERROR          -2          /* keine Diskette eingelegt */
#define GBPB_RECOMMENDED     0          /* recommended BPB verwenden */
#define GBPB_DD5             1          /* 5¨" DD-BPB verwenden */
#define GBPB_DD3             2          /* 3´" DD-BPB verwenden */
#define GBPB_HD3             3          /* 3´" HD-BPB verwenden */

/* Ergebnis des Vergleichs von angefordertem BPB mit eingelegter Disk */
#define FMT_EMPTY           0           /* unformatierte Diskette */
#define FMT_OK              1           /* gleiche Formate        */
#define FMT_NOT_OK          2           /* ungleiche Formate      */

/***********************************************************************/
/*      Stukturen                                                      */
/***********************************************************************/
/* SFILEINFO fÅr den DatentrÑgernamen (DosSetFSInfo) */
typedef struct _SFILEINFO
    {
    BYTE    ccLenLabel;
    CHAR    szLabel[12];
    } SFILEINFO;

/* QFILEINFO fÅr den DatentrÑgernamen (DosQueryFSInfo) */
typedef struct _QFILEINFO
    {
    ULONG   ulVolSerial;
    BYTE    ccLenLabel;
    CHAR    szLabel[12];
    } QFILEINFO;

/* DISKINFO fÅr wpDiskData zur Anzeige der Diskettendaten nach dem Formatiervorgang */
typedef struct _DISKINFO
    {
    PCHAR   pszVolName;
    USHORT  uscSectorsize;
    BYTE    bcClustersize;
    USHORT  uscBootsectors;
    USHORT  uscUnitsize;
    USHORT  uscDirEntries;
    BYTE    bcFATCount;
    USHORT  uscFATSize;
    USHORT  uscDefectsize;
    USHORT  uscUsedClusters;
    ULONG   ulDiskSerial;
    } DISKINFO;
typedef DISKINFO FAR *PDISKINFO;

/* DRIVEINFO fÅr DRVINFO.C */
typedef struct _DRIVEINFO
    {
    CHAR               cDrive;          /* Laufwerksbuchstabe */
    BIOSPARAMETERBLOCK strucBPB;        /* recommended BPB */
    } DRIVEINFO;
typedef DRIVEINFO FAR *PDRIVEINFO;

/* fÅr Array, in dem BPB-Daten fÅr die verschiedenen */
/* Standard-Diskettentypen abgelegt sind             */
typedef struct _BPBDATA
    {
    USHORT cSectors;
    USHORT usSectorsPerTrack;
    ULONG  idButton[NUMFBUTTON];
    } BPBDATA;

/* Argumentstruktur fÅr Formatier-Thread */
typedef struct _TARG
    {
    ULONG      ulDiskType;              /* ID des gewÑhlten Diskformat-Buttons */
    PDRIVEINFO pFSDriveInfo;            /* Struktur der Laufwerksdaten */
    PULONG     pulBtnID;                /* Zeiger auf Vektor der aktiven Buttons */
    } TARG;

/* Argumentstruktur fÅr Diskettentyp-Thread */
typedef struct _SDTARG
    {
    PULONG *ppulBtnID;                  /* Zeiger auf Zeiger auf Vektor der aktiven Buttons */
    HWND   hwnd;                        /* HWND des Hauptfensters */
    } SDTARG;

/* Create Parameter fÅr ID_WARNING */
typedef struct _WRN_CREATE
    {
    PCHAR pszText;                      /* Zeiger auf Textstring mit Warnungstext */
    ULONG flMsgStyle;                   /* aktive Buttons; Icon */
    } WRN_CREATE;

#define MBW_OK                      0x0001
#define MBW_CANCEL                  0x0002
#define MBW_QUICK                   0x0004
#define MBW_BOOTS                   0x0008

#define MBID_QUICK                  0x1000
#define MBID_BOOTS                  0x1001

/* ErgÑnzung von BSEDEV.H im Toolkit 2.0/2.1 */
//#define DSK_BEGINFORMAT           4

/* Funktionsdefinitionen */
MRESULT EXPENTRY DialogWndProc (HWND, USHORT, MPARAM, MPARAM);
MRESULT EXPENTRY wpTrackWnd    (HWND, USHORT, MPARAM, MPARAM);
MRESULT EXPENTRY wpAbout       (HWND, USHORT, MPARAM, MPARAM);
MRESULT EXPENTRY wpDiskData    (HWND, USHORT, MPARAM, MPARAM);
MRESULT EXPENTRY wpOptions     (HWND, USHORT, MPARAM, MPARAM);
MRESULT EXPENTRY wpWarning     (HWND, USHORT, MPARAM, MPARAM);
void APIENTRY    Terminate (USHORT);
USHORT           GetDevType (CHAR *);
void _System     FormatThread (TARG *);
void _System     SetDiskType (SDTARG *);
USHORT           FSDriveData (PDRIVEINFO *);
void             FSSDriveData (PDRIVEINFO, USHORT);
BOOL             IsDiskette (USHORT);
PULONG           GetDiskTypeBtn (PDRIVEINFO);
ULONG            CheckDisk (PCHAR, PULONG);
LONG             GetBPB (PDRIVEINFO, ULONG, PULONG);
USHORT           IsInvalidChar (CHAR *);
void             CreateEdit (HWND);
void             CreateTrackCtr (HWND);
void             ErrBox (HWND, MPARAM);
void             TestShareware (HWND);

/* LONG Window Words fÅr Formatier-Knopf*/
#define IS_FORMAT 0L            /* es soll formatiert werden   */
#define IS_STOP   1L            /* nÑchste Aktion ist STOP     */
#define IS_IGNORE 2L            /* es lÑuft gerade eine Aktion */

