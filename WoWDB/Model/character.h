#ifndef CHARACTER_H
#define CHARACTER_H

#include "domains.h"
#include <string>
#include <QString>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

using std::string;
using namespace Domains;


class Character
{
private:
    string name;
    Gender gender;
    Race race;
    Faction faction;
    int level;
    int HP;

public:
    // Constructors/destructors
    Character() = default;
    Character(string charName, Gender charGender, Race charRace, Faction charFaction, int charLevel, int charHP);
    virtual ~Character() = default;

    // Getters/setters
    string getName() const;
    string getGender() const;
    string getRace() const;
    string getFaction() const;
    int getLevel() const;
    int getHP() const;
    virtual string getClass() const = 0;
    void setName(const string& charName);
    void setGender(const string& charGender);
    void setRace(const string& charRace);
    void setFaction(const string& charFaction);
    void setLevel(const int charLevel);
    void setHP(const int charHP);

    // Operators overloading
    bool operator==(const Character& character) const;
    bool operator!=(const Character& character) const;
    bool operator<(const Character& character) const;

    // Methods
    virtual Character* clone() const = 0;
    virtual QJsonObject serialize() const;
    virtual void deserialize(const QJsonObject& obj);
};

#endif // CHARACTER_H
