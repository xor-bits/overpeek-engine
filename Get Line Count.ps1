"Engine lines: " + (gci -path "C:\Users\eemel\Documents\Visual Studio 2017\Projects\overpeek-engine\overpeek-engine" -include *.glsl, *.cpp, *.h -recurse | select-string .).Count
"Game lines: " + (gci -path "C:\Users\eemel\Documents\Visual Studio 2017\Projects\overpeek-engine\Project1" -include *.glsl, *.cpp, *.h -recurse | select-string .).Count
"Combined. " + ((gci -path "C:\Users\eemel\Documents\Visual Studio 2017\Projects\overpeek-engine\overpeek-engine" -include *.glsl, *.cpp, *.h -recurse | select-string .).Count + (gci -path "C:\Users\eemel\Documents\Visual Studio 2017\Projects\overpeek-engine\Project1" -include *.glsl, *.cpp, *.h -recurse | select-string .).Count)

pause