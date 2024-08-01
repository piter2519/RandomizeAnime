#pragma once
    #include <iostream>
    #include <curl/curl.h>
    #include <string>
    #include <fstream>

    struct FileData {
        std::ofstream* file;
    };

    std::string get_token_from_file(const std::string& filepath);
    std::string get_list_from_site_with_token(const std::string& client_id, const std::string& userName);
    std::string get_list_from_site_with_token(const std::string& client_id, const std::string& userName, const std::string& url);
    bool curl_save_img(const std::string& url, std::string filePath);

