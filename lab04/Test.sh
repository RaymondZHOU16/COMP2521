#!/bin/bash

<<COMMENT1
# Task 1
# Function to test and display results
test_case() {
    echo "Testing $1..."
    echo -e "$2" | ./runBst
    echo "----------------------"
}

# Test cases

test_case "Empty BST" "p\nl"
test_case "BST with a single node" "+ 5\np\nl"
test_case "BST with multiple levels but no leaves" "+ 5\n+ 3\n+ 7\np\nl"
test_case "BST with multiple leaves" "+ 5\n+ 3\n+ 2\n+ 4\n+ 7\n+ 6\n+ 8\np\nl"
test_case "Left-skewed BST" "+ 5\n+ 4\n+ 3\n+ 2\n+ 1\np\nl"
test_case "Right-skewed BST" "+ 1\n+ 2\n+ 3\n+ 4\n+ 5\np\nl"
test_case "Mixed BST" "+ 5\n+ 3\n+ 2\n+ 4\n+ 7\n+ 6\np\nl"

# End of script
echo "All tests completed!"
COMMENT1


<<COMMENT2
# Task 2
# Function to test and display results
test_case() {
    echo "Testing $1..."
    echo -e "$2" | ./runBst
    echo "----------------------"
}

# Test cases

test_case "Empty BST" "r"
test_case "BST with a single node" "+ 5\np\nr"
test_case "BST with multiple levels" "+ 5\n+ 3\n+ 7\n+ 2\n+ 4\n+ 6\n+ 9\n+ 1\n+ 8\np\nr"
test_case "Left-skewed BST" "+ 5\n+ 4\n+ 3\n+ 2\n+ 1\np\nr"
test_case "Right-skewed BST" "+ 1\n+ 2\n+ 3\n+ 4\n+ 5\np\nr"
test_case "Mixed BST" "+ 5\n+ 3\n+ 2\n+ 4\n+ 7\n+ 6\np\nr"
echo "More tests"
test_case "BST with negative values" "+ -5\n+ -3\n+ -7\n+ -2\n+ -4\n+ -6\n+ -9\n+ -1\n+ -8\np\nr"
test_case "BST with mixed positive and negative values" "+ 5\n+ -3\n+ 7\n+ -2\n+ 4\n+ -6\n+ 9\n+ -1\n+ 8\np\nr"
test_case "BST with large values" "+ 1000\n+ 500\n+ 1500\n+ 250\n+ 750\n+ 1250\n+ 1750\np\nr"

# End of script
echo "All tests completed!"
COMMENT2

<<COMMENT3
COMMENT3

#<<COMMENT4
# Task 4
# Function to test and display results
test_case() {
    echo "Testing $1..."
    echo -e "$2" | ./runBst
    echo "----------------------"
}

# Test cases

test_case "Exact Match" "+ 5\n+ 3\n+ 7\n+ 2\n+ 4\n+ 6\n+ 9\n+ 1\n+ 8\nc 7\np"
test_case "Closest Value" "+ 5\n+ 3\n+ 7\n+ 2\n+ 4\n+ 6\n+ 9\n+ 1\n+ 8\nc 15\np"
test_case "Multiple Closest Values" "+ 5\n+ 3\n+ 7\n+ 2\n+ 4\n+ 6\n+ 9\n+ 1\n+ 8\nc 7\np"

# End of script
echo "All tests completed!"
#COMMENT4

<<COMMENT5
# Task 5
# Function to test and display results
test_case() {
    echo "Testing $1..."
    echo -e "$2" | ./runBst
    echo "----------------------"
}

# Test cases

test_case "Empty BST" "p\nL"
test_case "BST with a single node" "+ 5\np\nL"
test_case "BST with multiple levels" "+ 5\n+ 3\n+ 7\n+ 2\n+ 4\n+ 6\n+ 9\n+ 1\n+ 8\np\nL"
test_case "Left-skewed BST" "+ 5\n+ 4\n+ 3\n+ 2\n+ 1\np\nL"
test_case "Right-skewed BST" "+ 1\n+ 2\n+ 3\n+ 4\n+ 5\np\nL"
test_case "Mixed BST" "+ 5\n+ 3\n+ 2\n+ 4\n+ 7\n+ 6\np\nL"

# End of script
echo "All tests completed!"
COMMENT5

