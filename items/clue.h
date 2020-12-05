#pragma once
#include "../item.h"
#include "../mapgenerator.h"

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

    std::string setFeatureText() {
        std::string r;

        // This isn't working yet and I'm really not sure how to do it

        if (tf)
        {
            //Generate accurate clue
            r = "There is a swamp or water or something...somewhere.";
        }
        else
        {   // Generate inaccurate clue
            r = "There...isn't...a swamp or water? I guess?";
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

        return r;
    }

public:
    // Option to specify T/F (1/0)
    Clue(bool tf, MapGenerator* mg, std::pair<int,int> clueCoords) :
        Item{'?', "Clue"}, tf{tf}
    {
        loc = clueCoords;
        borderText = "--did not get set";
        featureText = "--did not get set";
        diamondsText = "--did not get set";
        this->setDescription(mg); // Should overwrite the above
    }

    StringTuple getDescription() const
    {
        return StringTuple{name, borderText, featureText, diamondsText};
    }

    //void setDescription(std::pair<int,int> player, std::pair<int,int> diamond, Map& map) const
    void setDescription(MapGenerator* mg)
    {
        if (!mg) return;
        //std::pair<int,int> player  = mg->getPlayerCoords();
        std::pair<int,int> diamond = mg->getDiamondCoords();
        //int mapwidth               = map.getWidth();
        //int mapheight              = map.getHeight();

        /* Distance to border
         *     (cheating...just always use western edge, so distance is px)
         *     (If time, calculate which edge is closest? Or pick a random one?)
         * Distance to feature
         *     (I really have no idea yet)
         * Distance to diamonds
         *     (dx - px) / Positive east, negative west
         *     (dy - py) / Positive north, negative south
         */

        borderText = setBorderText(loc.first, loc.second);
        featureText = setFeatureText();
        diamondsText = setDiamondsText((loc.first - diamond.first), (loc.second - diamond.second));
    }
};