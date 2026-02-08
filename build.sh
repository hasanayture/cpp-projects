#!/usr/bin/env bash
set -e

# Default build type
BUILD_TYPE="Debug"
TARGET="all"

print_usage() {
    echo "Usage: $0 [OPTIONS]"
    echo
    echo "Options:"
    echo "  --help                Show this help message"
    echo "  --type <Debug|Release>  Set build type (default: Debug)"
    echo "  --target <name>       Build a specific target (default: all)"
    echo
    echo "Examples:"
    echo "  $0                     Build all in Debug mode"
    echo "  $0 --type Release       Build all in Release mode"
    echo "  $0 --target relaxed_vs_seq_cst  Build only one experiment"
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case "$1" in
        --help)
            print_usage
            exit 0
            ;;
        --type)
            shift
            if [[ "$1" != "Debug" && "$1" != "Release" ]]; then
                echo "Error: Build type must be Debug or Release"
                exit 1
            fi
            BUILD_TYPE="$1"
            ;;
        --target)
            shift
            TARGET="$1"
            ;;
        *)
            echo "Unknown option: $1"
            print_usage
            exit 1
            ;;
    esac
    shift
done

# Create build directory if it doesn't exist
BUILD_DIR="build"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Generate build system
cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" ..

# Build
if [[ "$TARGET" == "all" ]]; then
    cmake --build . -- -j$(nproc)
else
    cmake --build . --target "$TARGET" -- -j$(nproc)
fi

echo "Build finished: BuildType=$BUILD_TYPE, Target=$TARGET"

