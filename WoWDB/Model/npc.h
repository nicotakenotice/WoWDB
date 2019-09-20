#ifndef NPC_H
#define NPC_H

#include "character.h"

class NPC : public Character
{
private:
    Mob type;
    Merchandise merchandise;
    string quest;

public:
    // Constructors/destructors
    NPC() = default;
    NPC(string charName, Gender charGender, Race charRace, Faction charFaction, int charLevel, int charHP,
        Mob npcType, Merchandise npcMerc, string npcQuest);
    virtual ~NPC() = default;

    // Getters/setters
    string getType() const;
    string getMerchandise() const;
    string getQuest() const;
    virtual string getClass() const;
    void setType(const string& npcType);
    void setMerchandise(const string& npcMerch);
    void setQuest(const string& npcQuest);

    // Methods
    virtual NPC* clone() const;
    virtual QJsonObject serialize() const;
    virtual void deserialize(const QJsonObject& obj);
};

#endif // NPC_H
