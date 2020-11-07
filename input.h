#pragma once


class Input
{
    enum Mode
    {
        NORMAL,
        UI
    };

    Mode mode;

public:
    Input() :
        mode{NORMAL}
    {}

    bool input();
};