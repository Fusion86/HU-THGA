#!/bin/bash

cd "$(dirname "$0")" # Switch to directory containing this file

# Default options
build_type="Debug"

# Parse args
args=("$@")
for x in "${args[@]}"; do
  if [ "$x" == "clean" ]; then
    clean=1
  elif [ "$x" == "release" ]; then
    build_type="Release"
  elif [ "$x" == "help" ]; then
    echo "USAGE: ./build [arguments...]"
    echo ""
    echo "ARGUMENTS:"
    echo "debug     - create debug build (default)"
    echo "release   - create release build"
    echo "clean     - create a clean build"
    echo "help      - print this output"
    exit
  fi
done

if [ -n "$clean" ]; then
  echo "Cleaning working directory"
  rm -rf build
fi

echo "Build type: $build_type"

mkdir -p build                          # Create build dir
cd build                                # Enter build dir
cmake -DCMAKE_BUILD_TYPE=$build_type .. # Run CMake
make                                    # Make
