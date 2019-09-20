#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"

class Player : public Character
{
private:
    Status status;
    int rating;

    // Stats
    int strength;
    int agility;
    int intellect;
    int spirit;

public:
    // Constructors/destructors
    Player() = default;
    Player(string charName, Gender charGender, Race charRace, Faction charFaction, int charLevel, int charHP,
           Status playerStatus, int playerRating, int playerStrength, int playerAgility, int playerIntellect, int playerSpirit);
    virtual ~Player() = default;

    // Getters/setters
    string getStatus() const;
    int getRating() const;
    int getStrength() const;
    int getAgility() const;
    int getIntellect() const;
    int getSpirit() const;
    virtual string getClass() const = 0;
    void setStatus(const string& playerStatus);
    void setRating(const int playerRating);
    void setStrength(const int playerStrength);
    void setAgility(const int playerAgility);
    void setIntellect(const int playerIntellect);
    void setSpirit(const int playerSpirit);

    // Methods
    virtual Player* clone() const = 0;
    virtual QJsonObject serialize() const;
    virtual void deserialize(const QJsonObject& obj);
    virtual double strengthMultiplier() const = 0;
    virtual double agilityMultiplier() const = 0;
    virtual double intellectMultiplier() const = 0;
    virtual double spiritMultiplier() const = 0;
    virtual int attackPower() const;
    virtual int spellPower() const;
    virtual string armor() const = 0;
    virtual string role() const = 0;
    int statCap() const;
};

#endif // PLAYER_H
