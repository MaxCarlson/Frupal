#pragma once
#include "../item.h"
#include "../mapgenerator.h"
#include "../map.h"

#define TESTING

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

    std::string setFeatureText(Map* m) {
        std::string r;
        
        //Terrain t = m->sq(loc.first,loc.second).terrain;
        Terrain t = Terrain::MEADOW;    // Searched terrain
        Terrain f = Terrain::MEADOW;    // Found terrain
        int  w     = m->getWidth();     // Right edge for bounds checking
        int  h     = m->getHeight();    // Bottom edge for bounds checking
        int  max   = 20;                // Don't search farther than this
        int  count = 0;                 // Counter (obvs)
        int  fx    = 0;                 // Found x-coord
        int  fy    = 0;                 // Found y-coord
        int  dist  = max;               // Final distance
        bool valid = true;              // Flip this if we find edge so it doesn't get counted

        for (int i=-1;i<2;i+=2)
        {
            // 1. West and 3. East
            while (t == Terrain::MEADOW && count < dist)
            {
                ++count;
                if (loc.first+(count*i) < 0 || loc.first+(count*i) > w) // Out of bounds
                    { valid = false; break; }
                t = m->sq(loc.first+(count*i),loc.second).terrain;
            }

            if (valid && count < dist) {
                fx = loc.first + (count * i);
                fy = loc.second;
                f = t;
                dist = count;
            }
            count = 0;
            valid = true;
            t = Terrain::MEADOW;

            // 2. South and 4. North
            while (t == Terrain::MEADOW && count < dist)
            {
                ++count;
                if (loc.second+(count*i) < 0 || loc.second+(count*i) > h) // Out of bounds
                    { valid = false; break; }
                t = m->sq(loc.first,loc.second+(count*i)).terrain;
            }
            if (valid && count < dist) {
                fx = loc.first;
                fy = loc.second + (count * i);
                f = t;
                dist = count;
            }
            count = 0;
            valid = true;
            t = Terrain::MEADOW;
        }

        // Figure out results
        if (dist == max)
            return "There is nought but featureless meadow all around you.";
        
        r = "There is ";
        if (f==Terrain::SWAMP) { r += "a swamp "; }
        if (f==Terrain::WATER) { r += "water ";   }
        if (f==Terrain::WALL)  { r += "a wall ";  }

        r += std::to_string(dist) + " grovnick";
        r += (dist > 1) ? "s to the " : " to the ";

        if (fx == loc.first)    // No change in x means found is N/S
        {
            if (!tf)
                r += (fy>loc.second) ? "north." : "south.";
            else
                r += (fy>loc.second) ? "south." : "north.";
        }
        else                    // found is W/E
        {
            if (!tf)
                r += (fx>loc.first)  ? "west." : "east.";
            else
                r += (fx>loc.first)  ? "east." : "west.";
        }
        
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

        #ifdef TESTING
        if (!tf) { r+= " ;)"; }
        #endif

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
         *     This must be called separately when the cursor is moved onto it. See ui.cpp,
         *     function printSelectedInfo.
         *     If we try to set it at the same time as the others, it will segfault because the
         *     map is not filled in yet.
         * Distance to diamonds
         *     (dx - px) / Positive east, negative west
         *     (dy - py) / Positive north, negative south
         */

        borderText = setBorderText(loc.first, loc.second);
        //featureText = setFeatureText(mg,m);
        diamondsText = setDiamondsText((loc.first - diamond.first), (loc.second - diamond.second));
    }

    void setDescription(Map* m)
    {
        if (!m) return;

        featureText = setFeatureText(m);
    }
};