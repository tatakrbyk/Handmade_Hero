@echo off

mkdir ..\..\build
pushd ..\..\build
cl -O2 -FC -Zi ..\handmade\code\win32_handmade.cpp /link user32.lib Gdi32.lib
popd
