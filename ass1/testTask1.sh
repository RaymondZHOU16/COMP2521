#!/bin/bash

# Folder containing the input files (tree, enc, and expected output files)
folder="task1"

# List of input file names (without the folder path)
input_files=(
    "anti-hero"
    "dire_straits"
    "pangram"
    "peter_piper"
    "sea_shells"
    "tell-tale_heart"
    "wonderland"
)

# Loop through the input files and run the decode program for each
for file_name in "${input_files[@]}"; do
    tree_file="${folder}/${file_name}.tree"
    enc_file="${folder}/${file_name}.enc"
    txt_file="${folder}/${file_name}.txt"

    # Run the decode program
    ./decode "$tree_file" "$enc_file" "${folder}/${file_name}.out"

    # Compare the output with the expected text
    diff "${folder}/${file_name}.out" "$txt_file"

    # Check the result and print a message
    if [ $? -eq 0 ]; then
        echo "Test for ${file_name}.tree passed."
    else
        echo "Test for ${file_name}.tree failed."
    fi
done

