#include <iostream>
#include <random>
#include "curl_http.h"
#include "func.hpp"
#include <algorithm>
#include <chrono>


std::vector<Anime> returnTableOfAnime(const std::string& client_id, const std::string& userName)
{
    std::string resp = get_list_from_site_with_token(client_id, userName);
    std::string err;
    nlohmann::json j = nlohmann::json::parse(resp);
    if(j["error"] == "not_found")
    {
        std::cout<<"zly nick"<<std::endl;
        std::vector<Anime> temp;
        std::cout<<j["error"]<<std::endl;
        err = j["error"];
        temp.emplace_back(Anime(err));
        return temp;
    }
    std::vector<Anime> animeList;
    animeList.reserve(1000);
    // std::cout<<std::setw(4)<<j<<std::endl;
    for(nlohmann::json data : j["data"])
    {
        if(data["node"]["status"] == "finished_airing")
            animeList.emplace_back(Anime(data["node"]));
    }
    animeList.shrink_to_fit();
    return animeList;
}


void ShuffleVect(std::vector<Anime>& vect)
{
    static std::default_random_engine rEng;
    static unsigned long int time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    rEng.seed(time);
    static std::mt19937 gen(rEng());
    std::shuffle(vect.begin(), vect.end(), gen);
}

    // for(nlohmann::json data : j["data"])
    // {
    //     animeList.emplace_back(Anime(data["node"]));
    // }
    // for(Anime jakisAnimiec : animeList)
    // {
    //     jakisAnimiec.printAllData();
    // }
    // std::ofstream fileImg("res/img/image.png", std::ostream::binary);
    // if(curl_save_img(animeList[0].picture_url, fileImg))
    //     std::cout<<"Success!"<<std::endl;
    // fileImg.close();