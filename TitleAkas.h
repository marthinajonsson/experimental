//
// Created by mjonsson on 4/29/19.
//

#ifndef EXPERIMENTAL_TITLEAKAS_H
#define EXPERIMENTAL_TITLEAKAS_H


class TitleAkas {
public:
    const short titleId = 0;
    const short ordering = 1;
    const short title = 2;
    const short region = 3;
    const short language = 4;
};

class TitleBasics {
public:
    const short titleId = 0;
    const short primaryTitle = 2;
    const short originalTitle = 3;
    const short startYear = 5;
    const short endYear = 6;
    const short genre = 8;
};

#endif //EXPERIMENTAL_TITLEAKAS_H
