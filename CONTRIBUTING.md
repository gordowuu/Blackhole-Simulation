# Contributing to Black Hole Simulation

First off, thank you for considering contributing to this project! This simulation combines advanced physics with real-time graphics, and contributions of all kinds are welcome.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [How Can I Contribute?](#how-can-i-contribute)
- [Development Setup](#development-setup)
- [Coding Standards](#coding-standards)
- [Submitting Changes](#submitting-changes)
- [Areas for Contribution](#areas-for-contribution)

## Code of Conduct

This project adheres to a Code of Conduct. By participating, you are expected to uphold this code:

- Be respectful and inclusive
- Welcome newcomers
- Focus on what is best for the community
- Show empathy towards others

## How Can I Contribute?

### Reporting Bugs

Before creating bug reports, please check existing issues. When you create a bug report, include:

- **Clear title** describing the problem
- **System information**: OS, GPU, OpenGL version
- **Steps to reproduce** the behavior
- **Expected vs actual** behavior
- **Screenshots** if applicable
- **Error logs** from console output

### Suggesting Enhancements

Enhancement suggestions are tracked as GitHub issues. When creating an enhancement suggestion, include:

- **Use case**: Why is this enhancement useful?
- **Expected behavior**: What should happen?
- **Alternative solutions**: Have you considered alternatives?
- **Additional context**: Mock-ups, references, etc.

### Pull Requests

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Make your changes
4. Test thoroughly
5. Commit with clear messages
6. Push to your branch
7. Open a Pull Request

## Development Setup

### Prerequisites

- C++17 compatible compiler
- CMake 3.15+
- OpenGL 4.6 capable GPU
- Git

### Setup Process

```bash
# Clone your fork
git clone https://github.com/YOUR_USERNAME/BlackholeSim.git
cd BlackholeSim

# Add upstream remote
git remote add upstream https://github.com/ORIGINAL_OWNER/BlackholeSim.git

# Install dependencies
.\download_deps.ps1

# Setup GLAD (see external/glad/README_SETUP.md)

# Build
.\build.ps1
```

### Project Structure

```
src/
├── Core/           # Window, Camera, Shader, Input management
├── Physics/        # Black hole physics and calculations
├── Rendering/      # Rendering pipeline and effects
└── UI/             # User interface (ImGui)

shaders/            # GLSL compute and fragment shaders
external/           # Third-party dependencies
assets/             # Textures and resources
```

## Coding Standards

### C++ Style

- **Standard**: C++17
- **Naming**:
  - Classes: `PascalCase`
  - Functions: `camelCase`
  - Variables: `camelCase`
  - Private members: `m_variableName`
  - Constants: `UPPER_CASE`
  
- **Formatting**:
  - 4 spaces for indentation (no tabs)
  - Opening braces on same line for functions
  - Maximum 100 characters per line
  
- **Comments**:
  - Use `//` for single-line comments
  - Use `/* */` for multi-line explanations
  - Document complex physics calculations

### Example:

```cpp
class BlackHole {
public:
    BlackHole(float mass, float spin);
    
    // Calculate Schwarzschild radius
    float getSchwarzschildRadius() const { return m_schwarzschildRadius; }
    
private:
    void updateDerivedQuantities();
    
    float m_mass;
    float m_spin;
    float m_schwarzschildRadius;
};
```

### GLSL Style

- Consistent indentation
- Clear variable names
- Comment complex calculations
- Use `const` for constants

## Submitting Changes

### Commit Messages

Follow this format:

```
<type>: <subject>

<body>

<footer>
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Formatting, no code change
- `refactor`: Code restructuring
- `perf`: Performance improvement
- `test`: Adding tests
- `chore`: Maintenance tasks

**Example:**

```
feat: Add binary black hole support

Implements two-body dynamics with gravitational wave emission.
Uses numerical integration of Einstein field equations.

Closes #123
```

### Pull Request Process

1. **Update documentation** for any API changes
2. **Add tests** if applicable
3. **Update README.md** if needed
4. **Ensure builds** successfully on Windows
5. **Request review** from maintainers

### Review Checklist

Before submitting, verify:

- [ ] Code compiles without warnings
- [ ] No runtime errors in basic testing
- [ ] Follows coding standards
- [ ] Documentation is updated
- [ ] Commit messages are clear
- [ ] No debug code left in
- [ ] Shaders compile successfully

## Areas for Contribution

### 🟢 Beginner-Friendly

- **Documentation**: Improve README, comments
- **UI improvements**: Better layout, tooltips
- **Presets**: Add more black hole configurations
- **Bug fixes**: Check issues labeled "good first issue"
- **Testing**: Add test cases

### 🟡 Intermediate

- **Physics improvements**: More accurate models
- **Performance optimization**: Shader improvements
- **Additional effects**: Lens flares, particle systems
- **Cross-platform support**: Linux/macOS builds
- **Asset management**: Better texture loading

### 🔴 Advanced

- **Kerr-Newman metric**: Charged black holes
- **Binary black holes**: Two-body dynamics
- **Gravitational waves**: Visualization
- **Volumetric rendering**: 3D accretion disk
- **VR support**: Immersive experience
- **GPU optimization**: Better compute shader usage

## Specific Contribution Guides

### Adding a New Physics Feature

1. Create class in `src/Physics/`
2. Add calculations
3. Integrate into `Renderer`
4. Add UI controls in `Interface.cpp`
5. Update shaders if needed
6. Document the physics

### Adding a Visual Effect

1. Implement in `src/Rendering/`
2. Create/modify shaders in `shaders/`
3. Add toggle in UI
4. Test performance impact
5. Add screenshot to PR

### Improving Shaders

1. Edit in `shaders/`
2. Test changes thoroughly
3. Measure performance impact
4. Document any new uniforms
5. Add comments for complex math

## Testing

### Manual Testing

Before submitting:

1. Build in Release mode
2. Test all UI controls
3. Try all presets
4. Verify camera controls
5. Check performance (FPS)
6. Test on different qualities

### Performance Testing

Benchmark your changes:

```cpp
// Add timing code
auto start = std::chrono::high_resolution_clock::now();
// Your code here
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
```

Report any performance impact in PR.

## Documentation

### Code Documentation

- Add comments for non-obvious code
- Explain physics formulas
- Document shader uniforms
- Update headers for API changes

### User Documentation

- Update README.md for new features
- Add usage examples
- Include screenshots
- Update QUICKSTART.md if setup changes

## Getting Help

- **Discord**: [Join our community]
- **Issues**: Ask questions labeled "question"
- **Email**: maintainer@example.com

## Recognition

Contributors are recognized in:
- README.md Contributors section
- Release notes
- Project website (coming soon)

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

## Quick Reference

### Common Tasks

```bash
# Update from upstream
git fetch upstream
git merge upstream/main

# Create feature branch
git checkout -b feature/my-feature

# Build and test
.\build.ps1

# Run tests
.\build\bin\Release\BlackholeSim.exe

# Check for errors
# Look for console output

# Commit changes
git add .
git commit -m "feat: description"

# Push to your fork
git push origin feature/my-feature
```

### Need Help?

- Check existing issues
- Read documentation
- Ask in discussions
- Contact maintainers

---

**Thank you for contributing to making this simulation better!** 🌌
