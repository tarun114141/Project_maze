#ifndef SIMPLE_RECORDER_H
#define SIMPLE_RECORDER_H

#include <GL/glut.h>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <iostream>

// Platform-specific directory creation
#if defined(_WIN32)
    #include <direct.h>
    #define MAKE_DIR(d) _mkdir(d)
#else
    #include <sys/stat.h>
    #define MAKE_DIR(d) mkdir(d, 0777)
#endif

inline void saveAnimationFrame(
    int width,
    int height,
    const std::string& folderName = "frames"
)
{
    static int frameCounter = 0;
    static bool folderChecked = false;

    if (!folderChecked)
    {
        MAKE_DIR(folderName.c_str());
        folderChecked = true;
    }

    std::vector<unsigned char> pixels(width * height * 3);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    glReadPixels(
        0,
        0,
        width,
        height,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        pixels.data()
    );

    std::stringstream ss;

    ss << folderName
       << "/frame"
       << std::setw(5)
       << std::setfill('0')
       << frameCounter++
       << ".ppm";

    std::ofstream file(
        ss.str(),
        std::ios::binary
    );

    if (!file)
    {
        std::cerr
            << "Recorder Error: Failed to open "
            << ss.str()
            << "\n";

        return;
    }

    file << "P6\n"
         << width
         << " "
         << height
         << "\n255\n";

    for (int y = height - 1; y >= 0; y--)
    {
        file.write(
            reinterpret_cast<char*>(
                &pixels[y * width * 3]
            ),
            width * 3
        );
    }

    file.flush();
    file.close();
}

#endif // SIMPLE_RECORDER_H