# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /cygdrive/c/Users/gopia/.CLion2016.3/system/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/gopia/.CLion2016.3/system/cygwin_cmake/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/cygdrive/c/Users/gopia/Documents/Projects/C files/FileExperiments"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/cygdrive/c/Users/gopia/Documents/Projects/C files/FileExperiments/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/FileExperiments.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/FileExperiments.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FileExperiments.dir/flags.make

CMakeFiles/FileExperiments.dir/main.c.o: CMakeFiles/FileExperiments.dir/flags.make
CMakeFiles/FileExperiments.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/cygdrive/c/Users/gopia/Documents/Projects/C files/FileExperiments/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/FileExperiments.dir/main.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/FileExperiments.dir/main.c.o   -c "/cygdrive/c/Users/gopia/Documents/Projects/C files/FileExperiments/main.c"

CMakeFiles/FileExperiments.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FileExperiments.dir/main.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/cygdrive/c/Users/gopia/Documents/Projects/C files/FileExperiments/main.c" > CMakeFiles/FileExperiments.dir/main.c.i

CMakeFiles/FileExperiments.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FileExperiments.dir/main.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/cygdrive/c/Users/gopia/Documents/Projects/C files/FileExperiments/main.c" -o CMakeFiles/FileExperiments.dir/main.c.s

CMakeFiles/FileExperiments.dir/main.c.o.requires:

.PHONY : CMakeFiles/FileExperiments.dir/main.c.o.requires

CMakeFiles/FileExperiments.dir/main.c.o.provides: CMakeFiles/FileExperiments.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/FileExperiments.dir/build.make CMakeFiles/FileExperiments.dir/main.c.o.provides.build
.PHONY : CMakeFiles/FileExperiments.dir/main.c.o.provides

CMakeFiles/FileExperiments.dir/main.c.o.provides.build: CMakeFiles/FileExperiments.dir/main.c.o


# Object files for target FileExperiments
FileExperiments_OBJECTS = \
"CMakeFiles/FileExperiments.dir/main.c.o"

# External object files for target FileExperiments
FileExperiments_EXTERNAL_OBJECTS =

FileExperiments.exe: CMakeFiles/FileExperiments.dir/main.c.o
FileExperiments.exe: CMakeFiles/FileExperiments.dir/build.make
FileExperiments.exe: CMakeFiles/FileExperiments.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/cygdrive/c/Users/gopia/Documents/Projects/C files/FileExperiments/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable FileExperiments.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FileExperiments.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FileExperiments.dir/build: FileExperiments.exe

.PHONY : CMakeFiles/FileExperiments.dir/build

CMakeFiles/FileExperiments.dir/requires: CMakeFiles/FileExperiments.dir/main.c.o.requires

.PHONY : CMakeFiles/FileExperiments.dir/requires

CMakeFiles/FileExperiments.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FileExperiments.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FileExperiments.dir/clean

CMakeFiles/FileExperiments.dir/depend:
	cd "/cygdrive/c/Users/gopia/Documents/Projects/C files/FileExperiments/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/cygdrive/c/Users/gopia/Documents/Projects/C files/FileExperiments" "/cygdrive/c/Users/gopia/Documents/Projects/C files/FileExperiments" "/cygdrive/c/Users/gopia/Documents/Projects/C files/FileExperiments/cmake-build-debug" "/cygdrive/c/Users/gopia/Documents/Projects/C files/FileExperiments/cmake-build-debug" "/cygdrive/c/Users/gopia/Documents/Projects/C files/FileExperiments/cmake-build-debug/CMakeFiles/FileExperiments.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/FileExperiments.dir/depend

