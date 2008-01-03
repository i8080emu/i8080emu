#!/bin/sh
#
# Create archives for upload to site
#

set -e

AR_BIN=i8080emu.zip
AR_SRC=i8080emu-src.zip

rm -f $AR_BIN $AR_SRC

rm -rf dist-bin
rm -rf dist-src

mkdir dist-bin
mkdir dist-src


echo "Make binary archive"
cp src/i8080emu.exe              dist-bin
cp doc/i8080emu.hlp              dist-bin
cp doc/i8080emu.cnt              dist-bin
cp readme.html screenshot.png    dist-bin
mkdir dist-bin/plugins
#cp src/plg_spkr/speaker.dll      dist-bin/plugins
#cp src/plg_comport/com_port.dll  dist-bin/plugins
mkdir dist-bin/examples
cp examples/*                    dist-bin/examples
cp AUTHORS BUGS ChangeLog TODO   dist-bin
cp COPYING                       dist-bin

(cd dist-bin ; zip -r ../$AR_BIN *)



echo "Make source archive"

zip $AR_SRC AUTHORS BUGS ChangeLog COPYING doc examples images \
            readme.html screenshot.png TESTING TODO

zip -R $AR_SRC `find -name '*.h' -o -name '*.cpp' -o -name '*.c' -o -name '*.dfm' -o -name '*.bpr' -o -name '*.txt' -o -name '*.bpf'`

echo Visual test distributive:
echo Libraries use:
wine tdump.exe src/i8080emu.exe | grep "Imports from"

rm -rf dist-bin
rm -rf dist-src
