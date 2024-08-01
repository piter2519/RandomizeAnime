#include "anime.hpp"


Anime::Anime()
    : id(-1), title("Error"), picture_url("")
    {}
Anime::Anime(std::string err)
    : id(-2), title("Error"), picture_url(err)
    {}
Anime::Anime(nlohmann::json data)
    : id(data.at("id")), title(data.at("title")), picture_url(data["main_picture"]["medium"]), status(data.at("status")), 
    popularity(data.at("num_list_users")), nsfw(data.at("nsfw")), mediaType(data.at("media_type"))
    {
        if(data.contains("num_episodes"))
            numOfEp = data.at("num_episodes");
        if(data.contains("mean"))
            score = data.at("mean");
        if(data.contains("rating"))
            rating = data.at("rating");

        if(data.contains("genres"))
        for(nlohmann::json g : data.at("genres"))
        {
            genres.insert(std::make_pair(g.at("id"), g.at("name")));
            // std::cout<<id<<std::endl;
        }
        if(data.contains("start_season"))
        {
            if(data.at("start_season").contains("year"))
                year = data.at("start_season").at("year");
            if(data.at("start_season").contains("season"))
                season = data.at("start_season").at("season");
        }
        // for(auto p : genres)
        // std::cout<<p.first<<" "<<p.second<<std::endl;
    }
void Anime::printAllData()
{
    std::cout<< "Title: " << title << std::endl;
    std::cout<< "id: " << id << std::endl;
    std::cout<< "picture: " << picture_url << std::endl;
}

bool Anime::genreExist(uint16_t genreId)
{
    auto genre = genres.find(genreId);
    if (genre == genres.end())
        return 0;
    return 1;
}
int Anime::returnId()
{
    return id;
}

std::string Anime::stringOfGenres()
{
    std::string temp = "";
    bool first = true;
    for(auto p : genres)
    {
        if(!first)
            temp += ", ";
        first = false;
        temp += p.second;
    }
    return temp;
}

std::string Anime::stringOfSeasonData()
{
    std::string temp = "";
    if(season != "unknown");
        temp += season;
    if(year == 0)
        if(temp == "unknown")
            return temp;
        else
        {
            temp = std::to_string(year);
            return temp;
        }
    temp += " ";
    temp += std::to_string(year);
    return temp;

}