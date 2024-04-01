#!/bin/bash

# Array containing different sets of arguments
args=("elon_tweets" "dictionary" "the_office")

# Path to the binary
binary="./bin/boyer-moore-profile"

# Iterate over the arguments and run the binary with each set of arguments
for arg in "${args[@]}"; do
    echo "Running binary with argument: $arg"
    $binary $arg
done
