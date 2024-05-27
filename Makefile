profile:
	conan profile detect

install:
	conan install . --output-folder=cmake-build-debug --build=missing -s build_type=Debug

cmake:
	cd cmake-build-debug && cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake

# -DCMAKE_TOOLCHAIN_FILE=./cmake-build-debug/conan_toolchain.cmake