#!/bin/sh
#
# Create archives for upload to site
#
# Requires:
# * MSYS (only base system)
# * zip (mingw-get.exe install msys-zip)
# * borland C++ builder of cause
#
# Before run this script:
# * compile manually i8080 and plugins (at least com_port and speaker)
# * compile help file (see doc/readme.txt)
#
# How to run this script:
# * Start/All Programms/MinGW/MinGW Shell
# * cd /c/i8080
# * makedist.sh

set -e


#TDUMP="wine tdump.exe"
TDUMP="/c/Program Files/Borland/CBuilder6/Bin/tdump.exe"

OUTPUT_BIN=i8080emu.zip
OUTPUT_SRC=i8080emu-src.zip
rm -f $OUTPUT_BIN $OUTPUT_SRC

rm -rf dist-bin
mkdir  dist-bin


PLUGINS="src/plg_spkr/speaker.dll src/plg_comport/com_port.dll"

echo "Make binary archive"
cp src/i8080emu.exe              dist-bin
cp doc/I8080EMU.HLP              dist-bin/i8080emu.hlp
cp doc/i8080emu.cnt              dist-bin
cp readme.html screenshot.png    dist-bin
mkdir dist-bin/plugins
cp $PLUGINS                      dist-bin/plugins
mkdir dist-bin/examples
cp examples/*.asm                dist-bin/examples
cp AUTHORS BUGS ChangeLog TODO   dist-bin
cp COPYING                       dist-bin

( cd dist-bin ; zip -r ../$OUTPUT_BIN * )

rm -rf dist-bin


echo "Make source archive"

sh src/clean.sh

zip $OUTPUT_SRC AUTHORS BUGS ChangeLog COPYING doc images \
            readme.html screenshot.png TESTING TODO

zip -r $OUTPUT_SRC src doc examples images -x '*CVS/*' -x '*.exe' -x '*.dll' -x '*.hlp' -x '*~'

echo Visual test distributive:
echo Libraries use:
"$TDUMP" src/i8080emu.exe  | grep "Imports from"

for p in $PLUGINS ; do
  echo "Imports in $p"
  "$TDUMP" -em. $p | grep 'IMPORT:'
done
