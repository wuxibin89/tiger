#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

function run_test() {
	fname="${SCRIPT_DIR}/testcases/$1"

	echo -n "analyze $fname ... "
	log=$(basename $fname).log
	cat $fname >$log
	echo "=======================" >>$log

	error=$(grep -E "error|illegal" $fname)
	if [[ $error != "" ]]; then
		echo -ne "expected: ${RED}fail${NC}, "
	else
		echo -ne "expected: ${GREEN}success${NC}, "
	fi

	./$bin $fname >>$log 2>&1
	if [[ $(echo $?) != 0 ]]; then
		echo -e "result: ${RED}fail${NC}"
	else
		echo -e "result: ${GREEN}success${NC}"
	fi
}

for num in {1..49}; do
	run_test "test$num.tig"
done

for file in merge.tig queens.tig; do
	run_test $file
done