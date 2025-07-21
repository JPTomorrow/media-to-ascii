#ifndef ASCII_COVERSION_H
#define ASCII_COVERSION_H

#include <vector>
#include <map>

namespace ascii {
    typedef struct {
        std::string data;
        size_t line_offset;
    } AsciiFrame;

    inline std::map<float, char> brightness_to_ascii = {
        {0.0f, ' '},    // Darkest - space (black)
        {0.1f, '.'},    // Very dark
        {0.2f, ','},    // Dark
        {0.3f, ':'},    // Dark-medium
        {0.4f, ';'},    // Medium-dark
        {0.5f, '='},    // Medium
        {0.6f, '+'},    // Medium-bright
        {0.7f, '*'},    // Bright
        {0.8f, '#'},    // Very bright
        {0.9f, '%'},    // Nearly white
        {1.0f, '@'}     // Brightest - solid (white)
    };

    // pixel_brightness should be normalized between 0 and 1
    inline char cvt_pixel_to_ascii(float pixel_brightness) {

        auto it = brightness_to_ascii.upper_bound(pixel_brightness);
        if (it != brightness_to_ascii.begin()) {
            --it;
            if (pixel_brightness >= it->first) {
                return it->second;
            } else {
                std::cout << "No value found for the given range." << std::endl;
            }
        }

        return ' ';
    };

    inline void console_draw_frame(const AsciiFrame& frame) {
        std::ios_base::sync_with_stdio(false);

        int offset = 0;
        std::string final;
        for (int i = 0; i < frame.data.size(); i++) {
            final.push_back(frame.data[i]);
            offset++;
            if (offset >= frame.line_offset) {
                final.append("\n");
                offset = 0;
            }
        }

        std::cout << final << "\n";
        std::ios_base::sync_with_stdio(true);
    }
}

#endif //ASCII_COVERSION_H
