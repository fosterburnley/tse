#!/bin/bash

#print usage statement and exits if innaproiprate argumetns present
# prints nothing to backup and exits if no source files of .c and .h and no file namde "makefile" in current directory
# creates hidden.backup subdirectory in current directory
#copies and source files and any fil to backup directory if they differ (diff)
#if -q present, print a dot on screen for each file


if [ $# -gt 1 ]; then
		echo {"usage: backup.sh [-q]"}                                             
    exit
fi


if [ $# -eq 1 ] && [ $1 != "-q" ] ; then
		echo {"usage: backup.sh [-q]"}
		exit
fi

# if any directory has source or Makefile, make a backup directory
# in each backup, if there is a difference between the file and in the backup, copy the file into backup
# if -q is in the argument, then put . , else just print filenam

count_fileC=$(ls -l *.c 2> /dev/null | wc -l)
count_fileH=$(ls -l *.h 2> /dev/null | wc -l)
count_fileM=$(ls -l Makefile 2> /dev/null | wc -l)


if [ $count_fileC -eq 0 ] && [ $count_fileH -eq 0 ] && [ $count_fileM -eq 0 ]; then
		echo {"nothing to backup"}
		exit
fi

if [ ! -d .backup ]; then
		mkdir .backup
fi
		
for f in *.c *.h Makefile; do
		if [ -e ./.backup/${f} ]; then
				if ! cmp ${f} ./.backup/${f} >/dev/null 2>&1; then
						cp "./${f}" "./.backup" >/dev/null 2>&1
						if [ "$1" == "-q" ] ; then
								echo -n {"."}
		 				else
								echo ${f}
		 		   	fi
				fi
		elif [ -e ./${f} ]; then
				cp "./${f}" "./.backup"
				if [ "$1" == "-q" ] ; then
						echo -n {"."}
				else
						echo ${f}
				fi
		fi
		
done

