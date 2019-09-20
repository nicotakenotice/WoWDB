#include "character.h"

/******************** CONSTRUCTORS/DESTRUCTORS ********************/
Character::Character(string charName, Gender charGender, Race charRace, Faction charFaction, int charLevel, int charHP) :
    name(charName), gender(charGender), race(charRace), faction(charFaction), level(charLevel), HP(charHP) {}


/******************** GETTERS/SETTERS ********************/
string Character::getName() const { return name; }
string Character::getGender() const {
    switch (gender) {
        case Male: return "Male";
        case Female: return "Female";
    }
    return "Gender error";
}
string Character::getRace() const {
    switch (race) {
        case Draenei: return "Draenei";
        case Dwarf: return "Dwarf";
        case Gnome: return "Gnome";
        case Human: return "Human";
        case Night_Elf: return "Night Elf";
        case Blood_Elf: return "Blood Elf";
        case Orc: return "Orc";
        case Tauren: return "Tauren";
        case Troll: return "Troll";
        case Undead: return "Undead";
    }
    return "Race error";
}
string Character::getFaction() const {
    switch (faction) {
        case Neutral: return "Neutral";
        case Alliance: return "Alliance";
        case Horde: return "Horde";
    }
    return "Faction error";
}
int Character::getLevel() const { return level; }
int Character::getHP() const { return HP; }
void Character::setName(const string& charName) { name = charName; }
void Character::setGender(const string& charGender)
{
    if (charGender == "Male") gender = Male;
    if (charGender == "Female") gender = Female;
}
void Character::setRace(const string& charRace)
{
    if (charRace == "Draenei") race = Draenei;
    if (charRace == "Dwarf") race = Dwarf;
    if (charRace == "Gnome") race = Gnome;
    if (charRace == "Human") race = Human;
    if (charRace == "Night Elf") race = Night_Elf;
    if (charRace == "Blood Elf") race = Blood_Elf;
    if (charRace == "Orc") race = Orc;
    if (charRace == "Tauren") race = Tauren;
    if (charRace == "Troll") race = Troll;
    if (charRace == "Undead") race = Undead;
}
void Character::setFaction(const string& charFaction)
{
    if (charFaction == "Neutral") faction = Neutral;
    if (charFaction == "Alliance") faction = Alliance;
    if (charFaction == "Horde") faction = Horde;
}
void Character::setLevel(const int charLevel) {
    if (charLevel > 80) level = 80;
    else if (charLevel == 0) level = 1;
    else level = charLevel;
}
void Character::setHP(const int charHP) {
    if (charHP > 999999) HP = 999999;
    else HP = charHP;
}


/******************** OPERATORS OVERLOADING ********************/
bool Character::operator==(const Character& character) const { return name == character.name; }
bool Character::operator!=(const Character& character) const { return name != character.name; }
bool Character::operator<(const Character& character) const { return name < character.name; }


/******************** METHODS ********************/
QJsonObject Character::serialize() const
{
    QJsonObject charJson;
    charJson["class"] = QString::fromStdString(getClass());
    charJson["name"] = QString::fromStdString(getName());
    charJson["gender"] = QString::fromStdString(getGender());
    charJson["race"] = QString::fromStdString(getRace());
    charJson["faction"] = QString::fromStdString(getFaction());
    charJson["level"] = getLevel();
    charJson["hp"] = getHP();
    return charJson;
}
void Character::deserialize(const QJsonObject& obj)
{
    if (obj.contains("name") && obj["name"].isString())
        setName(obj["name"].toString().toStdString());
    if (obj.contains("gender") && obj["gender"].isString())
        setGender(obj["gender"].toString().toStdString());
    if (obj.contains("race") && obj["race"].isString())
        setRace(obj["race"].toString().toStdString());
    if (obj.contains("faction") && obj["faction"].isString())
        setFaction(obj["faction"].toString().toStdString());
    if (obj.contains("level") && obj["level"].isDouble())
        setLevel(obj["level"].toInt());
    if (obj.contains("hp") && obj["hp"].isDouble())
        setHP(obj["hp"].toInt());
}
