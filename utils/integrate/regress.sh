#!/bin/bash


DATE=$(date +%Y%m%d.%H%M%S)                                                               
#echo ${#}
if [ ${#} -lt 2 ] || [[ -f "${1}" ]]; then
		echo "incorrect arguments"
		exit 1
fi

#if [[ ! -r $(ls *.test) ]]; then
#		echo "files not readable"
#		exit 1
#fi

mkdir "${DATE}"

if [[ ! -d "${DATE}" ]]; then
		exit 1
fi


#echo "hi"
if [ ! -d "${1}" ]; then
		echo "change directory name"
	  initialDir="${1}"
		mv "${DATE}" "${1}"
		while [ "${#}" != 1 ]; do                                                                
        echo "${2}"                                                                         
        cp ${2} "${initialDir}/${2}"                                                   #fileCut="${1}?."
				#echo "${fileCut}"
        ${2} < /dev/null > "${initialDir}/${2}.stdout" 2> "${initialDir}/${2}.stderr" &
        echo ${?} > "${initialDir}/${2}.status" &
				shift
		done
		exit 0
else
		initialDir="${1}"
		while [ "${#}" != 1 ]; do
				echo "${2}"
	#			fileCut="${1}?."                                                       
   #     echo "${fileCut}" 
				cp ${2} "${DATE}/${2}"                                                   
				${2} < /dev/null > "${DATE}/${2}.stdout" 2> "${DATE}/${2}.stderr" &            echo ${?} > "${DATE}/${2}.status" &
				
				Diff=$(diff -q "${2}" "${initialDir}/${2}")
				if [ "${Diff}" != "" ]; then
						echo "differences: ${Diff}"
						exit 1
				fi
				echo "no differences"
				if [[ ! ${2} ]]; then
						echo "in error loop"
						exit 1
				fi
				shift 
		done
		echo "done with for loop"
		exit 0
fi

				
				

				
		
			
			







 

	 
		

	
	
		
