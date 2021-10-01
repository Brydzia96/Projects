#!/bin/bash
do_summary=false
do_remove=false
while getopts s:t:fr args; do
	case "${args}" in
		s) source_provided=${OPTARG};; 
		t) target_path=${OPTARG};;
		f) do_summary=true;;
		r) do_remove=true;;
		\?) echo "Unknown option -$OPTARG"; exit 1;;
	esac
done
#Initial source and target check 
#Source
if [[ -z "$source_provided" ]]; then
	echo "Source not provided"; exit 1
fi

if [[ -d "$(pwd)/$source_provided" ]]; then
	source_path="$(pwd)/$source_provided"
	parent_source_path=$(pwd) #It is used to cut the long path in archive
	source_directory=$source_provided
elif [[ -d "$source_provided" ]]; then
	source_path=$source_provided
	parent_source_path=$(dirname $source_provided)
	source_directory=$(echo $source_provided | grep -oP '(?:[^/](?!/))+$')
else
	echo "Wrong source provided"; exit 1
fi

#Target
if [[ -z "$target_path" ]]; then
	target_path="${source_path}.tar"
elif [[ ! -d $(echo "$target_path" | grep -oP '^.*(?=/)') || -z $(echo "$target_path" | grep -oP '(?:[^/](?!/))+$') ]]; then
	echo "Wrong target provided"; exit 1					
fi
###Declared functions
correct_directory(){ #It checks if the source directory contains all the default directories. It gives the option to continue or quit.	
	directories_string=$(ls -d $1/*/ | xargs -L1 basename)
	directories_default="abc
audio
comp
desk
flip
geo
hda
render
scripts
sim
tex
video"
	if [[ $directories_string = $directories_default ]]; then
		true
	else
		read -p "Project directories are not complete.Continue? <Enter yes if so>" answer
		if [[ $answer = "yes" ]]; then
			true
		else
			echo "No archive, done"; exit 1
		fi
	
	fi
}
#
file_exist(){ #It checks if source directory contains any hipnc or hip file.
	files_string=$(ls -p $1 | grep -v / | grep hipnc)
	files_string=$(ls -p $1 | grep -v / | grep hip)	
	if [[ -z "$files_string" ]]; then
		echo "The main directory does not contain any Houdini files"; exit 1
	else
		true
	fi		
}
#JSON FILE DATA:
project_name=(${source_directory}) #General project data.
project_size_string=$(du -sh ${source_path})
project_size=(${project_size_string// / })
#Information about project main files (hipnc or hip files) 
files_string=$(ls -p ${source_path} | grep -v / | grep hipnc)
files_array=(${files_string// / })
list=()
for i in "${!files_array[@]}"
do
	if [[ $i -eq 0 ]]; then
		name='{''"'"file_name"'"':'"'"${files_array[i]}"'"'
	else
		name='{''"'"file_name"'"':'"'"${files_array[i]}"'"'
	fi
	date='"'"modification_date"'"':'"'"$(date -r "$source_provided/${files_array[i]}")"'"'
	if [[ $i -eq "$((${#files_array[@]}-1))" ]]; then
		size='"'"file_size"'"':'"'"$(stat -c%s "$source_provided/${files_array[i]}")"' bytes"}'
	else
		size='"'"file_size"'"':'"'"$(stat -c%s "$source_provided/${files_array[i]}")"' bytes"},'
	fi
	list+=($name, $date, $size)	

done
#Information about project directories and files included within them.
directories_string=$(ls -d ${source_path}/*/ | xargs -L1 basename)
directories_array=(${directories_string// / })
directories_content(){ #It takes a directory path as an argument.
	list3=()
	date2=()
	name2='{''"'"directory_name"'"':'"'"$1"'","files":['
	files_string_inside=$(ls -p ${source_path}/$1 | grep -v /)
	files_array_inside=(${files_string_inside// / })
	if [[ -z "$files_array_inside" ]]; then
		name3='''{"'"no_files"'"':'"'"empty directory"'"}]}'
		list3=($name2 $name3)
	else
		for i in "${!files_array_inside[@]}"
		do
			if [[ $i -eq 0 ]]; then
				list3+=($name2)
			fi 		
			name3='''{"'"file_name"'"':'"'"${files_array_inside[i]}"'"'
			date2='"'"modification_date"'"':'"'"$(date -r "$source_provided/$1/${files_array_inside[i]}")"'"'
			
			file_string=$(file "$source_provided/$1/${files_array_inside[i]}")
			file_array=(${file_string//:/ })
			unset 'file_array[0]'
			file='"'"file_type"'"':'"'"$(echo ${file_array[@]})"'"'
			if [[ $i -eq "$((${#files_array_inside[@]}-1))" ]]; then
				size2='"'"file_size"'"':'"'"$(stat -c%s "$source_provided/$1/${files_array_inside[i]}")"' bytes"}]}'
			else
				size2='"'"file_size"'"':'"'"$(stat -c%s "$source_provided/$1/${files_array_inside[i]}")"' bytes"},'
			fi
			list3+=($name3, $date2, $file, $size2)
		done
	fi 	
}
for i in "${!directories_array[@]}" #For all directories in source directory.
do
	directories_content ${directories_array[i]}
	if [[ $i -eq "$((${#directories_array[@]}-1))" ]]; then
		list4+=(${list3[@]})
	else
		list4+=(${list3[@]},)
	fi
done	
json=$(cat <<-END
    {
        "project_name": "${project_name[-1]}", 
        "project_path": "${source_provided}", 
        "project_size": "${project_size[0]}", 
        "project_files": [$(echo -e "${list[@]}")], 
        "project_directories": [$(echo -e "${list4[@]}")], 
        "date_of_archive": "$(date)" 
    }
END
)
#Execution:
#Make Summary
if $do_summary; then
	echo "${json}" > "${source_path}/Summary_raw.json"
	jq . ${source_path}/Summary_raw.json > "${source_path}/Summary.json"
	rm -r ${source_path}/Summary_raw.json
	echo "Summary created"
fi
#Make archive
correct_directory $source_path
file_exist $source_path
if [[ correct_directory && file_exist ]]; then
	tar -cf $target_path -C $parent_source_path $source_directory
	echo "Archive done"
fi
#Remove
if $do_remove; then
	rm -r $source_provided
	echo "Original directory removed"
fi
