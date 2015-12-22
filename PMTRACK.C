/*----------------------------------------------------------
   PMFORMAT.C -- Ein Formatierprogramm fÅr Disketten

   Modul 2: Track-Anzeige

   Version 1.1 - 15.4.1991
   Version 1.2 - 9.6.1991
   Version 1.3 - 9.11.1991  > Einbau von WinMapDlgPoints:
                              HardwareunabhÑngige Koordinaten;
                              Anzeige von Spuren mit defekten Sektoren;
                              énderung der Hintergrund-Lîschmethode;
   Version 1.31- 9.2.1992
   Version 2.0 - 20.9.1992  > OS/2 2.0-Version fÅr C Set/2
   Version 2.16 - 4.5.2000  > DosSetCurrentDir wegen Help-Datei
  ----------------------------------------------------------*/
#define  INCL_DOSDEVIOCTL
#define  INCL_DOSMEMMGR
#define  INCL_DOSMISC
#define  INCL_WIN
#define  INCL_GPI
#include <os2.h>
#include <stdlib.h>
#include "FORMAT.H"

#define BUFFLEN     128

#define LINEPOS_X   4   /* EinrÅckung der Grafik (x-Richtung) */
#define LINEPOS_Y   5   /* EinrÅckung der Grafik (y-Richtung) */
#define LINEWIDTH   2   /* Liniendicke der Grafik             */

#define SEG_MAIN    1L  /* Id des Grafik-Segment */

extern HAB hab;

static CHAR szBuffer[BUFFLEN];
static CHAR szAct[3], szMax[3];

MRESULT EXPENTRY wpTrackWnd (HWND hwnd, USHORT msg, MPARAM mp1, MPARAM mp2)
    {
    static USHORT usMaxTrack, usActTrack;
    static RECTL  rectl;        /* Grî·e des Track-Fenster */
    static SIZEL  szlPS;        /* Presentation-Space Grî·e */
    static ULONG  ulMsgLen;     /* LÑnge der Roh-Message */
    static HPS    hps;
    static HDC    hdc;
    static PCHAR  pMsgBuff;
    static POINTL ptlOrg, ptlPrev, ptl, arPts[2];

    ULONG  ulBytes;
    RECTL  rc;

    PCHAR  lvTable[2];

    switch (msg)
        {
        case WM_CREATE:
            arPts[0].x = LINEPOS_X;
            arPts[0].y = LINEPOS_Y;
            arPts[1].x = 0;
            arPts[1].y = LINEWIDTH;
            usActTrack = usMaxTrack = 0;

            ulMsgLen = (ULONG) WinLoadString (hab, 0, IDS_TRACKNUM,
                BUFFLEN, szBuffer);
            DosAllocMem ((PVOID) &pMsgBuff, BUFFLEN, PAG_COMMIT | PAG_READ | PAG_WRITE);

            WinQueryWindowRect (hwnd, &rectl);
            szlPS.cx = rectl.xRight;
            szlPS.cy = rectl.yTop;
            hdc = WinOpenWindowDC (hwnd);
            hps = GpiCreatePS (hab, hdc, &szlPS,
                PU_PELS | GPIT_NORMAL | GPIA_ASSOC);

            WinMapDlgPoints (hwnd, arPts,
                sizeof (arPts) / sizeof (POINTL), TRUE);

            GpiSetAttrMode (hps, AM_NOPRESERVE);
            GpiSetDrawingMode (hps, DM_RETAIN);
            GpiSetBackMix (hps, BM_OVERPAINT);
            GpiSetLineWidthGeom (hps, arPts[1].y);

            GpiOpenSegment  (hps, SEG_MAIN);
            ptlOrg.x = arPts[0].x;                      /* LINEPOS_X */
            ptlOrg.y = ptlPrev.y = ptl.y = arPts[0].y;  /* LINEPOS_Y */
            GpiMove (hps, &ptlOrg);
            ptl.x = rectl.xRight - arPts[0].x - 1;
            GpiLine (hps, &ptl);        /* dÅnne Linie ziehen */
            GpiCloseSegment (hps);

            GpiSetDrawingMode (hps, DM_DRAW);

            ptl.x = ptlOrg.x;           /* ptl.x fÅr VorgÑnger initialisieren */
            return FALSE;               /* continue window creation */

        case WM_PAINT:
            WinBeginPaint (hwnd, hps, &rc);

            if (usMaxTrack)
                {
                /* schwarze Tracklinie ziehen */
                GpiBeginPath (hps, 1L);
                GpiMove (hps, &ptlOrg);
                GpiLine (hps, &ptl);
                GpiEndPath (hps);
                GpiStrokePath (hps, 1L, 0L);

                /* dÅnne Linie und rote Defect-Linie ziehen */
                GpiDrawSegment (hps, SEG_MAIN);

                /* Text ausgeben */
                lvTable [0] = _itoa (usActTrack, szAct, 10);
                lvTable [1] = _itoa (usMaxTrack, szMax, 10);
                DosInsertMessage (lvTable, 2L, szBuffer, ulMsgLen,
                    pMsgBuff, BUFFLEN, &ulBytes);
                WinDrawText (hps, (LONG) ulBytes, pMsgBuff, &rectl,
                    CLR_NEUTRAL, SYSCLR_DIALOGBACKGROUND, DT_CENTER);
                }

            WinEndPaint (hps);
            return 0;

        case WM_TRACK:
            usActTrack = (USHORT) (SHORT1FROMMP (mp1) + 1);
            usMaxTrack = SHORT2FROMMP (mp1);
            ptlPrev.x = ptl.x;
            ptl.x = ((szlPS.cx-2*arPts[0].x-1) * usActTrack) / usMaxTrack
                    + arPts[0].x;

            if (!SHORT1FROMMP (mp2))
                {
                GpiSetDrawingMode (hps, DM_RETAIN);

                /* rote Defect-Linie ins Grafik-Segment eintragen */
                GpiOpenSegment  (hps, SEG_MAIN);
                GpiSetElementPointer (hps, 0x7FFFFFFF);
                GpiSetColor (hps, CLR_RED);
                GpiMove (hps, &ptlPrev);
                GpiBeginPath (hps, 1L);
                GpiLine (hps, &ptl);
                GpiEndPath (hps);
                GpiStrokePath (hps, 1L, 0L);
                GpiSetColor (hps, CLR_DEFAULT);
                GpiCloseSegment (hps);

                GpiSetDrawingMode (hps, DM_DRAW);
                }

            WinInvalidateRect (hwnd, NULL, FALSE);
            return 0;

        case WM_DESTROY:
            DosFreeMem (pMsgBuff);
            return 0;
        }
    return WinDefWindowProc (hwnd, msg, mp1, mp2);
    }
