#!/bin/sh
#
# Create archives for upload to site
#

set -e

AR_BIN=i8080emu.zip
AR_SRC=i8080emu-src.zip
rm -f $AR_BIN $AR_SRC

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

(cd dist-bin ; zip -r ../$AR_BIN *)

rm -rf dist-bin


echo "Make source archive"

sh src/clean.sh

zip $AR_SRC AUTHORS BUGS ChangeLog COPYING doc images \
            readme.html screenshot.png TESTING TODO

zip -r $AR_SRC src doc examples images -x '*CVS/*' -x '*.exe' -x '*.dll' -x '*.hlp' -x '*~'

echo Visual test distributive:
echo Libraries use:
wine tdump.exe src/i8080emu.exe  | grep "Imports from"

for p in $PLUGINS ; do
  echo "Imports in $p"
  wine tdump.exe -em. $p | grep 'IMPORT:'
done

