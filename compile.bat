set validator_dir=%cd%
cd data\shaders
for %%i in (*) do (
	%validator_dir%\glslangValidator.exe -o %%i.spv -V %%i
)

if not exist "%cd%\compiled\" mkdir %cd%\compiled
move %cd%\*.spv %cd%\compiled\
exit 0