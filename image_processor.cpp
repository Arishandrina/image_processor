#include "image.h"
#include "filter_controller.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage:" << std::endl;
        std::cout << "image_processor {path to the input file} {path to the output file} \n"
                     "\t[-{filter name 1} [filter parameter 1] [filter parameter 2] ...] \n"
                     "\t[-{filter name 2} [filter parameter 1] [filter parameter 2] ...] ... \n\n"
                     "Filters: \n"
                     "\tCrop (-crop width height): crops the top left of the image to the given width and height \n"
                     "\tGrayscale (-gs): converts the image to grayscale \n"
                     "\tNegative (-neg): converts the image to negative \n"
                     "\tSharpening (-sharp): increases image sharpening \n"
                     "\tEdge Detection (-edge threshold): selects boundary and converts the image to grayscale \n";
        return 0;
    }
    const char* file_to_read = argv[1];
    const char* file_to_write = argv[2];

    Initialazation img(file_to_read);
    FilterController controller;
    auto filters = FilterController::SepFilters(argc, argv);
    if (!filters.empty()) {
        for (auto& filter : filters) {
            std::vector<std::string> params;
            for (size_t i = 1; i < filter.size(); ++i) {
                params.push_back(filter[i]);
            }
            FilterController::CallFilter(img, filter[0], params);
        }
    }
    img.WriteImage(file_to_write);
    return 0;
}