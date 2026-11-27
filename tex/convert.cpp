#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main(int argc, char *argv[]){
    std::string source = ".webp", dest = ".png";
    if (argc >= 2){
        if (argv[1][0] == '.') source = std::string(argv[1]);
        else source = "." + std::string(argv[1]);
        if (argv[2][0] == '.') dest = std::string(argv[2]);
        else dest = "." + std::string(argv[2]);
    }
    fs::path root = fs::current_path();
    std::cout << "Converting all files in \"" << root.string() << "\" from \"" << source << "\" to \"" << dest << "\".\n";
    int converted = 0;
    for (auto const& entry : fs::directory_iterator{root}){
        if (!entry.exists() || entry.is_directory()) continue;
        const fs::path path = entry.path();
        if (path.extension() == source){
            fs::path targetPath = fs::path(path.parent_path().string() + "/" + path.stem().string() + dest);
            std::string command = "ffmpeg -i \"" + path.string() + "\" \"" + targetPath.string() + "\"";
            system(command.c_str());
            fs::remove(path);
            converted++;
        }
    }
    std::cout << "\nDone! Converted " << converted << " files.\n";

    return 0;
}
