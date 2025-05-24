# Huffman File Compressor

A C++ implementation of Huffman coding for text file compression/decompression.

## Features
- ✅ Lossless compression using Huffman encoding
- 📊 Shows compression statistics (size, ratio)
- 🔄 Supports both compression and decompression
- 📁 Works with any text file format (.txt, .csv, etc.)

## How to Use

### Compression
1. Run the program
2. Select option 1 (Compress)
3. Enter input file path
4. Name your output file (without extension)

### Decompression  
1. Run the program
2. Select option 2 (Decompress)
3. Enter compressed file path (.bin)
4. Name your output text file

## Technical Details
- Built with standard C++ libraries
- Uses priority queue for Huffman tree construction
- Stores frequency table in compressed file header
- Handles byte padding automatically

## Requirements
- C++11 compatible compiler
- 64MB RAM minimum
- Enough disk space for output files

## Limitations
- Max file size: 2GB
- Text files only (no binary)
- Single-threaded
