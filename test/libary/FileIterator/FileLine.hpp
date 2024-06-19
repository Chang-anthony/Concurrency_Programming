#ifndef _FILELINE_H_
#define _FILELINE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class FileIterator;

class FileLine
{
private:
    std::string path;
public:
    FileLine(const std::string& path);
    ~FileLine();

    FileIterator begin() const;
    FileIterator end() const;
};


#endif /* _FILELINE_H_ */
