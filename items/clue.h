#pragma once
#include "../item.h"
#include "../mapgenerator.h"
#include "../map.h"

class Clue : public Item
{
    std::pair<int,int> loc;     // xy coordinates of this clue
    bool tf;                    // Clues are True or False
    std::string borderText,     // Ex: "You are x grovniks away from the [dir]ern border."
                featureText,    // Ex: "There is a [feature] x grovniks to the [dir]."
                diamondsText;   // Ex: "The royal diamonds are located x grovnicks [dir] and y grovnicks [dir]"

    std::string setBorderText(int x, int y) {
        std::string r;

        // Flip directions if clue is false
        if (!tf)
        {
            x *= -1;
            y *= -1;
        }

        r = "You are " + std::to_string(abs(x)) + " grovnicks from the ";
        r += (x>0) ? "west" : "east";
        r += "ern edge.";

        return r;
    }

    std::string setFeatureText(MapGenerator* mg, Map* m) {
        std::string r;
        r = "Test";
        
        //auto& [idx,vec] = mg->getVoronoiVec();
        //auto [x,y] = vec[0];    // Coordinates for a map square
        /* This segfaults, map is not filled in yet
        Terrain t = m->sq(loc.first+1,loc.second).terrain; //should be 1 square west
        
        r = "Terrain = ";
        if (t==Terrain::MEADOW) { r += "Meadow"; }
        if (t==Terrain::SWAMP) { r += "Swamp"; }
        if (t==Terrain::WATER) { r += "Water"; }
        if (t==Terrain::WALL) { r += "Wall"; }
        */
        /* Work in progress
        for (auto& [idx,vec]: mg->getVoronoiVec())
        {
            auto [x,y] = vec[0];    // Coordinates for a map square

            for (int j=0;i<limit;++i)
            {
                if ((m->sq(x,y).terrain == WATER) || (m->sq(x,y).terrain == SWAMP))
            }
        }
        */

        /* OLD
        if (tf)
        {
            //Generate accurate clue
            r = "There is a swamp or water or something...somewhere.";
        }
        else
        {   // Generate inaccurate clue
            r = "There...isn't...a swamp or water? I guess?";
        }
        */

        return r;
    }

    std::string setDiamondsText(int x, int y) {
        std::string r;

        // Flip directions if clue is false
        if (!tf)
        {
            x *= -1;
            y *= -1;
        }

        r = "The diamonds are " + std::to_string(abs(x)) + " grovnicks to the ";
        r += (x>0) ? "west" : "east";
        r += " and " + std::to_string(abs(y)) + " grovnicks to the ";
        r += (y>0) ? "north." : "south.";

        return r;
    }

public:
    // Option to specify T/F (1/0)
    Clue(bool tf, MapGenerator* mg, Map* m, std::pair<int,int> clueCoords) :
        Item{'?', "Clue"}, tf{tf}
    {
        loc = clueCoords;
        borderText = "Not set";
        featureText = "Not set";
        diamondsText = "Not set";
        setDescription(mg,m); // Should overwrite the above
    }

    StringTuple getDescription() const
    {
        return StringTuple{name, borderText, featureText, diamondsText};
    }

    //void setDescription(std::pair<int,int> player, std::pair<int,int> diamond, Map& map) const
    void setDescription(MapGenerator* mg, Map* m)
    {
        if (!mg) return;
        if (!m) return;
        std::pair<int,int> diamond = mg->getDiamondCoords();

        /* Distance to border
         *     (cheating...just always use western edge, so distance is the x-coord of the clue)
         *     (If time, calculate which edge is closest? Or pick a random one?)
         * Distance to feature
         *     (I really have no idea yet)
         * Distance to diamonds
         *     (dx - px) / Positive east, negative west
         *     (dy - py) / Positive north, negative south
         */

        borderText = setBorderText(loc.first, loc.second);
        featureText = setFeatureText(mg,m);
        diamondsText = setDiamondsText((loc.first - diamond.first), (loc.second - diamond.second));
    }
};