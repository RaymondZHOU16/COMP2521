#!/bin/bash

# Test 1
./listSum <<EOF
9
8 1 5 9 6 4 9 5 1
EOF
echo "Expected Output: 48"
echo "-----------------------------------------"

# Test 2
./listSum <<EOF
6
2 4 3 7 0 4
EOF
echo "Expected Output: 20"
echo "-----------------------------------------"

# Test 3
./listSum <<EOF
5
3 5 2 4 1
EOF
echo "Expected Output: 15"
echo "-----------------------------------------"

# Test 4
./listSum <<EOF
2
42 -4
EOF
echo "Expected Output: 38"
echo "-----------------------------------------"

# Test 5
./listSum <<EOF
0
EOF
echo "Expected Output: 0"
echo "-----------------------------------------"

# Test 6: Lists with only negative numbers
./listSum <<EOF
4
-5 -10 -15 -20
EOF
echo "Expected Output: -50"
echo "-----------------------------------------"

# Test 7: Lists with large numbers
./listSum <<EOF
3
1000000 2000000 3000000
EOF
echo "Expected Output: 6000000"
echo "-----------------------------------------"

# Test 8: Lists with a mix of positive, negative, and zero values
./listSum <<EOF
5
10 -5 0 5 -10
EOF
echo "Expected Output: 0"
echo "-----------------------------------------"

# Test 9: Lists with repeated values
./listSum <<EOF
6
7 7 7 -7 -7 -7
EOF
echo "Expected Output: 0"
echo "-----------------------------------------"

# Test 10: Lists with a single large negative value
./listSum <<EOF
1
-9999999
EOF
echo "Expected Output: -9999999"
echo "-----------------------------------------"

# Test 11: Lists with alternating positive and negative values of the same magnitude
./listSum <<EOF
6
10 -10 20 -20 30 -30
EOF
echo "Expected Output: 0"
echo "-----------------------------------------"

# Test 12: Lists with all zeros
./listSum <<EOF
5
0 0 0 0 0
EOF
echo "Expected Output: 0"
echo "-----------------------------------------"
