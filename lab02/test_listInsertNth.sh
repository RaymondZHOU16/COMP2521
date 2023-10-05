#!/bin/bash

# Test 1: Inserting at the beginning of the list
./listInsertNth <<EOF
3
16 7 8
0 12
EOF

# Test 2: Inserting in the middle of the list (position 1)
./listInsertNth <<EOF
3
16 7 8
1 12
EOF

# Test 3: Inserting in the middle of the list (position 2)
./listInsertNth <<EOF
3
16 7 8
2 12
EOF

# Test 4: Inserting at the end of the list
./listInsertNth <<EOF
3
16 7 8
3 12
EOF

# Test 5: Inserting at a position greater than the size of the list
./listInsertNth <<EOF
3
16 7 8
4 12
EOF

# Test 6: Inserting at position 0 of a list with one element
./listInsertNth <<EOF
1
42
0 16
EOF

# Test 7: Inserting into an empty list at position 0
./listInsertNth <<EOF
0
0 2
EOF

# Test 8: Inserting into an empty list at a position greater than 0
./listInsertNth <<EOF
0
10 2
EOF

# Test 9: Inserting into a list with repeated values
./listInsertNth <<EOF
5
7 7 7 7 7
2 5
EOF

# Test 10: Inserting a negative number into the list
./listInsertNth <<EOF
4
10 20 30 40
1 -15
EOF

# Test 11: Inserting at the beginning of a list with two elements
./listInsertNth <<EOF
2
50 60
0 40
EOF

# Test 12: Inserting in the middle of a list with two elements
./listInsertNth <<EOF
2
50 60
1 55
EOF

# Test 13: Inserting at the end of a list with two elements
./listInsertNth <<EOF
2
50 60
2 70
EOF

# Test 14: Inserting into a list with large numbers
./listInsertNth <<EOF
3
1000000 2000000 3000000
1 1500000
EOF

# Test 15: Inserting a large number of elements into an empty list
./listInsertNth <<EOF
0
0 9999999
EOF
