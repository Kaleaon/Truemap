package com.truemap.app

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import androidx.lifecycle.viewmodel.compose.viewModel
import com.truemap.app.ui.theme.TruemapTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            TruemapTheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    MainScreen()
                }
            }
        }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun MainScreen(viewModel: ReconstructionViewModel = viewModel()) {
    var showUrlDialog by remember { mutableStateOf(false) }
    var urlInput by remember { mutableStateOf("") }
    
    Scaffold(
        topBar = {
            TopAppBar(
                title = { Text("Truemap") },
                colors = TopAppBarDefaults.topAppBarColors(
                    containerColor = MaterialTheme.colorScheme.primaryContainer,
                    titleContentColor = MaterialTheme.colorScheme.onPrimaryContainer
                )
            )
        }
    ) { paddingValues ->
        Column(
            modifier = Modifier
                .fillMaxSize()
                .padding(paddingValues)
                .padding(16.dp),
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            // Header
            Text(
                text = "3D Reconstruction from YouTube Videos",
                style = MaterialTheme.typography.titleMedium,
                textAlign = TextAlign.Center,
                modifier = Modifier.padding(bottom = 16.dp)
            )
            
            Divider(modifier = Modifier.padding(vertical = 8.dp))
            
            // Video list or empty state
            Box(
                modifier = Modifier
                    .weight(1f)
                    .fillMaxWidth()
            ) {
                if (viewModel.videoUrls.isEmpty()) {
                    Column(
                        modifier = Modifier
                            .fillMaxSize()
                            .padding(32.dp),
                        horizontalAlignment = Alignment.CenterHorizontally,
                        verticalArrangement = Arrangement.Center
                    ) {
                        Icon(
                            imageVector = Icons.Default.VideoLibrary,
                            contentDescription = null,
                            modifier = Modifier.size(64.dp),
                            tint = MaterialTheme.colorScheme.outline
                        )
                        Spacer(modifier = Modifier.height(16.dp))
                        Text(
                            text = "No videos added yet",
                            style = MaterialTheme.typography.bodyLarge,
                            color = MaterialTheme.colorScheme.outline
                        )
                        Text(
                            text = "Add YouTube video URLs to begin",
                            style = MaterialTheme.typography.bodySmall,
                            color = MaterialTheme.colorScheme.outline
                        )
                    }
                } else {
                    LazyColumn(
                        modifier = Modifier.fillMaxSize(),
                        verticalArrangement = Arrangement.spacedBy(8.dp)
                    ) {
                        items(viewModel.videoUrls) { url ->
                            Card(
                                modifier = Modifier.fillMaxWidth()
                            ) {
                                Row(
                                    modifier = Modifier
                                        .fillMaxWidth()
                                        .padding(12.dp),
                                    verticalAlignment = Alignment.CenterVertically
                                ) {
                                    Icon(
                                        imageVector = Icons.Default.VideoFile,
                                        contentDescription = null,
                                        tint = MaterialTheme.colorScheme.primary
                                    )
                                    Spacer(modifier = Modifier.width(12.dp))
                                    Text(
                                        text = url,
                                        modifier = Modifier.weight(1f),
                                        maxLines = 1,
                                        style = MaterialTheme.typography.bodyMedium
                                    )
                                    IconButton(onClick = { viewModel.removeUrl(url) }) {
                                        Icon(
                                            imageVector = Icons.Default.Delete,
                                            contentDescription = "Remove",
                                            tint = MaterialTheme.colorScheme.error
                                        )
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            Divider(modifier = Modifier.padding(vertical = 8.dp))
            
            // Progress section
            if (viewModel.isProcessing) {
                Column(
                    modifier = Modifier
                        .fillMaxWidth()
                        .padding(vertical = 16.dp)
                ) {
                    LinearProgressIndicator(
                        progress = viewModel.progress.toFloat(),
                        modifier = Modifier.fillMaxWidth()
                    )
                    Spacer(modifier = Modifier.height(8.dp))
                    Text(
                        text = viewModel.statusMessage,
                        style = MaterialTheme.typography.bodySmall,
                        color = MaterialTheme.colorScheme.outline,
                        modifier = Modifier.fillMaxWidth(),
                        textAlign = TextAlign.Center
                    )
                    Spacer(modifier = Modifier.height(8.dp))
                    OutlinedButton(
                        onClick = { viewModel.cancelReconstruction() },
                        modifier = Modifier.fillMaxWidth()
                    ) {
                        Icon(Icons.Default.Cancel, contentDescription = null)
                        Spacer(modifier = Modifier.width(8.dp))
                        Text("Cancel")
                    }
                }
            } else {
                // Action buttons
                Column(
                    modifier = Modifier.fillMaxWidth(),
                    verticalArrangement = Arrangement.spacedBy(8.dp)
                ) {
                    Button(
                        onClick = { showUrlDialog = true },
                        modifier = Modifier.fillMaxWidth(),
                        enabled = !viewModel.isProcessing
                    ) {
                        Icon(Icons.Default.Add, contentDescription = null)
                        Spacer(modifier = Modifier.width(8.dp))
                        Text("Add Video URL")
                    }
                    
                    Button(
                        onClick = { viewModel.startReconstruction() },
                        modifier = Modifier.fillMaxWidth(),
                        enabled = viewModel.videoUrls.isNotEmpty() && !viewModel.isProcessing,
                        colors = ButtonDefaults.buttonColors(
                            containerColor = MaterialTheme.colorScheme.secondary
                        )
                    ) {
                        Icon(Icons.Default.PlayArrow, contentDescription = null)
                        Spacer(modifier = Modifier.width(8.dp))
                        Text("Start Reconstruction")
                    }
                    
                    if (viewModel.hasCompletedReconstruction) {
                        Button(
                            onClick = { viewModel.exportMesh() },
                            modifier = Modifier.fillMaxWidth(),
                            colors = ButtonDefaults.buttonColors(
                                containerColor = MaterialTheme.colorScheme.tertiary
                            )
                        ) {
                            Icon(Icons.Default.SaveAlt, contentDescription = null)
                            Spacer(modifier = Modifier.width(8.dp))
                            Text("Export Mesh")
                        }
                    }
                }
            }
        }
    }
    
    // URL Input Dialog
    if (showUrlDialog) {
        AlertDialog(
            onDismissRequest = { showUrlDialog = false },
            title = { Text("Add Video URL") },
            text = {
                OutlinedTextField(
                    value = urlInput,
                    onValueChange = { urlInput = it },
                    label = { Text("YouTube URL") },
                    placeholder = { Text("https://youtube.com/watch?v=...") },
                    modifier = Modifier.fillMaxWidth(),
                    singleLine = true
                )
            },
            confirmButton = {
                TextButton(
                    onClick = {
                        if (urlInput.isNotEmpty()) {
                            viewModel.addVideoUrl(urlInput)
                            urlInput = ""
                            showUrlDialog = false
                        }
                    },
                    enabled = urlInput.isNotEmpty()
                ) {
                    Text("Add")
                }
            },
            dismissButton = {
                TextButton(onClick = { showUrlDialog = false }) {
                    Text("Cancel")
                }
            }
        )
    }
    
    // Error dialog
    viewModel.errorMessage?.let { error ->
        AlertDialog(
            onDismissRequest = { viewModel.clearError() },
            title = { Text("Error") },
            text = { Text(error) },
            confirmButton = {
                TextButton(onClick = { viewModel.clearError() }) {
                    Text("OK")
                }
            }
        )
    }
}
