
@echo off
echo chcp 65001 >nul
echo ==============================================
echo        Qt Windows 自动部署脚本 (MinGW)
echo              自动拷贝缺失运行库
echo ==============================================
echo.

:: ===================== 配置区域（只改这里）=====================
set "QT_APP_NAME=alantop_config.exe"  :: 改成你的exe文件名
set "MINGW_PATH=D:\alantop_dir\alantop_sde\qt\5.15.2\mingw81_64\bin\"  :: MinGW路径
set "QT_BIN_PATH=D:\alantop_dir\alantop_sde\qt\5.15.2\mingw81_64\bin\" :: Qt bin目录，改成你自己的
:: ==============================================================

:: 把Qt的bin目录加入临时环境变量，让系统能找到windeployqt
set "PATH=%QT_BIN_PATH%;%PATH%"

echo 正在部署 Qt 依赖库...
windeployqt --no-opengl-sw --no-system-d3d-compiler "%QT_APP_NAME%"

echo.
echo 正在自动拷贝 MinGW 运行库 (libstdc++-6.dll 等)...
copy "%MINGW_PATH%\libstdc++-6.dll" .
copy "%MINGW_PATH%\libgcc_s_seh-1.dll" .
copy "%MINGW_PATH%\libwinpthread-1.dll" .

echo.
echo ==============================================
echo              部署完成！文件夹可直接分发
echo ==============================================
echo.
pause