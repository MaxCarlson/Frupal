#include "pathing.h"
#include "map.h"
#include <map>
#include <queue>
#include <set>
#include <unordered_map>

struct Node
{
    bool water;
    Point p;
    Node(bool water, Point p) : water{water}, p{p} {}
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


bool Pathing::playerToDiamond(const Map& map, Point player, Point diamond,
     std::vector<Point>& requiredBoats)
{

    return true;
}

std::vector<Node> neighbors(const Map& map)
{

}

// https://en.wikipedia.org/wiki/A*_search_algorithm#Pseudocode
//
void aStar(const Map& map, Point player, Point diamond, 
        std::vector<Point>& requiredBoats)
{
    Node start{false, player};
    Node end{false, diamond};

    CompareNodeDist cp{diamond};
    std::priority_queue<Node, std::vector<Node>, CompareNodeDist> openSet{cp};
    openSet.emplace(start);

    std::set<int> waterCells;
    std::priority_queue<Node, std::vector<Node>, CompareNodeDist> waterNodes{cp};

    // Key is a Node, and value is the previous node that node came from,
    // on the cheapest path from the start
    std::map<Node, Node> cameFrom;

    // For the Key, the value is the cost of the cheapest path from start to n known
    std::map<Node, float, CompareNode> gScore{};
    gScore.emplace(start, 0.f);

    std::map<Node, float, CompareNode> fScore{};
    fScore.emplace(start, dist(start.p, end.p));

    while(!(openSet.empty() || waterNodes.empty()))
    {
        auto [current, val] current = openSet.top();
        if(current == end)
            break; // TODO:

        openSet.pop();

        for(Node n : neighbors(map))
        {
            
        }
    }

}