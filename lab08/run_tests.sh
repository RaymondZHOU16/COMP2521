#!/bin/bash

# Path to the testGraphMst program
PROGRAM="./testGraphMst"

# Directory where your test cases are stored
TEST_DIR="tests/graphMst"

# Loop through all the input files in the test directory
for input_file in $TEST_DIR/*.in; do
    echo "Processing `basename $input_file`..."
    
    # Extract the test number from the input file name
    test_number=$(basename $input_file .in)
    
    # Run the program and redirect input and output
    $PROGRAM < $input_file > "$TEST_DIR/$test_number.out"
    
    # Use diff to compare the output file with the expected output file
    if diff -q "$TEST_DIR/$test_number.out" "$TEST_DIR/$test_number.exp"; then
        echo "Test $test_number: PASS"
    else
        echo "Test $test_number: FAIL"
        # If you want to see the differences uncomment the next line
        # diff "$TEST_DIR/$test_number.out" "$TEST_DIR/$test_number.exp"
    fi
done
