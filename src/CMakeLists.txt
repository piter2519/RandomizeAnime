#dodanie nagłówków do projektu
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} main.cpp curl_http.cpp func.cpp anime.cpp)


target_link_libraries(${PROJECT_NAME}
    -L${CURL_LIB_DIR}
    -lcurl
    -lgdiplus)

target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)

add_custom_command(TARGET ${PROJECT_NAME}
                   POST_BUILD
                   COMMAND
                     "${PROJECT_SOURCE_DIR}\\dep\\mt\\mt.exe" -manifest \"${PROJECT_SOURCE_DIR}\\main.exe.manifest\" -outputresource:"${PROJECT_SOURCE_DIR}\\RandomizeAnime.exe"\;1
                   COMMENT
                     "Adding custom manifest...")
