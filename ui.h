#pragma once

class Display;
class Player;
class Camera;

class UI
{
public:

    static constexpr int cols = 25;


    void print(Display& display, const Player& player, const Camera& camera);

private:

    void printOutline(Display& display, const Camera& camera);
};