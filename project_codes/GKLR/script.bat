@echo off
REM Set the compiled C++ program name
set PROGRAM=gklr.exe

REM Ensure the program exists
if not exist %PROGRAM% (
    echo Error: %PROGRAM% not found. Please compile your C++ program first.
    pause
    exit /b
)

@REM REM Ensure the input directory exists
@REM if not exist sparse_inputs (
@REM     echo Error: Input directory does not exist. Please create it and add the input files.
@REM     pause
@REM     exit /b
@REM )

REM Partition counts (10 to 60 in steps of 20)
for /L %%p in (10,10,60) do (
    REM Balance ranges (3 to 5)
    for /L %%b in (3,1,5) do (

        REM Input files in "input" directory (input/input_1.txt, input/input_2.txt, etc.)
        for %%f in (small_dense/data_*.txt) do (
            echo Running with Partition Count %%p, Balance %%b, and File %%f...
            %PROGRAM% %%p %%b %%f
        )
    )
)

echo All iterations completed!
pause
