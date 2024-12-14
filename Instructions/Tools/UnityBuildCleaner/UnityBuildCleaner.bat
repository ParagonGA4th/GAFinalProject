@echo off
setlocal

FOR %%A IN ("%~dp0..") DO set "current_directory=%%~dpA"
FOR %%A IN ("%current_directory%.") DO set "target_par_directory=%%~dpA"
set "target_directory=%target_par_directory%ParagonEngine"
echo UnityBuild Deletion Planned In %target_directory%

for %%A in ("%target_directory%") do (
    powershell -Command "Get-ChildItem -Path '%%~A' -Filter *.cpp -Recurse | Where-Object {$_.Name.Substring(0,6) -eq 'unity_'} | Remove-Item -Force"
    powershell -Command "Get-ChildItem -Path '%%~A' -Filter *.obj -Recurse | Where-Object {$_.Name.Substring(0,6) -eq 'unity_'} | Remove-Item -Force"
)
echo Completed In %target_directory%
timeout /t 5 /nobreak

endlocal