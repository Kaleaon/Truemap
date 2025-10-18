cmake_minimum_required(VERSION 3.20)
project(TruemapCore VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform-specific configurations
if(PLATFORM STREQUAL "iOS")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "15.0")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
elseif(PLATFORM STREQUAL "Android")
    set(CMAKE_ANDROID_STL_TYPE c++_shared)
endif()

# Compiler options
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-Wall -Wextra -Wpedantic -O3)
elseif(MSVC)
    add_compile_options(/W4 /O2)
endif()

# Dependencies
# Note: These would typically be added as submodules or external projects
# For now, we'll define the interface

# OpenCV (for feature detection)
find_package(OpenCV QUIET)
if(OpenCV_FOUND)
    message(STATUS "Found OpenCV: ${OpenCV_VERSION}")
else()
    message(WARNING "OpenCV not found. Feature detection will use placeholder implementation.")
endif()

# Eigen (for linear algebra)
find_package(Eigen3 QUIET)
if(Eigen3_FOUND)
    message(STATUS "Found Eigen3: ${EIGEN3_VERSION_STRING}")
else()
    message(WARNING "Eigen3 not found. Using bundled minimal implementation.")
endif()

# Source files
set(TRUEMAP_SOURCES
    src/core/truemap_core.cpp
    src/core/video_processor.cpp
    src/sfm/feature_extractor.cpp
    src/sfm/feature_matcher.cpp
    src/sfm/sfm_pipeline.cpp
    src/alignment/ransac_aligner.cpp
    src/alignment/icp_aligner.cpp
    src/optimization/bundle_adjuster.cpp
    src/mvs/mvs_processor.cpp
    src/mesh/poisson_reconstructor.cpp
    src/mesh/mesh_exporter.cpp
    src/utils/logger.cpp
    src/utils/math_utils.cpp
)

# Header files
set(TRUEMAP_HEADERS
    include/truemap/truemap_core.h
    include/truemap/video_processor.h
    include/truemap/feature_extractor.h
    include/truemap/feature_matcher.h
    include/truemap/sfm_pipeline.h
    include/truemap/ransac_aligner.h
    include/truemap/icp_aligner.h
    include/truemap/bundle_adjuster.h
    include/truemap/mvs_processor.h
    include/truemap/poisson_reconstructor.h
    include/truemap/mesh_exporter.h
    include/truemap/types.h
    include/truemap/logger.h
    include/truemap/math_utils.h
)

# Create library
add_library(truemap_core STATIC ${TRUEMAP_SOURCES} ${TRUEMAP_HEADERS})

# Include directories
target_include_directories(truemap_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link dependencies
if(OpenCV_FOUND)
    target_link_libraries(truemap_core PRIVATE ${OpenCV_LIBS})
    target_compile_definitions(truemap_core PRIVATE TRUEMAP_USE_OPENCV)
endif()

if(Eigen3_FOUND)
    target_link_libraries(truemap_core PRIVATE Eigen3::Eigen)
    target_compile_definitions(truemap_core PRIVATE TRUEMAP_USE_EIGEN)
endif()

# Platform-specific linking
if(PLATFORM STREQUAL "iOS")
    target_link_libraries(truemap_core PRIVATE "-framework Foundation" "-framework CoreGraphics")
elseif(PLATFORM STREQUAL "Android")
    target_link_libraries(truemap_core PRIVATE log android)
endif()

# Installation
install(TARGETS truemap_core
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${TRUEMAP_HEADERS} DESTINATION include/truemap)

# Testing (optional)
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
