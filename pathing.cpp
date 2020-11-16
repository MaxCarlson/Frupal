#include "pathing.h"
#include "map.h"
#include <map>
#include <queue>
#include <set>
#include <unordered_set>
#include <unordered_map>

struct Node
{
    Point p;
    Node() = default;
    Node(Point p) : p{p} {}

    bool operator==(const Node& n) const { return p == n.p; }
};

double dist(const Point& p0, const Point& p1)
{
    return (p0.x - p1.x) * (p0.x - p1.x) + (p0.y - p1.y) * (p0.y - p1.y);
}

struct CompareNodeDist
{
    const Point end;

    CompareNodeDist(Point end) : end{end} {}

    bool operator()(const Node& n0, const Node& n1) const
    {
        return dist(n0.p, end) < dist(n1.p, end);
    }
};

struct CompareNode
{
    bool operator()(const Node& n0, const Node& n1) const
    {
        if(n0.p.x < n1.p.x)
            return true;
        if(n0.p.y < n1.p.y)
            return true;
        return false;
    }
};

namespace std
{
    template<>
    struct hash<Node>
    {
        size_t operator()(const Node& n) const
        {
            return n.p.x + n.p.y * 128;
        }
    };
}


bool Pathing::playerToDiamond(const Map& map, Point player, Point diamond,
     std::vector<Point>& requiredBoats)
{
    std::vector<Point> path = aStar(map, player, diamond, requiredBoats);
    if(!path.empty())
        findRequiredBoatLocs(map, requiredBoats, path);

    return !path.empty();
}

void Pathing::findRequiredBoatLocs(const Map& map, std::vector<Point>& rqPoints, 
    const std::vector<Point>& path)
{
    // Walk path backwards (start of path is at end of vec)
    bool inWater = false;
    for(int i = path.size() - 1; i > 0; --i)
    {
        Point p = path[i];
        if(!inWater && map.sq(p).terrain == Terrain::WATER)
        {
            rqPoints.emplace_back(p);
            inWater = true;
        }
        else if(inWater && map.sq(p).terrain != Terrain::WATER)
            inWater = false;
    }
}


std::vector<Node> neighbors(const Map& map, const Point& p)
{
    std::vector<Node> neighbors;
    neighbors.reserve(4);

    auto isValidSq = [&](const MapSquare& sq)
    {
        if(sq.terrain == Terrain::WALL)
            return false;
        return true;
    };

    auto addSq = [&](const MapSquare& sq, Point p) {
        neighbors.emplace_back(Node{p});
    };

    // Up
    if(p.y > 0 && isValidSq(map.sq(p.x, p.y - 1)))
        addSq(map.sq(p.x, p.y - 1), Point{p.x, p.y - 1});

    // Right
    if(p.x < map.getWidth() - 1 && isValidSq(map.sq(p.x + 1, p.y)))
        addSq(map.sq(p.x + 1, p.y), Point{p.x + 1, p.y});

    // Down
    if(p.y < map.getHeight() - 1 && isValidSq(map.sq(p.x, p.y + 1)))
        addSq(map.sq(p.x, p.y + 1), Point{p.x, p.y + 1});

    // Left
    if(p.x > 0 && p.y > 0 && isValidSq(map.sq(p.x - 1, p.y)))
        addSq(map.sq(p.x - 1, p.y), Point{p.x - 1, p.y});

    return neighbors;
}

std::vector<Point> getPath(std::unordered_map<Node, Node>& cameFrom, Node current)
{
    std::vector<Point> path;
    path.emplace_back(current.p);
    while(true)
    {
        auto find = cameFrom.find(current);
        if(find == std::end(cameFrom))
            break;
        current = find->second;
        path.emplace_back(current.p);
    }
    return path;
}

// https://en.wikipedia.org/wiki/A*_search_algorithm#Pseudocode
//
std::vector<Point> Pathing::aStar(const Map& map, Point player, Point diamond, 
        std::vector<Point>& requiredBoats)
{
    Node start{player};
    Node end{diamond};

    CompareNodeDist cp{diamond};
    std::unordered_set<Node> openSetVals;
    std::priority_queue<Node, std::vector<Node>, CompareNodeDist> openSet{cp};

    openSet.emplace(start);
    openSetVals.emplace(start);


    // Key is a Node, and value is the previous node that node came from,
    // on the cheapest path from the start
    std::unordered_map<Node, Node> cameFrom;

    // For the Key, the value is the cost of the cheapest path from start to n known
    std::unordered_map<Node, float> gScore;
    gScore.emplace(start, 0.f);

    std::unordered_map<Node, float> fScore;
    fScore.emplace(start, dist(start.p, end.p));

    while(!openSet.empty())
    {
        Node current = openSet.top();
        if(current == end)
            return getPath(cameFrom, current); 

        openSet.pop();
        auto osFind = openSetVals.find(current);
        openSetVals.erase(osFind);

        for(Node n : neighbors(map, current.p))
        {
            auto gScoreFind         = gScore.find(current);
            auto gScoreNeighFind    = gScore.find(n);
            float tgScore           = gScoreFind->second + 1;
            float gScoreNeigh       = std::numeric_limits<float>::max();
            if(gScoreNeighFind != std::end(gScore))
                gScoreNeigh = gScoreNeighFind->second;

            // This path doesn't look better
            if(tgScore >= gScoreNeigh)
                continue;

            cameFrom[n] = current;
            gScore[n] = tgScore;
            fScore[n] = tgScore + dist(n.p, end.p);

            auto osFind = openSetVals.find(n);
            if(osFind == std::end(openSetVals))
            {
                openSet.emplace(n);
                openSetVals.emplace(n);
            }
        }
    }
    return std::vector<Point>{};
}