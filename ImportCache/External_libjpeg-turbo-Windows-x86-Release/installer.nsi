!include x64.nsh
Name "libjpeg-turbo SDK for Visual C++"
OutFile ".\${BUILDDIR}libjpeg-turbo-3.0.0-vc.exe"
InstallDir "c:\libjpeg-turbo"

SetCompressor bzip2

Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

Section "libjpeg-turbo SDK for Visual C++ (required)"
!ifdef WIN64
	${If} ${RunningX64}
	${DisableX64FSRedirection}
	${Endif}
!endif
	SectionIn RO
!ifdef GCC
	IfFileExists $SYSDIR/libturbojpeg.dll exists 0
!else
	IfFileExists $SYSDIR/turbojpeg.dll exists 0
!endif
	goto notexists
	exists:
!ifdef GCC
	MessageBox MB_OK "An existing version of the libjpeg-turbo SDK for Visual C++ is already installed.  Please uninstall it first."
!else
	MessageBox MB_OK "An existing version of the libjpeg-turbo SDK for Visual C++ or the TurboJPEG SDK is already installed.  Please uninstall it first."
!endif
	quit

	notexists:
	SetOutPath $SYSDIR
!ifdef GCC
	File ".\libturbojpeg.dll"
!else
	File ".\${BUILDDIR}turbojpeg.dll"
!endif
	SetOutPath $INSTDIR\bin
!ifdef GCC
	File ".\libturbojpeg.dll"
!else
	File ".\${BUILDDIR}turbojpeg.dll"
!endif
!ifdef GCC
	File ".\libjpeg-62.dll"
!else
	File ".\${BUILDDIR}jpeg62.dll"
!endif
	File ".\${BUILDDIR}cjpeg.exe"
	File ".\${BUILDDIR}djpeg.exe"
	File ".\${BUILDDIR}jpegtran.exe"
	File ".\${BUILDDIR}tjbench.exe"
	File ".\${BUILDDIR}rdjpgcom.exe"
	File ".\${BUILDDIR}wrjpgcom.exe"
	SetOutPath $INSTDIR\lib
!ifdef GCC
	File ".\libturbojpeg.dll.a"
	File ".\libturbojpeg.a"
	File ".\libjpeg.dll.a"
	File ".\libjpeg.a"
!else
	File ".\${BUILDDIR}turbojpeg.lib"
	File ".\${BUILDDIR}turbojpeg-static.lib"
	File ".\${BUILDDIR}jpeg.lib"
	File ".\${BUILDDIR}jpeg-static.lib"
!endif
	SetOutPath $INSTDIR\lib\pkgconfig
	File ".\pkgscripts\libjpeg.pc"
	File ".\pkgscripts\libturbojpeg.pc"
	SetOutPath $INSTDIR\lib\cmake\libjpeg-turbo
	File ".\pkgscripts\libjpeg-turboConfig.cmake"
	File ".\pkgscripts\libjpeg-turboConfigVersion.cmake"
	File ".\win\libjpeg-turboTargets.cmake"
	File ".\win\libjpeg-turboTargets-release.cmake"
!ifdef JAVA
	SetOutPath $INSTDIR\classes
	File ".\java\turbojpeg.jar"
!endif
	SetOutPath $INSTDIR\include
	File ".\jconfig.h"
	File "../../../../External/libjpeg-turbo\jerror.h"
	File "../../../../External/libjpeg-turbo\jmorecfg.h"
	File "../../../../External/libjpeg-turbo\jpeglib.h"
	File "../../../../External/libjpeg-turbo\turbojpeg.h"
	SetOutPath $INSTDIR\doc
	File "../../../../External/libjpeg-turbo\README.ijg"
	File "../../../../External/libjpeg-turbo\README.md"
	File "../../../../External/libjpeg-turbo\LICENSE.md"
	File "../../../../External/libjpeg-turbo\example.c"
	File "../../../../External/libjpeg-turbo\libjpeg.txt"
	File "../../../../External/libjpeg-turbo\structure.txt"
	File "../../../../External/libjpeg-turbo\usage.txt"
	File "../../../../External/libjpeg-turbo\wizard.txt"
	File "../../../../External/libjpeg-turbo\tjexample.c"
	File "../../../../External/libjpeg-turbo\java\TJExample.java"
!ifdef GCC
	SetOutPath $INSTDIR\man\man1
	File "../../../../External/libjpeg-turbo\cjpeg.1"
	File "../../../../External/libjpeg-turbo\djpeg.1"
	File "../../../../External/libjpeg-turbo\jpegtran.1"
	File "../../../../External/libjpeg-turbo\rdjpgcom.1"
	File "../../../../External/libjpeg-turbo\wrjpgcom.1"
!endif

	WriteRegStr HKLM "SOFTWARE\libjpeg-turbo 3.0.0" "Install_Dir" "$INSTDIR"

	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\libjpeg-turbo 3.0.0" "DisplayName" "libjpeg-turbo SDK v3.0.0 for Visual C++"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\libjpeg-turbo 3.0.0" "UninstallString" '"$INSTDIR\uninstall_3.0.0.exe"'
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\libjpeg-turbo 3.0.0" "NoModify" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\libjpeg-turbo 3.0.0" "NoRepair" 1
	WriteUninstaller "uninstall_3.0.0.exe"
SectionEnd

Section "Uninstall"
!ifdef WIN64
	${If} ${RunningX64}
	${DisableX64FSRedirection}
	${Endif}
!endif

	SetShellVarContext all

	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\libjpeg-turbo 3.0.0"
	DeleteRegKey HKLM "SOFTWARE\libjpeg-turbo 3.0.0"

!ifdef GCC
	Delete $INSTDIR\bin\libjpeg-62.dll
	Delete $INSTDIR\bin\libturbojpeg.dll
	Delete $SYSDIR\libturbojpeg.dll
	Delete $INSTDIR\lib\libturbojpeg.dll.a
	Delete $INSTDIR\lib\libturbojpeg.a
	Delete $INSTDIR\lib\libjpeg.dll.a
	Delete $INSTDIR\lib\libjpeg.a
!else
	Delete $INSTDIR\bin\jpeg62.dll
	Delete $INSTDIR\bin\turbojpeg.dll
	Delete $SYSDIR\turbojpeg.dll
	Delete $INSTDIR\lib\jpeg.lib
	Delete $INSTDIR\lib\jpeg-static.lib
	Delete $INSTDIR\lib\turbojpeg.lib
	Delete $INSTDIR\lib\turbojpeg-static.lib
!endif
	Delete $INSTDIR\lib\pkgconfig\libjpeg.pc
	Delete $INSTDIR\lib\pkgconfig\libturbojpeg.pc
	Delete $INSTDIR\lib\cmake\libjpeg-turbo\libjpeg-turboConfig.cmake
	Delete $INSTDIR\lib\cmake\libjpeg-turbo\libjpeg-turboConfigVersion.cmake
	Delete $INSTDIR\lib\cmake\libjpeg-turbo\libjpeg-turboTargets.cmake
	Delete $INSTDIR\lib\cmake\libjpeg-turbo\libjpeg-turboTargets-release.cmake
!ifdef JAVA
	Delete $INSTDIR\classes\turbojpeg.jar
!endif
	Delete $INSTDIR\bin\cjpeg.exe
	Delete $INSTDIR\bin\djpeg.exe
	Delete $INSTDIR\bin\jpegtran.exe
	Delete $INSTDIR\bin\tjbench.exe
	Delete $INSTDIR\bin\rdjpgcom.exe
	Delete $INSTDIR\bin\wrjpgcom.exe
	Delete $INSTDIR\include\jconfig.h
	Delete $INSTDIR\include\jerror.h
	Delete $INSTDIR\include\jmorecfg.h
	Delete $INSTDIR\include\jpeglib.h
	Delete $INSTDIR\include\turbojpeg.h
	Delete $INSTDIR\uninstall_3.0.0.exe
	Delete $INSTDIR\doc\README.ijg
	Delete $INSTDIR\doc\README.md
	Delete $INSTDIR\doc\LICENSE.md
	Delete $INSTDIR\doc\example.c
	Delete $INSTDIR\doc\libjpeg.txt
	Delete $INSTDIR\doc\structure.txt
	Delete $INSTDIR\doc\usage.txt
	Delete $INSTDIR\doc\wizard.txt
	Delete $INSTDIR\doc\tjexample.c
	Delete $INSTDIR\doc\TJExample.java
!ifdef GCC
	Delete $INSTDIR\man\man1\cjpeg.1
	Delete $INSTDIR\man\man1\djpeg.1
	Delete $INSTDIR\man\man1\jpegtran.1
	Delete $INSTDIR\man\man1\rdjpgcom.1
	Delete $INSTDIR\man\man1\wrjpgcom.1
!endif

	RMDir "$INSTDIR\include"
	RMDir "$INSTDIR\lib\pkgconfig"
	RMDir "$INSTDIR\lib\cmake\libjpeg-turbo"
	RMDir "$INSTDIR\lib\cmake"
	RMDir "$INSTDIR\lib"
	RMDir "$INSTDIR\doc"
!ifdef GCC
	RMDir "$INSTDIR\man\man1"
	RMDir "$INSTDIR\man"
!endif
!ifdef JAVA
	RMDir "$INSTDIR\classes"
!endif
	RMDir "$INSTDIR\bin"
	RMDir "$INSTDIR"

SectionEnd
