#!/bin/bash

<<COMMENT1
# Task 1 Inserting into a Hash Table
# Function to test and display results
test_case() {
    echo "Testing $1..."
    echo -e "$2" | ./runHashTable
    echo "----------------------"
}

# Test cases

#test_case "." "+ 2 2\n+ 3 3\n+ 4 4\n+ 5 5\n+ 6 6\n+ 7 7\n+ 8 8\n+ 9 9\np\n+ 0 0\np\n+ 2 4\np\nq"
test_case ".." "+ 10 10\n+ 120 120\n+ 130 130\n+ 140 140\n+ 150 150\n+ 160 160\n+ 1 1\n+ 2 2\n+ 3 3\n+ 4 4\np\n- 10\n- 20\n- 30\n- 40\np\nq"

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
# Function to test and display results for findWinner
test_case() {
    echo "Testing $1..."
    result=$(./winner $2)
    echo "Input: $2"
    echo "Output: $result"
    echo "----------------------"
}

# Test cases

# Basic Functionality Tests
test_case "Single Candidate Wins" "1 1 1 2 2"
test_case "Tie Between Candidates" "1 2 2 1"
test_case "All Candidates Have Same Votes" "1 2 3"

# Edge Cases
test_case "Only One Voter" "1"
test_case "Large Number of Voters" "1 1 1 1 2 2 2 2 2"
test_case "Non-Sequential Candidates" "100 200 100 300 300 300"

# Complex Scenarios
test_case "Multiple Ties" "1 2 3 2 3 1 1"
test_case "Large Number of Candidates" "1 2 3 4 5 6 7 8 9 10 1 2 3 4 5"

# Error Handling
test_case "No Input" ""

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

