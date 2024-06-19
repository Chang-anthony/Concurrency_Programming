@REM get current bat file path
cd ..
cd ..
cd .\build

echo default :%cd%
echo Executable files in the current directory:
dir /b *.exe

set desiredExecutable=Test.exe

if exist %desiredExecutable% (
    echo Find the executable file "%desiredExecutable%".
    call %desiredExecutable%
) else (
    echo The desired executable "%desiredExecutable%" does not exist in the current directory.
)


IF %ERRORLEVEL% EQU 0 (
    ECHO Tests passed successfully.
) ELSE (
    ECHO Tests failed.
)

pause

@REM %~dp0 當前執行檔bat的位置
@REM echo System batfilepath: %~dp0 
@REM call %~dp0Run\DesignPattern_Test.exe