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
        offsetX = x - halfCamX - 1;

    // Right edge case
    if(x > map->getWidth() - halfCamX)
        offsetX = map->getWidth() - cols;

    // Top case
    if(y > halfCamY)
        offsetY = y - halfCamY - 1;

    // Bottom case
    if(y > map->getHeight() - halfCamY)
        offsetY = map->getHeight() - lines;
}