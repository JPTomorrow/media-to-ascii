#ifndef ASCII_COVERSION_H
#define ASCII_COVERSION_H

#include <chrono>
#include <vector>
#include <map>
#include <thread>

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

    inline std::string frame_to_string(const AsciiFrame& frame) {
        std::string result;
        result.reserve(frame.data.size() + (frame.data.size() / frame.line_offset) + 1);

        size_t offset = 0;
        for (char c : frame.data) {
            result.push_back(c);
            offset++;
            if (offset >= frame.line_offset) {
                result.push_back('\n');
                offset = 0;
            }
        }
        return result;
    }

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

    inline void console_draw_multiple_frames_double_buffered(const std::vector<AsciiFrame>& frames, double framerate) {
        if (frames.empty() || framerate <= 0) {
            return;
        }

        // std::ios_base::sync_with_stdio(false);

        auto frame_duration = std::chrono::duration<double>(1.0 / framerate);

        // Pre-convert frames
        std::vector<std::string> frame_strings;
        for (const auto& frame : frames) {
            frame_strings.push_back(frame_to_string(frame));
        }

        // Hide cursor and save initial position
        std::cout << "\033[?25l";  // Hide cursor
        std::cout << "\033[s";     // Save cursor position
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << std::flush;

        for (size_t i = 0; i < frames.size(); ++i) {
            auto start_time = std::chrono::high_resolution_clock::now();

            // Always return to saved position (beginning of animation area)
            std::cout << "\033[u";  // Restore cursor position
            std::cout << "\033[J";  // Clear from cursor to end of screen

            // Draw frame
            std::cout << frame_strings[i] << std::flush;

            // Frame timing
            auto end_time = std::chrono::high_resolution_clock::now();
            auto elapsed = end_time - start_time;

            if (elapsed < frame_duration) {
                std::this_thread::sleep_for(frame_duration - elapsed);
            }
        }

        // Show cursor
        std::cout << "\033[?25h" << std::flush;
        std::cout << std::endl;

        // std::ios_base::sync_with_stdio(true);
    }
}

#endif //ASCII_COVERSION_H
