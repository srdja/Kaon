SET (common
     common/script/lua_util.c
     common/script/lua_util.h
     common/script/script.c
     common/script/script.h
     common/script/bindings.c
     common/script/bindings.h
     common/strutil.c
     common/strutil.h
     common/vec2.h
     common/map/map.c
     common/map/map.h)

SET (entity
     common/entity/entity.c
     common/entity/entity.h
     common/entity/trigger.h
     common/entity/trigger.c)

SET (game
     game/collision/collision.c
     game/collision/collision.h)

SET (graphics
     client/graphics/window.c
     client/graphics/window.h
     client/graphics/matrix.c
     client/graphics/matrix.h
     client/graphics/draw.c
     client/graphics/draw.h
     client/graphics/mesh.c
     client/graphics/mesh.h
     client/graphics/model.h
     client/graphics/model.c
     client/graphics/shaders.c
     client/graphics/shaders.h
     client/graphics/gl_string.c
     client/graphics/gl_string.h
     client/graphics/texture.h
     client/graphics/texture.c
     client/graphics/image.c
     client/graphics/image.h
     client/graphics/vbuffer.c
     client/graphics/vbuffer.h)

SET (audio
     client/audio/sound.c
     client/audio/sound.h)

SET (input
     client/input/input.c
     client/input/input.h
     client/input/input_bindings.c
     client/input/input_bindings.h
     client/input/input_context.c
     client/input/input_context.h)

SET (client
     client/console.c
     client/console.h
     client/cl_frame.c
     client/cl_frame.h
     client/cl_scene.c
     client/cl_scene.h)

SET (server
     server/sv_frame.c
     server/sv_frame.h)

SET (lua
     lib/lua/lapi.c
     lib/lua/lapi.h
     lib/lua/lauxlib.c
     lib/lua/lauxlib.h
     lib/lua/lbaselib.c
     lib/lua/lbitlib.c
     lib/lua/lcode.c
     lib/lua/lcode.h
     lib/lua/lcorolib.c
     lib/lua/lctype.c
     lib/lua/lctype.h
     lib/lua/ldblib.c
     lib/lua/ldebug.c
     lib/lua/ldebug.h
     lib/lua/ldo.c
     lib/lua/ldo.h
     lib/lua/ldump.c
     lib/lua/lfunc.c
     lib/lua/lfunc.h
     lib/lua/lgc.c
     lib/lua/lgc.h
     lib/lua/linit.c
     lib/lua/liolib.c
     lib/lua/llex.c
     lib/lua/llex.h
     lib/lua/llimits.h
     lib/lua/lmathlib.c
     lib/lua/lmem.c
     lib/lua/lmem.h
     lib/lua/loadlib.c
     lib/lua/lobject.c
     lib/lua/lobject.h
     lib/lua/lopcodes.c
     lib/lua/lopcodes.h
     lib/lua/loslib.c
     lib/lua/lparser.c
     lib/lua/lparser.h
     lib/lua/lstate.c
     lib/lua/lstate.h
     lib/lua/lstring.c
     lib/lua/lstring.h
     lib/lua/lstrlib.c
     lib/lua/ltable.c
     lib/lua/ltable.h
     lib/lua/ltablib.c
     lib/lua/ltm.c
     lib/lua/ltm.h
     lib/lua/lua.h
     lib/lua/luaconf.h
     lib/lua/lualib.h
     lib/lua/lundump.c
     lib/lua/lundump.h
     lib/lua/lvm.c
     lib/lua/lvm.h
     lib/lua/lzio.c
     lib/lua/lzio.h)

SET (file_system
     file-system/fs_entry.h
     file-system/fs_io.h
     file-system/fs_io.c
     file-system/paths.c
     file-system/paths.h
     file-system/path_util.c
     file-system/path_util.h)

SET (network
     network/net.c
     network/net.h
     network/stp.c
     network/stp.h)

IF (UNIX)
SET (os_implementation
     platform/posix/main.c
     platform/posix/fs_entry.c)
ENDIF (UNIX)

IF (WIN32)
SET (os_implementation
     platform/win/main.c
     platform/win/fs_entry.c)   
ENDIF (WIN32)

add_executable(game ${common}
                    ${entity}
                    ${graphics}
                    ${game}
                    ${audio}
                    ${input}
                    ${client}
                    ${server}
                    ${lua}
                    ${file_system}
                    ${network}
                    ${os_implementation})

#*******************************************************************************
#
#                                 LIBRARIES         
#
#*******************************************************************************                  
                    
IF (UNIX)
    SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".so")
ENDIF (UNIX)

IF (WIN32)
    SET(CMAKE_FIND_LIBRARY_PREFIXES "lib" "")
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".dll" ".dll.a" ".a")
ENDIF (WIN32)


#*******************************************************************************
#                           32/64bit GNU/Linux         
#*******************************************************************************   

IF (UNIX)

    # ================== SDL ========================
    
    include(FindPkgConfig)
    
    pkg_search_module(SDL2 REQUIRED sdl2)
    include_directories(${SDL2_INCLUDE_DIRS})
    
    pkg_search_module(SDL_ttf REQUIRED SDL2_ttf>=2.0.0)
    include_directories(${SDL_ttf_INCLUDE_DIRS})
    
    pkg_search_module(SDL_image REQUIRED SDL2_image>=2.0.0)
    include_directories(${SDL_image_INCLUDE_DIRS})
    
    pkg_search_module(SDL_mixer REQUIRED SDL2_mixer>=2.0.0)
    include_directories(${SDL_mixer_INCLUDE_DIRS})
    
    pkg_search_module(SDL_net REQUIRED SDL2_net>=2.0.0)
    include_directories(${SDL_net_INCLUDE_DIRS})
    
    # ================= GLEW ========================
    
    find_package(GLEW REQUIRED)
    include_directories(${GLEW_INCLUDE_DIRS})
    
    # ============== Collections ====================
    
    IF (${ARCH} STREQUAL "64")
        find_library(COLLECTIONS 
                     NAME collectc
                     PATHS ${PROJECT_SOURCE_DIR}/lib/64bit/gnu/collections)
             
        include_directories(${PROJECT_SOURCE_DIR}/lib/64bit/gnu/collections/header)  
    ENDIF ()
    
    IF (${ARCH} STREQUAL "32")
        find_library(COLLECTIONS 
                     NAME collectc
                     PATHS ${PROJECT_SOURCE_DIR}/lib/32bit/gnu/collections)
             
        include_directories(${PROJECT_SOURCE_DIR}/lib/32bit/gnu/collections/header)  
    ENDIF ()
    # ================= Math ========================
    
    find_library(M_LIB m)
    
ENDIF (UNIX)

#*******************************************************************************
#                             32bit Windows         
#*******************************************************************************   

IF (WIN32 AND ${ARCH} STREQUAL "32")

    # ================== SDL ========================
    
    find_library(SDL2
                 NAME libSDL2
                 PATHS ${PROJECT_SOURCE_DIR}/lib/32bit/win/SDL/SDL2/bin)				 
	
	find_library(SDL_ttf
                 NAME SDL2_ttf
                 PATHS ${PROJECT_SOURCE_DIR}/lib/32bit/win/SDL/SDL2_ttf/bin)				
	
	find_library(SDL_image
                 NAME SDL2_image
                 PATHS ${PROJECT_SOURCE_DIR}/lib/32bit/win/SDL/SDL2_image/bin)
                 
    find_library(SDL_mixer
                 NAME SDL2_mixer
                 PATHS ${PROJECT_SOURCE_DIR}/lib/32bit/win/SDL/SDL2_mixer/bin)
                 
    find_library(SDL_net
                 NAME SDL2_net
                 PATHS ${PROJECT_SOURCE_DIR}/lib/32bit/win/SDL/SDL2_net/bin)
                 
				 
    include_directories(${PROJECT_SOURCE_DIR}/lib/32bit/win/SDL/include)
    
    # ================= GLEW ========================
    
    find_library(GLEW
                 NAME glew32
                 PATHS ${PROJECT_SOURCE_DIR}/lib/32bit/win/glew)
                 
    include_directories(${PROJECT_SOURCE_DIR}/lib/32bit/win/glew/header)
    
    # ============== Collections ====================
    
    find_library(COLLECTIONS 
                 NAME libcollectc
                 PATHS ${PROJECT_SOURCE_DIR}/lib/32bit/win/collections)
			
	include_directories(${PROJECT_SOURCE_DIR}/lib/32bit/win/collections/header)
    
    # ================= Math ========================
    
    find_library(M_LIB 
                 NAME m 
                 PATHS C:/Qt/Tools/mingw482_32/i686-w64-mingw32/lib)
				 
ENDIF ()

#*******************************************************************************
#                             64bit Windows         
#*******************************************************************************   

IF (WIN32 AND ${ARCH} STREQUAL "64")

    # ================== SDL ======================== THE INCLUDES ARE WRONG... SEE 32bit
    
    find_library(SDL2
                 NAME libSDL2
                 PATHS ${PROJECT_SOURCE_DIR}/lib/64bit/win/SDL/SDL2/lib/bin)				 
	
	find_library(SDL_ttf
                 NAME SDL2_ttf
                 PATHS ${PROJECT_SOURCE_DIR}/lib/64bit/win/SDL/SDL2_ttf/bin)				
	
	find_library(SDL_image
                 NAME SDL2_image
                 PATHS ${PROJECT_SOURCE_DIR}/lib/64bit/win/SDL/SDL2_image/bin)
                 
    find_library(SDL_mixer
                 NAME SDL2_mixer
                 PATHS ${PROJECT_SOURCE_DIR}/lib/64bit/win/SDL/SDL2_image/bin)             
				 
    include_directories(${PROJECT_SOURCE_DIR}/lib/64bit/win/SDL/SDL2/include)
    include_directories(${PROJECT_SOURCE_DIR}/lib/64bit/win/SDL/SDL2_image/include)
    include_directories(${PROJECT_SOURCE_DIR}/lib/64bit/win/SDL/SDL2_ttf/include)
    include_directories(${PROJECT_SOURCE_DIR}/lib/64bit/win/SDL/SDL2_mixer/include)
    
    # ================= GLEW ========================
    
    find_library(GLEW
                 NAME glew32
                 PATHS ${PROJECT_SOURCE_DIR}/lib/64bit/win/glew)
                 
    include_directories(${PROJECT_SOURCE_DIR}/lib/64bit/win/glew/header)
    
    # ============== Collections ====================
    
    find_library(COLLECTIONS 
                 NAME libcollectc
                 PATHS ${PROJECT_SOURCE_DIR}/lib/64bit/win/collections)
			
	include_directories(${PROJECT_SOURCE_DIR}/lib/64bit/win/collections/header)
    
    # ================= Math ========================
    
    find_library(M_LIB 
                 NAME m PATHS C:/Program Files\ \(x86\)/CodeBlocks/MinGW/lib)
				 
ENDIF ()

#==============================
# OpenGL
#==============================

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})


#==============================================================================================

#IF (${ARCH} STREQUAL "64")
#    SET (CMAKE_C_FLAGS "-m64")
#ENDIF()

SET (CMAKE_C_FLAGS "-std=c99")
SET (CMAKE_C_FLAGS "-Werror")
SET (CMAKE_C_FLAGS "-Wall")
SET (CMAKE_C_FLAGS "-O0")
SET (CMAKE_C_FLAGS "-g")

set_target_properties(game PROPERTIES LINKER_LANGUAGE C)

IF (UNIX)
target_link_libraries(game ${M_LIB} 
                           ${OPENGL_LIBRARIES} 
                           ${GLEW_LIBRARIES}
                           ${SDL2_LIBRARIES} 
                           ${COLLECTIONS}
                           ${SDL_ttf_LIBRARIES}
                           ${SDL_net_LIBRARIES}
                           ${SDL_image_LIBRARIES}
                           ${SDL_mixer_LIBRARIES})
ENDIF (UNIX)

IF (WIN32)
target_link_libraries(game ${M_LIB}                           
                           ${OPENGL_LIBRARIES}
                           ${GLEW}                           
                           ${SDL2}                         
                           ${COLLECTIONS}
                           ${SDL_ttf}
                           ${SDL_net}
                           ${SDL_image}
                           ${SDL_mixer})                         
ENDIF (WIN32)

