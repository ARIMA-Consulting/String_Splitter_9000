# String Splitter 9000

A powerful string splitting tool available in both command-line and GUI versions.

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

## Version History

- **v1.0.0** - Initial MVP release with CLI and GUI versions

