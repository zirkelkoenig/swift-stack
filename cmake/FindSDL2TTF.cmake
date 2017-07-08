# Locate SDL2_ttf library
# This module defines
# SDL2TTF_LIBRARY, the name of the library to link against
# SDL2TTF_FOUND, if false, do not try to link to SDL2_ttf
# SDL2TTF_INCLUDE_DIR, where to find SDL_ttf.h

# Additional Note: If you see an empty SDL2TTF_LIBRARY_TEMP in your configuration and no SDL2TTF_LIBRARY, it means CMake
# did not find your SDL2_ttf library (SDL2_ttf.dll, libSDL2_ttf.so, SDL2_ttf.framework, etc). Set SDL2TTF_LIBRARY_TEMP
# to point to your SDL2_ttf library, and configure again. These values are used to generate the final SDL2TTF_LIBRARY
# variable, but when these values are unset, SDL2TTF_LIBRARY does not get created.

# $SDL2DIR is an environment variable that would correspond to the ./configure --prefix=$SDL2DIR used in building SDL2.
# l.e.galup  9-20-02

# Modified by Marvin Kipping based on FindSDL2.cmake by Kitware (see copyright notices below).
# Changed to find SDL2_ttf.

# Modified by Eric Wing.
# Added code to assist with automated building by using environmental variables and providing a more
# controlled/consistent search behavior.
# Added new modifications to recognize OS X frameworks and additional Unix paths (FreeBSD, etc).
# Also corrected the header search path to follow "proper" SDL guidelines.
# Added a search for SDL2main which is needed by some platforms.
# Added a search for threads which is needed by some platforms.
# Added needed compile switches for MinGW.

# On OSX, this will prefer the Framework version (if found) over others. People will have to manually change the cache
# values of SDL2TTF_LIBRARY to override this selection or set the CMake environment CMAKE_INCLUDE_PATH to modify the
# search paths.

# Note that the header path has changed from SDL2/SDL_ttf.h to just SDL_ttf.h. This needed to change because "proper"
# SDL convention is #include "SDL_ttf.h", not <SDL2/SDL_ttf.h>. This is done for portability reasons because not all
# systems place things in SDL2/ (see FreeBSD).

#=======================================================================================================================
# Copyright 2003-2009 Kitware, Inc.

# Distributed under the OSI-approved BSD License (the "License"); see accompanying file Copyright.txt for details.

# This software is distributed WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the License for more information.
#=======================================================================================================================
# (To distribute this file outside of CMake, substitute the full License text for the above reference.)

SET(SDL2TTF_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
)

FIND_PATH(SDL2TTF_INCLUDE_DIR SDL_ttf.h
	HINTS
	$ENV{SDL2DIR}
	PATH_SUFFIXES include/SDL2 include
	PATHS ${SDL2TTF_SEARCH_PATHS}
)

FIND_LIBRARY(SDL2TTF_LIBRARY_TEMP
	NAMES SDL2_ttf
	HINTS
	$ENV{SDL2DIR}
	PATH_SUFFIXES lib64 lib
	PATHS ${SDL2_SEARCH_PATHS}
)

# SDL2 may require threads on your system. The Apple build may not need an explicit flag because one of the frameworks
# may already provide it. But for non-OSX systems, I will use the CMake Threads package.
IF(NOT APPLE)
	FIND_PACKAGE(Threads)
ENDIF(NOT APPLE)

# MinGW needs an additional library, mwindows. It's total link flags should look like
# -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lmwindows
# (Actually on second look, I think it only needs one of the m* libraries.)
IF(MINGW)
	SET(MINGW32_LIBRARY mingw32 CACHE STRING "mwindows for MinGW")
ENDIF(MINGW)

IF(SDL2TTF_LIBRARY_TEMP)

	# For OS X, SDL2 uses Cocoa as a backend so it must link to Cocoa. CMake doesn't display the -framework Cocoa
	# string in the UI even though it actually is there if I modify a pre-used variable. I think it has something to
	# do with the CACHE STRING. So I use a temporary variable until the end so I can set the "real" variable in
	# one-shot.
	IF(APPLE)
		SET(SDL2TTF_LIBRARY_TEMP ${SDL2TTF_LIBRARY_TEMP} "-framework Cocoa")
	ENDIF(APPLE)

	# For threads, as mentioned Apple doesn't need this. In fact, there seems to be a problem if I used the Threads
	# package and try using this line, so I'm just skipping it entirely for OS X.
	IF(NOT APPLE)
		SET(SDL2TTF_LIBRARY_TEMP ${SDL2TTF_LIBRARY_TEMP} ${CMAKE_THREAD_LIBS_INIT})
	ENDIF(NOT APPLE)

	# For MinGW library
	IF(MINGW)
		SET(SDL2TTF_LIBRARY_TEMP ${MINGW32_LIBRARY} ${SDL2TTF_LIBRARY_TEMP})
	ENDIF(MINGW)

	# Set the final string here so the GUI reflects the final state.
	SET(SDL2TTF_LIBRARY ${SDL2TTF_LIBRARY_TEMP} CACHE STRING "Where the SDL2_ttf Library can be found")
	# Set the temp variable to INTERNAL so it is not seen in the CMake GUI
	SET(SDL2TTF_LIBRARY_TEMP "${SDL2TTF_LIBRARY_TEMP}" CACHE INTERNAL "")
ENDIF(SDL2TTF_LIBRARY_TEMP)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2_ttf REQUIRED_VARS SDL2TTF_LIBRARY SDL2TTF_INCLUDE_DIR)
