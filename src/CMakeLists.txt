#dodanie nagłówków do projektu
include_directories(${PROJECT_SOURCE_DIR}/include)

# set(imgui_cpp imgui.cpp imgui_impl_opengl2.cpp imgui_impl_sdl2.cpp imgui_demo.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp)

add_executable(${PROJECT_NAME} main.cpp curl_http.cpp func.cpp anime.cpp)


target_link_libraries(${PROJECT_NAME}
    -L${CURL_LIB_DIR}
    -lcurl
    -lgdiplus)

target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)

set(WinKitsVersion "10.0.22621.0")

add_custom_command(TARGET ${PROJECT_NAME}
                   POST_BUILD
                   COMMAND
                     "C:\\Program Files (x86)\\Windows Kits\\10\\bin\\${WinKitsVersion}\\x86\\mt.exe" -manifest \"${PROJECT_SOURCE_DIR}\\main.exe.manifest\" -outputresource:"${PROJECT_SOURCE_DIR}\\RandomizeAnime.exe"\;1
                   COMMENT
                     "Adding custom manifest...")
