
set(shaderc_ROOT
	"${shaderc_ROOT}"
	CACHE
	PATH
	"Root directory to look for shaderc in"
)

if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
	find_library(shaderc_combined_LIBRARY
		NAMES
		shaderc_combined
		PATHS
		"${shaderc_ROOT}"
		PATH_SUFFIXES
		Lib32
	)
else()
	find_library(shaderc_combined_LIBRARY
		NAMES
		shaderc_combined
		PATHS
		"${shaderc_ROOT}"
		PATH_SUFFIXES
		Lib
	)
endif()

find_path(shaderc_INCLUDE_DIR
	NAMES
	"shaderc/shaderc.h"
	PATHS
	"${shaderc_ROOT}"
	PATH_SUFFIXES
	include
)

set(shaderc_LIBRARIES ${shaderc_combined_LIBRARY})
set(shaderc_INCLUDE_DIRS ${shaderc_INCLUDE_DIR})
