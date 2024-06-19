#pragma once
#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <codecvt>
#include <locale>
#include <algorithm>
#include <cmath>
#include <set>


class CustomException : public std::exception{
public:
    CustomException(const std::wstring message) 
        :msg(ConvertToString(message))
    {
        // msg = std::string( msg.begin(), msg.end());	
        // auto convert_tostring = [](const std::wstring& input)
        // {
        //     std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		//     return converter.to_bytes(input);
        // };
        // msg = convert_tostring(message);
    }

    const std::string what()
    {
        return msg;
    }

private:
    // convert string to wstring
	inline std::wstring ConvertToWstring(const std::string& input)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.from_bytes(input);
	}
	// convert wstring to string 
	inline std::string ConvertToString(const std::wstring& input)
	{
		//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(input);
	}

    std::string msg;
};

class utils 
{
private:
    /* data */
public:
    utils(/* args */){};
    ~utils(){};

    template<typename T>
    static T* RequireNonNull(T* object)
    {
        if(object == nullptr)
            throw std::invalid_argument("object can't be null.");
        return object;
    }


    template<typename T>
    static std::vector<T*> RemoveObjectFromVector(std::vector<T*> V,T* target)
    {
        V.erase(std::remove_if(V.begin(),V.end(),[target](T* v)
        {
            return v == target;
        }),V.end());

        return V;
    }


    template<typename T>
    static std::vector<T*> SetToVector(std::set<T*> set)
    {
        std::vector<T*> v = std::vector<T*>();
        for(auto it:set)
            v.push_back(it);
        return v;

        // auto tovector = [](std::set<T*> set)
        // {
        //     std::vector<T*> v = std::vector<T*>();
        //     for(auto it:set)
        //         v.push_back(it);
        //     return v;
        // };
        // return tovector(set);
    }

    template<typename T>
    static std::vector<T*> DeleteVal(T* val,std::vector<T*> vector)
    {
        vector.erase(std::remove_if(vector.begin(),vector.end(),[val](T* remove){
            return remove == val;
        }),vector.end());

        return vector;
    }



    template<typename T>
    static void ShouldBePositive(T val)
    {
        if(val <= 0)
            throw std::invalid_argument("val must be positive.");
    }

    static void LengthShouldBe(std::string str,int min,int max)
    {
        if(str.size() > max || str.size() < min)
        {
            std::ostringstream oss;//   透過 ostringstream 來建立一個整數字符串
            oss << str <<" size" << " should be " << min << " ~ " << max << ".";
            throw std::invalid_argument(oss.str());
        }
    }

    static void LengthShouldBe(std::wstring str,int min,int max)
    {
        if(str.size() > max || str.size() < min)
        {
            std::wostringstream woss;//   透過 ostringstream 來建立一個整數字符串
            woss << str << L" size" << L" should be " << min << L" ~ " << max << L".";
            // const wchar_t* foo = woss.str().c_str();
            CustomException except = CustomException(woss.str());
            throw std::invalid_argument(except.what());
            
            //std::wcout << str << std::endl;
            //std::wcout << except.what() << std::endl;
            //throw except.what();
            //throw except;
        } 
    }

    static int HandleInput(int min,int max)
    {
        std::cout << "請選擇數值介於" << min << "~" << max  << "之間!!!"<< std::endl;
        int input;
        std::cin >> input;
        if( input < min || input > max)
        {
            return HandleInput(min,max);
        }
        else
        {
            return input;
        }
    }

    static std::vector<int> InputMultipleNums()
    {
        std::set<int> numbers;
        std::string input = "";

        while(input.size() == 0)
            std::getline(std::cin, input);

        std::istringstream stream(input);
        int num;

        //read numbers from string stream
        while (stream >> num)
        {
            /* code */
            numbers.insert(num);
        }
        std::vector<int> results(numbers.begin(), numbers.end());

        return results;
    }

    template<typename T>
    static void ValShouldBigger(T val,int min)
    {
        if(val < min)
        {
            std::ostringstream oss;//透過 ostringstream 來建立一個整數字符串
            oss << "val " << " should bigger than " << min << ".";
            throw std::invalid_argument(oss.str());
        }
    }

    template<typename T>
    static void ValShouldBe(T val,int min,int max)
    {
        if(val > max || val < min)
        {
            std::ostringstream oss;//   透過 ostringstream 來建立一個整數字符串
            oss <<" val" << " should be " << min << " ~ " << max << ".";
            throw std::invalid_argument(oss.str());
        }
    }

    template<typename T>
    static void SizeShouldBe(const std::vector<T>& arr,int val)
    {
        if(arr.size() != val)
            throw std::invalid_argument("arr size should be val");
    }

    template<typename T>
    static void SizeShouldBigger(const std::vector<T>& arr,int val)
    {
        if(arr.size() < val)
            throw std::invalid_argument("arr size should bigger than val");
    }

    template<typename T>
    static void SizeShouldSmaller(const std::vector<T>& arr,int val)
    {
        if(arr.size() > val)
            throw std::invalid_argument("arr size should Smaller than val");
    }


    template<typename T>
    static void ArrayShouldNotBeEmpty(const std::vector<T>& arr)
    {
        if(arr.empty())
            throw std::invalid_argument("arr can't be empty");
    }

    template<typename T>
    static std::string ToString(const T& val)
    {
        auto convert = [](const T& val)
        {
            std::ostringstream oss;
            oss << val;
            return oss.str();
        };
        return convert(val);
    }

    template<typename T>
    static std::string ToString(const std::vector<T>& arr)
    {
        auto convert = [](const T& val)
        {
            std::ostringstream oss;
            oss << val;
            return oss.str();
        };
        
        std::string result = "[";
        for(const T& element : arr)
            result += convert(element) + " , ";
        if(!arr.empty())
            result = result.substr(0,result.size() - 3);//移除最後兩個空格與逗號
        result += "]";
        return result;
    }

};

#endif