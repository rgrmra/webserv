#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

#include "AFile.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <dirent.h>
#include <algorithm>

class AutoIndex : public AFile {
    private:
        std::string _path;
        std::string _uri;
        std::string _content;

    public:
        AutoIndex(const std::string &path, const std::string &uri);
        AutoIndex(const AutoIndex &src);
        AutoIndex &operator=(const AutoIndex &rhs);
        ~AutoIndex(void);

        virtual bool empty(void) const;
        std::string getBuffer(size_t bytes);

    private:
        void generateContent(void);
};

#endif /* AUTOINDEX_HPP */