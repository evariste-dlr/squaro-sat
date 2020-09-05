#!/bin/bash

prompt="> "


if [ -z $1 ]
then
    
    echo "Utilisation : "
    echo "-------------"
    echo " "
    echo "     $0 <command> [opt] fichier [fichier2, [...]]"
    echo " "
    echo "Commandes : "
    echo "-----------"
    echo " "
    echo "     gen [-v]  <taille>    fichier.plt"
    echo "     res [opt] fichier.plt "
    echo " "
    echo "Tapez $0 help ou regardez le fichier README "
    echo "pour plus de détail"

else

    # récupération des arguments
    cmd=$1
    
    if [ $cmd = "gen" ] 
    then
	if [ "$2" = "-v" ]
	then
	    taille="$3"
	    fichier="$4"
	else
	    taille="$2"
	    fichier="$3"
	fi
	
	./bin/alea "$taille" "$fichier"
    elif [ $cmd = "res" ]
    then
	sat="sat.tmp"
	
	if [ "$#" = "3" ]
	then
	    opt="$2"
	    plt="$3"
	else
	    opt=""
	    plt="$2"
	fi

	cd bin
	./plt_sat "../$plt" "$sat"

	if [ "$opt" = "-m" ]
	then
	    ./MiniSat "$sat" "asg.tmp"
	else
	    ./dpll "$sat" "asg.tmp" > "../trace.txt"
	    if [ "$opt" != "-v" ]
	    then
		rm "../trace.txt"
	    fi
	fi

	./print "../$plt" "asg.tmp"

	rm *.tmp
	cd ..
    elif [ $cmd = "help" ]
    then
	cat README.txt
    fi
    

fi