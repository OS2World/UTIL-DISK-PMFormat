/*----------------------------------------------------------
   PMFORMAT.C -- Ein Formatierprogramm fÅr Disketten

   Include-Datei fÅr Dialogboxen

   Version 1.1 - 15.4.1991
   Version 1.2 - 9.6.1991
   Version 1.3 - 9.11.1991
   Version 1.31- 9.2.1992
   Version 2.0 - 20.9.1992
   Version 2.11- 8.4.1994
   Version 2.16 - 4.5.2000
  ----------------------------------------------------------*/
#define ID_FORMAT       100
#define ID_ABOUT        200
#define ID_DISKSTAT     300
#define ID_OPTIONS      400
#define ID_WARNING      500
#define ID_ICON         600

/* allgemeine Dialog-IDs */
#define DID_HELP        10

#define FIRST_DID       101     // niedrigste CONTROL-ID in Dialog-Box
#define LAST_DID        112     // hîchste CONTROL-ID in Dialog-Box

/* IDs fÅr die Gruppe "Laufwerke" */
#define DID_GR_DISK     101
#define DID_LW_A        102
#define DID_LW_B        103
#define DID_LW_SPIN     104

/* IDs fÅr die Gruppe "Diskettentyp" */
#define DID_GR_TYPE     105
#define DID_DD          106
#define DID_HD          107
#define DID_ED          108
#define DID_SP          109

/* IDs fÅr die Pushbuttons */
#define DID_FORMAT      110

/* IDs fÅr weitere Controls */
#define DID_ENTRY       111
#define DID_TRACKS      112

/* IDs fÅr die Dialogbox "Daten der formatierten Diskette" */
#define DID_VOLN        301
#define DID_NUMBER      302
#define DID_SUM_BYTE    303
#define DID_AVL_BYTE    304
#define DID_BPC         305
#define DID_SUM_UNIT    306
#define DID_AVL_UNIT    307
#define DID_ERR_BYTE    308
#define DID_ERR_TEXT    309

/* IDs fÅr die Dialogbox "Optionen" */
#define DID_STD         401
#define DID_EXT         402
#define DID_CHECK       403

/* IDs fÅr die Dialogbox "Warnung" */
#define DID_ICON        501
#define DID_TEXT        502
#define DID_WARN        503
#define DID_WFORMAT     504
#define DID_WQUICK      505
#define DID_WBOOTS      506

/* Help-Table IDs */
#define IDH_MAIN        10
#define IDH_SUBTAB1     11
#define IDH_SUBTAB2     12
#define IDH_SUBTAB3     13
#define IDH_SUBTAB4     14

/* Error Messages */
#define ERROR_INV_DRV_TYPE  2000
#define ERROR_INCOMP_TYPE   3000
#define ERROR_UNUSABLE      4000

#define IDS_ERROR_INV_DRV_TYPE  ERROR_INV_DRV_TYPE
#define IDS_ERROR_INCOMP_TYPE   ERROR_INCOMP_TYPE
#define IDS_ERROR_UNUSABLE      ERROR_UNUSABLE

/* MenÅ-EintrÑge */
#define IDM_ABOUT               20
#define IDM_HELP                21
#define IDM_OPTION              22
#define IDM_HELPMENU            23
#define IDM_HFH                 24

/* sonstige Textstrings */
#define IDS_ALREADY_FORMATTED   1000
#define IDS_NONAME_FORMATTED    1001
#define IDS_DISK_ID             1002
#define IDS_QST_FORMAT          1003
#define IDS_INVALID_CHARACTER   1004

#define IDS_WRONGFMT            1005

#define IDS_TRACKNUM            1006
#define IDS_LABEL               1007
#define IDS_SWENTRY             1008

#define IDS_HELP                1009
#define IDS_ABOUT               1010
#define IDS_OPTION              1011

#define IDS_NAME                1012
#define IDS_NONAME              1013

#define IDS_BTN_FMT             1014
#define IDS_BTN_STOP            1015

#define IDS_TITLE_STD           1016
#define IDS_TITLE_EXT           1017

#define IDS_HELPWINDOWTITLE     1018
#define IDS_HELPLIBRARYNAME     1019
#define IDS_NOHELP              1020

#define IDS_VERSION             1021

