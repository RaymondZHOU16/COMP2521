#!/bin/bash

# Test 1: Basic Test Cases
./listMax <<EOF
5
7 2 6 8 0
EOF

./listMax <<EOF
5
9 6 1 8 8
EOF

./listMax <<EOF
4
2 5 2 1
EOF

./listMax <<EOF
1
42
EOF

# Test 2: Larger Lists
./listMax <<EOF
10
1 2 3 4 5 6 7 8 9 10
EOF

./listMax <<EOF
9
100 200 300 400 500 600 700 800 900
EOF

# Test 3: Negative Values
./listMax <<EOF
5
-1 -2 -3 -4 -5
EOF

# Test 4: Mixed Values
./listMax <<EOF
5
0 -10 5 -7 12
EOF

# Test 5: List with Duplicates
./listMax <<EOF
5
3 3 3 3 3
EOF

# Test 6: Larger Lists with Mixed Values
./listMax <<EOF
10
2 -4 6 -8 10 -12 14 -16 18 -20
EOF

# Test 7: List with Zero
./listMax <<EOF
5
0 0 0 0 0
EOF

# Test 8: Random Values
./listMax <<EOF
9
7 2 9 4 6 8 1 5 3
EOF
