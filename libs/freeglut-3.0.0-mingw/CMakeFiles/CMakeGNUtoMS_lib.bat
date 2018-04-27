@echo off
set VSCMD_START_DIR=.
call "C:\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
lib /machine:"amd64" %*
