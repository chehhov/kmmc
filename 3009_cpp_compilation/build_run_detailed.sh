#!/bin/bash
echo "=== 🔧 Detailed C++ Build Process ==="

echo "1. 🧹 Cleaning previous builds..."
rm -f main

echo "2. 📦 Compiling with all warnings..."
g++ -Wall -Wextra -o main main.cpp

echo "3. ✅ Checking if compilation successful..."
if [ -f "main" ]; then
    echo "4. 🏃 Running program..."
    echo "=== PROGRAM OUTPUT ==="
    ./main
    echo "=== END OUTPUT ==="
    echo "5. ✅ Done!"
else
    echo "❌ Compilation failed!"
    exit 1
fi
