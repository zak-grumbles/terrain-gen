
set(glslang_ROOT
	"${glslang_ROOT}"
	CACHE
	PATH
	"Root directory to look for glslang in"
)

find_library(glslang_LIBRARY
	NAMES
	glslang
	PATHS
	"${glslang_ROOT}"
	PATH_SUFFIXES
	lib
)

find_path(glslang_INCLUDE_DIR
	NAMES
	"include/glslang/Public/ShaderLang.h"
	PATHS
	"${glslang_ROOT}"
	PATH_SUFFIXES
	include)

if(EXISTS "${glslang_ROOT}/include")
	set(glslang_INCLUDE_DIR "${glslang_ROOT}/include")
else()
	message(SEND_ERROR "Could not find glslang include directory at ${glslang_ROOT}/include")
endif()



include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(glslang
	DEFAULT_MSG
	glslang_LIBRARY
	glslang_INCLUDE_DIR
)

if(glslang_FOUND)

	set(glslang_LIBRARIES
		glslang_LIBRARY
	)
	
	set(glslang_INCLUDE_DIRS ${glslang_INCLUDE_DIR})
endif()

mark_as_advanced(glslang_LIBRARY
	glslang_INCLUDE_DIR
	glslang_OGLCompiler_LIBRARY
	glslang_OSDependent_LIBRARY
	glslang_SPIRV_LIBRARY
	glslang_SPVRemapper_LIBRARY
)

if(glslang_FOUND)
	mark_as_advanced(glslang_ROOT)
endif()