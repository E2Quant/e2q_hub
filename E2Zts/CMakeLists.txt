#---------------------------
# zts library
#---------------------------


find_library(ZTS_LIBRARY_TRADER NAMES libxtptraderapi.${LIB_EXT}  HINTS "${PROJECT_SOURCE_DIR}/hub/E2Zts/api") 

if(NOT ZTS_LIBRARY_TRADER)
    message (FATAL_ERROR "Cannot find zts lib install url: https://xtp.zts.com.cn/ ")   
else()
    add_compile_definitions( ZTS_TRADER )

    set(INCLUDE_DIRS_ZTS "${PROJECT_SOURCE_DIR}/hub/E2Zts/api")

    add_library(E2Zts
        STATIC

         "${PROJECT_SOURCE_DIR}/hub/E2Zts/TraderSpi.cpp" 
         "${PROJECT_SOURCE_DIR}/hub/E2Zts/E2Zts.cpp" 
         "${PROJECT_SOURCE_DIR}/hub/E2Zts/TraderEvent.cpp" 
         "${PROJECT_SOURCE_DIR}/hub/E2Zts/FileUtils.cpp" 
         "${PROJECT_SOURCE_DIR}/hub/E2Zts/ZtsMatch.cpp" 

        ${ZTS_LIBRARY_TRADER}
        )

    target_include_directories(E2Zts 
        PUBLIC 

        /usr/local/include/e2
        ${PROJECT_SOURCE_DIR}/include

        ${CMAKE_CURRENT_LIST_DIR}

        ${INCLUDE_DIRS_ZTS}
        )

    target_link_libraries(E2Zts PUBLIC 
        ${ZTS_LIBRARY_TRADER}
        )


endif()


