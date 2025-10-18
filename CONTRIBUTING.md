# Contributing to Truemap

Thank you for your interest in contributing to Truemap! This document provides guidelines for contributing to the project.

## Code of Conduct

Please be respectful and constructive in all interactions with the community.

## How to Contribute

### Reporting Issues

- Use the GitHub issue tracker
- Clearly describe the issue with steps to reproduce
- Include device/OS information
- Attach logs if applicable

### Suggesting Enhancements

- Use GitHub issues with "enhancement" label
- Clearly describe the feature and its benefits
- Consider implementation complexity

### Pull Requests

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Add tests if applicable
5. Ensure code follows style guidelines
6. Commit with clear messages (`git commit -m 'Add amazing feature'`)
7. Push to your fork (`git push origin feature/amazing-feature`)
8. Open a Pull Request

## Development Guidelines

### Code Style

#### C++
- Follow C++17 standard
- Use meaningful variable names
- Comment complex algorithms
- Keep functions focused and small
- Use RAII for resource management

#### Swift
- Follow Swift API Design Guidelines
- Use SwiftLint for consistency
- Prefer value types over reference types
- Use proper access control

#### Kotlin
- Follow Kotlin coding conventions
- Use ktlint for formatting
- Prefer immutability
- Use coroutines for async operations

### Testing

- Write unit tests for new features
- Ensure existing tests pass
- Test on multiple devices/OS versions
- Include integration tests where appropriate

### Documentation

- Update README if adding features
- Document public APIs
- Include inline comments for complex logic
- Update ARCHITECTURE.md for structural changes

### Commit Messages

- Use present tense ("Add feature" not "Added feature")
- Use imperative mood ("Move cursor to..." not "Moves cursor to...")
- Limit first line to 72 characters
- Reference issues and pull requests

### Review Process

1. Automated CI checks must pass
2. Code review by maintainers
3. Address review comments
4. Squash commits if requested
5. Maintainer will merge when approved

## Building

See [SETUP.md](docs/SETUP.md) for build instructions.

## Questions?

Feel free to open an issue for questions about contributing.

## License

By contributing, you agree that your contributions will be licensed under the MIT License.
