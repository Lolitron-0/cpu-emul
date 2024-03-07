#!/usr/bin/env bash

set -e

mkdir -p build

nix flake update
#nix develop


sudo cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug -G Ninja -B build -S . 
sudo cmake --build build --parallel 5 

cp build/compile_commands.json ./compile_commands.json 
build/$1 ${@:2}
