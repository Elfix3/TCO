#Version de CMAKE requise
cmake_minimum_required(VERSION 3.16)

#Nom du projet
project(TCO)

#Langage utilisé
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Trouver Qt6 avec Serial Port
find_package(Qt6 REQUIRED COMPONENTS Widgets SerialPort) 


#Ajoute les répertoires sources

file(GLOB_RECURSE HEADERS "headers/*.h")
file(GLOB_RECURSE HEADER_DIRS LIST_DIRECTORIES true headers/*)

file(GLOB_RECURSE SOURCES "sources/*.cpp")
file(GLOB_RECURSE SOURCE_DIRS LIST_DIRECTORIES true sources/*)

file(GLOB UI "ui/*.ui")

file(GLOB RESOURCES "resources/*.qrc")

# Utilise qt6_add_resources pour générer les fichiers nécessaires
qt6_add_resources(QRC_SOURCES ${RESOURCES})

# Utilise qt6_wrap_ui pour générer les fichiers nécessaires
qt6_wrap_ui(UI_SOURCES ${UI})


#AUTO
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#set(CMAKE_AUTOUIC ON) 


add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${UI_SOURCES} ${QRC_SOURCES}) 

foreach(_headerDir ${HEADER_DIRS})
    if(IS_DIRECTORY ${_headerDir})
        target_include_directories(${PROJECT_NAME} PRIVATE ${_headerDir})
    endif()
endforeach()

foreach(_sourceDir ${SOURCE_DIRS})
    if(IS_DIRECTORY ${_sourceDir})
        target_include_directories(${PROJECT_NAME} PRIVATE ${_sourceDir})
    endif()
endforeach()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::SerialPort)
target_include_directories(${PROJECT_NAME} PRIVATE headers ${CMAKE_CURRENT_BINARY_DIR})