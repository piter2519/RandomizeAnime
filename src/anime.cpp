#include "anime.hpp"


Anime::Anime()
    : id(-1), title("Error"), picture_url("")
    {}
Anime::Anime(std::string err)
    : id(-2), title("Error"), picture_url(err)
    {}
Anime::Anime(nlohmann::json data)
    : id(data["id"]), title(data["title"]), picture_url(data["main_picture"]["medium"])
    {}
void Anime::printAllData()
{
    std::cout<< "Title: " << title << std::endl;
    std::cout<< "id: " << id << std::endl;
    std::cout<< "picture: " << picture_url << std::endl;
}
