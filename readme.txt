
 _                _                              
| |__   __ _  ___| | ___   _ _ __   _____  _____ 
| '_ \ / _` |/ __| |/ / | | | '_ \ / _ \ \/ / _ \
| |_) | (_| | (__|   <| |_| | |_) |  __/>  <  __/
|_.__/ \__,_|\___|_|\_\\__,_| .__(_)___/_/\_\___|
                            |_|                  

Tiny superpowerful Win32 backup program

==Windows==

http://windowsupdate.microsoft.com
Windows XP SP3 and Windows Update
Visual Studio 2008 and SP1

http://www.microsoft.com/downloads/details.aspx?FamilyID=c17ba869-9671-4330-a63e-1fd44e0e2505&displaylang=en
Windows SDK Version 7, published July 24 2009

==Code==

git clone git@github.com:zootella/backup.git

==Or, make a new project==

File, New, Project
Project types: Visual C++, Win32, Win32 Project
Name: backup
Location: C:\Documents
Uncheck Create directory for solution
OK
Application Settings, check Empty Project
Finish

Place files in the backup folder and then drag them into Visual Studio
*.h
*.cpp
*.ico
backup.rc
backup.exe.manifest

==Or, start resources from scratch==

File, Add New Item
choose Resource File (.rc), enter name, Open

==Properties==

Project, Properties, Configuration Properties, Configuration: Debug | Release
General, Use of ATL: Not Using ATL
General, Character Set: Use Unicode Character Set
General, Whole Program Optimization: No Whole Program Optimization | Use Link Time Code Generation
C/C++, Optimization, Optimization: Disabled (/Od) | Minimize Size (/O1)
C/C++, Code Generation, Enable Minimal Rebuild: Yes (/Gm) | No
C/C++, Code Generation, Enable C++ Exceptions: Yes With SEH Exceptions (/EHa)
C/C++, Code Generation, Basic Runtime Checks: Both | Default
C/C++, Code Generation, Runtime Library: Multi-threaded Debug (/MTd) | Multi-threaded (/MT)
C/C++, Precompiled Headers, Create/Use Precompiled Header: Not Using Precompiled Headers
Linker, Manifest File, Generate Manifest: No
Manifest Tool, Input and Output, Embed Manifest: No

Project, Properties, Configuration Properties, C/C++, Preprocessor, Preprocessor Definitions, Debug/Release
WIN32
_DEBUG/NDEBUG
_WINDOWS
_WIN32_WINNT=0x0501
BOOST_ALL_NO_LIB
BOOST_THREAD_USE_LIB
_FILE_OFFSET_BITS=64
WITH_SHIPPED_GEOIP_H

Project, Properties, Configuration Properties, Linker, Input, Additional Dependencies, Debug
comctl32.lib
ssleay32.lib
libeay32.lib
libboost_date_time-vc90-mt-sgd.lib
libboost_filesystem-vc90-mt-sgd.lib
libboost_system-vc90-mt-sgd.lib
libboost_thread-vc90-mt-sgd.lib
libtorrent-debug.lib

Release
comctl32.lib
ssleay32.lib
libeay32.lib
libboost_date_time-vc90-mt-s.lib
libboost_filesystem-vc90-mt-s.lib
libboost_system-vc90-mt-s.lib
libboost_thread-vc90-mt-s.lib
libtorrent-release.lib

See the whole command line at C/C++, Command Line and Linker, Command Line

==The end==
