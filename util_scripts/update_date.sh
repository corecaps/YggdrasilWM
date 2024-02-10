#!/bin/bash

# Get the current date in the desired format (YYYY-MM-DD)
current_date=$(date +'%Y-%m-%d')

# List files in specified directories
files=$(find ../src/ ../inc/ ../test/ -type f)

# Loop through each file
for file in $files; do
    # Check if the last modification date is today
    last_modified_date=$(date -r "$file" +'%Y-%m-%d')
    if [ "$last_modified_date" == "$current_date" ]; then
        # Update the @date line in the file
        sed -i "s/@date.*/@date $current_date/" "$file"
        echo "Updated @date in $file"
    fi
  done
