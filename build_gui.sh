#!/bin/bash

# Build script for String Splitter 9000 GUI

# Use Homebrew pkg-config if available (preferred for Homebrew-installed GTK)
if [ -f "/opt/homebrew/bin/pkg-config" ]; then
    PKG_CONFIG="/opt/homebrew/bin/pkg-config"
elif [ -f "/usr/local/bin/pkg-config" ]; then
    PKG_CONFIG="/usr/local/bin/pkg-config"
else
    PKG_CONFIG="pkg-config"
fi

# Set PKG_CONFIG_PATH to include Homebrew paths (for both Intel and Apple Silicon Macs)
if [ -d "/opt/homebrew/opt/gtk+3" ]; then
    # Apple Silicon Mac
    export PKG_CONFIG_PATH="/opt/homebrew/lib/pkgconfig:/opt/homebrew/opt/gtk+3/lib/pkgconfig:/opt/homebrew/share/pkgconfig:$PKG_CONFIG_PATH"
elif [ -d "/usr/local/opt/gtk+3" ]; then
    # Intel Mac
    export PKG_CONFIG_PATH="/usr/local/lib/pkgconfig:/usr/local/opt/gtk+3/lib/pkgconfig:/usr/local/share/pkgconfig:$PKG_CONFIG_PATH"
fi

# Check if GTK+3.0 is installed
if ! $PKG_CONFIG --exists gtk+-3.0 2>/dev/null; then
    echo "GTK+3.0 is not found by pkg-config."
    echo "If GTK+3 is installed via Homebrew, this script should detect it."
    echo "To install GTK+3, run: brew install gtk+3"
    exit 1
fi

# Get compiler flags using pkg-config
CFLAGS=$($PKG_CONFIG --cflags gtk+-3.0)
LIBS=$($PKG_CONFIG --libs gtk+-3.0)

# Compile
echo "Compiling string_splitter_9000_gui.c..."
gcc -o string_splitter_9000_gui string_splitter_9000_gui.c $CFLAGS $LIBS

if [ $? -eq 0 ]; then
    echo "Build successful! Run with: ./string_splitter_9000_gui"
else
    echo "Build failed!"
    exit 1
fi

