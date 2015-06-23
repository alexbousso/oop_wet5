#! /bin/bash

COMPILE="g++ -Wno-unused-variable -Wno-unused-but-set-variable -Wall -std=c++11"

function runCommand {
    command=$1
    if [[ "${command}" == "" ]]; then
        echo "runCommand: Got an empty command!"
        return
    fi

    echo "runCommand: Running ${command}"
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

for test in $(ls compError*.cpp); do
	res=$($COMPILE $test)
	if [[ $res -eq 0 ]]; then
		echo "TEST ERROR: $test passed but was supposed to fail!"
		exit(1)
	fi
done

rm -f *.h.gch
rm -f OOP5
runCommand "$COMPILE OOP5.cpp -o OOP5"

chmod +x OOP5
runCommand "OOP5"

rm -f OOP5

echo
echo "All tests passed successfully!"
echo "This test has Super Cow powers."
