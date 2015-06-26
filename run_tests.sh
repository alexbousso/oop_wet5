#! /bin/bash

COMPILE="g++ -Wno-unused-variable -Wno-unused-but-set-variable -Wall -std=c++11"

function runCommand {
    command=$1
    if [[ "${command}" == "" ]]; then
        echo "runCommand: Got an empty command!"
        return
    fi

    #echo "runCommand: Running ${command}"
    ${command}
    retval=$?
    if [[ ${retval} -ne 0 ]]; then
        echo "********************************************************************************"
        echo "Error:"
        echo "runCommand: Command \"${command}\" failed and returned: ${retval}"
        echo "Aborting compilation"
        echo "********************************************************************************"
        # Exit script
        exit 1
    fi
}

rm -f Tests/error_log.txt

for test in $(ls Tests/compError*.cpp); do
	echo "Trying to compile $test ..."
	echo "**********************************************************************************" >> Tests/error_log.txt
	echo "Trying to compile $test :" >> Tests/error_log.txt
	echo >> Tests/error_log.txt
	$($COMPILE $test 2>> Tests/error_log.txt)
	if [[ $? -eq 0 ]]; then
		echo "TEST ERROR: $test passed but was supposed to fail!"
		exit 1
	fi
done

echo "To see the error log, run the command:"
echo "    less Tests/error_log.txt"
echo
echo "Compiling OOP5.cpp ..."

rm -f *.h.gch
rm -f OOP5
#runCommand "$COMPILE OOP5.cpp -o OOP5"
runCommand "make clean"
runCommand "make tests"

chmod +x OOP5

echo "Running the compiled code of OOP5.cpp :"
echo

runCommand "./OOP5"

rm -f OOP5

echo
echo
echo "All tests passed successfully!"
echo "This test has Super Cow powers."
