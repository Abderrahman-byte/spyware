$cwd=$(pwd)

cmake -S . -B ./build && cd ./build && MSBuild ALL_BUILD.vcxproj ; cd $cwd