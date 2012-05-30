
 DBTool - KiwiSun/Sensolite Vez�rl� kieg�sz�t� alkalmaz�s
 ========================================================

 1. Telep�t�s
 ------------

 T�m�r�tse ki a DBTool.zip �llom�nyt egy �res alk�nyvt�rba a sz�m�t�g�pe
 merevlemez�re. A t�m�r�tett �llom�ny a k�vetkez� �llom�nyokat tartalmazza:

      libgcc_s_dw2-1.dll
      libmysql.dll
      mingwm10.dll
      QtCore4.dll
      QtGui4.dll
      QtNetwork4.dll
      DBTool.exe
      dbtool_hu.qm

 2. Program feladata
 -------------------

 A DBTool alkalmaz�s t�bb k�l�nb�z� r�szfeladat ell�t�s�ra alkalmas. 
 Jelenleg csak egy funkci�ja �rhet� el, ez a b�rlett�pus adat�llom�ny
 kieg�sz�t�se import �llom�nyb�l.

 3. Az alkalmaz�s haszn�lata
 ---------------------------

 3.1. Biztons�gi ment�s k�sz�t�se
 --------------------------------

 FONTOS: Az alkalmaz�s haszn�lat�val m�dos�tja a Szol�rium/Sensolite Vez�rl� alkalmaz�s 
 adat�llom�ny�t. Az alkalmaz�s nem megfelel� haszn�lata adatveszt�st eredm�nyezhez, 
 ez�rt minden esetben az alkalmaz�s ind�t�sa el�tt csin�ljon biztons�gi ment�st!

 A sz�m�t�g�p�n a Windows Int�z� seg�ts�g�vel l�pjen be a Szol�rium/Sensolite Vez�rl� 
 alkalmaz�s k�nyvt�r�t (Solarium vagy Sensolite) tartalmaz� k�nyvt�rba. 
 Alap�rtelmezett esetben ez a k�nyvt�r a 

      C:\Program Files

 M�solja �t a Szol�rium/Sensolite Vez�rl� alkalmaz�s teljes k�nyvt�r�t

      C:\Program Files\Solarium  (vagy C:\Program Files\Sensolite)

 egy elk�l�n�tett helyre, p�ld�ul a C:\Mentes k�nyvt�rba.

 3.2. Program haszn�lata
 -----------------------

 L�pjen be abba a k�nyvt�rba, ahov� az 1. l�p�sben kit�m�r�tette a DBTool
 alkalmaz�s �llom�nyait, majd kattintson dupl�n a DBTool.exe �llom�nyra.

 A program elindul�sa ut�n k�vesse az itt le�rt l�p�seket:

 1. Az 'Ind�t�s' f�l�n v�lassza ki a kezelni k�v�nt program t�pus�t
 2. V�lassza ki a Szol�rium/Sensolite Vez�rl� alkalmaz�st tartalmaz� k�nyvt�rat a
    k�nyvt�r v�laszt� gomb lenyom�s�val (a felugr� kis ablakban navig�ljon a
    megfelel� k�nyvt�rhoz �s kattintson az OK gombra)
 3. Kattintson az 'Adatb�zis import�l�s' gombra.
 4. Kattintson a 'B�rlett�pusok' f�lre
 5. V�lassza ki az import�lni k�v�nt b�rlett�pusokat tartalmaz� import �llom�nyt
    a 'V�laszt' gombra kattintva (a felugr� kis ablakban navig�ljon a megfelel�
    k�nyvt�rba, jel�lje ki az import �llom�nyt �s kattintson a 'Megnyit�s' gombra)
 6. Import�lja be a b�rlett�pusokat az 'Import�l�s' gombra kattintva.
 7. Hozza l�tre a kieg�sz�tett b�rlett�pus �llom�nyt az 'Export�l�s' gombra kattintva.

 Amennyiben befejezte az import�l�st, kattintson a 'Kil�p�s az alkalmaz�sb�l' gombra.

 3.3. Az �j b�rlett�pus �llom�ny �tm�sol�sa
 ------------------------------------------

 A DBTool alkalmaz�s a 3.2. pontban le�rtak v�grehajt�sa sor�n l�trehozott
 egy brlttpsfsv_new.dat �llom�nyt, ami a m�dos�tott b�rlett�pusok adatait tartalmazza. 
 Ahhoz, hogy az �j adat�llom�nyt haszn�lja a Szol�rium/Sensolite Vez�rl� alkalmaz�s, 
 hajtsa v�gre az al�bbi l�p�seket.

 1. A Szol�rium Vez�rl�/Sensolite alkalmaz�s k�nyvt�r�b�l t�r�lje a 'Db' alk�nyvt�rat �s a
    brlttpsfsv.dat �llom�nyt.
 2. M�solja �t a 3.2. pontban l�trej�tt brlttpsfsv_new.dat �llom�nyt a Szol�rium/Sensolite 
    Vez�rl� alkalmaz�s k�nyvt�r�ba.
 3. Nevezze �t a 'brlttpsfsv_new.dat' �llom�nyt 'brlttpsfsv.dat'-ra.

 Ezzel az �j �llom�nyt sikeresen haszn�latba helyezte.

 4. �ltal�nos inform�ci�k
 ------------------------

 B�rmilyen k�rd�s eset�n k�rem l�pjen kapcsolatba a KiwiSun/Sensolite kapcsolattart�j�val.
 
 Figyelem! Az alkalmaz�s haszn�lata adatveszt�st eredm�nyezhet! K�rem csak akkor haszn�lja
 az alkalmaz�st, ha tiszt�ban van a folyamatokkal �s v�llalja az alkalmaz�s haszn�lat�val
 j�r� k�vetkezm�nyeket.
 