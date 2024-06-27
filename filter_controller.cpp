#include "filter_controller.h"
#include <iostream>

std::vector<std::vector<std::string>> FilterController::SepFilters(int args, char** argv) {
    int cur_pos = 3;
    std::vector<std::vector<std::string>> filter_sequence;
    while (cur_pos < args) {
        std::pair<int, std::vector<std::string>> current_filter = CurrentFilter(args, argv, cur_pos);
        cur_pos = current_filter.first;
        filter_sequence.push_back(current_filter.second);
    }
    return filter_sequence;
}

std::pair<int, std::vector<std::string>> FilterController::CurrentFilter(int args, char** argv, int cur_pos) {
    std::vector<std::string> filter;
    filter.push_back(argv[cur_pos]);
    ++cur_pos;
    while (cur_pos < args && argv[cur_pos][0] != '-') {
        filter.push_back(argv[cur_pos]);
        ++cur_pos;
    }
    std::pair<int, std::vector<std::string>> ans;
    ans.first = cur_pos;
    ans.second = filter;
    return ans;
}

void FilterController::CallFilter(Initialazation& image, const std::string& filter,
                                  const std::vector<std::string>& parameters) {
    if (filter == "-crop") {
        if (parameters.size() != 2) {
            std::cout << "Incorrect number of parameters were entered for filter {" + filter + "}." << std::endl;
            throw;
        }
        FilterCrop the_filter;
        the_filter.Crop(image, std::stoi(parameters[0]), std::stoi(parameters[1]));
    } else if (filter == "-gs") {
        if (!parameters.empty()) {
            std::cout << "Incorrect number of parameters were entered for filter {" + filter + "}." << std::endl;
            throw;
        }
        FilterGrayscale the_filter;
        the_filter.Grayscale(image);
    } else if (filter == "-neg") {
        if (!parameters.empty()) {
            std::cout << "Incorrect number of parameters were entered for filter {" + filter + "}." << std::endl;
            throw;
        }
        FilterNegative the_filter;
        the_filter.Negative(image);
    } else {
        std::cout << "Filter {" + filter + "} was not found." << std::endl;
        throw;
    }
}