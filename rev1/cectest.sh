#!/bin/sh
echo "" > ${1}
while true
do

	if  grep -q "key pressed: left" ${1} 
	then
		 sudo simkey L
	fi
	if grep -q "key pressed: right" ${1}
        then
		 sudo simkey R
        fi
        if grep -q "key pressed: up" ${1}
        then
                 sudo simkey U
        fi
        if grep -q "key pressed: down" ${1}
        then
                 sudo simkey D
        fi
        if grep -q "key pressed: exit" ${1}
        then
                 sudo simkey C
        fi
        if grep -q "key pressed: select" ${1}
        then
                 sudo simkey E
        fi
	echo "" > ${1}
	sleep 1s
done
