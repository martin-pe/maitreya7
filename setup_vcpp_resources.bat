@rem
@rem setup for stardard files needed by Visual Studio
@rem

cd po
mkdir de
copy de.gmo de\maitreya7.mo

mkdir te
copy te.gmo te\maitreya7.mo

mkdir ru
copy ru.gmo ru\maitreya7.mo

mkdir pl
copy pl.gmo pl\maitreya7.mo

mkdir it
copy it.gmo it\maitreya7.mo

mkdir hu
copy hu.gmo hu\maitreya7.mo

mkdir ro
copy ro.gmo ro\maitreya7.mo

mkdir es
copy es.gmo es\maitreya7.mo

mkdir fr
copy fr.gmo fr\maitreya7.mo

mkdir ta
copy ta.gmo ta\maitreya7.mo

cd ..

cd src\gui

copy ..\atlas\geonames1000.sql .

pause
