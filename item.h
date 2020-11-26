#pragma once
#include <string>
#include <tuple>

class Item
{
protected:
    char ch;
    std::string name;

    using StringTuple = std::tuple<std::string,std::string,std::string>; 

public:
    Item() = default;    

    Item(char ch, std::string name) :
        ch{ch}, name{name}
    {}
    virtual ~Item() = default;

    char getCh() const { return ch; }
    std::string getName() const { return name; }

    virtual std::tuple<std::string,std::string,std::string> getDescription() const{return std::tuple<std::string,std::string,std::string>{};}
};
