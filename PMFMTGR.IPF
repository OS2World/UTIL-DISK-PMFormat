
.* PMFORMAT.C -- Ein Formatierprogramm f�r Disketten
.*
.* IPF Help File
.*
.* Version 2.14 - 28.7.1995  >
.* Version 2.16 - 4.5.2000  > DosSetCurrentDir wegen Help-Datei
.*----------------------------------------------------------
:userdoc.

.*----------------------------------------------------------
.*  Hilfe Hauptmen�
.*      res = PANEL_MAIN
.*----------------------------------------------------------
:h1 res=1000 id=MAIN name=PANEL_MAIN.PMFORMAT
:i1 id=MAIN.PMFORMAT
:p.:hp4.PMFORMAT:ehp4. formatiert alle g�ngigen Diskettenformate mit jeder
�blichen (und un�blichen) Kapazit�t.
Das Programm bietet folgende Features&colon.
:ul.
:li.Einfache :link reftype=hd refid=DRIVE.Anwahl von 2 Laufwerken:elink.
im Standardmodus.
:li.:link reftype=hd refid=DRIVE.Anwahl von bis zu 26 Laufwerken:elink.
im erweiterten Modus.
:li.Unterst�tzung aller :link reftype=hd refid=TYPE.Standarddiskettenformate:elink.
(DD, HD, ED) sowie von besonderen Formaten f�r EXTDSKDD-Laufwerke.
:li.:link reftype=hd refid=FMTTYPE.Schnellformatierfunktion:elink.. Diese l�scht
die Systembereiche der Diskette, �bernimmt jedoch die fehlerhaften Sektoren.
:li.:link reftype=hd refid=FMTTYPE.Bootsektor tauschen:elink.. Damit kann
der Bootsektor der eingelegten Diskette gegen einen neuen ausgetauscht werden.
Dieser startet das System auch bei eingelegter Diskette von Festplatte, wenn die
Diskette kein Betriebssystem enth�lt.
:li.:link reftype=hd refid=WARN.Abschaltbare Sicherheitsabfrage:elink.
bei bereits bespielten Disketten.
:eul.

.*----------------------------------------------------------
.*  Hilfe Optionen
.*      res = PANEL_OPTIONS
.*----------------------------------------------------------
:h1 res=1010 id=OPTIONS name=PANEL_OPTIONS.Optionen
:i1 id=OPTION.Optionen
:p.Im Optionen-Dialogfenster werden Voreinstellungen
getroffen, die im Benutzerprofil des Systems (OS2.INI) abgelegt werden.
:p.
Die Einstellungen werden mit der :hp1.OK:ehp1.-Taste �bernommen, :hp1.Abbruch:ehp1.
ignoriert alle �nderungen.
:p.
Zugeh�rige Informationen&colon.
:ul compact.
:li.:link reftype=hd refid=MODE.Laufwerksauswahlmodus:elink. (Darstellung)
:li.:link reftype=hd refid=CHECK.Format pr�fen:elink.
:eul.

.*----------------------------------------------------------
.*  Hilfe Warnungen
.*      res = PANEL_WARNING
.*----------------------------------------------------------
:h1  res=1020 id=WARNING name=PANEL_WARNING.Warnungen
:i1 id=WARNING.Warnungen
:p.In dieser Dialogbox werden Probleme, die w�hrend des Formatiervorganges
auftreten, an den Benutzer gemeldet.
Dies sind&colon.
:ul.
:li.:hp2.Diskette bereits formatiert :ehp2.:hp1.(abschaltbar):ehp1..
Der Benutzer hat die M�glichkeit, den Formatiervorgang trotzdem
durchzuf�hren (:hp1.OK:ehp1.) oder abzubrechen (:hp1.Abbruch:ehp1.).
F�r die Formatierung der Diskette stehen insgesamt drei verschiedene
:link reftype=hd refid=FMTTYPE.Formatiermodi:elink. zur Verf�gung.
Wird :link reftype=hd refid=WARN.Warnungen:elink. ausgeschaltet, wird eine
Meldung dieser Art bis zur Beendigung des Programmes nicht mehr angezeigt.
:li.:hp2.Diskette hat falsches Format. :ehp2.
Diese Meldung erscheint, wenn die Diskette bereits mit einem anderen
Diskettenformat formatiert ist, als der Benutzer angew�hlt hat.
Mit :hp1.OK:ehp1. wird versucht, die Diskette mit dem neuen Format zu formatieren,
:hp1.Abbruch:ehp1. bricht den Vorgang ab. Als Formatiermodus ist nur :hp1.Formatieren:ehp1.
m�glich.
:eul.
:p.
Zugeh�rige Informationen&colon.
:ul compact.
:li.:link reftype=hd refid=FMTTYPE.Formatiermodus:elink.
:li.:link reftype=hd refid=WARN.Warnungen:elink.
:eul.

.*----------------------------------------------------------
.*  Hilfe Keys
.*      res = PANEL_HFH
.*----------------------------------------------------------
:h1  res=1040 name=PANEL_HFH.Verwenden der Hilfefunktion
:i1.Verwenden der Hilfefunktion
:p.Die Hilfefunktion kann wie folgt aufgerufen werden&colon.
:ul.
:li.Hilfe im Men� zu einem Objekt ausw�hlen.
:li.Hilfe in einem Notizbuch ausw�hlen.
:li.Taste F1 in einem Fenster dr�cken, dessen Men�leiste die Auswahl
Hilfe enth�lt.
:li.Hilfe im Men� des Titelleistensymbols einer unter OS/2 oder DOS
ausgef�hrten Sitzung ausw�hlen.
:li.Den Druckknopf Hilfe ausw�hlen.
:eul.
:p.Der angezeigte Hilfetext h�ngt von der Funktion ab, die beim
Aufrufen der Hilfe aktiviert ist.
:p.Wenn Sie beispielsweise die Hilfefunktion verwenden,
wenn eine Auswahl in der Men�leiste aktiviert ist,
wird ein spezifischer Hilfetext zu dieser Auswahl
angezeigt. Haben Sie ein Fenster aufgerufen, werden
allgemeine oder spezifische Informationen zu diesem
Fenster angezeigt. Im Hilfefenster werden allgemeine
Informationen zu den in der Hilfefunktion verf�gbaren
Auswahlm�glichkeiten in der Men�leiste und in den
Men�s angezeigt.

.*----------------------------------------------------------
.*  Hilfe Laufwerke
.*      res = PANEL_DRIVE
.*----------------------------------------------------------
:h1  res=1050 id=DRIVE name=PANEL_DRIVE.Auswahl des Laufwerkes
:i1 id=DRIVE.Auswahl des Laufwerkes
:p.Die Auswahl des Laufwerkes erfolgt in der Gruppe :hp1.Disk:ehp1..
Die Art, wie die Auswahl erfolgt, h�ngt von dem unter
:link reftype=hd refid=OPTIONS.Optionen:elink. angew�hlten
:link reftype=hd refid=MODE.Laufwerksauswahlmodus:elink. ab.
In dieser Gruppe werden ausschlie�lich im Rechner installierte
Diskettenlaufwerke angezeigt.
:p.
Zugeh�rige Informationen&colon.
:ul compact.
:li.:link reftype=hd refid=MODE.Laufwerksauswahlmodus:elink.
:eul.

.*----------------------------------------------------------
.*  Hilfe Diskettentyp
.*      res = PANEL_TYPE
.*----------------------------------------------------------
:h1  res=1060 id=TYPE name=PANEL_TYPE.Auswahl des Diskettentyps
:i1 id=TYPE.Auswahl des Diskettentyps
:p.In dieser Gruppe wird das gew�nschte Diskettenformat ausgew�hlt.
Folgende Auswahlm�glichkeiten stehen zur Verf�gung&colon.
:ul compact.
:li.:hp2.DD:ehp2.&colon. Disketten normaler Schreibdichte.
:li.:hp2.HD:ehp2.&colon. Disketten hoher Schreibdichte.
:li.:hp2.ED:ehp2.&colon. Disketten sehr hoher Schreibdichte (2,88 MByte Disketten).
:li.:hp2.Spezial:ehp2.&colon. Dieser Knopf ist aktiv, wenn f�r das Laufwerk ein
logisches Laufwerk mit EXTDSKDD definiert wurde, dessen Parameter von
den Standards abweichen (z. B. Spur- oder Sektorzahl).
:eul.

.*----------------------------------------------------------
.*  Hilfe Formatieren
.*      res = PANEL_FORMAT
.*----------------------------------------------------------
:h1  res=1070 name=PANEL_FORMAT.Start des Formatiervorganges
:i1.Start des Formatiervorganges
:p.Nach Dr�cken dieses Knopfes wird die eingelegte Diskette mit
den eingestellten Parametern formatiert.

.*----------------------------------------------------------
.*  Hilfe Label
.*      res = PANEL_LABEL
.*----------------------------------------------------------
:h1  res=1080 name=PANEL_LABEL.Diskettenname
:i1.Diskettenname
:p.In dieses Feld kann optional ein Diskettenname (maximal 11 Zeichen)
eingegeben werden. Dieser Name dient zur besseren Identifizierbarkeit
der Diskette.

.*----------------------------------------------------------
.*  Hilfe Format mode
.*      res = PANEL_FMTTYPE
.*----------------------------------------------------------
:h1  res=1090 id=FMTTYPE name=PANEL_FMTTYPE.Formatiermodus
:i2 refid=MAIN.Formatiermodus
:p.:hp4.PMFORMAT:ehp4. bietet drei verschiedene Formatiermodi.
:ul.
:li.:hp2.Formatieren:ehp2. formatiert die Disketten auf dieselbe Weise
wie das bekannte Formatierprogramm FORMAT.COM von OS/2 und DOS.
:li.:hp2.L�schen:ehp2.&colon. Damit wird die Diskette nur gel�scht, nicht
jedoch formatiert. Dazu wird ein komplett :hp5.neuer Bootsektor:ehp5., ein leeres
Inhaltsverzeichnis sowie zwei leere FATs geschrieben.
In die neuen FATs werden jedoch eventuell vorhandene Markierungen defekter
Sektoren der alten FATs �bernommen.
:p.Nach dieser Methode formatierte Disketten lassen sich mit
"Unerase-Utilities" nicht wieder restaurieren. Au�erdem
werden eventuell vorhandene Bootsektor-Viren gel�scht.
Ein weiterer Vorteil gegen�ber einem "DEL *.*" ist die wesentlich
h�here Geschwindigkeit.
:li.:hp2.Bootsektor schreiben:ehp2.&colon. Damit wird nur der Bootsektor
einer Diskette ausgetauscht, der Rest bleibt unver�ndert.
Wurde im Hauptfenster ein Diskettenname eingetragen, wird dieser ebenfalls
ver�ndert.
:p.Der neue Bootsektor arbeitet etwas anders als der von FORMAT.COM
bekannte&colon. Liegt eine Diskette ohne Betriebssystem in
Laufwerk A&colon., startet der Rechner trotzdem von Festplatte. Der l�stige
Neustart entf�llt. Diese Funktion kann au�erdem zum Entfernen von
Bootsektorviren verwendet werden.
:p.Soll der neue Bootsektor auf einer Diskette mit Betriebssystem verwendet
werden, gilt zu beachtet, da� er nur f�r OS/2 Versionen ab 2.1 geeignet ist!
:eul.
:p.Diese Optionen steht nicht zur Verf�gung, wenn
:link reftype=hd refid=WARN.Warnung ausschalten:elink. angew�hlt wurde.

.*----------------------------------------------------------
.*  Hilfe Warnung ausschalten
.*      res = PANEL_WARNOFF
.*----------------------------------------------------------
:h1  res=1100 id=WARN name=PANEL_WARNOFF.Warnung ausschalten
:i2 refid=MAIN.Warnung ausschalten
:p.Wird versucht, eine bereits formatierte Diskette noch einmal
zu formatieren, wird normalerweise eine Warnung ausgegeben
(s. :link reftype=hd refid=WARNING.Warnungen:elink.).
Die Warnung, da� eine Diskette noch einmal mit demselben Format formatiert
werden soll, l��t sich jedoch mit dem Knopf :hp1.Warnungen:ehp1.
ausschalten.
Diese Einstellung gilt dann jedoch f�r die gesamte Sitzung.
In diesem Fall wird :hp2.Formatieren:ehp2. als
:link reftype=hd refid=FMTTYPE.Formatiermodus:elink. eingestellt.

.*----------------------------------------------------------
.*  Hilfe Auswahlmodus Laufwerke
.*      res = PANEL_BTNTYPE
.*----------------------------------------------------------
:h1  res=1110 id=MODE name=PANEL_BTNTYPE.Auswahlmodus Laufwerke
:i2 refid=OPTION.Auswahlmodus Laufwerke
:p.Zur Auswahl des Diskettenlaufwerkes stehen zwei Methoden zur
Verf�gung&colon.
:ul.
:li.:hp1.Standardmodus&colon.:ehp1. In diesem Modus werden f�r maximal
zwei Laufwerke ("A&colon." und "B&colon.") Radiokn�pfe angezeigt. Diese erm�glichen
einen sehr schnellen Zugriff auf das entsprechende Laufwerk.
:li.:hp1.erweiterter Modus&colon.:ehp1. In diesem Modus erfolgt die Auswahl
durch einen "Spin-Button". Hierdurch ist es m�glich, aus mehr als zwei
Laufwerken auszuw�hlen.
:eul.

.*----------------------------------------------------------
.*  Hilfe Diskettencheck
.*      res = PANEL_CHECK
.*----------------------------------------------------------
:h1  res=1120 id=CHECK name=PANEL_CHECK.Diskettencheck
:i2 refid=OPTION.Diskettencheck
:p.Ist der Knopf :hp1.Format pr�fen:ehp1. markiert, wird bei jedem
Programmstart sowie Laufwerkswechsel der Typ der eingelegten Diskette
gepr�ft und im Fenster
:link reftype=hd refid=TYPE.Auswahl des Diskettentyps:elink defaultm��ig
eingestellt, falls die Diskette bereits formatiert ist.

.*----------------------------------------------------------
.*  Hilfe Diskettendaten
.*      res = PANEL_DSKDATA
.*----------------------------------------------------------
:h1  res=1130 name=PANEL_DSKDATA.Diskettendaten
:i2 refid=OPTION.Diskettendaten
:p.Dieses Fenster zeigt die Daten der zuletzt bearbeiteten Diskette.
Es k�nnen nachfolgende Eintr�ge vorhanden sein&colon.
:ol compact.
:li.:hp2.Byte Gesamtspeicherbereich:ehp2.&colon. Gesamtzahl der Bytes auf
der Diskette.
:li.:hp2.Byte auf dem Datentr�ger verf�gbar:ehp2.&colon. Zahl der noch
freien Bytes auf
der Diskette (Gesamtspeicherbereich - defekte Bereiche - belegt Bereiche).
:li.:hp2.Byte in defekten Sektoren:ehp2.&colon. Zahl der Bytes in defekten
Sektoren.
:li.:hp2.Byte auf jeder Zuordnungseinheit:ehp2.&colon. Zahl der Bytes in
einer Zuordnungseinheit (Sektoren).
:li.:hp2.Zuordnungseinheiten:ehp2.&colon. Gesamtzahl der Zuordnungseinheiten
(Sektoren).
:li.:hp2.verf�gbare Zuordnungseinheiten:ehp2.&colon. Zahl der noch freien
Zuordnungseinheiten (Sektoren) (Gesamtzahl - defekte Sektoren - belegte Sektoren).
:eol.

:euserdoc.
