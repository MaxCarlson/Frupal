#pragma once
#include "../item.h"

class Clue : public Item
{
    bool tf;                    // Clues are True or False
    std::string borderText,     // Ex: "You are x grovniks away from the [dir]ern border."
                featureText,    // Ex: "There is a [feature] x grovniks to the [dir]."
                diamondsText;   // Ex: "The royal diamonds are located x grovnicks [dir] and y grovnicks [dir]"

    std::string setBorderText() {
        std::string r;

        if (tf)
        {
            //Generate accurate clue
            r = "Accurate clue.";
        }
        else
        {   // Generate inaccurate clue
            r = "Inaccurate clue.";
        }

        return r;
    }

    std::string setFeatureText() {
        std::string r;

        if (tf)
        {
            //Generate accurate clue
            r = "Accurate clue.";
        }
        else
        {   // Generate inaccurate clue
            r = "Inaccurate clue.";
        }

        return r;
    }

    std::string setDiamondsText() {
        std::string r;

        if (tf)
        {
            //Generate accurate clue
            r = "Accurate clue.";
        }
        else
        {   // Generate inaccurate clue
            r = "Inaccurate clue.";
        }

        return r;
    }

public:
    Clue() : Item{'?', "Clue"}
    {
        borderText = setBorderText();
        featureText = setFeatureText();
        diamondsText = setDiamondsText();
    }

    // Option to specify T/F (1/0)
    Clue(bool tf) :
        Item{'?', "Clue"}, tf{tf}
    {
        borderText = setBorderText();
        featureText = setFeatureText();
        diamondsText = setDiamondsText();
    }
    
    //Shouldn't need this one
    Clue(std::string name) 
        : Item{'?', name}
    {}

    StringTuple getDescription() const
    {
        return StringTuple{name, borderText, featureText, diamondsText};
    }
};