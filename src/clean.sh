#!/bin/sh

find -iname '*.~*' -o -iname '*.obj' -o -iname '*.td?' -o -name 'Thumbs.db' \
  -o -name '*.ddp' -o -name '*.res' | xargs rm -f

rm -f i8080emu.INI

