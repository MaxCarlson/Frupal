#include "camera.h"
#include "map.h"
#include "player.h"

void Camera::setOffsets(const Player& player, const Map& map)
{
    auto [x, y]             = player.getXY();
    auto [width, height]    = map.getXY();

    int halfCamX = cols  / 2;
    int halfCamY = lines / 2;

    // Left edge case
    if(x > halfCamX)
        offsetX = x - halfCamX - 1;

    // Right edge case
    if(x > width - halfCamX)
        offsetX = width - cols;



    // Y axis seems off-center with camera

    // Top case
    if(y > halfCamY)
        offsetY = y - halfCamY - 1;

    // Bottom case
    if(y > height - halfCamY)
        offsetY = height - lines;
}