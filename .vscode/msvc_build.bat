@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul

:: 1. Compile and link against Bloomberg headers and blpapi3_64.lib
cl.exe /Zi /EHsc /nologo /I"C:\Users\team40\Downloads\blpapi_cpp_3.26.5.1-windows\blpapi_cpp_3.26.5.1\include" /Fe:"%~dp1%~n1.exe" "%~1" "C:\Users\team40\Downloads\blpapi_cpp_3.26.5.1-windows\blpapi_cpp_3.26.5.1\lib\blpapi3_64.lib"

:: 2. Automatically copy blpapi3_64.dll into the project folder so your .exe can run!
if not exist "%~dp1blpapi3_64.dll" (
    copy /y "C:\Users\team40\Downloads\blpapi_cpp_3.26.5.1-windows\blpapi_cpp_3.26.5.1\bin\blpapi3_64.dll" "%~dp1" >nul
)