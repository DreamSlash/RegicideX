SET UE4_ROOT="F:/Epic/UE_4.27"
SET UE4_UAT=%UE4_ROOT%/Engine/Build/BatchFiles/RunUAT.bat
SET PROJECT_ROOT=F:/Master/UE4/RegicideX
SET UPROJECT=%PROJECT_ROOT%/RegicideX.uproject
SET CONFIG=Development

%UE4_UAT% ^
  -ScriptsForProject=%UPROJECT% ^
  BuildCookRun ^
  -project=%UPROJECT% ^
  -noP4 ^
  -clientconfig=%CONFIG% ^
  -serverconfig=%CONFIG% ^
  -utf8output ^
  -platform=Win64 ^
  -targetplatform=Win64 ^
  -build ^
  -cook ^
  -map= ^
  -pak ^
  -stage ^
  -distribution ^
  -compressed ^
  -cmdline=" -Messaging" ^
  -stagingdirectory=%PROJECT_ROOT%/Builds/%CONFIG% ^
  -nocompile ^
  -nocompileeditor ^
  -addcmdline="-SessionId=F4A7206C469A8E268084EB99D416D7EF -SessionOwner='AULES' -SessionName='Development With Pak' "

REM   -deploy ^
REM  -encryptinifiles ^
REM  -unversionedcookedcontent ^
REM  -installed ^

REM Running: C:\Program Files\Epic Games\UE_4.27\Engine\Binaries\DotNET\UnrealBuildTool.exe RegicideX Win64 Development -Project=D:\RegicideX\RegicideX.uproject  D:\RegicideX\RegicideX.uproject -NoUBTMakefiles  -remoteini="D:\RegicideX" -skipdeploy -Manifest=D:\RegicideX\Intermediate\Build\Manifest.xml -NoHotReload -log="C:\Users\aules\AppData\Roaming\Unreal Engine\AutomationTool\Logs\C+Program+Files+Epic+Games+UE_4.27\UBT-RegicideX-Win64-Development.txt"
