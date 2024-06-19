#include <FileIterator.hpp>
#include <fstream>

FileIterator::FileIterator(const std::string& filepath) : file(filepath), path(filepath)
{
    // if(!file.is_open())
    //     std::cout << "Unable to open file" << std::endl;
    ++(*this);
}

FileIterator& FileIterator::operator++()
{
    this->prev2Line_ = this->prev_;
    this->prev_ = this->str_;

    if(std::getline(file, str_))
    {
        return *this;
    } else {
        file.close();
        str_.clear();
        return *this;
    }
}

FileIterator& FileIterator::operator--()
{
    file.clear(); //clear any previous error flags
    file.seekg(0, std::ios::beg); //Move to the beginning of the file
    str_ = "";
    prev_ = prev2Line_;

    if(prev2Line_.empty()) {
        throw std::runtime_error("No previous lines available");
    }
    prev2Line_.clear();
    return *this;
}

FileIterator& FileIterator::operator=(const FileIterator& other)
{
    if(this != &other) {
        if(file.is_open()) {
            file.close();
        }
        file.close();
        file.open(other.path);

        str_.clear();
        prev2Line_.clear();
        prev_.clear();
    }
    return *this;
}

const std::string& FileIterator::operator*() const 
{
    return str_;
}

bool FileIterator::operator!=(const FileIterator& other) const 
{
    return file.is_open() || other.file.is_open();
}

bool FileIterator::operator!() const 
{
    return !file.eof();
}

FileIterator::operator bool() const 
{
    return !file.eof();
}

bool FileIterator::operator()() const
{
    std::cout << "in" << std::endl;
    return !file.eof();
}

FileIterator::~FileIterator()
{
}
