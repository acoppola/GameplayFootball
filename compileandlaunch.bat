cd build
rem cmake .. -DCMAKE_GENERATOR_PLATFORM=Win32 -DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE
cmake --build . --parallel --config Debug
cd Debug
rem cp *.dll ../../
Copy-Item -Filter *.dll -Destination ../../ –Recurse
rem cp *.exe ../../
Copy-Item -Filter *.exe -Destination ../../ –Recurse
cd ..
cd ..