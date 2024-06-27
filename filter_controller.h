#pragma once

#include <vector>
#include "image.h"

class FilterController {
public:
    static std::vector<std::vector<std::string>> SepFilters(int argc, char **argv);

    static std::pair<int, std::vector<std::string>> CurrentFilter(int, char **, int);
    static void CallFilter(Initialazation &, const std::string &, const std::vector<std::string> &);

private:
    std::vector<ImageFilters *> filters_;
};