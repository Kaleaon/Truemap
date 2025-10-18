# Quick Start Guide

## Running the Application

Simply open `index.html` in any modern web browser:
- Chrome 90+
- Firefox 88+
- Safari 14+
- Edge 90+

## Using with a Local Server

For the best experience with local file loading, run a local web server:

### Python 3
```bash
python -m http.server 8000
# Open http://localhost:8000
```

### Node.js
```bash
npx http-server
# Open http://localhost:8080
```

### PHP
```bash
php -S localhost:8000
# Open http://localhost:8000
```

## Loading a Scene

1. **Click** the purple "Choose .ply file" button or
2. **Drag and drop** a .ply file onto the viewer area

## Camera Controls

| Action | Input |
|--------|-------|
| Rotate | Left mouse button + drag |
| Pan | Right mouse button + drag |
| Zoom | Mouse wheel scroll |
| Reset | Click "Reset Camera" button |

## Adjusting Settings

- **Field of View**: Use slider to adjust camera FOV (30° - 120°)
- **Point Size**: Use slider to change point size (1 - 10)

## Supported File Format

PLY files with the following properties:
- Position: x, y, z (float)
- Color: red, green, blue (uchar, 0-255)
- Format: ASCII or binary (little-endian)

## Example PLY File

```
ply
format ascii 1.0
element vertex 3
property float x
property float y
property float z
property uchar red
property uchar green
property uchar blue
end_header
0.0 0.0 0.0 255 0 0
1.0 0.0 0.0 0 255 0
0.0 1.0 0.0 0 0 255
```

## Troubleshooting

**File won't load:**
- Ensure the file has a .ply extension
- Check that the file format is valid PLY
- Try opening browser console (F12) to see error messages

**Performance issues:**
- Large files (>100MB) may take time to load
- Try reducing point size if rendering is slow
- Consider using a smaller subset of your data

**WebGL not working:**
- Ensure WebGL is enabled in your browser
- Update graphics drivers
- Try a different browser
