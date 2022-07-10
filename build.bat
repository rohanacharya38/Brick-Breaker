@echo off
md bin
pushd bin
copy  ..\lib\*.dll
popd
setlocal
set source_name="code\start.c"
set executable_name=bin\Brick-Breaker.exe
set lib_path=lib\
set include_path=include\
set compiler_flags=/nologo /EHsc /Zi /FC
@REM set libraries=SDL2.lib SDL2main.lib user32.lib shell32.lib gdi32.lib
set libraries=SDL2.lib SDL2main.lib user32.lib shell32.lib gdi32.lib SDL2_ttf.lib SDL2_image.lib SDL2test.lib SDL2_mixer.lib
cl   %source_name% /Fe%executable_name% %compiler_flags% /I%include_path% /link /LIBPATH:%lib_path% %libraries% /SUBSYSTEM:windows