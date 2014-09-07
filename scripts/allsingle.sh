#!/bin/bash


if [ $# -ne 1 ]
then
	echo "Usage: $0 g-value"
	exit 1
fi

STATES=(30 35 40 35 35 30 30 40 40)
CID=(0 0 0 1 2 1 2 1 2)
FIXPOINTS=(28438 39340 52002 38497 37935 27956 27233 27233 51039 50076)

NMAX=(5 10 15 20 25 30 35)

#First construct normal convergence data:

for i in `seq 0 9`
do
	./single.sh 30 ${STATES[$i]} ${CID[$i]} ${FIXPOINTS[$i]} $1
fi

for nmax in ${NMAX[@]}
do
	./single.sh $nmax 30 0 ${FIXPOINTS[0]} $1
done



