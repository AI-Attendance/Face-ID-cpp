#!/bin/bash
c=0; d=0; r=0; a=0; o=0
while [ $# -gt 0 ] && [ $a -eq 0 ] && [ "${1:0:1}" == "-" ]
do
	i=1
	while [ "${1:${i}:1}" ]; do
		char=${1:${i}:1}
		if [ "$char" == "c" ]; then c=1
		elif [ "$char" == "d" ]; then d=1
		elif [ "$char" == "r" ]; then r=1
		elif [ "$char" == "a" ]; then a=1
		elif [ "$char" == "o" ]; then o=1
		else
			printf "This program compiles c/cpp files using make and runs the\
 executable file while calculating the elapsed time.
The default build has debugging configurations.\n
./compile_and_run.sh [options] [-a [arg1 arg2 ...]]\n
Options:
\t-c\tclean (deletes) already built files that have the configurations given (debug or release)
\t-r\tbuild using release configurations
\t-d\tbuild using debug configurations, and runs the debugger
\t-h\tshow this help message
\t-o\tcompile only\n
Arguments:
\t-a\tsends the following parameters as arguments to main function (this should be the last argument)\n"
			exit 0
		fi
		i=$((i + 1))
	done
	shift
done
if [ $c -eq 1 ]; then
	make RELEASE=$r clean
fi
if [ $d -eq 1 ]; then
	make debug
	exit 0
fi
if [ $a -eq 0 ]; then
	shift $#
fi
make RELEASE=$r all
filename=$(make RELEASE=$r getTarget)
if [ $o -eq 1 ]; then
	exit 0
fi
echo --------------------------------------------------
if command time true &>/dev/null; then
	command time -f "\n--------------------------------------------------\nElapsed Time: %e sec\nCPU Percentage: %P"\
		"$filename" "$@"
else
	time "$filename" "$@"
fi
exit $?
