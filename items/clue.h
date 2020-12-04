#pragma once
#include "../item.h"

class Clue : public Item
{
    bool tf;                    // Clues are True or False
    std::string borderText,     // Ex: "You are x grovniks away from the [dir]ern border."
                featureText,    // Ex: "There is a [feature] x grovniks to the [dir]."
                diamondsText;   // Ex: "The royal diamonds are located x grovnicks [dir] and y grovnicks [dir]"

    std::string setBorderText(int x, int y) const {
        std::string r;

        // Flip directions if clue is false
        if (!tf)
        {
            x *= -1;
            y *= -1;
        }

        r = "You are " + std::to_string(x) + " grovnicks from the ";
        r += (x>0) ? "west" : "east";
        r += "ern edge.";

        return r;
    }

    std::string setFeatureText() const {
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

    std::string setDiamondsText(int x, int y) const {
        std::string r;

        // Flip directions if clue is false
        if (!tf)
        {
            x *= -1;
            y *= -1;
        }

        r = "The diamonds are " + std::to_string(x) + " grovnicks to the ";
        r += (x>0) ? "west" : "east";
        r += " and " + std::to_string(y) + " grovnicks to the ";
        r += (y>0) ? "north." : "south.";

        return r;
    }

public:
    Clue() : Item{'?', "*Clue"}
    {
        borderText = "Empty. No args.";
        featureText = "Empty. No args.";
        diamondsText = "Empty. No args.";
    }

    // Option to specify T/F (1/0)
    Clue(bool tf) :
        Item{'?', "^Clue"}, tf{tf}
    {
        borderText = "empty. Clue with tf.";
        featureText = "empty. Clue with tf.";
        diamondsText = "empty. Clue with tf.";
    }
    
    //Shouldn't need this one
    /*
    Clue(std::string name) 
        : Item{'?', name}
    {
        borderText = "empty. Clue with name.";
        featureText = "empty. Clue with name.";
        diamondsText = "empty. Clue with name.";
    }
    */

    StringTuple getDescription() const
    {
        return StringTuple{name, borderText, featureText, diamondsText};
    }

    void setDescription(std::pair<int,int> player, std::pair<int,int> diamond, Map& map) const
    {
        //std::pair<int,int> player  = mg.getPlayerCoords();
        //std::pair<int,int> diamond = mg.getDiamondCoords();
        //int mapwidth               = map.getWidth();
        //int mapheight              = map.getHeight();

        /* Distance to border
         *     (cheating...just always use western edge, so distance is px)
         *     (If time, calculate which edge is closest)
         * Distance to feature
         *     (I really have no idea yet)
         * Distance to diamonds
         *     (dx - px) / Positive east, negative west
         *     (dy - py) / Positive north, negative south
         */

        setBorderText(player.first, player.second);
        setFeatureText();
        setDiamondsText((player.first - diamond.first), (player.second - diamond.second));
    }
};