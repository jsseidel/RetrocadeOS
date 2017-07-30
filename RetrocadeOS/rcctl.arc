#!/bin/bash

MAMEBIN="/home/jsseidel/rc/emu/runmame"
RCBIN="/home/jsseidel/rc/RetrocadeOS/retrocadeos"
PICDIRNORM="/home/jsseidel/rc/arcade_flyers_norm"
PICDIRROR="/home/jsseidel/rc/arcade_flyers_ror"
ININORM="/home/jsseidel/rc/rcini/rc_norm.ini"
INIROR="/home/jsseidel/rc/rcini/rc_ror.ini"

#
# Args for retrocade
NORM="-cx 1024 -cy 768 -bmpx 222 -bmpy 290 -cfg ${ININORM} -pic ${PICDIRNORM} -ss 10"
ROR="-cx 1024 -cy 768 -bmpx 290 -bmpy 222 -cfg ${INIROR} -pic ${PICDIRROR} -ss 10 -ror"

if [[ $1 = "norm" ]] 
then
	ARGS=${NORM}
else
	ARGS=${ROR}
fi

while [[ 1 ]]
do
	# Run retrocade
	`${RCBIN} ${ARGS}`

	# Source the output of retrocade
	. ./RUNGAME
	
	if [[ ${STATE} = "SHELL" ]]
	then	
		exit
	else
		if [[ ${STATE} = "SHUTDOWN" ]] 
		then
			# Only works if running kiosk mode as root
			`shutdown -h now`
		elif [[ ${STATE} = "ROTATE" ]]
		then
			if [[ ${ARGS} = ${NORM} ]] 
			then
				ARGS=${ROR}
			else
				ARGS=${NORM}
			fi
		elif [[ ${STATE} = "SWITCH" ]]
		then	
			exec /home/jsseidel/rc/RetrocadeOS/rcctl.nes
		else
			`${MAMEBIN} ${PREFS} ${STATE}`
		fi
	fi
done
