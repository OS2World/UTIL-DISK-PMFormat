:userdoc.
:title.PMFORMAT
:docprof toc='*3'.
:h1.PMFORMAT
:lines align=center.

:elines.
:cgraphic.
:color fc=red.
€ﬂﬂﬂﬂﬂﬂﬂ€ﬂﬂﬂﬂﬂﬂﬂ€ﬂﬂﬂﬂﬂﬂﬂ€ﬂﬂﬂﬂﬂﬂﬂ€ﬂﬂﬂﬂﬂﬂ€€ﬂﬂﬂﬂﬂﬂﬂ€€ﬂﬂﬂﬂﬂ€€ﬂﬂﬂﬂﬂﬂﬂ€
€ €€€€€ € €€ €€ € €€€€€€€ €€€‹  € €€€€ €€ €€ €€ €€ €€€ €€€€€ €€€€
€  ‹‹‹‹‹€  € €€ €  ‹‹‹‹‹€ €€€€€ €  ‹‹‹‹ €  € €€ €  ‹‹‹‹ €€€€  €€€
€  €€€€€€  € €€ €  €€€€€€ €€€€€ €  €€€€ €  € €€ €  €€€€ €€€€  €€€
€‹‹€€€€€€‹‹€‹€€‹€‹‹€€€€€€‹‹‹‹‹‹‹€‹‹€€€€‹€‹‹€‹€€‹€‹‹€€€€‹€€€€‹‹€€€
:color fc=blue.

:hp2.Der PM - Disketten - Formatierer              Version 2.16


    (C) 1992 ... 2001 noller &amp. breining software

    * * * * * * * * * * * * * * * * * * * * * * * * *:ehp2.
:ecgraphic.

:h1.Programminfo zu PMFORMAT

:artwork align=center name='PMFORMAT.BMP'.

:lines align=center.
PMFORMAT  Version 2.16

(c) 1992...2001, noller &amp. breining software

e-mail: klaus.breining@t-online.de
Telefon&colon. (49)7191-960345
:elines.

:h1.EinfÅhrung
:p.Dieses Programm dient zum Formatieren von Disketten unter
dem PM (Presentation Manager) von OS/2. Die Bedienung wird
dabei komplett in einem kleinen Fenster abgehandelt und ist
daher sehr Åbersichtlich. Die vielen Schaltervariablen des
Standard-Formatier-Programmes werden daher ÅberflÅssig.

:p.Das Programm erzeugt bei neuen Disketten automatisch eine
DatentrÑger-Kennung, bei bereits formatierten wird diese
Kennung beim Formatieren beibehalten. DarÅber hinaus wird
der Benutzer gewarnt, falls die zu formatierende Diskette
bereits Daten enthÑlt. In diesem Fall werden zusÑtzliche
Optionen, die :hp2.Formatiermodi:ehp2. angeboten.

:p.Wir, das sind Klaus Breining und Markus Noller, vertreiben
unsere gemeinsam geschriebene Software unter dem Namen
noller &amp. breining software. Obwohl es unser Bestreben ist,
nur voll funktionsfÑhige und grÅndlich ausgetestete Programme
an die Allgemeinheit weiterzugeben, kann sich immer ein Fehler
einschleichen.
Wir sind daher offen fÅr jede Art von Fehlermeldung.
:h1.HINWEIS

:p.:hp8.HINWEIS:ehp8.

:p.:hp8.PMFORMAT wird ohne eine Garantie irgendwelcher Art zur
VerfÅgung gestellt. Die Benutzung von PMFORMAT geschieht auf
Ihr eigenes Risiko.:ehp8.

:p.Die folgenden Namen sind geschÅtzte Warenzeichen und werden
hier nur als Referenz verwendet:
:ul.
:li.IBM und OS/2 - IBM Corporation
:li.MSDOS - Microsoft Corporation
:eul.

:h1.Aufruf

:p.Das Programm wird einfach mit :hp2.PMFORMAT:ehp2. aufgerufen. Parameter
kînnen nicht Åbergeben werden.

:cgraphic.
&bxh.&bxh.&bxh.&bxh.&bxh.&bxde.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxde.&bxh.&bxh.&bxh.&bxh.&bxh.&bxde.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxde.&bxh.&bxh.&bxh.:link
reftype=launch object='pmformat.exe'. PMFORMAT :elink.&bxh.&bxh.&bxde.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxde.&bxde.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxh.&bxde.&bxri.
     &bxll.&bxh.:link refid=drive reftype=fn. Laufwerk :elink.&bxh.&bxlr.     &bxll.&bxh.:link refid=drive reftype=fn. Pfad :elink.&bxh.&bxlr.               &bxll.&bxh.:link
reftype=fn refid=ddrive. D&colon. :elink.&bxh.&bxlr.&bxll.&bxh.:link reftype=fn refid=label. label :elink.&bxh.&bxlr.
:ecgraphic.

:fn id=drive.
:lines align=center.
Externer Befehl - Parameter: Laufwerk, Pfad
 
:elines.
:p.Laufwerk und Pfad fÅr den auszufÅhrenden Befehl; externe Befehle
sind in Form von Programmdateien auf der Festplatte gespeichert.
Wird ein externer Befehl eingegeben, sucht das System im
aktuellen Verzeichnis des angegebenen Laufwerks oder des
Standardlaufwerks nach diesem Befehl. Wird der Befehl dort nicht
gefunden, wird die Suche in den Verzeichnissen fortgesetzt, die in
der Anweisung PATH angegeben sind.
:efn.

:fn id=ddrive.
:lines align=center.
Laufwerk
:elines.
:p.Bestimmt das Laufwerk, in dem die zu formatierende Diskette liegt.
:efn.

:fn id=label.
:lines align=center.
Diskettenname
:elines.
:p.Hier kann ein Diskettenname mit bis zu 11 Zeichen angegeben werden.
:efn.

:p.Das Programm lÑ·t sich sehr einfach in einen Ordner der ArbeitsoberflÑche
einfÅgen. Es kann von dort dann mit nur einem Doppelklick
starten.

:h1.Bedienung

:p.Die Bedienung des Programms ist denkbar einfach. Es wird der
gewÅnschte Name der neuen Diskette eingetragen, das Laufwerk
und der Diskettentyp eingestellt.
Folgende Typen sind verfÅgbar:
:ul.
:li.DD - Disketten mit doppelter Schreibdichte (3´" und 5¨").
:li.HD - Disketten mit hoher Schreibdichte (3´" und 5¨").
:li.ED - Disketten mit 2,88 MByte (3´").
:li.Spezial - Spezialformate fÅr den Treiber EXTDSKDD (3´" und 5¨").
:eul.
:p.Die Angabe des Diskettennamens ist optional und kann entfallen.
Durch Druck auf den FORMAT-Knopf wird die Formatierung gestartet.

:p.WÑhrend des Formatiervorganges dient der Knopf am unteren
Ende des Fensters als STOP-Knopf zum Abbrechen der Aktion.
Das Ende des Formatiervorgangs wird durch einen
Ton und ein Informationsfenster angezeigt. Im Fenster werden
die Daten der Diskette angezeigt.

:p.Sollte die Diskette bereits formatiert sein, so wird der
Benutzer darauf hingewiesen: Ein zusÑtzliches Warnfenster
klappt hoch; darin werden die DatentrÑger-Kennung und ggf.
der Diskettenname angezeigt. Man kann dann entscheiden, ob
trotzdem formatiert werden soll. Auch das Aufklappen dieses
Fensters ist mit einem Ton verbunden, damit man auch nach
dem Wechsel in eine Gesamtbildschirm-Sitzung aufmerksam wird
und nicht endlos auf das Ende der Formatierung wartet.
Au·erdem werden in diesem Fall Auswahlknîpfe fÅr die
:hp2.Formatiermodi:ehp2. aktiviert.
:ul compact.
:li.:hp1.Formatieren:ehp1. Mit diesem Knopf wird ein normaler
Formatiervorgang ausgewÑhlt.
:li.:hp1.Lîschen:ehp1. Diese Auswahl dient zum Lîschen bereits
formatierter Disketten. Dies geschieht durch Schreiben eines neuen
Bootsektors, eines leeren Inhaltsverzeichnisses sowie zwei leeren
FAT's. Von der alten Formatierung werden nur die EintrÑge defekter
Sektoren in der FAT Åbernommen.
:li.:hp1.Bootsektor schreiben:ehp1. Hiermit wird lediglich der
Bootsektor der Diskette gegen einen neuen ausgetauscht.
:p.:hp8.Der Bootsektor von :ehp8.:hp4.PMFORMAT:ehp4.:hp8. ist kompatibel zum
Bootsektor von OS/2 ab Version 2.1. Er ist damit in der Lage, dieses Betriebssystem von der
Diskette zu starten. Im Gegensatz zum Bootsektor von FORMAT.COM,
der mit OS/2 mitgeliefert wird, startet er das Betriebssystem von
der Festplatte, wenn auf einer eingelegten Diskette keine
Systemdateien vorhanden sind.:ehp8.
:eul.

:p.Das Erscheinen dieses Fenster kann durch Deaktivieren des
Knopfes :hp2.Warnungen:ehp2. fÅr die gesamte, weitere Programmlaufzeit
unterdrÅckt werden.

:p.Falls das gewÑhlte Diskettenformat nicht mit dem der bereits
formatierten Diskette Åbereinstimmt, stehen die :hp2.Formatiermodi:ehp2.
nicht zur VerfÅgung! Au·erdem lÑ·t sich diese Warnung durch den
Knopf :hp2.Warnungen:ehp2. nicht unterdrÅcken.

:p.Sollte wÑhrend der Formatierung ein Systemfehler auftreten,
so wird dieser ebenfalls in einem Dialogfenster angezeigt.
Die angezeigte Fehlernummer kann wie gewohnt auf einer
Kommandozeile mit HELP abgefragt werden. Ein Fehler auf der
Diskette wird durch das Einsetzen eines roten Abschnitts
im Anzeigebalken angezeigt.

:p.Wird die Formatierung durch einen Systemfehler oder die
Beendigung im SystemmenÅ abgebrochen, hat dies keine Folgen
fÅr das Laufwerk, die Diskette ist dagegen
unbrauchbar und sollte vor Verwendung als Speichermedium
noch einmal formatiert werden.

:p.Das Programm sichert, wie viele WPS-Progamme, seine
Fensterposition beim Schlie·en des Fensters.

:h1.Erweiterungen der Version 2.16
:p.:hp2.PrÅfen der Laufwerke:ehp2.
:p.Das Formatierprogramm sucht nach dem Start sÑmtliche im
Rechner vorhandenen Diskettenlaufwerke und bestimmt die mîglichen
Diskettentypen.
Liegt im Laufwerk eine Diskette, wird auch versucht, deren Typ bestimmen,
falls die Option :hp2.Format prÅfen:ehp2. im Optionen-MenÅ gesetzt wurde.

:p.:hp2.Kommandozeilenargumente:ehp2.
:p.PMFORMAT unterstÅtzt Kommandozeilenargumente, die
kompatibel sind zu den Argumenten des PMFORMAT-Programmes,
das mit OS/2 ab Version 2.1 ausgeliefert wird. Es gehorcht folgender
Syntax:
:p.:font facename=Courier size=10x8.
PMFORMAT [drive[disklabel]]
:font facename=default.
:p.Hierbei ist :font facename=Courier size=10x8.drive:font facename=default.
der Laufwerksbuchstabe ohne Doppelpunkt '&colon.'.

:p.:hp2.Standard- und erweiterter Modus:ehp2.
:p.In den meisten Rechnern sind 2 Diskettenlaufwerke installiert.
Zur Auswahl dieser bietet sich der sog. Radioknopf an, da er
am schnellsten mit der Maus zu erreichen ist.
Sind jedoch mehr als 2 physikalische oder logische
Diskettenlaufwerke installiert (z. B. mit Hilfe des Treibers EXTDSKDD),
kînnen diese im erweiterten Modus erreicht werden.
Hier erfolgt die Auswahl der Laufwerke Åber einen Spinbutton, der
bei minimalem Platzbedarf bis zu 26 Laufwerke fa·t.

:p.:hp2.ED und Spezial-Diskettenformate:ehp2.
:p.Seit einiger Zeit sind fÅr den PC 3´"-Diskettenlaufwerke mit einer
KapazitÑt von 2,88 MByte verfÅgbar.
PMFORMAT 2.12 ermîglicht es jetzt, diese Disketten unter der Formatbezeichnung
ED zu formatieren.

:p.Zum Lieferumfang von OS/2 gehîrt der GerÑtetreiber EXTDSKDD.SYS.
Dieser ermîglicht es, auf ein physikalisches Laufwerk Åber weitere logische
Laufwerksbuchstaben zuzugreifen.
Hierbei kînnen auch Diskettenformate angegeben werden, die kein MSDOS bzw. 
OS/2 Standard sind.
FÅr solche Formate ist der vierte Typ "Spezial" vorgesehen. Er holt sich das
Format der Diskette vom Treiber ab und formatiert damit die in das Laufwerk
eingelegte Diskette.

:p.:hp2.Formatiermodi:ehp2.
:p.Oftmals dient die Formatierung einer Diskette nur dem schnellen Lîschen.
HierfÅr bietet PMFORMAT jetzt einen Lîschmodus. Er lîscht nicht
nur die VerzeichniseintrÑge und FAT's, er schreibt auch einen neuen
OS/2-Bootsektor.
Von der alten Diskettenformtierung werden nur die "Bad-Sektor"-EintrÑge der alten
FAT's Åbernommen.
:p.Durch den Modus :hp1.Bootsektor schreiben:ehp1. ist es mîglich, den
Bootsektor der Diskette gegen einen etwas modifizierten zu tauschen.
Damit lassen sich auch auf einfache Weise Boot-Sektor-Viren
wieder entfernen.

:h1.Registrierung

:p.Das Programm wird als :hp2.SHAREWARE:ehp2. in Umlauf gebracht.

:p.Dabei wollen wir nur :hp2.GANZ WENIG GELD:ehp2. (eigentlich nicht der
Rede wert). FÅr Benutzer in Deutschland (und alle, die in der Lage sind,
uns in DM zu bezahlen) gibt es die registrierte Version zum Preis von
nur

:p.:hp2.10.- DM (in Worten: zehn):ehp2.

:p.als Geldschein oder Scheck im Brief. Man bekommt dafÅr die aktuelle
Version von :hp4.PMFORMAT:ehp4. in deutsch und englisch
auf einer Diskette nach Wahl (5 1/4 oder 3 1/2 Zoll)
und eine Mitteilung, falls durch Programmfehler (was wir
nicht hoffen) oder durch AnwenderwÅnsche eine neue Version
entstehen sollte.

:h2.Registrierungsformular
:fig.
                                         _____________, den __.__.__




 
      noller &amp. breining software
      Lauffener Str. 4

      D - 71522 Backnang





      Betreff: Registrierung PM-Format


      Ja, ich mîchte PMFORMAT registrieren.  Mir ist bekannt,  da·
      es  sich dabei um Software handelt,  die durch ihr  Verteil-
      konzept  (Shareware) und ihren Preis keine Garantie  irgend-
      welcher  Art enthalten kann.  Ich lege die  RegistriergebÅhr
      von DM 10.- als

      o   Geldschein/-stÅck
      o   Scheck

      bei. Ich wÅnsche die Zusendung der Software auf einer

      o   5 1/4 - Zoll Diskette
      o   3 1/2 - Zoll Diskette.

      Die Zusendung soll an folgende Adresse gerichtet sein:



      Vorname, Name: _______________________________

      Stra·e, Nr.:   _______________________________

      PLZ, Wohnort:  _______________________________



      __________________________________
      (Unterschrift)

:efig.

:h1.Versionen
:cgraphic.:hp2.
   2.16 Help-Datei wird auch im Verzeichnis der binary-Datei gesucht

   2.15 Entfernen des Registrierdialoges

   2.14 CodeÑnderung im Bootsektor wegen Virenscanner

   2.13 Ausbau der Shareware-BeschrÑnkungen

   2.12 Anpassung an verÑnderte RÅckgabewerte in DosDevIOCtl
        unter OS/2 Warp 3.0.

   2.11 Korrektur der Versionsabfrage, um LauffÑhigkeit unter OS/2 2.11
        zu ermîglichen (Service Pack XR06200). EinfÅhren der
        drei Formatiermodi und eines neuen Bootsektors.

   2.1  OS/2 2.1 Version. Verwendet eine einfachere Methode zur
        Untersuchung der Laufwerkstypen, da der OS2DASD.DMD jetzt
        endlich komplett ist. EnthÑlt einen Schnellformatiermodus.

   2.0  OS/2 2.0 Version. UnterstÅtzt mehr als 2 Laufwerke,
        weitere Diskettenformate, sowie Kommandozeilenargumente
        (Diese Version ist wegen Problemen mit OS/2 2.0 nie
        allgemein verfÅgbar gewesen).

   1.3  GerÑteunabhÑngige Positionsberechnung im Fenster.
        Ausgabe von Disketten-Daten nach Formatierung.

   1.2  Verbesserung bei Thread-Handling. Edit-Box behÑlt
        Diskettennamen bei Fehler.

   1.1  Abspeicherung der Fenster-Position in OS2.INI bei
        BetÑtigung von "System / Sichern" und eigenem MenÅ.

   1.01 Korrektur bei Abweisen "ungÅliger Zeichen" im Label.

   1.0  Bereitstellung der ersten îffentlichen Version:ehp2.
:ecgraphic.
:euserdoc.
