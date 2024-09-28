set -eu

# apt update
# apt install clang-18 clang-format-18 clang-tidy-18 libclang-18-dev llvm-18-dev ninja-build clang cmake
cmake -B build -G Ninja -S .
cmake --build build/
clang-tidy-18 --checks=coveo-awesomeprefixcheck --load build/libAwesomePrefixCheck.so test.cpp --
