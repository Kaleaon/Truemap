# Truemap iOS

iOS implementation of the Truemap 3D reconstruction application.

## Requirements

- iOS 15.0+
- Xcode 14.0+
- Swift 5.7+
- CocoaPods

## Building

1. Install dependencies:
   ```bash
   pod install
   ```

2. Open workspace:
   ```bash
   open Truemap.xcworkspace
   ```

3. Build and run in Xcode (⌘R)

## Architecture

- **SwiftUI**: Modern declarative UI
- **Combine**: Reactive programming for state management
- **AVFoundation**: Video processing
- **SceneKit**: 3D rendering
- **C++ Core**: Reconstruction algorithms via Objective-C++ bridge

## Testing

Run tests in Xcode (⌘U) or via command line:
```bash
xcodebuild test -workspace Truemap.xcworkspace -scheme Truemap -destination 'platform=iOS Simulator,name=iPhone 14'
```

## Project Structure

```
Truemap/
├── Views/          # SwiftUI views
├── ViewModels/     # MVVM view models
├── Services/       # Business logic and C++ bridge
├── Models/         # Data models
└── Resources/      # Assets and resources
```
