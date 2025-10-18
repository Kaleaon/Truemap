// Gaussian Splatting 3D Viewer Application - Pure WebGL Implementation
class GaussianSplattingViewer {
    constructor() {
        this.canvas = null;
        this.gl = null;
        this.program = null;
        this.buffers = null;
        this.camera = {
            position: [0, 2, 5],
            rotation: [0, 0],
            target: [0, 0, 0],
            fov: 75,
            zoom: 5
        };
        this.controls = {
            isRotating: false,
            isPanning: false,
            lastX: 0,
            lastY: 0,
            rotationX: 0,
            rotationY: 0,
            targetRotationX: 0,
            targetRotationY: 0,
            panX: 0,
            panY: 0,
            zoom: 5
        };
        this.pointCount = 0;
        this.pointSize = 3;
        this.animationId = null;
        
        this.init();
        this.setupEventListeners();
    }
    
    init() {
        // Get canvas and WebGL context
        this.canvas = document.getElementById('canvas3d');
        this.gl = this.canvas.getContext('webgl', {
            antialias: true,
            alpha: true
        });
        
        if (!this.gl) {
            alert('WebGL not supported in this browser');
            return;
        }
        
        this.resizeCanvas();
        
        // Initialize WebGL
        this.setupShaders();
        this.setupControls();
        
        // Start rendering
        this.animate();
        
        // Show drop zone initially
        document.getElementById('dropZone').classList.add('active');
    }
    
    setupShaders() {
        const gl = this.gl;
        
        // Vertex shader
        const vertexShaderSource = `
            attribute vec3 position;
            attribute vec3 color;
            
            uniform mat4 uModelViewMatrix;
            uniform mat4 uProjectionMatrix;
            uniform float uPointSize;
            
            varying vec3 vColor;
            
            void main() {
                gl_Position = uProjectionMatrix * uModelViewMatrix * vec4(position, 1.0);
                gl_PointSize = uPointSize;
                vColor = color;
            }
        `;
        
        // Fragment shader
        const fragmentShaderSource = `
            precision mediump float;
            
            varying vec3 vColor;
            
            void main() {
                // Create circular points
                vec2 coord = gl_PointCoord - vec2(0.5);
                if (length(coord) > 0.5) {
                    discard;
                }
                gl_FragColor = vec4(vColor, 1.0);
            }
        `;
        
        // Compile shaders
        const vertexShader = this.compileShader(gl, vertexShaderSource, gl.VERTEX_SHADER);
        const fragmentShader = this.compileShader(gl, fragmentShaderSource, gl.FRAGMENT_SHADER);
        
        // Create program
        this.program = gl.createProgram();
        gl.attachShader(this.program, vertexShader);
        gl.attachShader(this.program, fragmentShader);
        gl.linkProgram(this.program);
        
        if (!gl.getProgramParameter(this.program, gl.LINK_STATUS)) {
            console.error('Shader program failed to link:', gl.getProgramInfoLog(this.program));
            return;
        }
        
        gl.useProgram(this.program);
        
        // Get attribute and uniform locations
        this.locations = {
            position: gl.getAttribLocation(this.program, 'position'),
            color: gl.getAttribLocation(this.program, 'color'),
            modelViewMatrix: gl.getUniformLocation(this.program, 'uModelViewMatrix'),
            projectionMatrix: gl.getUniformLocation(this.program, 'uProjectionMatrix'),
            pointSize: gl.getUniformLocation(this.program, 'uPointSize')
        };
    }
    
    compileShader(gl, source, type) {
        const shader = gl.createShader(type);
        gl.shaderSource(shader, source);
        gl.compileShader(shader);
        
        if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
            console.error('Shader compilation error:', gl.getShaderInfoLog(shader));
            gl.deleteShader(shader);
            return null;
        }
        
        return shader;
    }
    
    setupControls() {
        const canvas = this.canvas;
        
        canvas.addEventListener('mousedown', (e) => {
            if (e.button === 0) {
                this.controls.isRotating = true;
            } else if (e.button === 2) {
                this.controls.isPanning = true;
            }
            this.controls.lastX = e.clientX;
            this.controls.lastY = e.clientY;
        });
        
        canvas.addEventListener('mousemove', (e) => {
            if (this.controls.isRotating) {
                const deltaX = e.clientX - this.controls.lastX;
                const deltaY = e.clientY - this.controls.lastY;
                
                this.controls.targetRotationY += deltaX * 0.005;
                this.controls.targetRotationX += deltaY * 0.005;
                
                // Clamp vertical rotation
                this.controls.targetRotationX = Math.max(-Math.PI / 2, Math.min(Math.PI / 2, this.controls.targetRotationX));
            }
            
            if (this.controls.isPanning) {
                const deltaX = e.clientX - this.controls.lastX;
                const deltaY = e.clientY - this.controls.lastY;
                
                this.controls.panX -= deltaX * 0.01;
                this.controls.panY += deltaY * 0.01;
            }
            
            this.controls.lastX = e.clientX;
            this.controls.lastY = e.clientY;
        });
        
        canvas.addEventListener('mouseup', () => {
            this.controls.isRotating = false;
            this.controls.isPanning = false;
        });
        
        canvas.addEventListener('wheel', (e) => {
            e.preventDefault();
            this.controls.zoom += e.deltaY * 0.01;
            this.controls.zoom = Math.max(1, Math.min(50, this.controls.zoom));
        });
        
        canvas.addEventListener('contextmenu', (e) => e.preventDefault());
    }
    
    resizeCanvas() {
        const displayWidth = this.canvas.clientWidth;
        const displayHeight = this.canvas.clientHeight;
        
        if (this.canvas.width !== displayWidth || this.canvas.height !== displayHeight) {
            this.canvas.width = displayWidth;
            this.canvas.height = displayHeight;
            
            if (this.gl) {
                this.gl.viewport(0, 0, displayWidth, displayHeight);
            }
        }
    }
    
    animate() {
        this.animationId = requestAnimationFrame(() => this.animate());
        
        this.render();
    }
    
    render() {
        if (!this.gl || !this.program || !this.buffers) {
            // Clear canvas with background color
            if (this.gl) {
                this.gl.clearColor(0.04, 0.04, 0.04, 1.0);
                this.gl.clear(this.gl.COLOR_BUFFER_BIT | this.gl.DEPTH_BUFFER_BIT);
            }
            return;
        }
        
        const gl = this.gl;
        
        // Resize if needed
        this.resizeCanvas();
        
        // Clear
        gl.clearColor(0.04, 0.04, 0.04, 1.0);
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
        gl.enable(gl.DEPTH_TEST);
        gl.enable(gl.BLEND);
        gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);
        
        // Update camera with smooth movement
        this.controls.rotationX += (this.controls.targetRotationX - this.controls.rotationX) * 0.1;
        this.controls.rotationY += (this.controls.targetRotationY - this.controls.rotationY) * 0.1;
        
        // Calculate matrices
        const projectionMatrix = this.createProjectionMatrix();
        const modelViewMatrix = this.createModelViewMatrix();
        
        // Set uniforms
        gl.uniformMatrix4fv(this.locations.projectionMatrix, false, projectionMatrix);
        gl.uniformMatrix4fv(this.locations.modelViewMatrix, false, modelViewMatrix);
        gl.uniform1f(this.locations.pointSize, this.pointSize);
        
        // Draw
        gl.drawArrays(gl.POINTS, 0, this.pointCount);
    }
    
    createProjectionMatrix() {
        const fov = this.camera.fov * Math.PI / 180;
        const aspect = this.canvas.width / this.canvas.height;
        const near = 0.1;
        const far = 1000.0;
        
        const f = 1.0 / Math.tan(fov / 2);
        const rangeInv = 1.0 / (near - far);
        
        return new Float32Array([
            f / aspect, 0, 0, 0,
            0, f, 0, 0,
            0, 0, (near + far) * rangeInv, -1,
            0, 0, near * far * rangeInv * 2, 0
        ]);
    }
    
    createModelViewMatrix() {
        const distance = this.controls.zoom;
        const rotX = this.controls.rotationX;
        const rotY = this.controls.rotationY;
        
        // Calculate camera position
        const x = distance * Math.sin(rotY) * Math.cos(rotX);
        const y = distance * Math.sin(rotX);
        const z = distance * Math.cos(rotY) * Math.cos(rotX);
        
        const eye = [x + this.controls.panX, y + this.controls.panY + 2, z];
        const center = [this.controls.panX, this.controls.panY, 0];
        const up = [0, 1, 0];
        
        return this.lookAt(eye, center, up);
    }
    
    lookAt(eye, center, up) {
        const z = this.normalize([
            eye[0] - center[0],
            eye[1] - center[1],
            eye[2] - center[2]
        ]);
        
        const x = this.normalize(this.cross(up, z));
        const y = this.cross(z, x);
        
        return new Float32Array([
            x[0], y[0], z[0], 0,
            x[1], y[1], z[1], 0,
            x[2], y[2], z[2], 0,
            -this.dot(x, eye), -this.dot(y, eye), -this.dot(z, eye), 1
        ]);
    }
    
    normalize(v) {
        const len = Math.sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
        return [v[0] / len, v[1] / len, v[2] / len];
    }
    
    cross(a, b) {
        return [
            a[1] * b[2] - a[2] * b[1],
            a[2] * b[0] - a[0] * b[2],
            a[0] * b[1] - a[1] * b[0]
        ];
    }
    
    dot(a, b) {
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    }
    
    setupEventListeners() {
        // File input
        const fileInput = document.getElementById('fileInput');
        const fileName = document.getElementById('fileName');
        
        fileInput.addEventListener('change', (e) => {
            const file = e.target.files[0];
            if (file) {
                fileName.textContent = file.name;
                this.loadPLYFile(file);
            }
        });
        
        // Drag and drop
        const dropZone = document.getElementById('dropZone');
        const canvas = this.canvas;
        
        ['dragenter', 'dragover', 'dragleave', 'drop'].forEach(eventName => {
            canvas.addEventListener(eventName, (e) => {
                e.preventDefault();
                e.stopPropagation();
            });
        });
        
        canvas.addEventListener('dragenter', () => {
            dropZone.classList.add('active');
        });
        
        canvas.addEventListener('dragleave', (e) => {
            if (e.target === canvas) {
                dropZone.classList.remove('active');
            }
        });
        
        canvas.addEventListener('drop', (e) => {
            dropZone.classList.remove('active');
            const file = e.dataTransfer.files[0];
            if (file && file.name.endsWith('.ply')) {
                fileName.textContent = file.name;
                this.loadPLYFile(file);
            }
        });
        
        dropZone.addEventListener('click', () => {
            fileInput.click();
        });
        
        // FOV slider
        const fovSlider = document.getElementById('fovSlider');
        const fovValue = document.getElementById('fovValue');
        
        fovSlider.addEventListener('input', (e) => {
            const fov = parseFloat(e.target.value);
            fovValue.textContent = fov + '°';
            this.camera.fov = fov;
        });
        
        // Point size slider
        const pointSizeSlider = document.getElementById('pointSizeSlider');
        const pointSizeValue = document.getElementById('pointSizeValue');
        
        pointSizeSlider.addEventListener('input', (e) => {
            const size = parseFloat(e.target.value);
            pointSizeValue.textContent = size;
            this.pointSize = size;
        });
        
        // Reset camera
        document.getElementById('resetCamera').addEventListener('click', () => {
            this.resetCamera();
        });
        
        // Window resize
        window.addEventListener('resize', () => {
            this.resizeCanvas();
        });
    }
    
    resetCamera() {
        this.controls.targetRotationX = 0;
        this.controls.targetRotationY = 0;
        this.controls.rotationX = 0;
        this.controls.rotationY = 0;
        this.controls.panX = 0;
        this.controls.panY = 0;
        this.controls.zoom = 5;
        this.camera.fov = 75;
        
        document.getElementById('fovSlider').value = 75;
        document.getElementById('fovValue').textContent = '75°';
    }
    
    showLoading(show) {
        const overlay = document.getElementById('loadingOverlay');
        if (show) {
            overlay.classList.add('active');
        } else {
            overlay.classList.remove('active');
        }
    }
    
    updateStatus(status) {
        document.getElementById('statusText').textContent = status;
    }
    
    updatePointCount(count) {
        document.getElementById('pointCount').textContent = count.toLocaleString();
    }
    
    async loadPLYFile(file) {
        this.showLoading(true);
        this.updateStatus('Loading...');
        document.getElementById('dropZone').classList.remove('active');
        
        try {
            const arrayBuffer = await file.arrayBuffer();
            const data = await this.parsePLY(arrayBuffer);
            
            // Create buffers for the point cloud
            this.createPointCloud(data);
            
            this.updateStatus('Loaded');
            this.updatePointCount(data.vertexCount);
            this.showLoading(false);
            
        } catch (error) {
            console.error('Error loading PLY file:', error);
            this.updateStatus('Error: ' + error.message);
            this.showLoading(false);
            alert('Error loading PLY file: ' + error.message);
        }
    }
    
    async parsePLY(arrayBuffer) {
        const decoder = new TextDecoder('utf-8');
        const headerEnd = this.findHeaderEnd(arrayBuffer);
        
        if (headerEnd === -1) {
            throw new Error('Invalid PLY file: header not found');
        }
        
        const headerText = decoder.decode(arrayBuffer.slice(0, headerEnd));
        const lines = headerText.split('\n');
        
        let vertexCount = 0;
        let properties = [];
        let format = 'ascii';
        
        for (let i = 0; i < lines.length; i++) {
            const line = lines[i].trim();
            
            if (line.startsWith('format')) {
                format = line.includes('binary') ? 'binary' : 'ascii';
            } else if (line.startsWith('element vertex')) {
                vertexCount = parseInt(line.split(' ')[2]);
            } else if (line.startsWith('property')) {
                const parts = line.split(' ');
                properties.push({
                    type: parts[1],
                    name: parts[2]
                });
            }
        }
        
        // Parse vertex data
        const dataStart = headerEnd;
        const positions = new Float32Array(vertexCount * 3);
        const colors = new Float32Array(vertexCount * 3);
        
        if (format === 'binary') {
            this.parseBinaryPLY(arrayBuffer, dataStart, vertexCount, properties, positions, colors);
        } else {
            this.parseAsciiPLY(arrayBuffer, dataStart, vertexCount, properties, positions, colors);
        }
        
        return {
            vertexCount,
            positions,
            colors
        };
    }
    
    findHeaderEnd(arrayBuffer) {
        const view = new Uint8Array(arrayBuffer);
        const endHeaderBytes = new TextEncoder().encode('end_header\n');
        
        for (let i = 0; i < view.length - endHeaderBytes.length; i++) {
            let match = true;
            for (let j = 0; j < endHeaderBytes.length; j++) {
                if (view[i + j] !== endHeaderBytes[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                return i + endHeaderBytes.length;
            }
        }
        return -1;
    }
    
    parseBinaryPLY(arrayBuffer, dataStart, vertexCount, properties, positions, colors) {
        const view = new DataView(arrayBuffer, dataStart);
        let offset = 0;
        
        // Calculate bytes per vertex
        let bytesPerVertex = 0;
        for (const prop of properties) {
            if (prop.type === 'float') bytesPerVertex += 4;
            else if (prop.type === 'uchar') bytesPerVertex += 1;
            else if (prop.type === 'uint') bytesPerVertex += 4;
            else if (prop.type === 'int') bytesPerVertex += 4;
            else if (prop.type === 'double') bytesPerVertex += 8;
        }
        
        for (let i = 0; i < vertexCount; i++) {
            let x = 0, y = 0, z = 0;
            let r = 255, g = 255, b = 255;
            let propOffset = 0;
            
            for (const prop of properties) {
                if (prop.type === 'float') {
                    const value = view.getFloat32(offset + propOffset, true);
                    if (prop.name === 'x') x = value;
                    else if (prop.name === 'y') y = value;
                    else if (prop.name === 'z') z = value;
                    propOffset += 4;
                } else if (prop.type === 'uchar') {
                    const value = view.getUint8(offset + propOffset);
                    if (prop.name === 'red') r = value;
                    else if (prop.name === 'green') g = value;
                    else if (prop.name === 'blue') b = value;
                    propOffset += 1;
                } else if (prop.type === 'uint') {
                    propOffset += 4;
                } else if (prop.type === 'int') {
                    propOffset += 4;
                } else if (prop.type === 'double') {
                    propOffset += 8;
                }
            }
            
            positions[i * 3] = x;
            positions[i * 3 + 1] = y;
            positions[i * 3 + 2] = z;
            
            colors[i * 3] = r / 255;
            colors[i * 3 + 1] = g / 255;
            colors[i * 3 + 2] = b / 255;
            
            offset += bytesPerVertex;
        }
    }
    
    parseAsciiPLY(arrayBuffer, dataStart, vertexCount, properties, positions, colors) {
        const decoder = new TextDecoder('utf-8');
        const dataText = decoder.decode(arrayBuffer.slice(dataStart));
        const lines = dataText.split('\n');
        
        for (let i = 0; i < vertexCount && i < lines.length; i++) {
            const values = lines[i].trim().split(/\s+/).map(v => parseFloat(v));
            
            if (values.length >= 3) {
                positions[i * 3] = values[0];
                positions[i * 3 + 1] = values[1];
                positions[i * 3 + 2] = values[2];
                
                // Colors (if available)
                if (values.length >= 6) {
                    colors[i * 3] = values[3] / 255;
                    colors[i * 3 + 1] = values[4] / 255;
                    colors[i * 3 + 2] = values[5] / 255;
                } else {
                    colors[i * 3] = 1;
                    colors[i * 3 + 1] = 1;
                    colors[i * 3 + 2] = 1;
                }
            }
        }
    }
    
    createPointCloud(data) {
        const gl = this.gl;
        
        // Clean up old buffers
        if (this.buffers) {
            gl.deleteBuffer(this.buffers.position);
            gl.deleteBuffer(this.buffers.color);
        }
        
        // Center the point cloud
        let minX = Infinity, minY = Infinity, minZ = Infinity;
        let maxX = -Infinity, maxY = -Infinity, maxZ = -Infinity;
        
        for (let i = 0; i < data.vertexCount; i++) {
            const x = data.positions[i * 3];
            const y = data.positions[i * 3 + 1];
            const z = data.positions[i * 3 + 2];
            
            minX = Math.min(minX, x);
            minY = Math.min(minY, y);
            minZ = Math.min(minZ, z);
            maxX = Math.max(maxX, x);
            maxY = Math.max(maxY, y);
            maxZ = Math.max(maxZ, z);
        }
        
        const centerX = (minX + maxX) / 2;
        const centerY = (minY + maxY) / 2;
        const centerZ = (minZ + maxZ) / 2;
        
        // Center positions
        for (let i = 0; i < data.vertexCount; i++) {
            data.positions[i * 3] -= centerX;
            data.positions[i * 3 + 1] -= centerY;
            data.positions[i * 3 + 2] -= centerZ;
        }
        
        // Calculate bounding sphere radius
        const radius = Math.sqrt(
            Math.pow(maxX - minX, 2) +
            Math.pow(maxY - minY, 2) +
            Math.pow(maxZ - minZ, 2)
        ) / 2;
        
        // Adjust camera zoom based on size
        this.controls.zoom = radius * 2;
        
        // Create position buffer
        const positionBuffer = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
        gl.bufferData(gl.ARRAY_BUFFER, data.positions, gl.STATIC_DRAW);
        gl.enableVertexAttribArray(this.locations.position);
        gl.vertexAttribPointer(this.locations.position, 3, gl.FLOAT, false, 0, 0);
        
        // Create color buffer
        const colorBuffer = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, colorBuffer);
        gl.bufferData(gl.ARRAY_BUFFER, data.colors, gl.STATIC_DRAW);
        gl.enableVertexAttribArray(this.locations.color);
        gl.vertexAttribPointer(this.locations.color, 3, gl.FLOAT, false, 0, 0);
        
        this.buffers = {
            position: positionBuffer,
            color: colorBuffer
        };
        
        this.pointCount = data.vertexCount;
    }
}

// Initialize the application when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    const viewer = new GaussianSplattingViewer();
    
    // Make viewer globally accessible for debugging
    window.viewer = viewer;
});
