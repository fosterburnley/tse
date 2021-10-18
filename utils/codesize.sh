# !/bin/bash
# It acceps an optional arguemnt -q
# prints usae statement usage: codesize.sh[-q] and exits if more than 1 args or not -q
#if -q not present, list all c file and print number of lines in each file (use while loop)
#compute/print total number of lines in all c files (wildcard to filter) in current directory (-d)
# print usage
 
if [ ${#} -gt 1 ]; then
		echo "usage: codesize.sh [-q]"
		exit
fi

if [ $# -eq 1 ] && [ $1 != "-q" ]; then		
		echo {"usage: backup.sh [-q]"}                                             
    exit 
fi

X=0
LINECOUNT=0
#list all c files and print number of lines in each file if flag not present

for f in *.c ; do  {
		if [ ${#} == 0 ]; then
        let LINECOUNT=$(wc -l ${f} | awk '{print $1}')                         
        echo -n "$LINECOUNT "
				echo ${f}                                                              
        let X=$X+$LINECOUNT
		else
				let LINECOUNT=$(wc -l ${f} | awk '{print $1}')
				let X=$X+$LINECOUNT  
		fi
}
done
echo "Total = ${X}"
									 
								 
	
		
		
		
		
		
