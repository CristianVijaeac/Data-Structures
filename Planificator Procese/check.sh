#!/bin/bash

BIN=tema2
TESTS_DIR=./tests
REFS_DIR=./refs
NUM_TESTS=11
FIRST_BONUS=10
TMP_FILE=out.tmp
VALGRIND_CMD="valgrind --leak-check=full -q --log-file=$TMP_FILE"
TOTAL=0

TESTS_SCORES=(0 4 4 8 8 8 8 8 8 8 8 8)
TESTS_SCORES_VALGRIND=(0 1 1 2 2 2 2 2 2 2 2 2)
MAX_SCORE=80

test_do_fail()
{
	printf "failed  [ 0/%02d]\n" "$MAX_SCORE"
}

test_do_pass()
{
	printf "passed  [%02d/%02d]\n" "${1}" "$MAX_SCORE"
	((TOTAL+=${1}))
}

print_header()
{
	header="${1}"
	header_len=${#header}
	printf "\n"
	if [ $header_len -lt 61 ]; then
		padding=$(((61 - $header_len) / 2))
		for ((i = 0; i < $padding; i++)); do
			printf " "
		done
	fi
	printf "= %s =\n\n" "${header}"
}

print_header "Tema 2 SD Stive si Cozi"

for i in $(seq 1 $NUM_TESTS); do

	if [[ $i -eq $FIRST_BONUS ]]; then
		printf "\n[BONUS]\n"
	fi

	printf "Test %02d ............................................ " "$i"
	
	if [[ ! -f $BIN ]]; then
		test_do_fail
		continue
	fi

	./$BIN $TESTS_DIR/in$i.txt $TMP_FILE

	if [[ ! -f $TMP_FILE ]]; then
		touch $TMP_FILE
	fi

	cmp $TMP_FILE $REFS_DIR/out$i.txt > /dev/null
	if test $? -eq 0; then
		test_do_pass ${TESTS_SCORES[$i]}

		rm -f $TMP_FILE
		$VALGRIND_CMD ./$BIN $TESTS_DIR/in$i.txt /dev/null > /dev/null

		printf "[VALGRIND] Test %02d ................................. " "$i"

		res=$(cat $TMP_FILE)

		if [[ -z $res ]]; then
			test_do_pass ${TESTS_SCORES_VALGRIND[$i]}
		else
			test_do_fail
		fi

	else
		test_do_fail
	fi

	rm -f $TMP_FILE

done

printf "\n%59s [%02d/80]\n" "Total:" $TOTAL;
