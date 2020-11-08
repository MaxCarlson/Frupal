#include "camera.h"
#include "map.h"
#include "player.h"

void Camera::setOffsets(const Player& player)
{
    auto [x, y] = player.getXY();

    int halfCamX = cols  / 2;
    int halfCamY = lines / 2;

    // Left edge case
    if(x > halfCamX)
        offsetX = x - halfCamX;

    // Right edge case
    if(x > cols - halfCamX)
        offsetX = cols - halfCamX;

    // Top case
    if(y > halfCamY)
        offsetY = y - halfCamY;

    // Bottom case
    if(y > lines - halfCamY)
        offsetY = lines - halfCamY;
}