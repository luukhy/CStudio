cmake_minimum_required(VERSION 3.28)

project(CStudio)

add_executable(CStudio)
target_sources(CStudio
  PRIVATE
    src/main.c
)

# cstudio library
add_subdirectory(libs/cstudio)

# pipewire
find_package(PkgConfig REQUIRED)
pkg_check_modules(PIPEWIRE REQUIRED libpipewire-0.3)

target_include_directories(CStudio PRIVATE ${PIPEWIRE_INCLUDE_DIRS})
target_link_libraries(CStudio PRIVATE ${PIPEWIRE_LIBRARIES})
target_compile_options(CStudio PRIVATE ${PIPEWIRE_CFLAGS_OTHER})

target_link_libraries(CStudio
    PRIVATE
      cstudio_lib
      ${PIPEWIRE_LIBRARIES}
      m # for math.h
)