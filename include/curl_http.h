#pragma once
    #include <iostream>
    #include <curl/curl.h>
    #include <string>
    std::string get_token_from_file(const std::string& filepath);
    std::string get_list_from_site_with_token(const std::string& client_id, const std::string& userName);
    bool curl_save_img(const std::string& url, std::ostream& os);