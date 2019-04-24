//
// Created by mjonsson on 4/24/19.
//

#ifndef EXPERIMENTAL_FILESTRUCTURE_H
#define EXPERIMENTAL_FILESTRUCTURE_H

#include <string>
#include <vector>
#include <map>
#include <list>

class FileStructure {
private:
    const std::string m_id = "titleId";
    const std::string m_title = "title";
    const std::string m_region = "region";
    enum POSITION {TITLEID, TITLE, REGION, GENRE, STARTYEAR, ENDYEAR, WRITERS, DIRECTORS, ACTORS, RATING};

    template<typename T> struct map_init_helper
    {
        T& data;

        explicit map_init_helper(T& d) : data(d) {}
        map_init_helper& operator() (typename T::key_type const& key, typename T::mapped_type const& value)
        {
            data[key] = value;
            return *this;
        }
    };

    template<typename T> map_init_helper<T> map_init(T& item)
    {
        return map_init_helper<T>(item);
    }
public:

    FileStructure() {
        map_init(m_Columns)
                 (m_id, POSITION::TITLEID)
                 (m_title,POSITION::TITLE)
                 (m_region, POSITION::REGION)
        ;
    }

    std::map<std::string, POSITION> m_Columns;
    std::vector<std::pair<std::string, std::string>> m_colData;
    std::list<std::vector<std::pair<std::string,std::string>>> m_listResult;

};

#endif //EXPERIMENTAL_FILESTRUCTURE_H
