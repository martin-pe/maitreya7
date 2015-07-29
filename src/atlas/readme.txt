Atlas Howto
-----------

Prereqisites
------------
- Install SQLite3 command line interface on your machine
- Install Perl and SQLite 3 driver (something like libdbd-sqlite3)
- Download and unpack required files from geonames.org download center and move them into the folder "data"

Required files
--------------
timeZones.txt
countryInfo.txt
featureCodes_en.txt
admin1CodesASCII.txt

and one of
- cities15000.txt (cities with more than 15000 inhabitants, about 3.5MB)
- cities1000.txt (more than 1000 inhabitants, about 16MB)
- allCountries.txt (all, about 940MB)


Create tables
-------------
Start SQLite command line tool, import db_ddl.sql 

Example (Linux)

~/.maitreya-7.0> sqlite3 geonames.db
SQLite version 3.6.22
Enter ".help" for instructions
Enter SQL statements terminated with a ";"
sqlite> .read db_ddl.sql 
sqlite> 
sqlite> 
sqlite> .quit
~/.maitreya-7.0> 

Import data
-----------
Edit Perl script geoname.pl if necessary
- name of city file
- name of database

Start script

