
 DBTool - KiwiSun/Sensolite Vezérlõ kiegészítõ alkalmazás
 ========================================================

 1. Telepítés
 ------------

 Tömörítse ki a DBTool.zip állományt egy üres alkönyvtárba a számítógépe
 merevlemezére. A tömörített állomány a következõ állományokat tartalmazza:

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

 A DBTool alkalmazás több különbözõ részfeladat ellátására alkalmas. 
 Jelenleg csak egy funkciója érhetõ el, ez a bérlettípus adatállomány
 kiegészítése import állományból.

 3. Az alkalmazás használata
 ---------------------------

 3.1. Biztonsági mentés készítése
 --------------------------------

 FONTOS: Az alkalmazás használatával módosítja a Szolárium/Sensolite Vezérlõ alkalmazás 
 adatállományát. Az alkalmazás nem megfelelõ használata adatvesztést eredményezhez, 
 ezért minden esetben az alkalmazás indítása elött csináljon biztonsági mentést!

 A számítógépén a Windows Intézõ segítségével lépjen be a Szolárium/Sensolite Vezérlõ 
 alkalmazás könyvtárát (Solarium vagy Sensolite) tartalmazó könyvtárba. 
 Alapértelmezett esetben ez a könyvtár a 

      C:\Program Files

 Másolja át a Szolárium/Sensolite Vezérlõ alkalmazás teljes könyvtárát

      C:\Program Files\Solarium  (vagy C:\Program Files\Sensolite)

 egy elkülönített helyre, például a C:\Mentes könyvtárba.

 3.2. Program használata
 -----------------------

 Lépjen be abba a könyvtárba, ahová az 1. lépésben kitömörítette a DBTool
 alkalmazás állományait, majd kattintson duplán a DBTool.exe állományra.

 A program elindulása után kövesse az itt leírt lépéseket:

 1. Az 'Indítás' fülön válassza ki a kezelni kívánt program típusát
 2. Válassza ki a Szolárium/Sensolite Vezérlõ alkalmazást tartalmazó könyvtárat a
    könyvtár választó gomb lenyomásával (a felugró kis ablakban navigáljon a
    megfelelõ könyvtárhoz és kattintson az OK gombra)
 3. Kattintson az 'Adatbázis importálás' gombra.
 4. Kattintson a 'Bérlettípusok' fülre
 5. Válassza ki az importálni kívánt bérlettípusokat tartalmazó import állományt
    a 'Választ' gombra kattintva (a felugró kis ablakban navigáljon a megfelelõ
    könyvtárba, jelölje ki az import állományt és kattintson a 'Megnyitás' gombra)
 6. Importálja be a bérlettípusokat az 'Importálás' gombra kattintva.
 7. Hozza létre a kiegészített bérlettípus állományt az 'Exportálás' gombra kattintva.

 Amennyiben befejezte az importálást, kattintson a 'Kilépés az alkalmazásból' gombra.

 3.3. Az új bérlettípus állomány átmásolása
 ------------------------------------------

 A DBTool alkalmazás a 3.2. pontban leírtak végrehajtása során létrehozott
 egy brlttpsfsv_new.dat állományt, ami a módosított bérlettípusok adatait tartalmazza. 
 Ahhoz, hogy az új adatállományt használja a Szolárium/Sensolite Vezérlõ alkalmazás, 
 hajtsa végre az alábbi lépéseket.

 1. A Szolárium Vezérlõ/Sensolite alkalmazás könyvtárából törölje a 'Db' alkönyvtárat és a
    brlttpsfsv.dat állományt.
 2. Másolja át a 3.2. pontban létrejött brlttpsfsv_new.dat állományt a Szolárium/Sensolite 
    Vezérlõ alkalmazás könyvtárába.
 3. Nevezze át a 'brlttpsfsv_new.dat' állományt 'brlttpsfsv.dat'-ra.

 Ezzel az új állományt sikeresen használatba helyezte.

 4. Általános információk
 ------------------------

 Bármilyen kérdés esetén kérem lépjen kapcsolatba a KiwiSun/Sensolite kapcsolattartójával.
 
 Figyelem! Az alkalmazás használata adatvesztést eredményezhet! Kérem csak akkor használja
 az alkalmazást, ha tisztában van a folyamatokkal és vállalja az alkalmazás használatával
 járó következményeket.
 