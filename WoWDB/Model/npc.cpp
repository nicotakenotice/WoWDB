#include "npc.h"

/******************** CONSTRUCTORS/DESTRUCTORS ********************/
NPC::NPC(string charName, Gender charGender, Race charRace, Faction charFaction, int charLevel, int charHP,
         Mob npcType, Merchandise npcMerc, string npcQuest) :
    Character(charName, charGender, charRace, charFaction, charLevel, charHP),
    type(npcType), merchandise(npcMerc), quest(npcQuest) {}


/******************** GETTERS/SETTERS ********************/
string NPC::getType() const
{
    switch (type) {
        case Normal: return "Normal";
        case Elite: return "Elite";
        case Boss: return "Boss";
    }
    return "Type error";
}
string NPC::getMerchandise() const
{
    switch(merchandise) {
        case None: return "None";
        case Weapons: return "Weapons";
        case Armors: return "Armors";
        case Containers: return "Containers";
        case Consumables: return "Consumables";
        case Recipes: return "Recipes";
        case Gems: return "Gems";
        case Miscellaneous: return "Miscellaneous";
    }
    return "Merchandise error";
}
string NPC::getQuest() const { return quest; }
string NPC::getClass() const { return "NPC"; }
void NPC::setType(const string& npcType)
{
    if (npcType == "Normal") type = Normal;
    if (npcType == "Elite") type = Elite;
    if (npcType == "Boss") type = Boss;
}
void NPC::setMerchandise(const string& npcMerch)
{
    if (npcMerch == "None") merchandise = None;
    if (npcMerch == "Weapons") merchandise = Weapons;
    if (npcMerch == "Armors") merchandise = Armors;
    if (npcMerch == "Containers") merchandise = Containers;
    if (npcMerch == "Consumbles") merchandise = Consumables;
    if (npcMerch == "Recipes") merchandise = Recipes;
    if (npcMerch == "Gems") merchandise = Gems;
    if (npcMerch == "Miscellaneous") merchandise = Miscellaneous;
}
void NPC::setQuest(const string& npcQuest) { quest = npcQuest; }


/******************** METHODS ********************/
NPC* NPC::clone() const { return new NPC(*this); }
QJsonObject NPC::serialize() const
{
    QJsonObject npcJson = Character::serialize();
    npcJson["type"] = QString::fromStdString(getType());
    npcJson["merchandise"] = QString::fromStdString(getMerchandise());
    npcJson["quest"] = QString::fromStdString(getQuest());
    return npcJson;
}
void NPC::deserialize(const QJsonObject& obj)
{
    Character::deserialize(obj);
    if (obj.contains("type") && obj["type"].isString())
        setType(obj["type"].toString().toStdString());
    if (obj.contains("merchandise") && obj["merchandise"].isString())
        setMerchandise(obj["merchandise"].toString().toStdString());
    if (obj.contains("quest") && obj["quest"].isString())
        setQuest(obj["quest"].toString().toStdString());
}
