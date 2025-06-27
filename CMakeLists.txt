cmake_minimum_required(VERSION 3.16)
project(Atlas)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    main.cpp
)

# Header files
set(HEADERS
    Animation.h
    Atlas.h
    Bullet.h
    Camera.h
    Chicken.h
    ChickenFast.h
    ChickenMedium.h
    ChickenSlow.h
    Timer.h
    Vector2.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Platform-specific configuration
if(WIN32)
    # Windows specific settings
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
    
    # Set Windows subsystem
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
    
    # Find SDL2 libraries for Windows
    if(CMAKE_CROSSCOMPILING)
        # When cross-compiling, we need to specify the paths manually
        set(SDL2_PATH "${CMAKE_CURRENT_SOURCE_DIR}/SDL2-mingw" CACHE PATH "Path to SDL2 for MinGW")
        set(SDL2_INCLUDE_DIR "${SDL2_PATH}/include")
        set(SDL2_LIBRARY "${SDL2_PATH}/lib/libSDL2.dll.a")
        set(SDL2_DLL "${SDL2_PATH}/bin/SDL2.dll")
        
        set(SDL2_IMAGE_INCLUDE_DIR "${SDL2_PATH}/include")
        set(SDL2_IMAGE_LIBRARY "${SDL2_PATH}/lib/libSDL2_image.dll.a")
        set(SDL2_IMAGE_DLL "${SDL2_PATH}/bin/SDL2_image.dll")
        
        set(SDL2_TTF_INCLUDE_DIR "${SDL2_PATH}/include")
        set(SDL2_TTF_LIBRARY "${SDL2_PATH}/lib/libSDL2_ttf.dll.a")
        set(SDL2_TTF_DLL "${SDL2_PATH}/bin/SDL2_ttf.dll")
        
        set(SDL2_MIXER_INCLUDE_DIR "${SDL2_PATH}/include")
        set(SDL2_MIXER_LIBRARY "${SDL2_PATH}/lib/libSDL2_mixer.dll.a")
        set(SDL2_MIXER_DLL "${SDL2_PATH}/bin/SDL2_mixer.dll")
        
        # Include directories
        target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIR})
        
        # Link libraries
        target_link_libraries(${PROJECT_NAME}
            ${SDL2_LIBRARY}
            ${SDL2_IMAGE_LIBRARY}
            ${SDL2_TTF_LIBRARY}
            ${SDL2_MIXER_LIBRARY}
            -lmingw32
            -lm
        )
        
        # Copy DLLs to output directory
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SDL2_DLL} $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SDL2_IMAGE_DLL} $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SDL2_TTF_DLL} $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SDL2_MIXER_DLL} $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
        
        # Also copy additional DLLs that SDL2_image, SDL2_ttf, and SDL2_mixer might need
        file(GLOB ADDITIONAL_DLLS "${SDL2_PATH}/bin/*.dll")
        foreach(DLL ${ADDITIONAL_DLLS})
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${DLL} $<TARGET_FILE_DIR:${PROJECT_NAME}>
            )
        endforeach()
    endif()
else()
    # Linux specific settings
    find_package(PkgConfig REQUIRED)
    
    # Find SDL2
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
    pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
    
    # Include directories
    target_include_directories(${PROJECT_NAME} PRIVATE 
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${SDL2_MIXER_INCLUDE_DIRS}
    )
    
    # Link libraries
    target_link_libraries(${PROJECT_NAME}
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        ${SDL2_MIXER_LIBRARIES}
        m
    )
    
    # Compile options
    target_compile_options(${PROJECT_NAME} PRIVATE ${SDL2_CFLAGS_OTHER})
endif()

# Copy assets directory to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

# Debug configuration
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)
endif()
