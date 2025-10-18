import Foundation
import Combine

/// ViewModel for managing 3D reconstruction process
class ReconstructionViewModel: ObservableObject {
    @Published var videoURLs: [String] = []
    @Published var isProcessing = false
    @Published var progress: Double = 0.0
    @Published var statusMessage = ""
    @Published var hasCompletedReconstruction = false
    @Published var errorMessage: String?
    
    private let reconstructionService = ReconstructionService()
    private var cancellables = Set<AnyCancellable>()
    
    init() {
        // Subscribe to reconstruction service updates
        reconstructionService.progressPublisher
            .receive(on: DispatchQueue.main)
            .sink { [weak self] progressUpdate in
                self?.progress = progressUpdate.progress
                self?.statusMessage = progressUpdate.message
            }
            .store(in: &cancellables)
        
        reconstructionService.statusPublisher
            .receive(on: DispatchQueue.main)
            .sink { [weak self] status in
                self?.isProcessing = (status != .idle && status != .completed && status != .error)
                self?.hasCompletedReconstruction = (status == .completed)
            }
            .store(in: &cancellables)
    }
    
    func addVideoURL(_ url: String) {
        guard !url.isEmpty else { return }
        videoURLs.append(url)
    }
    
    func deleteURL(at offsets: IndexSet) {
        videoURLs.remove(atOffsets: offsets)
    }
    
    func startReconstruction() {
        guard !videoURLs.isEmpty else {
            errorMessage = "Please add at least one video URL"
            return
        }
        
        isProcessing = true
        hasCompletedReconstruction = false
        progress = 0.0
        statusMessage = "Starting reconstruction..."
        
        reconstructionService.startReconstruction(urls: videoURLs) { [weak self] result in
            DispatchQueue.main.async {
                switch result {
                case .success:
                    self?.statusMessage = "Reconstruction completed successfully!"
                case .failure(let error):
                    self?.errorMessage = error.localizedDescription
                    self?.isProcessing = false
                }
            }
        }
    }
    
    func cancelReconstruction() {
        reconstructionService.cancelReconstruction()
        isProcessing = false
        statusMessage = "Cancelled"
    }
    
    func exportMesh() {
        reconstructionService.exportMesh { [weak self] result in
            DispatchQueue.main.async {
                switch result {
                case .success(let url):
                    self?.statusMessage = "Mesh exported to: \(url.lastPathComponent)"
                    // In a real app, would present share sheet here
                case .failure(let error):
                    self?.errorMessage = error.localizedDescription
                }
            }
        }
    }
}
