#---------------------------
# ta-lib library eg. HUB_talib
#---------------------------

find_library(TA-Lib 
    NAMES ta_lib
    libta_lib.${LIB_EXT} libta-lib.${LIB_EXT} 
    PATHS "/usr/local/lib" "/usr/lib")

if(NOT TA-Lib)
    message (FATAL_ERROR "Cannot find ta_lib install url: https://ta-lib.org/api/ ")   
    set(TALIB_OPTIONS "" TALIB STRING "options for ta-lib")
    set(INCLUDE_DIRS_TALIB "")
else()

    set(INCLUDE_DIRS_TALIB "/usr/local/include/ta-lib")

    message ("Looking ta_lib found")

    add_library(etalib
        STATIC

        ${TA-Lib}
        "${PROJECT_SOURCE_DIR}/hub/etalib/etalib.hpp" 
        "${PROJECT_SOURCE_DIR}/hub/etalib/etalib.cpp" 
        )

    target_include_directories(etalib 
        PUBLIC 

        /usr/local/include/e2
        ${PROJECT_SOURCE_DIR}/include
        ${INCLUDE_DIRS_TALIB}
        ${CMAKE_CURRENT_LIST_DIR}

        )

    target_link_libraries(etalib PUBLIC 
        ${TA-Lib}

        )


endif()


