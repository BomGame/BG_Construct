#include <dirent.h>
#include <sys/types.h>

static void list_dir(const char *path, std::vector<std::string> &v)
{
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        return;
    }

    v.clear();

    while ((entry = readdir(dir)) != NULL) {
        if((std::string)entry->d_name == "." || (std::string)entry->d_name == "..")
        {}
        else
          v.push_back(entry->d_name);
    }

    closedir(dir);
}
