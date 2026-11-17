#!/usr/bin/env bash
set -e

# Default values
TARGET="raspberry_pico_w"
JOBS=$(nproc)
CLEAN=0

# Colors for logging
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

print_info() {
    echo -e "${GREEN}[INFO] $1${NC}"
}

print_warn() {
    echo -e "${YELLOW}[WARN] $1${NC}"
}

print_error() {
    echo -e "${RED}[ERROR] $1${NC}"
}

# Argument parsing
while [[ "$#" -gt 0 ]]; do
    case $1 in
        --clean) CLEAN=1 ;;
        --jobs) JOBS="$2"; shift ;;
        --target) TARGET="$2"; shift ;;
        -h|--help)
            echo "Usage: $0 [options]"
            echo "Options:"
            echo "  --clean           Clean build directory before building"
            echo "  --jobs N          Number of compilation threads (default: $(nproc))"
            echo "  --target NAME     Build target: raspberry_pico_w or linux (default: raspberry_pico_w)"
            echo "  -h, --help        Show this help message"
            exit 0
            ;;
        *) print_error "Unknown flag: $1"; exit 1 ;;
    esac
    shift
done

print_info "Checking required tools..."
MISSING_TOOLS=()

for tool in cmake make python3; do
    if ! command -v "$tool" &> /dev/null; then
        MISSING_TOOLS+=("$tool")
    fi
done

if [ "$TARGET" == "raspberry_pico_w" ]; then
    if ! command -v arm-none-eabi-gcc &> /dev/null; then
        MISSING_TOOLS+=("arm-none-eabi-gcc")
    fi
elif [ "$TARGET" == "linux" ]; then
    if ! dpkg -l | grep -q libsfml-dev 2>/dev/null && ! pkg-config --exists sfml-graphics 2>/dev/null; then
        print_warn "SFML library might not be installed (required for linux target)."
    fi
fi

if [ ${#MISSING_TOOLS[@]} -gt 0 ]; then
    print_error "Missing required tools: ${MISSING_TOOLS[*]}"
    print_error "Please install them before running the script again."
    exit 1
fi

print_info "Checking git submodules..."
if [ -d ".git" ]; then
    git submodule update --init --recursive
else
    print_warn "Directory is not a git repository or .git missing, skipping submodule initialization."
fi

if [ $CLEAN -eq 1 ] && [ -d "build" ]; then
    print_info "Cleaning build directory..."
    rm -rf build
fi

mkdir -p build
cd build

print_info "Configuring CMake for target: $TARGET..."
cmake -DCMAKE_BUILD_TYPE=Release -DTARGET="$TARGET" ..

print_info "Compiling project (jobs: $JOBS)..."
cmake --build . -- -j"$JOBS"

print_info "Compilation completed successfully!"

if [ "$TARGET" == "raspberry_pico_w" ]; then
    if [ -f "RpiPicoW_YALC.uf2" ]; then
        print_info "Generated UF2 file: build/RpiPicoW_YALC.uf2"
    elif [ -f "RpiPicoW_YALC.elf" ]; then
        print_info "Generated ELF file: build/RpiPicoW_YALC.elf"
    else
        print_warn "Build finished, but UF2/ELF output file was not found directly in build root."
    fi
fi
