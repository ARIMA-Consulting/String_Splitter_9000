#!/bin/bash

# GitHub Sync Script for String Splitter 9000
# This script syncs your local repository with GitHub

set -e  # Exit on error

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== GitHub Sync Script ===${NC}"
echo ""

# Check if remote is configured
if ! git remote get-url origin &>/dev/null; then
    echo -e "${RED}Error: GitHub remote not configured!${NC}"
    echo ""
    echo "Please run the setup first:"
    echo "  1. Create a repository on GitHub (github.com)"
    echo "  2. Run: git remote add origin https://github.com/YOUR_USERNAME/YOUR_REPO.git"
    echo "  3. Or use: git remote add origin git@github.com:YOUR_USERNAME/YOUR_REPO.git (for SSH)"
    echo ""
    exit 1
fi

# Show current remote
echo -e "${YELLOW}Remote repository:${NC}"
git remote get-url origin
echo ""

# Check for uncommitted changes
if ! git diff-index --quiet HEAD --; then
    echo -e "${YELLOW}Warning: You have uncommitted changes!${NC}"
    echo "Would you like to commit them first? (y/n)"
    read -r response
    if [[ "$response" =~ ^[Yy]$ ]]; then
        echo ""
        echo "Please commit your changes first using:"
        echo "  ./commit.sh \"Your commit message\""
        echo ""
        exit 1
    fi
fi

# Fetch latest changes
echo -e "${YELLOW}Fetching latest changes from GitHub...${NC}"
git fetch origin

# Check if we're behind
LOCAL=$(git rev-parse @)
REMOTE=$(git rev-parse @{u} 2>/dev/null || echo "")

if [ -n "$REMOTE" ] && [ "$LOCAL" != "$REMOTE" ]; then
    echo -e "${YELLOW}Your local branch is behind the remote.${NC}"
    echo "Would you like to pull changes first? (y/n)"
    read -r response
    if [[ "$response" =~ ^[Yy]$ ]]; then
        echo ""
        echo -e "${YELLOW}Pulling changes...${NC}"
        git pull origin main
        echo ""
    fi
fi

# Show what will be pushed
echo -e "${YELLOW}Commits to push:${NC}"
git log origin/main..HEAD --oneline 2>/dev/null || git log --oneline -5
echo ""

# Push to GitHub
echo -e "${YELLOW}Pushing to GitHub...${NC}"
if git push origin main; then
    echo ""
    echo -e "${GREEN}✓ Successfully synced with GitHub!${NC}"
    echo ""
    echo "View your repository at:"
    git remote get-url origin | sed 's/\.git$//' | sed 's/^git@github\.com:/https:\/\/github.com\//'
else
    echo ""
    echo -e "${RED}✗ Push failed!${NC}"
    echo "You may need to:"
    echo "  1. Set up authentication (SSH key or personal access token)"
    echo "  2. Check your internet connection"
    echo "  3. Verify you have push access to the repository"
    exit 1
fi

