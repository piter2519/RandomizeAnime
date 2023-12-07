#include "anime.hpp"


Anime::Anime()
    : id(-1), title("Error"), picture_url("")
    {}
Anime::Anime(std::string err)
    : id(-2), title("Error"), picture_url(err)
    {}
Anime::Anime(nlohmann::json data)
    : id(data["id"]), title(data.at("title")), picture_url(data["main_picture"]["medium"]), status(data.at("status")), start_date(data.at("start_date")), 
    end_date(data.at("end_date")), score(data.at("mean")), popularity(data.at("popularity")), nsfw(data.at("nsfw")), rating(data.at("rating"))
    {
        for(nlohmann::json g : data.at("genres"))
        {
            genres.insert(std::make_pair(g.at("id"), g.at("name")));
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
