(gci -path audio,graphics,utility -include *.glsl,*.cpp,*.h -recurse | select-string .).Count
pause