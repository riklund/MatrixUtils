#!/bin/bash

if [ $# -ne 5 ]
then
	echo "Usage:" $0 "nmax states contourID fix_state g-value"
	exit 1
fi

for i in $1 $2 $3 $4
do
	if [[ ! $i =~ ^[0-9]+$ ]]
	then
		echo "Input must be integer."
		exit 2
	fi
done

outDir="/net/data1/riklund/DiagOut"
memDir="/scratch.local1/riklund/STORAGE/SamePotDifferentPoints/matrices/"
slowScratch="/scratch.local1/riklund"

if [ "$HOSTNAME" == "NO_SUCH_HOST" ]
then
	fastScratch="/tmp/riklund"
fi


if [ "$fastScratch" == "" ]
then
	fastScratch=$slowScratch
fi

dir="nmax"$1"states"$2"contour"$3
unpure0=$memDir$dir"V0.obj"
unpure1=$memDir$dir"V1.obj"
pureName=$slowScratch/$dir".pure"


if [ ! -f $unpure0 ]
then
	echo "Could not find matrix $unpure0"
	exit 3
fi

if [ ! -f $unpure1 ]
then
	echo "Could not find matrix $unpure1"
	exit 4
fi

if [ ! -d $slowScratch ]
then
	echo "Could not find specified scratch drive."
	exit 5
fi

if [ -f $pureName ]
then
	echo "Found pure matrix already, not constructing again."
else
	./Purify $unpure0 $unpure1 $pureName
fi

if [ $? -ne 0 ]
then
	echo "Error when purifying matrix."
	exit 6
fi

mkdir -p $outDir"/"$dir

g=$5
echo "Starting computations for g=$g"
matName=$fastScratch"/"$dir"_g="$g".obj"
outName=$outDir"/"$dir"/eigenTwo_"$g".dat"
if [ -f $outName ]
then
	echo "Found file $outName already, not recomputing."
else
	./ConstructMatrix  $pureName $matName $g
	if [ $? -ne 0 ]
	then
		echo "Error when constructing matrix " $matName
		exit 7
	fi
	./Jacobi $matName $4 $outName
	
	#To prevent overflow of drive.
	rm $matName
fi