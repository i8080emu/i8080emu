#!/bin/sh
# i8080asm preprocessor test

cd test
rm -f pre.log/*
rm -f asm.log/*


for i in *.asm
do

#  echo Testing $i

  name=`echo $i | sed -e 's/\(.*\)\.asm/\1/'`

  # preprocess test	
  ../i8080asm -d 0 -p $i > res/$i 2>pre.log/${name}.log

  if ! cmp -s res/$i must/$i
  then
    echo $i: BAD preprocess
  fi
  

  # assemble test
  ../i8080asm -d 0 -o res/${name}.bin $i 2>&1 > asm.log/${name}.log

  if ! cmp -s res/${name}.bin must/${name}.bin
  then
	  echo $i: BAD assemble
  fi

done
