#!/bin/bash

for file in `ls kcluster*.lp`
do
	glpsol --lp $file --tmlim 900 > $file.sol
done
