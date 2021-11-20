@ECHO OFF
SETLOCAL ENABLEEXTENSIONS

SET CC=g++
SET FLAGS=
SET LIBS=fraction/fraction.cpp argh/argh.cpp linear_algebra/linear_algebra.cpp function_guesser/function_guesser.cpp

CALL %CC% %FLAGS% -o main main.cpp %LIBS%

CALL main %*

ENDLOCAL