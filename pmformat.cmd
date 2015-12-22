/* Erzeugung des PMFORMAT.EXE Programmes              */
/* Version 2.0 - 20.9.1992 > fr C Set/2 Compiler     */
/* Es wird die Debug-Information in den OBJ-Dateien   */
/* erzeugt, jedoch nicht in die EXE-Datei geschrieben */
SAY 'Welche Sprachversion soll erzeugt werden?'
SAY '        - deutsch (D)'
SAY '        - englisch (E)'
PULL language
SELECT
  WHEN language = 'D'
    THEN DO
      langopt = "GR"
      masmopt = "/Dgerman"
    END
  WHEN language = 'E'
    THEN DO
      langopt = "US"
      masmopt = ""
    END
  OTHERWISE
    SAY "ungltige Sprachoption"
    EXIT
END

/* Assemblieren des Bootsektors mit MASM 6.0 */
'masm bootsect, boots'langopt masmopt '/Zi;'
IF rc > 0
  THEN
    DO
    SAY 'Fehler im MASM; Fehlercode: ' rc
    EXIT
    END

/* Compilieren der C-Module */
c_option = "/Ss /Q /Wcnviniparprorearet /Ti /Gm /G4 /C /O /Id:\work\c\cutil"

'icc' c_option 'pmtrack.c pmformat.c format.c drvinfo.c diskthrd.c'
IF rc > 0
  THEN
    DO
    SAY 'Fehler in CL; Fehlercode: ' rc
    EXIT
    END

/* Linken der Objekt-Dateien mit LINK386 */
l_option = "/nofree /pm:pm /ignorecase"
'ilink ' l_option ' pmformat+pmtrack+format+drvinfo+diskthrd+boots'langopt',,,d:\work\c\cutil\cutil,pmformat'
IF rc > 0
  THEN
    DO
    SAY 'Fehler in LINK; Fehlercode: ' rc
    EXIT
    END

/* Compilieren der Resource- und Help-Dateien */
'del pmformat.hlp'
'ipfc pmfmt'langopt'.ipf'
'ren pmfmt'langopt'.hlp pmformat.hlp'
'rc pmfmt'langopt'.rc pmformat.exe'
IF rc > 0
  THEN SAY 'Fehler in RC; Fehlercode: ' rc
