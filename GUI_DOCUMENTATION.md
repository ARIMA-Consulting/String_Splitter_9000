# String Splitter 9000 - GUI Documentation

## Overview

String Splitter 9000 GUI is a user-friendly graphical interface for splitting strings into chunks with support for multiple programming language syntaxes. Built with GTK+3, it provides an intuitive way to process strings without using the command line.

## Features

- **Dual Split Modes**: Fixed length or variable size splitting
- **Base64 Encoding**: Optional plaintext-to-base64 encoding before splitting
- **15 Language Syntaxes**: Output formatted for Bash, Python, Ruby, C, C++, C#, Perl, Java, JavaScript, VBA, SQL, R, PHP, Rust, and Go
- **Large Input Support**: Handles strings up to 500,000 characters
- **Real-time Processing**: Instant results with a single click
- **Copy-Friendly Output**: Monospace font output area for easy copying

## Requirements

### macOS
- macOS 10.15 (Catalina) or later
- GTK+3.0 installed via Homebrew:
  ```bash
  brew install gtk+3
  ```

### Linux
- GTK+3.0 development libraries
- On Debian/Ubuntu:
  ```bash
  sudo apt-get install libgtk-3-dev
  ```
- On Fedora/RHEL:
  ```bash
  sudo dnf install gtk3-devel
  ```

### Windows
- GTK+3 runtime libraries (see Windows build instructions)

## Installation

### Building from Source

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/string-splitter-9000.git
   cd string-splitter-9000
   ```

2. Build the GUI version:
   ```bash
   ./build_gui.sh
   ```

3. Run the application:
   ```bash
   ./string_splitter_9000_gui
   ```

## User Interface

### Main Window Layout

The GUI is organized into several sections:

```
┌─────────────────────────────────────────┐
│  Encoding Section                       │
│  ☐ Encode plaintext to Base64          │
├─────────────────────────────────────────┤
│  Split Mode                             │
│  ⦿ Fixed Length  ○ Variable Size       │
├─────────────────────────────────────────┤
│  Options                                │
│  Size: [50]                             │
│  Min: [30]  Max: [70]                   │
│  Language: [Python ▼]                   │
├─────────────────────────────────────────┤
│  Enter the string to be split:          │
│  ┌─────────────────────────────────┐  │
│  │                                   │  │
│  │  (Large text input area)          │  │
│  │                                   │  │
│  └─────────────────────────────────┘  │
├─────────────────────────────────────────┤
│  [Split String]                         │
├─────────────────────────────────────────┤
│  Output:                                │
│  ┌─────────────────────────────────┐  │
│  │                                   │  │
│  │  (Monospace output area)         │  │
│  │                                   │  │
│  └─────────────────────────────────┘  │
└─────────────────────────────────────────┘
```

## Usage Guide

### Step 1: Choose Encoding (Optional)

- **Encode plaintext to Base64**: Check this box if you want to encode your input text to base64 before splitting
  - Useful for encoding sensitive data or preparing strings for transmission
  - The encoding happens automatically when you click "Split String"

### Step 2: Select Split Mode

Choose between two splitting modes:

#### Fixed Length Mode
- Select **"Fixed Length"** radio button
- Enter the desired chunk size in the **Size** field
- Example: Size 50 will split "hello world" into chunks of exactly 50 characters

#### Variable Size Mode
- Select **"Variable Size"** radio button
- Enter **Min** (minimum chunk size) and **Max** (maximum chunk size)
- The tool will randomly split the string into chunks between these bounds
- Example: Min 30, Max 70 will create chunks between 30-70 characters

**Note**: The input fields automatically enable/disable based on your selected mode.

### Step 3: Select Language Syntax

Choose your target programming language from the dropdown menu:

- **Bash**: `str="${str}chunk"`
- **Python**: `str += "chunk"`
- **Ruby**: `str += "chunk"`
- **C**: `strcat(str, "chunk");`
- **C++**: `str += "chunk";`
- **C#**: `str += "chunk";`
- **Perl**: `$str .= "chunk";`
- **Java**: `str += "chunk";`
- **JavaScript**: `str += "chunk";`
- **VBA**: `str = str & "chunk"`
- **SQL**: `SET str = str + 'chunk'`
- **R**: `str <- paste0(str, "chunk")`
- **PHP**: `$str .= "chunk";`
- **Rust**: `str.push_str("chunk");`
- **Go**: `str += "chunk"`

### Step 4: Enter Your String

1. Click in the **"Enter the string to be split"** text area
2. Paste or type your string
3. The text area supports:
   - Large strings (up to 500,000 characters)
   - Word wrapping for better readability
   - Standard text editing (copy, paste, select all)

### Step 5: Process the String

1. Click the **"Split String"** button
2. The output will appear in the **Output** area below
3. The output is:
   - Read-only (you can't edit it)
   - Monospace font (for code formatting)
   - Scrollable for long outputs
   - Easy to select and copy

### Step 6: Copy the Output

1. Select all text in the output area (Ctrl+A / Cmd+A)
2. Copy (Ctrl+C / Cmd+C)
3. Paste into your code editor

## Examples

### Example 1: Fixed Length Splitting

**Input:**
```
powershell.exe -nop -w hidden -enc SQBFAFgAKABOAGUAdwA...
```

**Settings:**
- Mode: Fixed Length
- Size: 50
- Language: Python
- Encoding: Unchecked

**Output:**
```python
str += "powershell.exe -nop -w hidden -enc SQBFAFgAKABOAGU"
str += "AdwA..."
```

### Example 2: Variable Size with Base64 Encoding

**Input:**
```
This is my secret message
```

**Settings:**
- Mode: Variable Size
- Min: 30
- Max: 70
- Language: JavaScript
- Encoding: ✓ Checked

**Output:**
```javascript
str += "VGhpcyBpcyBteSBzZWNyZXQgbWVzc2FnZQ=="
```

### Example 3: Large Base64 String

**Input:**
```
V2UncmUgbm8gc3RyYW5nZXJzIHRvIGxvdmUKWW91IGtub3cgdGhlIHJ1bGVzIGFuZCBzbyBkbyBJCkEgZnVsbCBjb21taXRtZW50J3Mgd2hhdCBJJ20gdGhpbmtpbmcgb2YK...
```

**Settings:**
- Mode: Fixed Length
- Size: 50
- Language: Bash
- Encoding: Unchecked

**Output:**
```bash
str="${str}V2UncmUgbm8gc3RyYW5nZXJzIHRvIGxvdmUKWW91IGtub3cgdGhlIHJ1bGVz"
str="${str}IGFuZCBzbyBkbyBJCkEgZnVsbCBjb21taXRtZW50J3Mgd2hhdCBJJ20gdGhp"
str="${str}bmtpbmcgb2YK..."
```

## Keyboard Shortcuts

While the GUI doesn't have custom keyboard shortcuts, standard GTK shortcuts work:

- **Ctrl+C / Cmd+C**: Copy selected text
- **Ctrl+V / Cmd+V**: Paste text
- **Ctrl+A / Cmd+A**: Select all
- **Ctrl+Z / Cmd+Z**: Undo (in input area)
- **Tab**: Navigate between fields
- **Enter**: Activates focused button (Split String)

## Tips and Best Practices

1. **Large Strings**: The GUI can handle very large strings. If you're processing extremely long text, consider using the CLI version for better performance.

2. **Base64 Encoding**: Remember that base64 encoding increases the string length by approximately 33%. Plan your chunk sizes accordingly.

3. **Variable Size**: Variable size mode creates random chunk sizes, which can be useful for obfuscation. Each run will produce different output.

4. **Language Selection**: Choose the language that matches your target environment. The syntax is automatically formatted correctly.

5. **Output Formatting**: The output uses monospace font to maintain proper code formatting. This makes it easy to copy into your IDE.

6. **Error Messages**: If you see an error message in the output area, check:
   - Size values are greater than 0
   - Min is less than or equal to Max (for variable mode)
   - Input string is not empty

## Troubleshooting

### Application Won't Start

**Problem**: The application doesn't launch or shows an error.

**Solutions**:
1. Verify GTK+3 is installed:
   ```bash
   pkg-config --modversion gtk+-3.0
   ```
2. Check the build was successful:
   ```bash
   ./build_gui.sh
   ```
3. Try running from terminal to see error messages:
   ```bash
   ./string_splitter_9000_gui
   ```

### GTK+3 Not Found

**Problem**: Build script can't find GTK+3.

**Solutions**:
- macOS: `brew install gtk+3`
- Linux: Install GTK+3 development packages (see Requirements)
- Ensure PKG_CONFIG_PATH is set correctly (build script handles this)

### Application Crashes with Large Input

**Problem**: Application crashes when processing very large strings.

**Solutions**:
- The GUI supports up to 500,000 characters
- For larger inputs, use the CLI version
- Try splitting the input into smaller chunks manually

### Output is Empty

**Problem**: Clicking "Split String" produces no output.

**Solutions**:
1. Check that input text area is not empty
2. Verify size values are valid (greater than 0)
3. For variable mode, ensure Min ≤ Max
4. Check for error messages in the output area

### Can't Copy Output

**Problem**: Unable to select or copy text from output area.

**Solutions**:
- The output area is read-only but selectable
- Use standard copy shortcuts (Ctrl+C / Cmd+C)
- Try selecting text with mouse drag
- Use Select All (Ctrl+A / Cmd+A) then copy

## Comparison: GUI vs CLI

| Feature | GUI | CLI |
|---------|-----|-----|
| Ease of Use | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ |
| Large Files | ⭐⭐⭐ | ⭐⭐⭐⭐⭐ |
| Automation | ⭐⭐ | ⭐⭐⭐⭐⭐ |
| Piping Support | ❌ | ✅ |
| File I/O | ❌ | ✅ |
| Interactive | ✅ | ✅ |
| Visual Feedback | ✅ | ⭐⭐ |

**Use GUI when:**
- You prefer a visual interface
- Working with moderate-sized strings
- Need to experiment with different settings
- Want to see results immediately

**Use CLI when:**
- Processing very large files
- Need to automate or script the process
- Want to pipe data between commands
- Working in a terminal environment

## Contributing

Found a bug or have a feature request? Please open an issue on GitHub!

## License

[Add your license information here]

## Support

For questions, issues, or contributions, please visit the [GitHub repository](https://github.com/yourusername/string-splitter-9000).

---

**Version**: 1.0.0  
**Last Updated**: [Current Date]

