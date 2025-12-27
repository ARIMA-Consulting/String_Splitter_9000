# String Splitter 9000

A string splitting tool available in both command-line and GUI versions.

## Developer Note to Users ##

This is a very basic string splitting tool that was made to help making the process of encoding text into a bunch of different strings that get concatenated together at the end for cybersecurity work. This was tedious to do by hand, so hopefully this helps you too. 

## Features

- **Fixed Length Splitting**: Split strings into chunks of a specified size
- **Variable Size Splitting**: Split strings into chunks with random sizes between min and max bounds
- **Large Buffer Support**: Handles very long strings (up to 500,000 characters)
- **Dual Interface**: Both CLI and GTK-based GUI versions available

## Building

### CLI Version
```bash
gcc -o string_splitter_9000 string_splitter_9000.c
```

### GUI Version
```bash
./build_gui.sh
```

**Prerequisites for GUI:**
- GTK+3.0 installed via Homebrew: `brew install gtk+3`

## Usage

### CLI Version
```bash
./string_splitter_9000
```

Follow the prompts to:
1. Choose fixed or variable split mode
2. Enter size/bounds
3. Paste your string

### GUI Version
```bash
./string_splitter_9000_gui
```

Use the graphical interface to:
1. Select split mode (fixed/variable)
2. Enter parameters
3. Paste your string in the input area
4. Click "Split String" to see results

## Project Structure

- `string_splitter_9000.c` - CLI version
- `string_splitter_9000_gui.c` - GUI version
- `build_gui.sh` - Build script for GUI version
- `commit.sh` - Helper script for git commits
- `.gitignore` - Git ignore rules for build artifacts

## Git Usage

### Making Commits

Use the helper script for easy commits:
```bash
./commit.sh "Your commit message here"
```

This script will:
- Show current git status
- Add all changes
- Commit with your message
- Show recent commit history

### Manual Git Commands

```bash
# Check status
git status

# Add specific files
git add filename.c

# Add all changes
git add -A

# Commit
git commit -m "Your message"

# View commit history
git log --oneline
```

## Version History

- **v1.0.0** - Initial MVP release with CLI and GUI versions

