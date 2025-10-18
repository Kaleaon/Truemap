# Truemap Android

Android implementation of the Truemap 3D reconstruction application.

## Requirements

- Android SDK 26+
- Android NDK r25+
- Android Studio Electric Eel+
- Kotlin 1.9+
- Gradle 8.0+

## Building

1. Open project in Android Studio:
   ```bash
   # Open the 'android' directory in Android Studio
   ```

2. Sync Gradle files

3. Build and run (Shift+F10)

Or via command line:
```bash
./gradlew assembleDebug
./gradlew installDebug
```

## Architecture

- **Jetpack Compose**: Modern declarative UI
- **Kotlin Coroutines**: Asynchronous programming
- **ViewModel**: State management
- **MediaCodec**: Video processing
- **Filament**: 3D rendering
- **JNI**: Bridge to C++ core

## Testing

Run tests:
```bash
./gradlew test                    # Unit tests
./gradlew connectedAndroidTest    # Instrumented tests
```

## Project Structure

```
app/src/main/
├── kotlin/com/truemap/app/
│   ├── MainActivity.kt           # Main activity
│   ├── ReconstructionViewModel.kt # View model
│   ├── ReconstructionService.kt  # Business logic
│   └── ui/theme/                 # UI theming
├── cpp/                          # JNI bridge (optional)
└── res/                          # Resources
```

## Native Library

The C++ core is built via CMake and linked through JNI. See `app/build.gradle` for configuration.
