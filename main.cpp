

#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <forward_list>

const std::string TITLE = "title";
const std::string TITLE_ID = "titleId";
const std::string TITLE_TYPE = "titleType";
const std::string GENRE = "genre";
const std::string START_YEAR = "startYear";
const std::string END_YEAR = "endYear";
const std::string DIRECTORS = "directors";
const std::string WRITERS = "writes";
const std::string ACTORS = "actors";
const std::string RUNTIME_MINUTES = "runtimeMinutes";
const std::string PARENT_CONST = "parentTconst";
const std::string REGION = "region";
const std::string LANGUAGE = "language";
const std::string EPISODE = "episode";
const std::string SEASON = "season";
const std::string PATH  = "path";
const std::string LIBRARY_ID = "library_id";
const std::string FILENAME = "name";

struct Database {
    std::string m_title = "UNDEF";
    std::string m_titleId = "UNDEF";
    std::string m_titleType = "UNDEF";
    std::string m_genre = "UNDEF";
    std::string m_startYear = "UNDEF";
    std::string m_endYear = "UNDEF";
    std::string m_directors = "UNDEF";
    std::string m_writers = "UNDEF";
    std::string m_actors = "UNDEF";
    std::string m_runtimeMinutes;
    std::string m_region = "UNDEF";
    std::string m_language = "UNDEF";
    std::string m_parentTconst = "UNDEF";
    std::string m_episode = "UNDEF";
    std::string m_season = "UNDEF";
    std::string m_path = "UNDEF";
    std::string m_library_id = "UNDEF";
    std::string m_filename = "UNDEF";
};

template <typename S> std::string* get_begin(S *s)
{
    return (std::string*)s;
}

template <typename S> std::string* get_end(S *s)
{
    return (std::string*)((std::string*)s+sizeof(*s));
}


template <class Object, typename T>
void write (Object obj, T keys) noexcept 
{
    boost::property_tree::ptree root;
    boost::property_tree::ptree parent;
    boost::property_tree::ptree child;

    boost::property_tree::read_json("../data/cache.json", root);
    long size = root.size();
    auto index = std::to_string(size);

    auto key = keys.begin();
    std::string* p = get_begin(&obj);
    parent.put(*key, *p);
    key++;
    p++;

    for (;p < get_end(&obj) && key != keys.end();p++, key++) {
        if ( p == get_begin(&obj)) {

        }
        child.put(*key, *p);
    }

    parent.put_child("additional", child);

    root.push_back(std::make_pair(index, parent));
    boost::property_tree::json_parser::write_json("../data/cache.json", root);
}


int main() {

    std::forward_list<std::string> keys { TITLE, TITLE_ID, TITLE_TYPE, GENRE, START_YEAR, END_YEAR, DIRECTORS,
                                                 WRITERS, ACTORS, RUNTIME_MINUTES, REGION, LANGUAGE, PARENT_CONST,
                                                 EPISODE, SEASON, PATH, LIBRARY_ID, FILENAME };

    Database obj;
    obj.m_title = "test";
    obj.m_titleId = "tt111111";
    obj.m_titleType = "movie";
    obj.m_genre = "drama";
    obj.m_runtimeMinutes = "103";
    obj.m_startYear = "1992";
    obj.m_directors = "name";
    obj.m_path = "/video1/film/title.mp4";

    write<Database, std::forward_list<std::string>> (obj, keys);
    return 0;
}


