# Packaging and Distribution Plan
## String Splitter 9000

## Overview
This document outlines the plan for packaging and distributing String Splitter 9000 across multiple platforms and package formats.

---

## 1. Source Code Distribution

### Current Status
✅ Source code already available:
- `string_splitter_9000.c` - CLI version
- `string_splitter_9000_gui.c` - GUI version

### Deliverables
- Source code tarball (`.tar.gz`)
- Source code zip archive (`.zip`)
- Git repository tags for releases

### Implementation
- Create `make dist` target or script to generate source archives
- Include: source files, README, LICENSE, build scripts
- Exclude: compiled binaries, build artifacts

---

## 2. Linux Packages

### 2.1 Debian/Ubuntu (.deb Package)

#### Tools Required
- `dpkg-deb` (Debian package builder)
- `fakeroot` (for building packages without root)
- `dh-make` (optional, for initial package setup)
- `debuild` (optional, for building signed packages)

#### Package Structure
```
string-splitter-9000/
├── DEBIAN/
│   ├── control          # Package metadata
│   ├── postinst         # Post-installation script
│   ├── prerm            # Pre-removal script
│   └── copyright        # License information
├── usr/
│   ├── bin/
│   │   ├── string-splitter-9000      # CLI binary
│   │   └── string-splitter-9000-gui  # GUI binary
│   ├── share/
│   │   ├── applications/
│   │   │   └── string-splitter-9000.desktop  # Desktop entry
│   │   └── doc/
│   │       └── string-splitter-9000/
│   │           ├── README.md
│   │           └── changelog
│   └── man/
│       └── man1/
│           ├── string-splitter-9000.1
│           └── string-splitter-9000-gui.1
```

#### Dependencies
- CLI: None (static or minimal)
- GUI: `libgtk-3-0` (>= 3.20), `libglib2.0-0` (>= 2.50)

#### Build Process
1. Cross-compile or build on Debian/Ubuntu system
2. Create package directory structure
3. Copy binaries and files
4. Generate control file with metadata
5. Build package: `dpkg-deb --build string-splitter-9000`
6. Test installation: `dpkg -i string-splitter-9000.deb`

#### Build Script
- `build_deb.sh` - Automated .deb package builder

---

### 2.2 Red Hat/CentOS/Fedora (.rpm Package)

#### Tools Required
- `rpmbuild` (RPM package builder)
- `rpmdevtools` (development utilities)
- `spectool` (for downloading sources)

#### Package Structure
```
~/rpmbuild/
├── SPECS/
│   └── string-splitter-9000.spec  # RPM spec file
├── SOURCES/
│   └── string-splitter-9000-1.0.0.tar.gz
├── BUILD/
├── BUILDROOT/
└── RPMS/
    └── x86_64/
        └── string-splitter-9000-1.0.0-1.x86_64.rpm
```

#### Dependencies (in .spec file)
- CLI: None
- GUI: `gtk3-devel`, `glib2-devel` (build-time), `gtk3`, `glib2` (runtime)

#### Build Process
1. Create RPM spec file with metadata and build instructions
2. Prepare source tarball
3. Build RPM: `rpmbuild -ba string-splitter-9000.spec`
4. Test installation: `rpm -ivh string-splitter-9000-1.0.0-1.x86_64.rpm`

#### Build Script
- `build_rpm.sh` - Automated .rpm package builder

---

## 3. macOS Distribution

### 3.1 DMG Package

#### Tools Required
- Xcode Command Line Tools
- `hdiutil` (macOS disk image utility)
- `create-dmg` (optional, third-party tool for better DMG creation)

#### Package Structure
```
String Splitter 9000.app/
└── Contents/
    ├── Info.plist          # App metadata
    ├── MacOS/
    │   └── string-splitter-9000-gui  # GUI binary
    └── Resources/
        └── icon.icns       # Application icon
```

#### DMG Contents
- `String Splitter 9000.app` (application bundle)
- `Applications` symlink (for drag-and-drop installation)
- Optional: README, LICENSE files

#### Build Process
1. Create `.app` bundle structure
2. Copy GUI binary to `Contents/MacOS/`
3. Create `Info.plist` with app metadata
4. Add application icon (if available)
5. Create DMG: `hdiutil create -volname "String Splitter 9000" -srcfolder "String Splitter 9000.app" -ov -format UDZO string-splitter-9000.dmg`
6. Optional: Code sign the app and DMG

#### Build Script
- `build_dmg.sh` - Automated DMG creation script

---

### 3.2 Homebrew Formula

#### Tools Required
- Homebrew (already installed)
- Git (for formula repository)

#### Formula Location
- Option 1: Homebrew Core (requires approval, complex)
- Option 2: Custom Tap (recommended for initial release)
  - Repository: `homebrew-tap` or similar
  - Formula file: `Formula/string-splitter-9000.rb`

#### Formula Structure
```ruby
class StringSplitter9000 < Formula
  desc "String splitting tool with CLI and GUI versions"
  homepage "https://github.com/yourusername/string-splitter-9000"
  url "https://github.com/yourusername/string-splitter-9000/releases/download/v1.0.0/string-splitter-9000-1.0.0.tar.gz"
  sha256 "..."
  license "MIT"
  
  depends_on "gtk+3"
  
  def install
    system "gcc", "-o", "string-splitter-9000", "string_splitter_9000.c"
    system "./build_gui.sh"
    bin.install "string_splitter_9000"
    bin.install "string_splitter_9000_gui"
  end
  
  test do
    system "#{bin}/string-splitter-9000", "--version"
  end
end
```

#### Installation
Users install via: `brew install yourusername/tap/string-splitter-9000`

#### Build Script
- `build_homebrew_formula.sh` - Generate/update Homebrew formula

---

## 4. Windows Distribution

### 4.1 Executable (.exe)

#### Tools Required
- MinGW-w64 or MSYS2 (GCC compiler for Windows)
- GTK+3 for Windows (pre-built binaries)
- Optional: NSIS or Inno Setup (for installer)

#### Build Options

**Option A: Native Windows Build (MinGW)**
- Install MSYS2
- Install GTK+3: `pacman -S mingw-w64-x86_64-gtk3`
- Compile with MinGW GCC
- Bundle GTK DLLs with executable

**Option B: Cross-compile from Linux/macOS**
- Use MXE (M cross environment)
- Cross-compile GTK+3 and application
- Package Windows binaries

#### Package Structure
```
string-splitter-9000-windows/
├── bin/
│   ├── string-splitter-9000.exe      # CLI executable
│   └── string-splitter-9000-gui.exe  # GUI executable
├── lib/
│   └── (GTK+3 DLLs and dependencies)
└── share/
    └── (GTK+3 data files)
```

#### Dependencies
- GTK+3 runtime DLLs (can be bundled or installed separately)
- MSVCRT (Microsoft Visual C++ Runtime)

#### Build Process
1. Set up MinGW/MSYS2 environment
2. Install GTK+3 for Windows
3. Compile CLI: `gcc -o string-splitter-9000.exe string_splitter_9000.c`
4. Compile GUI: `gcc -o string-splitter-9000-gui.exe string_splitter_9000_gui.c $(pkg-config --cflags --libs gtk+-3.0)`
5. Bundle GTK DLLs (or create installer)
6. Test on clean Windows system

#### Optional: Windows Installer
- Use NSIS or Inno Setup to create `.exe` installer
- Include: binaries, DLLs, Start Menu shortcuts, uninstaller

#### Build Script
- `build_windows.sh` or `build_windows.bat` - Windows build script

---

## 5. Directory Structure

### Proposed Repository Layout
```
String_Splitter_9000/
├── src/                          # Source code
│   ├── string_splitter_9000.c
│   └── string_splitter_9000_gui.c
├── build/                        # Build scripts
│   ├── build_gui.sh
│   ├── build_deb.sh
│   ├── build_rpm.sh
│   ├── build_dmg.sh
│   ├── build_homebrew_formula.sh
│   └── build_windows.sh
├── packaging/                    # Packaging files
│   ├── debian/
│   │   ├── control
│   │   ├── postinst
│   │   └── copyright
│   ├── rpm/
│   │   └── string-splitter-9000.spec
│   ├── homebrew/
│   │   └── string-splitter-9000.rb
│   └── windows/
│       └── installer.nsi (or setup.iss)
├── dist/                         # Distribution files (gitignored)
│   ├── linux/
│   ├── macos/
│   └── windows/
├── docs/                         # Documentation
│   ├── INSTALL.md
│   └── PACKAGING.md
├── README.md
├── LICENSE
└── .gitignore
```

---

## 6. Build Automation

### Makefile Approach
Create a `Makefile` with targets:
- `make all` - Build both CLI and GUI
- `make dist` - Create source tarball
- `make deb` - Build Debian package
- `make rpm` - Build RPM package
- `make dmg` - Build macOS DMG
- `make windows` - Build Windows executables
- `make clean` - Clean build artifacts

### CI/CD Integration (Future)
- GitHub Actions workflows for:
  - Automated builds on push/tag
  - Multi-platform testing
  - Package generation
  - Release publishing

---

## 7. Testing Strategy

### For Each Platform:
1. **Build Testing**: Verify compilation succeeds
2. **Installation Testing**: Test package installation
3. **Functionality Testing**: Run both CLI and GUI versions
4. **Dependency Testing**: Test on clean system (minimal dependencies)
5. **Uninstallation Testing**: Verify clean removal

### Test Environments
- Linux: Ubuntu 20.04+, Debian 11+, Fedora 35+, CentOS 8+
- macOS: macOS 11+ (Big Sur and later)
- Windows: Windows 10+, Windows 11

---

## 8. Versioning and Releases

### Version Format
- Semantic versioning: `MAJOR.MINOR.PATCH` (e.g., 1.0.0)
- Git tags: `v1.0.0`, `v1.1.0`, etc.

### Release Checklist
- [ ] Update version numbers in source code
- [ ] Update CHANGELOG.md
- [ ] Create git tag
- [ ] Build all platform packages
- [ ] Test all packages
- [ ] Create GitHub release with assets
- [ ] Update Homebrew formula (if applicable)
- [ ] Announce release

---

## 9. Documentation Updates

### Files to Create/Update
- `INSTALL.md` - Installation instructions for all platforms
- `PACKAGING.md` - Detailed packaging instructions
- `CONTRIBUTING.md` - Contribution guidelines
- `CHANGELOG.md` - Version history
- `LICENSE` - License file (MIT, GPL, etc.)

### README.md Updates
- Add installation sections for each platform
- Add download links
- Add package manager installation instructions

---

## 10. Dependencies and Requirements

### Build Dependencies
- **All Platforms**: GCC or Clang compiler
- **Linux**: GTK+3 development packages
- **macOS**: Xcode Command Line Tools, GTK+3 (via Homebrew)
- **Windows**: MinGW/MSYS2, GTK+3 for Windows

### Runtime Dependencies
- **CLI**: None (standalone)
- **GUI**: GTK+3 runtime libraries

---

## 11. Implementation Priority

### Phase 1: Source Distribution
1. Create source tarball/zip generation
2. Add LICENSE file
3. Update README with source installation

### Phase 2: Linux Packages
1. Debian package (.deb)
2. RPM package (.rpm)

### Phase 3: macOS Distribution
1. DMG package
2. Homebrew formula (custom tap)

### Phase 4: Windows Distribution
1. Windows executable (.exe)
2. Optional: Windows installer

---

## 12. Notes and Considerations

### Code Signing
- **macOS**: Consider code signing for Gatekeeper compatibility
- **Windows**: Consider code signing for SmartScreen compatibility
- **Linux**: Generally not required

### Static vs Dynamic Linking
- **CLI**: Can be statically linked (no dependencies)
- **GUI**: Requires GTK+3 (dynamic linking recommended for package size)

### License
- Determine and add LICENSE file
- Include in all packages

### Icon/Assets
- Create application icon (for GUI)
- Add to packages where applicable

### Desktop Integration
- Create `.desktop` file for Linux
- Create `Info.plist` for macOS
- Optional: Windows Start Menu integration

---

## 13. Estimated Effort

- **Source Distribution**: 1-2 hours
- **Linux Packages**: 4-6 hours (both .deb and .rpm)
- **macOS DMG**: 2-3 hours
- **Homebrew Formula**: 1-2 hours
- **Windows Build**: 4-6 hours
- **Documentation**: 2-3 hours
- **Testing**: 2-3 hours

**Total Estimated Time**: 16-25 hours

---

## 14. Next Steps

1. Review and approve this plan
2. Create directory structure
3. Implement Phase 1 (Source Distribution)
4. Implement Phase 2 (Linux Packages)
5. Implement Phase 3 (macOS Distribution)
6. Implement Phase 4 (Windows Distribution)
7. Create comprehensive documentation
8. Test all packages
9. Create initial release

---

## Questions for Review

1. **License**: What license should we use? (MIT, GPL, BSD, etc.)
2. **Version**: What should be the initial release version? (1.0.0?)
3. **Icon**: Do we want to create an application icon?
4. **Code Signing**: Should we pursue code signing for macOS/Windows?
5. **CI/CD**: Should we set up automated builds from the start?
6. **Homebrew**: Custom tap or attempt Homebrew Core submission?

---

*This plan is a living document and should be updated as implementation progresses.*

