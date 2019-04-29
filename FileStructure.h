//
// Created by mjonsson on 4/24/19.
//

#ifndef EXPERIMENTAL_FILESTRUCTURE_H
#define EXPERIMENTAL_FILESTRUCTURE_H

#include <string>
#include <vector>
#include <map>
#include <list>
#include "TitleAkas.h"

class FileStructure {
private:

public:
    TitleAkas akas;
    TitleBasics basics;
    std::vector<std::pair<std::string, std::string>> m_colData;
    std::list<std::vector<std::pair<std::string,std::string>>> m_listResult;

};

#endif //EXPERIMENTAL_FILESTRUCTURE_H
