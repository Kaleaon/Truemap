# Truemap Setup Guide

## Prerequisites

### General Requirements
- Git
- CMake (version 3.20 or higher)
- C++17 compatible compiler

### iOS Development
- macOS (Monterey or later)
- Xcode 14.0 or later
- CocoaPods
- iOS 15.0+ target device or simulator

### Android Development
- Android Studio (Electric Eel or later)
- Android SDK (API level 26 or higher)
- Android NDK (r25 or later)
- JDK 11 or later
- Gradle 8.0+

## Installation

### 1. Clone the Repository

```bash
git clone https://github.com/Kaleaon/Truemap.git
cd Truemap
```

### 2. Initialize Submodules (C++ Dependencies)

```bash
git submodule update --init --recursive
```

### 3. iOS Setup

#### Install Dependencies

```bash
cd ios
pod install
```

#### Build Shared C++ Library

```bash
cd ../shared/cpp
mkdir build && cd build
cmake .. -DPLATFORM=iOS
make -j4
```

#### Open Xcode Project

```bash
cd ../../../ios
open Truemap.xcworkspace
```

Select your development team in Xcode and build the project.

### 4. Android Setup

#### Set Environment Variables

Add to your `~/.bashrc` or `~/.zshrc`:

```bash
export ANDROID_HOME=$HOME/Android/Sdk
export ANDROID_NDK_HOME=$ANDROID_HOME/ndk/25.2.9519653
export PATH=$PATH:$ANDROID_HOME/platform-tools
```

#### Build Shared C++ Library

```bash
cd shared/cpp
mkdir build-android && cd build-android
cmake .. -DPLATFORM=Android \
         -DANDROID_ABI=arm64-v8a \
         -DANDROID_PLATFORM=android-26 \
         -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake
make -j4
```

#### Open Android Project

```bash
cd ../../../android
```

Open the `android` folder in Android Studio and sync Gradle files.

## Building the Project

### iOS

1. Open `ios/Truemap.xcworkspace` in Xcode
2. Select target device or simulator
3. Press `Cmd+B` to build or `Cmd+R` to run

### Android

1. Open Android Studio
2. Open the `android` folder
3. Click "Build" → "Make Project" or press `Cmd+F9` (Mac) / `Ctrl+F9` (Windows/Linux)
4. Click "Run" → "Run 'app'" or press `Shift+F10` to run on device/emulator

## Running Tests

### iOS

In Xcode:
1. Press `Cmd+U` to run all tests
2. Or select specific test and press `Cmd+Ctrl+Option+U`

From command line:
```bash
cd ios
xcodebuild test -workspace Truemap.xcworkspace -scheme Truemap -destination 'platform=iOS Simulator,name=iPhone 14'
```

### Android

In Android Studio:
1. Right-click on test file and select "Run"

From command line:
```bash
cd android
./gradlew test
./gradlew connectedAndroidTest
```

## Troubleshooting

### iOS

**Issue**: CocoaPods not found
```bash
sudo gem install cocoapods
```

**Issue**: CMake build fails
- Ensure Xcode Command Line Tools are installed: `xcode-select --install`
- Update CMake: `brew upgrade cmake`

**Issue**: OpenMVG linking errors
- Clean build: `rm -rf build && mkdir build`
- Rebuild C++ library with verbose output: `cmake .. -DPLATFORM=iOS -DCMAKE_VERBOSE_MAKEFILE=ON`

### Android

**Issue**: NDK not found
- Install NDK via Android Studio SDK Manager
- Or download from: https://developer.android.com/ndk/downloads

**Issue**: Gradle sync fails
- Update Gradle wrapper: `./gradlew wrapper --gradle-version 8.0`
- Invalidate caches: Android Studio → File → Invalidate Caches / Restart

**Issue**: Native library not found
- Ensure C++ library is built for correct ABI
- Check `android/app/src/main/jniLibs/` for .so files

### General

**Issue**: Out of memory during build
- Increase heap size in `~/.gradle/gradle.properties`:
  ```
  org.gradle.jvmargs=-Xmx4096m
  ```

**Issue**: OpenMVG/OpenMVS compilation errors
- Ensure all submodules are updated: `git submodule update --init --recursive`
- Check compiler version supports C++17

## Development Workflow

### Making Changes to C++ Code

1. Edit files in `shared/cpp/`
2. Rebuild the library for your platform
3. iOS: Clean and rebuild in Xcode
4. Android: Sync Gradle and rebuild in Android Studio

### Adding New Dependencies

#### iOS (Swift/Objective-C)
Add to `ios/Podfile` and run `pod install`

#### Android (Kotlin)
Add to `android/app/build.gradle` and sync

#### C++ Shared Library
Update `shared/cpp/CMakeLists.txt` and rebuild

## Performance Profiling

### iOS
Use Instruments:
1. Product → Profile in Xcode
2. Select "Time Profiler" or "Allocations"

### Android
Use Android Profiler:
1. Run app in Android Studio
2. View → Tool Windows → Profiler

## Contributing

See [CONTRIBUTING.md](../CONTRIBUTING.md) for guidelines on contributing to the project.

## License

See [LICENSE](../LICENSE) for license information.
