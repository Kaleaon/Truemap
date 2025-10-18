# 🎨 Truemap - Gaussian Splatting 3D Viewer

A modern web application for viewing and interacting with Gaussian Splatting 3D scenes directly in your browser.

## Features

- 🎯 **3D Scene Visualization**: View Gaussian Splatting scenes with full 3D interaction
- 🖱️ **Interactive Controls**: Orbit, pan, and zoom with intuitive mouse controls
- 📁 **File Upload**: Support for .ply file format commonly used in Gaussian Splatting
- 🎨 **Color Rendering**: Full RGB color support from splat data
- ⚡ **WebGL Powered**: Hardware-accelerated rendering using Three.js
- 📱 **Responsive Design**: Works on desktop and mobile devices
- 🌙 **Modern UI**: Clean, dark-themed interface with smooth animations

## Getting Started

### Quick Start

1. Open `index.html` in a modern web browser (Chrome, Firefox, Safari, Edge)
2. Click "Choose .ply file" or drag and drop a Gaussian Splatting .ply file onto the viewer
3. Interact with your 3D scene using mouse controls

### Mouse Controls

- **Rotate**: Left mouse button + drag
- **Pan**: Right mouse button + drag
- **Zoom**: Mouse wheel scroll

### Camera Settings

- **Field of View (FOV)**: Adjust the camera's field of view (30° - 120°)
- **Point Size**: Control the size of rendered points (1 - 10)
- **Reset Camera**: Return to default camera position and settings

## File Format

Truemap supports .ply (Polygon File Format) files with the following features:

- Both ASCII and binary PLY formats
- Vertex positions (x, y, z)
- RGB color data (red, green, blue)
- Gaussian Splatting specific properties

### Example PLY Structure

```
ply
format binary_little_endian 1.0
element vertex 1000
property float x
property float y
property float z
property uchar red
property uchar green
property uchar blue
end_header
[binary data...]
```

## Technology Stack

- **Three.js**: 3D rendering engine
- **WebGL**: Hardware-accelerated graphics
- **HTML5 Canvas**: Rendering surface
- **Vanilla JavaScript**: No framework dependencies
- **CSS3**: Modern styling and animations

## Browser Compatibility

- ✅ Chrome 90+
- ✅ Firefox 88+
- ✅ Safari 14+
- ✅ Edge 90+

## Development

### Project Structure

```
Truemap/
├── index.html          # Main HTML file
├── style.css           # Styling and layout
├── app.js              # Application logic and rendering
└── README.md           # Documentation
```

### Local Development

No build process required! Simply:

1. Clone the repository
2. Open `index.html` in your browser
3. Start viewing 3D scenes

For local file testing, you may need to run a local server:

```bash
# Using Python 3
python -m http.server 8000

# Using Node.js
npx http-server

# Using PHP
php -S localhost:8000
```

Then navigate to `http://localhost:8000`

## How It Works

1. **File Loading**: The application reads .ply files using the File API
2. **Parsing**: Binary or ASCII PLY data is parsed to extract vertex positions and colors
3. **Geometry Creation**: Three.js BufferGeometry is created from the parsed data
4. **Rendering**: WebGL renders the point cloud with perspective projection
5. **Interaction**: Mouse events control camera position and orientation

## Limitations

- Large files (>100MB) may take time to load and render
- Performance depends on GPU capabilities
- Browser memory limits apply for very large datasets

## Future Enhancements

- [ ] Advanced Gaussian Splatting shader
- [ ] Support for additional file formats
- [ ] Scene export functionality
- [ ] Multiple scene comparison
- [ ] Screenshot capture
- [ ] Performance optimizations for large datasets
- [ ] Mobile touch controls
- [ ] Preset camera positions
- [ ] Scene annotations

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## License

MIT License - feel free to use this project for any purpose.

## Acknowledgments

- Inspired by the Gaussian Splatting research
- Built with Three.js library
- Modern web technologies

## Support

For issues, questions, or suggestions, please open an issue on GitHub.

---

Made with ❤️ for the 3D graphics community