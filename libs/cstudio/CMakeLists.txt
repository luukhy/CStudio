cmake_minimum_required(VERSION 3.10)
project(cstudio_lib C)

add_library(cstudio_lib
    audioAPI/audio.c
    formatters/wav.c
)

target_include_directories(cstudio_lib PUBLIC 
    audioAPI
    formatters
)