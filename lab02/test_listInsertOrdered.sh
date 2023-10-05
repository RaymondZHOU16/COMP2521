#!/bin/bash

# Test 1
./listInsertOrdered <<EOF
3
2 5 7
1
EOF
echo "Expected Output: List after inserting 1: [1, 2, 5, 7]"
echo "-----------------------------------------"

# Test 2
./listInsertOrdered <<EOF
3
2 5 7
3
EOF
echo "Expected Output: List after inserting 3: [2, 3, 5, 7]"
echo "-----------------------------------------"

# Test 3
./listInsertOrdered <<EOF
3
2 5 7
5
EOF
echo "Expected Output: List after inserting 5: [2, 5, 5, 7]"
echo "-----------------------------------------"

# Test 4
./listInsertOrdered <<EOF
3
2 5 7
6
EOF
echo "Expected Output: List after inserting 6: [2, 5, 6, 7]"
echo "-----------------------------------------"

# Test 5
./listInsertOrdered <<EOF
3
2 5 7
8
EOF
echo "Expected Output: List after inserting 8: [2, 5, 7, 8]"
echo "-----------------------------------------"

# Test 6
./listInsertOrdered <<EOF
0
42
EOF
echo "Expected Output: List after inserting 42: [42]"
echo "-----------------------------------------"

# Test 7: Inserting a value into a list with negative numbers
./listInsertOrdered <<EOF
3
-5 -3 -1
-4
EOF
echo "Expected Output: List after inserting -4: [-5, -4, -3, -1]"
echo "-----------------------------------------"

# Test 8: Inserting a value into a list with a mix of positive and negative numbers
./listInsertOrdered <<EOF
5
-10 -5 0 5 10
3
EOF
echo "Expected Output: List after inserting 3: [-10, -5, 0, 3, 5, 10]"
echo "-----------------------------------------"

# Test 9: Inserting a zero into the list
./listInsertOrdered <<EOF
3
-3 -2 -1
0
EOF
echo "Expected Output: List after inserting 0: [-3, -2, -1, 0]"
echo "-----------------------------------------"

# Test 10: Inserting a value into a list with repeated values
./listInsertOrdered <<EOF
5
1 1 1 1 1
1
EOF
echo "Expected Output: List after inserting 1: [1, 1, 1, 1, 1, 1]"
echo "-----------------------------------------"

# Test 11: Inserting a large number into a list with large numbers
./listInsertOrdered <<EOF
3
100000 200000 300000
250000
EOF
echo "Expected Output: List after inserting 250000: [100000, 200000, 250000, 300000]"
echo "-----------------------------------------"

# Test 12: Inserting a small number into a list with large numbers
./listInsertOrdered <<EOF
3
100000 200000 300000
500
EOF
echo "Expected Output: List after inserting 500: [500, 100000, 200000, 300000]"
echo "-----------------------------------------"

