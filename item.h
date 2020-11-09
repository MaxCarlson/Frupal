#pragma once
#include <string>

class Item
{
protected:
    char ch;
    std::string name;

public:
    Item() = default;    

    Item(char ch, std::string name) :
        ch{ch}, name{name}
    {}

    virtual ~Item() = default;

    char getCh() const { return ch; }
    std::string getName() const { return name; }
};