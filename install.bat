setx Path "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE;%Path%"
setx MAJ_ENGINE_PATH "%cd%"
setx QT_QPA_PLATFORM_PLUGIN_PATH "%cd%\Lib\Qt\plugins\platforms"
"C:\Program Files\7-Zip\7z.exe" x Lib.zip -o"%cd%"

if not exist "%cd%\bin\" mkdir %cd%\bin
xcopy "%cd%\Lib\Qt\dll\*" "%cd%\bin"
xcopy "%cd%\Lib\physX\dll\*" "%cd%\bin"

xcopy "%cd%\Lib\physX\dll\*" "%cd%\Game"
exit 0