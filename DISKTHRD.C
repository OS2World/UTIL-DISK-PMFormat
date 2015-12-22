/*----------------------------------------------------------
   PMFORMAT.C -- Ein Formatierprogramm fr Disketten

   Modul 5: Thread fr Laufwerkswechsel

   Version 2.0 - 20.9.1992  > OS/2 2.0-Version fr C Set/2
   Version 2.16 - 4.5.2000  > DosSetCurrentDir wegen Help-Datei
  ----------------------------------------------------------*/
#define INCL_DOSDEVICES
#define INCL_DOSDEVIOCTL
#define INCL_WINBUTTONS
#define INCL_WINWINDOWMGR
#define INCL_WINMESSAGEMGR
#include <os2.h>
#include "format.h"

extern PCHAR pszDevName;                /* Laufwerksname (ASCIIZ) */
extern ULONG ulDevice;                  /* ausgew„hltes Laufwerk; 1 = 'A' */
extern PDRIVEINFO pFSDriveInfo;         /* Zeiger auf DriveInfo-Segment */

void _System SetDiskType (SDTARG *SDThreadArg)
    {
    *SDThreadArg->ppulBtnID = GetDiskTypeBtn (&pFSDriveInfo[ulDevice-1]);

    WinPostMsg (SDThreadArg->hwnd, WM_QDISKTYPE_E,
        (MPARAM) CheckDisk (pszDevName, *SDThreadArg->ppulBtnID), 0L);

    return;
    }

