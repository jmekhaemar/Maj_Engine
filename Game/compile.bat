cd data\shaders
for %%i in (*) do (
	glslangValidator.exe -o %%i.spv -V %%i
)

if not exist "%cd%\compiled\" mkdir %cd%\compiled
move %cd%\*.spv %cd%\compiled\
exit 0