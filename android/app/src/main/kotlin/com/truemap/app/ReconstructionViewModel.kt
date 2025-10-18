package com.truemap.app

import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.setValue
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.launch

class ReconstructionViewModel : ViewModel() {
    var videoUrls by mutableStateOf<List<String>>(emptyList())
        private set
    
    var isProcessing by mutableStateOf(false)
        private set
    
    var progress by mutableStateOf(0.0)
        private set
    
    var statusMessage by mutableStateOf("")
        private set
    
    var hasCompletedReconstruction by mutableStateOf(false)
        private set
    
    var errorMessage by mutableStateOf<String?>(null)
        private set
    
    private val reconstructionService = ReconstructionService()
    
    init {
        // Observe reconstruction service updates
        reconstructionService.setProgressCallback { status, prog, message ->
            viewModelScope.launch {
                progress = prog
                statusMessage = message
                isProcessing = (status != ProcessingStatus.IDLE && 
                               status != ProcessingStatus.COMPLETED && 
                               status != ProcessingStatus.ERROR)
                hasCompletedReconstruction = (status == ProcessingStatus.COMPLETED)
            }
        }
    }
    
    fun addVideoUrl(url: String) {
        if (url.isNotEmpty()) {
            videoUrls = videoUrls + url
        }
    }
    
    fun removeUrl(url: String) {
        videoUrls = videoUrls - url
    }
    
    fun startReconstruction() {
        if (videoUrls.isEmpty()) {
            errorMessage = "Please add at least one video URL"
            return
        }
        
        isProcessing = true
        hasCompletedReconstruction = false
        progress = 0.0
        statusMessage = "Starting reconstruction..."
        
        viewModelScope.launch {
            try {
                reconstructionService.startReconstruction(videoUrls)
                statusMessage = "Reconstruction completed successfully!"
            } catch (e: Exception) {
                errorMessage = "Reconstruction failed: ${e.message}"
                isProcessing = false
            }
        }
    }
    
    fun cancelReconstruction() {
        reconstructionService.cancelReconstruction()
        isProcessing = false
        statusMessage = "Cancelled"
    }
    
    fun exportMesh() {
        viewModelScope.launch {
            try {
                val path = reconstructionService.exportMesh()
                statusMessage = "Mesh exported to: $path"
            } catch (e: Exception) {
                errorMessage = "Export failed: ${e.message}"
            }
        }
    }
    
    fun clearError() {
        errorMessage = null
    }
}
