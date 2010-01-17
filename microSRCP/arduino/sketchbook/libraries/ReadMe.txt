
Hier sind die Klassen aus dem SRCPCore Projekt zu kopieren.

Die Arduino IDE findet diese dann automatisch und kompiliert diese mit.

Zusaetzlich muss im Verzeichnis i2c und lan ein Unterverzeichnis utility
angelegt werden. 

Dort sind folgende Dateien aus hardware/cores/arduino und 
libraries/Ethernet/utility zu kopieren:

i2c:
twi.c
twi.h
types.h

lan:
socket.h
types.h
spi.h
w5100.h

17. Januar 2010 / Marcel Bernet

