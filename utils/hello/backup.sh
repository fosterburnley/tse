#!/bin/bash

#print usage statement and exits if innaproiprate argumetns present
# prints nothing to backup and exits if no source files of .c and .h and no file namde "makefile" in current directory
# creates hidden.backup subdirectory in current directory
#copies and source files and any fil to backup directory if they differ (diff)
#if -q present, print a dot on screen for each file


if [ $# -gt 1 ] && [ "${1}" != "-q" ] ; then
		echo {"usage: backup.sh [-q]"}
		exit
fi

# if any directory has source or Makefile, make a backup directory
# in each backup, if there is a difference between the file and in the backup, copy the file into backup
# if -q is in the argument, then put . , else just print filenam

count_fileC=$(ls . | wc -l *.c  2>/dev/null)
count_fileH=$(ls . | wc -l *.h 2>/dev/null)
count_fileM=$(ls . | wc -l Makefile 2>/dev/null)

declare -a fileExtension
fileExtension=(empty1 empty2 empty3)

if [ count_fileC == 0 ] && [ count_fileH == 0 ] && [ count_fileM == 0 ]; then
		echo {"nothing to backup"}
		exit
fi

if [ count_fileC -gt 0 ]; then
		fileExtension+=(*.c)
fi

if [ count_fileH -gt 0 ]; then
		fileExtension+=(*.h)
fi

if [ count_fileM -gt 0 ]; then
		fileExtension+=(Makefile)
fi

echo ${fileExtension[*]}

for f in ${fileextension[*]}; do
		if [ cmp ${f} .backup/${f} ] ; then
				 cp -f "${f}" "${f}.backup"
				if [ "$1" == "-q" ] ; then
								echo -n {"."}
		 		else
								echo ${f}
		 		fi
		fi
done
