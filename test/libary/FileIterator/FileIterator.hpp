#ifndef _FILEITERATOR_H_
#define _FILEITERATOR_H_

#include <iostream>
#include <fstream>
#include <string>


class FileIterator
{
private:
    std::ifstream file;
    std::string str_;

    std::string path;
public:
    FileIterator(const std::string& filepath);
    ~FileIterator();

    //FileIterator& operator++(int); right incrementing
    FileIterator& operator++();
    FileIterator& operator--();
    FileIterator& operator=(const FileIterator& other);
    const std::string& operator*() const;
    bool operator!=(const FileIterator& other) const;
    
    bool operator!() const;
    explicit operator bool() const;
    bool operator()() const;

protected:
    std::string prev_; // prev one line
    std::string prev2Line_; // prev 2 line 
};

#endif /* _FILEITERATOR_H_ */
