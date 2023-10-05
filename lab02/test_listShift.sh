#!/bin/bash

# Test 1
./listShift <<EOF
3
16 5 2
EOF
echo "Expected Output: List after shifting: [2, 16, 5]"
echo "-----------------------------------------"

# Test 2
./listShift <<EOF
5
9 6 1 8 8
EOF
echo "Expected Output: List after shifting: [8, 9, 6, 1, 8]"
echo "-----------------------------------------"

# Test 3: Shifting a list with a single element
./listShift <<EOF
1
42
EOF
echo "Expected Output: List after shifting: [42]"
echo "-----------------------------------------"

# Test 4: Shifting an empty list
./listShift <<EOF
0
EOF
echo "Expected Output: List after shifting: []"
echo "-----------------------------------------"

# Test 5: Shifting a list with two elements
./listShift <<EOF
2
10 20
EOF
echo "Expected Output: List after shifting: [20, 10]"
echo "-----------------------------------------"

# Test 6: Shifting a list with repeated values
./listShift <<EOF
4
7 7 7 7
EOF
echo "Expected Output: List after shifting: [7, 7, 7, 7]"
echo "-----------------------------------------"

# Test 7: Shifting a list with negative numbers
./listShift <<EOF
3
-5 -10 -15
EOF
echo "Expected Output: List after shifting: [-15, -5, -10]"
echo "-----------------------------------------"

# Test 8: Shifting a list with a mix of positive, negative, and zero values
./listShift <<EOF
5
10 -5 0 5 -10
EOF
echo "Expected Output: List after shifting: [-10, 10, -5, 0, 5]"
echo "-----------------------------------------"

# Test 9: Shifting a list with alternating positive and negative values
./listShift <<EOF
6
10 -10 20 -20 30 -30
EOF
echo "Expected Output: List after shifting: [-30, 10, -10, 20, -20, 30]"
echo "-----------------------------------------"

# Test 10: Shifting a list with all zeros
./listShift <<EOF
5
0 0 0 0 0
EOF
echo "Expected Output: List after shifting: [0, 0, 0, 0, 0]"
echo "-----------------------------------------"

# Test 11: Shifting a list with large numbers
./listShift <<EOF
3
1000000 2000000 3000000
EOF
echo "Expected Output: List after shifting: [3000000, 1000000, 2000000]"
echo "-----------------------------------------"

# Test 12: Shifting a list where the last element is zero
./listShift <<EOF
4
5 10 15 0
EOF
echo "Expected Output: List after shifting: [0, 5, 10, 15]"
echo "-----------------------------------------"

# Test 13: Shifting a list with a mix of large and small numbers
./listShift <<EOF
5
1 2 9999999 -9999999 3
EOF
echo "Expected Output: List after shifting: [3, 1, 2, 9999999, -9999999]"
echo "-----------------------------------------"