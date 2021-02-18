@echo off

echo Choose compiler type:
echo  - 1. Any
echo  - 2. Clang
set /p COMPILER_TYPE=Answer: 

if "%COMPILER_TYPE%" neq "1" (
	echo Choose build type:
	echo  - 1. Debug
	echo  - 2. Release
	set /p BUILD_TYPE=Answer: 
	if "%BUILD_TYPE%"=="1" (
		set BUILD_TYPE=Debug
	) else (
		set BUILD_TYPE=Release
	)
)

echo Choose build type #2:
echo  - 1. Developer
echo  - 2. Production
set /p BUILD_DEVELOPER=Answer: 
if "%BUILD_DEVELOPER%"=="1" (
	set BUILD_DEVELOPER=True
) else (
	set BUILD_DEVELOPER=False
)

if "%COMPILER_TYPE%"=="1" (
	goto default
) else if "%COMPILER_TYPE%"=="2" (
	goto clang
)

:default

mkdir build
cd build
cmake .. -DBUILD_DEVELOPER=%BUILD_DEVELOPER%
goto end

:clang

mkdir build
cd build
cmake .. -DBUILD_DEVELOPER=%BUILD_DEVELOPER% -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
ninja
goto end

:end

pause
