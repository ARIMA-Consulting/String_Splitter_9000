#!/bin/bash

# Git commit helper script for String Splitter 9000
# Usage: ./commit.sh "Your commit message"

if [ $# -eq 0 ]; then
    echo "Usage: ./commit.sh \"Your commit message\""
    echo ""
    echo "This script will:"
    echo "  1. Show git status"
    echo "  2. Add all changes"
    echo "  3. Commit with your message"
    echo "  4. Show the commit log"
    exit 1
fi

COMMIT_MSG="$1"

echo "=== Git Status ==="
git status --short
echo ""

echo "=== Adding all changes ==="
git add -A

echo "=== Committing with message: $COMMIT_MSG ==="
git commit -m "$COMMIT_MSG"

if [ $? -eq 0 ]; then
    echo ""
    echo "=== Commit successful! ==="
    echo ""
    echo "=== Recent commits ==="
    git log --oneline -5
else
    echo "Commit failed!"
    exit 1
fi

