#!/bin/bash



version=`grep AC_INIT configure.in | sed "s/.*, //g;s/)//g"`

dmgfile="maitreya-${version}.dmg"

basedir="maitreya-${version}"
appdir="${basedir}/Maitreya7.app"

contentsdir="${appdir}/Contents"
macosdir="${contentsdir}/MacOS"
resourcesdir="${contentsdir}/Resources"

create_dmg()
{
	echo "Creating disk image ${dmgfile}"
	rm -f ${dmgfile}
	hdiutil create -srcfolder ${basedir} ${dmgfile}
	echo "Done"
}

cleanup()
{
	echo "Cleaning directory $basedir"
	rm -fr $basedir
	echo "Done"
}

create_targetdir()
{
	echo "Preparing Mac release in directory $appdir ..."

	mkdir -p ${macosdir}
	mkdir -p ${resourcesdir}

	cp src/gui/maitreya7.bin $macosdir/Maitreya7
	cp MacOS/Info.plist $contentsdir
	cp MacOS/PkgInfo $contentsdir
	cp MacOS/readme_macos.txt $basedir

	echo -n "Languages: "
	for l in `cat po/LINGUAS`
	do
		echo -n " $l"
		mkdir -p $resourcesdir/$l
		cp po/$l.gmo $resourcesdir/$l/maitreya7.mo
	done
	echo " done."

	echo "Pictures ..."
	mkdir -p $resourcesdir/pics
	cp src/pics/*.jpg $resourcesdir/pics
	cp src/pics/*.png $resourcesdir/pics
	cp src/icons/*.icns $resourcesdir

	mkdir -p $resourcesdir/fonts
	cp -r src/fonts/MaitreyaSymbols.TTF $resourcesdir/fonts
	cd $basedir
	ln -fs Maitreya7.app/Contents/Resources/fonts/MaitreyaSymbols.TTF
	cd -

	cp COPYING $basedir

	echo "Locations ..."
	mkdir -p $resourcesdir/atlas
	cp src/atlas/geonames1000.sql $resourcesdir/atlas

	echo "XML files ..."
	mkdir -p $resourcesdir/xml/yogas
	cp -r src/xml/*.xml $resourcesdir/xml
	cp -r src/xml/*.dtd $resourcesdir/xml
	cp -r src/xml/yogas/*.xml $resourcesdir/xml/yogas
	cp -r src/xml/yogas/*.dtd $resourcesdir/xml/yogas

	echo "Done"
}

echo "Start $1"
echo "Version is \"$version\" dmg is ${dmgfile}"
echo "Appdir is $appdir"


if [[ $1 == "clean" ]]
then
	cleanup
elif [[ $1 == "dmg" ]]
then
	create_targetdir
	create_dmg
else
	create_targetdir
fi


