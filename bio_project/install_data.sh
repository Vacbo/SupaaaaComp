#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Define the data directory and base URL
DATA_DIR="./data"
BASE_URL="ftp://hgdownload.cse.ucsc.edu/goldenPath/hg19/snp147Mask"

# Create the data directory if it doesn't exist
echo "Creating data directory..."
mkdir -p "$DATA_DIR"

# Loop to download and unzip each chromosome file
echo "Downloading and unzipping chromosome files..."
for i in $(seq 1 22); do
    FILE_NAME="chr${i}.subst.fa.gz"
    FILE_URL="${BASE_URL}/${FILE_NAME}"
    OUTPUT_FILE="${DATA_DIR}/chr${i}.subst.fa"

    # Download the .gz file
    if [ ! -f "${DATA_DIR}/${FILE_NAME}" ]; then
        echo "Downloading ${FILE_NAME}..."
        wget -P "$DATA_DIR" "$FILE_URL"
    else
        echo "${FILE_NAME} already exists, skipping download."
    fi

    # Unzip the file if the .fa file doesn't already exist
    if [ ! -f "$OUTPUT_FILE" ]; then
        echo "Unzipping ${FILE_NAME}..."
        gunzip -c "${DATA_DIR}/${FILE_NAME}" > "$OUTPUT_FILE"
    else
        echo "${OUTPUT_FILE} already exists, skipping unzip."
    fi
done

echo "Data installation completed successfully."