include(FetchContent)

macro(fetch_dependencies src)
    file(READ ${src} MY_JSON_STRING)
    string(JSON length_of_array LENGTH "${MY_JSON_STRING}")

    math(EXPR COUNT "${length_of_array}-1")
    foreach(IDX RANGE ${COUNT})
        string(JSON CUR_NAME GET ${MY_JSON_STRING} ${IDX} name)
        string(JSON CUR_URL GET ${MY_JSON_STRING} ${IDX} url)
        string(JSON CUR_TAG GET ${MY_JSON_STRING} ${IDX} tag)

        get_property(already_declared GLOBAL PROPERTY ${CUR_NAME}_DEPENDENCY_DECLARED DEFINED)
        if(NOT already_declared)
            message(STATUS "using ${CUR_NAME} from ${CUR_URL}:${CUR_TAG}")
            define_property(GLOBAL PROPERTY ${CUR_NAME}_DEPENDENCY_DECLARED BRIEF_DOCS "dependency declared" FULL_DOCS "dependency declared")
            set_property(GLOBAL PROPERTY ${CUR_NAME}_DEPENDENCY_DECLARED TRUE)
            FetchContent_Declare(
                ${CUR_NAME}
                SOURCE_DIR        ${CMAKE_SOURCE_DIR}/${CUR_NAME}
                GIT_REPOSITORY    ${CUR_URL}
                GIT_TAG           ${CUR_TAG}
            )
        endif()
    endforeach()
endmacro()

macro(add_repository rep)
    get_property(already_populated GLOBAL PROPERTY ${rep}_ALREADY_POPULATED DEFINED)
    if(NOT already_populated)
        message(STATUS "check ${CMAKE_SOURCE_DIR}/${rep} exist...")
        if(EXISTS ${CMAKE_SOURCE_DIR}/${rep} AND NOT FETCH_OVERRIDE)
            message(STATUS "TRUE - not downloading new content")
            define_property(GLOBAL PROPERTY ${rep}_ALREADY_POPULATED BRIEF_DOCS "dependency populated" FULL_DOCS "dependency populated")
            set_property(GLOBAL PROPERTY ${rep}_ALREADY_POPULATED TRUE)
            set(${rep}_DIR ${CMAKE_SOURCE_DIR}/${rep})
        else()
            message(STATUS "FALSE - fetching new content")
            FetchContent_GetProperties(${rep})
                FetchContent_Populate(${rep})
                message(STATUS "fetched ${rep} into ${${rep}_SOURCE_DIR}")
                define_property(GLOBAL PROPERTY ${rep}_ALREADY_POPULATED BRIEF_DOCS "dependency populated" FULL_DOCS "dependency populated")
                set_property(GLOBAL PROPERTY ${rep}_ALREADY_POPULATED TRUE)
                set(${rep}_DIR ${${rep}_SOURCE_DIR})
        endif()
    else()
        message(STATUS "${dep} already populated")
    endif()
endmacro()

option(FETCH_OVERRIDE "force fetch" FALSE)
