#include <FileLine.hpp>
#include <FileIterator.hpp>

FileLine::FileLine(const std::string& path) : path(path)
{
}

// 後面 add const 表示 這個是一個 const 的函數，意即class內部屬性不會被更改
FileIterator FileLine::begin() const 
{
    return FileIterator(path);
}

FileIterator FileLine::end() const
{
    return FileIterator("");
}

FileLine::~FileLine()
{
}