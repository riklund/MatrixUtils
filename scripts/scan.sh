#!/bin/bash

if [ $# -ne 4 ]
then
	echo "Usage:" $0 "nmax states contourID fix_state"
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

scratchDrive="/scratch.local1/riklund"

dir="nmax"$1"states"$2"contour"$3
unpure0="/net/data2/riklund/SamePotDifferentPoints/matrices/"$dir"V0.obj"
unpure1="/net/data2/riklund/SamePotDifferentPoints/matrices/"$dir"V1.obj"
pureName=$scratchDrive/$dir".pure"


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

if [ ! -d $scratchDrive ]
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

mkdir -p $scratchDrive"/"$dir

for gH in `seq -100 100`
do
	no=`echo $gH/1000 | bc -l`
	g=`printf %.03f  $no`
	echo "Starting computations for g=$g"
	matName=$scratchDrive"/"$dir"_g="$g".obj"
	outName=$scratchDrive"/"$dir"/eigenTwo_"$g".dat"
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
done