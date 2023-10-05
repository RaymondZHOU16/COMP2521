#!/bin/bash

# Function to calculate GCD using the provided C program (assuming it's compiled as 'gcd')
calculate_gcd() {
    expected_gcd=$3
    result=$(./gcd $1 $2)
    echo "Testing gcd($1, $2)=$result (Expected: $expected_gcd)"
}

# Test Cases
calculate_gcd 12 18 6      # Test Case 1
calculate_gcd 24 36 12     # Test Case 2
calculate_gcd 0 7 7        # Test Case 3
calculate_gcd 15 0 15      # Test Case 4
calculate_gcd 0 0 0        # Test Case 5
calculate_gcd 11 13 1      # Test Case 6
calculate_gcd 987654321 123456789 9   # Test Case 7
calculate_gcd -24 36 12    # Test Case 8 (Negative numbers, if supported)
calculate_gcd 24 -36 12    # Test Case 9 (Negative numbers, if supported)
calculate_gcd 18 12 6      # Test Case 10
calculate_gcd 42 42 42     # Test Case 11


