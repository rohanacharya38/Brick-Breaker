if(!(Test-Path("..\bin")))
{
        New-Item -Itemtype Directory "..\bin"
}
$source_name = "..\code\start.c"
$executable_name = "Brick-Breaker.exe"
$lib_path ="..\lib\"
$include_path ="..\include\"

$compiler_flags = "/nologo","/EHsc","/Zi" ,"/FC"
# $linker_flags =
$libraries = "SDL2main.lib","SDL2.lib", "user32.lib", "shell32.lib", "gdi32.lib","SDL2_Mixer","SDL2_ttf"
Push-Location ..\bin
if(!(Test-Path("SDL2.dll")))
{
        Copy-Item "../lib/SDL2.dll"
}

# cl /MD $source_name /Fe$executable_name $compiler_flags /I$include_path /link /LIBPATH:$lib_path $libraries $linker_flags /SUBSYSTEM:console
# cl $source_name /Fe$executable_name $compiler_flag
# clang -g  -I..\include\  -L..\lib\ -l $libraries  $source_name -o $executable_name $compiler_flags
cl  $source_name /Fe$executable_name $compiler_flags /I$include_path /link /LIBPATH:$lib_path $libraries /SUBSYSTEM:CONSOLE
