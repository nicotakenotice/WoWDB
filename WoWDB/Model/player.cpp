#include "player.h"

/******************** CONSTRUCTORS/DESTRUCTORS ********************/
Player::Player(string charName, Gender charGender, Race charRace, Faction charFaction, int charLevel, int charHP,
               Status playerStatus, int playerRating, int playerStrength, int playerAgility, int playerIntellect, int playerSpirit) :
    Character(charName, charGender, charRace, charFaction, charLevel, charHP),
    status(playerStatus), rating(playerRating), strength(playerStrength), agility(playerAgility), intellect(playerIntellect), spirit(playerSpirit) {}


/******************** GETTERS/SETTERS ********************/
string Player::getStatus() const
{
    switch(status) {
        case Offline: return "Offline";
        case Online: return "Online";
    }
    return "Status error";
}
int Player::getRating() const { return rating; }
int Player::getStrength() const { return strength; }
int Player::getAgility() const { return agility; }
int Player::getIntellect() const { return intellect; }
int Player::getSpirit() const { return spirit; }
void Player::setStatus(const string& playerStatus)
{
    if (playerStatus == "Offline") status = Offline;
    if (playerStatus == "Online") status = Online;
}
void Player::setRating(const int playerRating) {
    if (playerRating > 4000) rating = 4000;
    else rating = playerRating;
}
void Player::setStrength(const int playerStrength) {
    if (playerStrength > statCap()) strength = statCap();
    else strength = playerStrength;
}
void Player::setAgility(const int playerAgility) {
    if (playerAgility > statCap()) agility = statCap();
    else agility = playerAgility;
}
void Player::setIntellect(const int playerIntellect) {
    if (playerIntellect > statCap()) intellect = statCap();
    else intellect = playerIntellect;
}
void Player::setSpirit(const int playerSpirit) {
    if (playerSpirit > statCap()) spirit = statCap();
    else spirit = playerSpirit;
}


/******************** METHODS ********************/
QJsonObject Player::serialize() const
{
    QJsonObject playerJson = Character::serialize();
    playerJson["status"] = QString::fromStdString(getStatus());
    playerJson["rating"] = getRating();
    playerJson["strength"] = getStrength();
    playerJson["agility"] = getAgility();
    playerJson["intellect"] = getIntellect();
    playerJson["spirit"] = getSpirit();
    return playerJson;
}
void Player::deserialize(const QJsonObject& obj)
{
    Character::deserialize(obj);
    if (obj.contains("status") && obj["status"].isString())
        setStatus(obj["status"].toString().toStdString());
    if (obj.contains("rating") && obj["rating"].isDouble())
        setRating(obj["rating"].toInt());
    if (obj.contains("strength") && obj["strength"].isDouble())
        setStrength(obj["strength"].toInt());
    if (obj.contains("agility") && obj["agility"].isDouble())
        setAgility(obj["agility"].toInt());
    if (obj.contains("intellect") && obj["intellect"].isDouble())
        setIntellect(obj["intellect"].toInt());
    if (obj.contains("spirit") && obj["spirit"].isDouble())
        setSpirit(obj["spirit"].toInt());
}
int Player::attackPower() const { return static_cast<int>(strength*strengthMultiplier() + agility*agilityMultiplier()); }
int Player::spellPower() const { return static_cast<int>(intellect*intellectMultiplier() + spirit*spiritMultiplier()); }
int Player::statCap() const { return 2000; }
