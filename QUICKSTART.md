# Quick Start Guide

This guide will help you get started with the Truemap 3D reconstruction application quickly.

## Prerequisites Check

Before starting, make sure you have:

- [ ] Git installed
- [ ] For iOS: macOS, Xcode 14+, CocoaPods
- [ ] For Android: Android Studio, Android SDK/NDK
- [ ] CMake 3.20+

## 5-Minute Setup

### Option A: iOS Development

```bash
# 1. Clone and enter directory
git clone https://github.com/Kaleaon/Truemap.git
cd Truemap

# 2. Install iOS dependencies (if CocoaPods is installed)
cd ios
pod install

# 3. Open in Xcode
open Truemap.xcworkspace

# 4. Select your development team
# 5. Build and run (⌘R)
```

### Option B: Android Development

```bash
# 1. Clone and enter directory
git clone https://github.com/Kaleaon/Truemap.git
cd Truemap

# 2. Open in Android Studio
# File → Open → Select 'android' folder

# 3. Wait for Gradle sync
# 4. Build and run (Shift+F10)
```

## What You'll See

When you run the app, you'll see:

1. **Main Screen**: A clean interface to add video URLs
2. **Add Videos**: Button to input YouTube URLs
3. **Start Reconstruction**: Button to begin processing
4. **Progress View**: Real-time updates during processing
5. **Export**: Option to save the final mesh

## Try It Out

### Test the UI Flow

1. **Add a URL**: 
   - Tap "Add Video URL"
   - Enter any test URL (e.g., "https://youtube.com/watch?v=test")
   - Tap "Add"

2. **Start Processing**:
   - Tap "Start Reconstruction"
   - Watch the progress bar and status messages
   - See the simulated pipeline stages

3. **Cancel** (optional):
   - Tap "Cancel" to stop processing

4. **Export** (after completion):
   - Tap "Export Mesh" to simulate saving the result

## Current Functionality

✅ **What Works Now**:
- Adding/removing video URLs
- UI state management
- Progress tracking
- Status updates
- Error handling
- Simulated processing pipeline

⏱️ **What's Simulated**:
- Video downloading
- Frame extraction
- 3D reconstruction
- Mesh generation
- Export to file

The UI demonstrates the complete user experience. The actual 3D reconstruction algorithms are stubbed and ready for implementation.

## Project Structure Overview

```
Truemap/
├── README.md              ← Start here for full overview
├── IMPLEMENTATION.md      ← Current status and next steps
├── docs/
│   ├── ARCHITECTURE.md    ← System design
│   ├── SETUP.md           ← Detailed setup
│   └── API.md             ← API reference
├── ios/                   ← iOS application
├── android/               ← Android application
└── shared/cpp/            ← C++ core library
```

## Next Steps

### For Users
1. Explore the UI on both platforms
2. Read the documentation in `docs/`
3. Check `IMPLEMENTATION.md` for current status

### For Developers
1. Read `ARCHITECTURE.md` to understand the design
2. Read `API.md` to understand the interfaces
3. Read `CONTRIBUTING.md` before contributing
4. Start implementing the C++ algorithms (see IMPLEMENTATION.md)

## Common Questions

**Q: Can it actually reconstruct 3D models?**
A: Not yet. The foundation is complete, but the 3D reconstruction algorithms need to be integrated (OpenMVG, OpenCV, etc.).

**Q: How long does reconstruction take?**
A: The current simulation runs through stages in a few seconds. Real reconstruction would take minutes to hours depending on video count and device performance.

**Q: What video formats are supported?**
A: The interface accepts YouTube URLs. Actual download and processing needs to be implemented.

**Q: Can I contribute?**
A: Yes! See CONTRIBUTING.md for guidelines.

## Troubleshooting

### iOS
- **Pods not found**: Run `pod install` in the ios directory
- **Team signing**: Select your development team in Xcode project settings
- **Simulator issues**: Try different iOS simulator versions

### Android
- **Gradle sync fails**: Check that Android SDK and NDK are installed
- **Build errors**: Clean project (Build → Clean Project)
- **Emulator issues**: Create a new AVD in Device Manager

## Resources

- **Main README**: [README.md](README.md)
- **Architecture**: [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)
- **Setup Guide**: [docs/SETUP.md](docs/SETUP.md)
- **API Reference**: [docs/API.md](docs/API.md)
- **Contributing**: [CONTRIBUTING.md](CONTRIBUTING.md)
- **Status**: [IMPLEMENTATION.md](IMPLEMENTATION.md)

## Getting Help

- Open an issue on GitHub
- Check the documentation
- Review the code comments

## What's Next?

After exploring the app:

1. **Learn**: Read the architecture and API docs
2. **Build**: Follow the setup guide for your platform
3. **Contribute**: Help implement the 3D reconstruction algorithms
4. **Experiment**: Modify the UI or add features

---

**Happy Coding!** 🚀

For detailed information, see [README.md](README.md) and [IMPLEMENTATION.md](IMPLEMENTATION.md).
