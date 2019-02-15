"Engine lines: " + (gci -path "C:\Users\Eemeli Lehtonen\source\repos\overpeek-engine\overpeek-engine" -include *.glsl, *.cpp, *.h -recurse | select-string .).Count
"Game lines: " + (gci -path "C:\Users\Eemeli Lehtonen\source\repos\overpeek-engine\Project1" -include *.glsl, *.cpp, *.h -recurse | select-string .).Count
"Combined. " + ((gci -path "C:\Users\Eemeli Lehtonen\source\repos\overpeek-engine\overpeek-engine" -include *.glsl, *.cpp, *.h -recurse | select-string .).Count + (gci -path "C:\Users\Eemeli Lehtonen\source\repos\overpeek-engine\Project1" -include *.glsl, *.cpp, *.h -recurse | select-string .).Count)

pause