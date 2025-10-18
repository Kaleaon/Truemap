import SwiftUI

struct ContentView: View {
    @StateObject private var viewModel = ReconstructionViewModel()
    @State private var showingURLInput = false
    @State private var newURL = ""
    
    var body: some View {
        NavigationView {
            VStack(spacing: 20) {
                // Header
                Text("Truemap")
                    .font(.largeTitle)
                    .fontWeight(.bold)
                    .padding(.top)
                
                Text("3D Reconstruction from YouTube Videos")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
                    .multilineTextAlignment(.center)
                    .padding(.horizontal)
                
                Divider()
                    .padding(.vertical)
                
                // Video URLs List
                if viewModel.videoURLs.isEmpty {
                    VStack(spacing: 10) {
                        Image(systemName: "video.slash")
                            .font(.system(size: 50))
                            .foregroundColor(.gray)
                        Text("No videos added yet")
                            .foregroundColor(.secondary)
                        Text("Add YouTube video URLs to begin")
                            .font(.caption)
                            .foregroundColor(.secondary)
                    }
                    .padding()
                } else {
                    List {
                        ForEach(viewModel.videoURLs, id: \.self) { url in
                            HStack {
                                Image(systemName: "video.fill")
                                    .foregroundColor(.blue)
                                Text(url)
                                    .lineLimit(1)
                                    .truncationMode(.middle)
                            }
                        }
                        .onDelete(perform: viewModel.deleteURL)
                    }
                }
                
                Spacer()
                
                // Status and Progress
                if viewModel.isProcessing {
                    VStack(spacing: 10) {
                        ProgressView(value: viewModel.progress)
                            .progressViewStyle(.linear)
                            .padding(.horizontal)
                        
                        Text(viewModel.statusMessage)
                            .font(.caption)
                            .foregroundColor(.secondary)
                        
                        Button("Cancel") {
                            viewModel.cancelReconstruction()
                        }
                        .buttonStyle(.bordered)
                        .tint(.red)
                    }
                    .padding()
                } else {
                    // Action Buttons
                    VStack(spacing: 15) {
                        Button(action: { showingURLInput = true }) {
                            Label("Add Video URL", systemImage: "plus.circle.fill")
                                .frame(maxWidth: .infinity)
                                .padding()
                                .background(Color.blue)
                                .foregroundColor(.white)
                                .cornerRadius(10)
                        }
                        .disabled(viewModel.isProcessing)
                        
                        Button(action: { viewModel.startReconstruction() }) {
                            Label("Start Reconstruction", systemImage: "play.fill")
                                .frame(maxWidth: .infinity)
                                .padding()
                                .background(viewModel.videoURLs.isEmpty ? Color.gray : Color.green)
                                .foregroundColor(.white)
                                .cornerRadius(10)
                        }
                        .disabled(viewModel.videoURLs.isEmpty || viewModel.isProcessing)
                        
                        if viewModel.hasCompletedReconstruction {
                            Button(action: { viewModel.exportMesh() }) {
                                Label("Export Mesh", systemImage: "square.and.arrow.up")
                                    .frame(maxWidth: .infinity)
                                    .padding()
                                    .background(Color.purple)
                                    .foregroundColor(.white)
                                    .cornerRadius(10)
                            }
                        }
                    }
                    .padding(.horizontal)
                }
                
                Spacer()
            }
            .navigationBarHidden(true)
            .sheet(isPresented: $showingURLInput) {
                URLInputView(url: $newURL, onAdd: {
                    if !newURL.isEmpty {
                        viewModel.addVideoURL(newURL)
                        newURL = ""
                        showingURLInput = false
                    }
                })
            }
            .alert("Error", isPresented: .constant(viewModel.errorMessage != nil), presenting: viewModel.errorMessage) { _ in
                Button("OK") { viewModel.errorMessage = nil }
            } message: { message in
                Text(message)
            }
        }
    }
}

struct URLInputView: View {
    @Binding var url: String
    var onAdd: () -> Void
    @Environment(\.dismiss) var dismiss
    
    var body: some View {
        NavigationView {
            Form {
                Section(header: Text("YouTube Video URL")) {
                    TextField("https://youtube.com/watch?v=...", text: $url)
                        .textInputAutocapitalization(.never)
                        .keyboardType(.URL)
                }
                
                Section {
                    Button("Add Video") {
                        onAdd()
                    }
                    .disabled(url.isEmpty)
                }
            }
            .navigationTitle("Add Video URL")
            .navigationBarTitleDisplayMode(.inline)
            .toolbar {
                ToolbarItem(placement: .cancellationAction) {
                    Button("Cancel") {
                        dismiss()
                    }
                }
            }
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
