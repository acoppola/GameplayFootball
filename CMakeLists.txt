
cmake_minimum_required(VERSION 2.8)

set(PACKAGE gameplayfootball)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)
set(VERSION_PATCH 0)
if(${VERSION_PATCH})
   set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
else(${VERSION_PATCH})
   set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR})
endif(${VERSION_PATCH})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules")

project(gameplayfootball)

if(UNIX)
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -Wall")
   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
endif(UNIX)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
set(Boost_ADDITIONAL_VERSIONS "1.55" "1.55.0")
set(Boost_USE_STATIC_LIBS TRUE)
#add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)
#add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
#add_definitions(-DBOOST_ALL_NO_LIB -DBOOST_ALL_DYN_LINK -DBOOST_LOG_DYN_LINK)
#add_definitions(-DFO_BOOST_STATIC_LINK=TRUE)
add_definitions(-DWINVER=0x500 -DBOOST_SIGNALS_NO_DEPRECATION_WARNING -DBOOST_DISABLE_ASSERTS -DNDEBUG -mthreads -Wall -Wno-unused-variable -Wno-unused-local-typedefs -Wno-narrowing -fno-strict-aliasing -fsignaling-nans -mtune=generic -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free -O2 -s)
#add_definitions(-DCMAKE_RULE_MESSAGES:BOOL=OFF -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON)
#SET(CMAKE_EXE_LINKER_FLAGS "-static")
# Find needed libraries
FIND_PACKAGE(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

FIND_PACKAGE(SDL REQUIRED)
include_directories(${SDL_INCLUDE_DIR})

FIND_PACKAGE(SDL_image REQUIRED)
include_directories(${SDL_IMAGE_DIRS})

FIND_PACKAGE(SDL_ttf REQUIRED)
include_directories(${SDL_TTF_DIRS})

FIND_PACKAGE(SDL_gfx REQUIRED)
include_directories(${SDL_GFX_DIRS})

FIND_PACKAGE(SDL_net REQUIRED)
include_directories(${SDL_NET_INCLUDE_DIRS})

#set(Boost_USE_STATIC_LIBS ON)
#set(Boost_DEBUG 1)
FIND_PACKAGE(Boost REQUIRED COMPONENTS system thread signals filesystem)
include_directories(SYSTEM ${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

set(SGE_INCLUDE_DIR /home/acoppola/mxe/usr/i686-w64-mingw32.shared/include/SDL)
set(SGE_LIBRARY /home/acoppola/mxe/usr/i686-w64-mingw32.shared/lib/libSGE.a)
FIND_PACKAGE(SGE REQUIRED)
include_directories(${SGE_INCLUDE_DIR})

set(OPENAL_INCLUDE_DIR /home/acoppola/mxe/usr/i686-w64-mingw32.shared/include/AL)
set(OPENAL_LIBRARY /home/acoppola/mxe/usr/i686-w64-mingw32.shared/lib/libOpenAL32.dll.a)
#FIND_PACKAGE(OpenAL REQUIRED)
include_directories(${OPENAL_INCLUDE_DIR})

#find_package(PkgConfig REQUIRED)
#FIND_PACKAGE(dlfcn-win32 REQUIRED)
#set(CMAKE_DL_LIBS dlfcn-win32::dl)

include_directories(${PROJECT_SOURCE_DIR}/src)

# Include the sources
include(sources.cmake)

# Compile it as multiple static libraries (note: not compiling physics,
# as not used by gameplayfootball)
add_library(leaguelib STATIC ${LEAGUE_SOURCES} ${LEAGUE_HEADERS})
add_library(gamelib STATIC ${GAME_SOURCES} ${GAME_HEADERS})
add_library(hidlib STATIC ${HID_SOURCES} ${HID_HEADERS})
add_library(menulib STATIC ${MENU_SOURCES} ${MENU_HEADERS})
add_library(datalib STATIC ${DATA_SOURCES} ${DATA_HEADERS})


#set(LIBRARIES gamelib hidlib menulib datalib leaguelib ${BLUNTED2_LIBRARY} ${Boost_LIBRARIES} ${OPENAL_LIBRARY} ${SGE_LIBRARY}
set(LIBRARIES gamelib hidlib menulib datalib leaguelib blunted2 ${Boost_LIBRARIES}
   ${SDL_IMAGE_LIBRARIES} ${SDL_TTF_LIBRARIES} ${SDL_GFX_LIBRARIES}
   ${SDL_NET_LIBRARIES}
   ${SDL_LIBRARY} ${OPENAL_LIBRARY} ${SGE_LIBRARY} dl m ${OPENGL_LIBRARIES})
add_library(baselib OBJECT ${BASE_SOURCES} ${BASE_HEADERS}
   ${BASE_GEOMETRY_HEADERS} ${BASE_MATH_HEADERS})
add_library(systemscommonlib OBJECT ${SYSTEMS_COMMON_SOURCES} 
   ${SYSTEMS_COMMON_HEADERS})
add_library(systemsgraphicslib OBJECT ${SYSTEMS_GRAPHICS_SOURCES} 
   ${SYSTEMS_GRAPHICS_HEADERS} ${SYSTEMS_GRAPHICS_OBJECTS_HEADERS}
   ${SYSTEMS_GRAPHICS_RESOURCES_HEADERS} ${SYSTEMS_GRAPHICS_RENDERING_HEADERS})
add_library(systemsaudiolib OBJECT ${SYSTEMS_AUDIO_SOURCES} 
   ${SYSTEMS_AUDIO_HEADERS} ${SYSTEMS_AUDIO_OBJECTS_HEADERS}
   ${SYSTEMS_AUDIO_RESOURCES_HEADERS} ${SYSTEMS_AUDIO_RENDERING_HEADERS})
add_library(loaderslib OBJECT ${LOADERS_SOURCES} ${LOADERS_HEADERS})
add_library(typeslib OBJECT ${TYPES_SOURCES} ${TYPES_HEADERS})
add_library(frameworklib OBJECT ${FRAMEWORK_SOURCES} ${FRAMEWORK_HEADERS})
add_library(scenelib OBJECT ${SCENE_SOURCES} ${SCENE_HEADERS}
   ${SCENE2D_HEADERS} ${SCENE3D_HEADERS} ${SCENE_OBJECTS_HEADERS}
   ${SCENE_RESOURCES_HEADERS})
add_library(managerslib OBJECT ${MANAGERS_SOURCES} ${MANAGERS_HEADERS})
add_library(utilslib OBJECT ${UTILS_SOURCES} ${UTILS_HEADERS}
   ${UTILS_EXT_HEADERS})
add_library(gui2lib OBJECT ${UTILS_GUI2_SOURCES} ${UTILS_GUI2_HEADERS}
   ${UTILS_GUI2_WIDGETS_HEADERS})

set(OWN_LIBRARIES $<TARGET_OBJECTS:baselib> $<TARGET_OBJECTS:systemscommonlib>
   $<TARGET_OBJECTS:systemsgraphicslib> 
   $<TARGET_OBJECTS:systemsaudiolib> $<TARGET_OBJECTS:loaderslib>
   $<TARGET_OBJECTS:typeslib> $<TARGET_OBJECTS:frameworklib> 
   $<TARGET_OBJECTS:scenelib> $<TARGET_OBJECTS:managerslib>
   $<TARGET_OBJECTS:utilslib> $<TARGET_OBJECTS:gui2lib>)
#set(OWN_LIBRARIES baselib systemcommonlib systemgraphicslib loaderslib typeslib frameworklib scenelib managerslib utlislib gui2lib)
# Join all created static libraries into the single static or share one.

# For shared library, uncomment bellow:

#set_property(TARGET baselib PROPERTY POSITION_INDEPENDENT_CODE 1)
#set_property(TARGET systemscommonlib PROPERTY POSITION_INDEPENDENT_CODE 1)
#set_property(TARGET systemsgraphicslib PROPERTY POSITION_INDEPENDENT_CODE 1)
#set_property(TARGET systemsaudiolib PROPERTY POSITION_INDEPENDENT_CODE 1)
#set_property(TARGET loaderslib PROPERTY POSITION_INDEPENDENT_CODE 1)
#set_property(TARGET typeslib PROPERTY POSITION_INDEPENDENT_CODE 1)
#set_property(TARGET frameworklib PROPERTY POSITION_INDEPENDENT_CODE 1)
#set_property(TARGET scenelib PROPERTY POSITION_INDEPENDENT_CODE 1)
#set_property(TARGET managerslib PROPERTY POSITION_INDEPENDENT_CODE 1)
#set_property(TARGET utilslib PROPERTY POSITION_INDEPENDENT_CODE 1)
#set_property(TARGET gui2lib PROPERTY POSITION_INDEPENDENT_CODE 1)
#add_library(blunted2 SHARED ${CORE_SOURCES} ${CORE_HEADERS} ${OWN_LIBRARIES})
#set_target_properties(blunted2 PROPERTIES VERSION ${VERSION} SOVERSION ${VERSION_MAJOR} )
#add_library(blunted2 SHARED ${CORE_SOURCES} ${LIBS_SOURCES} ${CORE_HEADERS} ${ALL_LIBS_HEADERS} ${OWN_LIBRARIES})
# For static library.

add_library(blunted2 ${CORE_SOURCES} ${LIBS_SOURCES} ${CORE_HEADERS}
   ${ALL_LIBS_HEADERS} ${OWN_LIBRARIES})
# Install our headers, keeping directory structure (if they are separated from
# the sources would be easier, but separate them would be too disruptive to
# merge on the original repository).
install(FILES ${CORE_HEADERS} DESTINATION include/blunted2)
install(FILES ${BASE_HEADERS} DESTINATION include/blunted2/base)
install(FILES ${BASE_GEOMETRY_HEADERS} DESTINATION 
   include/blunted2/base/geometry)
install(FILES ${BASE_MATH_HEADERS} DESTINATION include/blunted2/base/math)

install(FILES ${SYSTEMS_COMMON_HEADERS} DESTINATION include/blunted2/systems)

install(FILES ${SYSTEMS_GRAPHICS_HEADERS} DESTINATION
   include/blunted2/systems/graphics)
install(FILES ${SYSTEMS_GRAPHICS_OBJECTS_HEADERS} DESTINATION
   include/blunted2/systems/graphics/objects)
install(FILES ${SYSTEMS_GRAPHICS_RESOURCES_HEADERS} DESTINATION
   include/blunted2/systems/graphics/resources)
install(FILES ${SYSTEMS_GRAPHICS_RENDERING_HEADERS} DESTINATION
   include/blunted2/systems/graphics/rendering)

install(FILES ${SYSTEMS_AUDIO_HEADERS} DESTINATION
   include/blunted2/systems/audio)
install(FILES ${SYSTEMS_AUDIO_OBJECTS_HEADERS} DESTINATION
   include/blunted2/systems/audio/objects)
install(FILES ${SYSTEMS_AUDIO_RESOURCES_HEADERS} DESTINATION
   include/blunted2/systems/audio/resources)
install(FILES ${SYSTEMS_AUDIO_RENDERING_HEADERS} DESTINATION
   include/blunted2/systems/audio/rendering)

install(FILES ${LOADERS_HEADERS} DESTINATION include/blunted2/loaders)
install(FILES ${TYPES_HEADERS} DESTINATION include/blunted2/types)
install(FILES ${FRAMEWORK_HEADERS} DESTINATION include/blunted2/framework)
install(FILES ${MANAGERS_HEADERS} DESTINATION include/blunted2/managers)

install(FILES ${SCENE_HEADERS} DESTINATION include/blunted2/scene)
install(FILES ${SCENE2D_HEADERS} DESTINATION include/blunted2/scene/scene2d) 
install(FILES ${SCENE3D_HEADERS} DESTINATION include/blunted2/scene/scene3d) 
install(FILES ${SCENE_OBJECTS_HEADERS} DESTINATION 
   include/blunted2/scene/objects)
install(FILES ${SCENE_RESOURCES_HEADERS} DESTINATION 
   include/blunted2/scene/resources)

install(FILES ${UTILS_HEADERS} DESTINATION include/blunted2/utils)
install(FILES ${UTILS_EXT_HEADERS} DESTINATION 
   include/blunted2/utils/animationextensions)

install(FILES ${UTILS_GUI2_HEADERS} DESTINATION
   include/blunted2/utils/gui2) 
install(FILES ${UTILS_GUI2_WIDGETS_HEADERS} DESTINATION
      include/blunted2/utils/gui2/widgets)

install(FILES ${LIBS_HEADERS} DESTINATION include/blunted2/libs)
install(FILES ${LIBS_FASTEVENTS_HEADERS} DESTINATION
   include/blunted2/libs/fastevents)
install(FILES ${LIBS_SQLITE3_HEADERS} DESTINATION
   include/blunted2/libs/sqlite3) 
install(FILES ${LIBS_GLEE_HEADERS} DESTINATION include/blunted2/libs/glee)


add_executable(gameplayfootball WIN32 ${CORE_SOURCES} ${CORE_HEADERS})
target_link_libraries(gameplayfootball ${LIBRARIES})

