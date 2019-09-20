#include "classes.h"

/******************** MAGE ********************/
// Getters/setters
string Mage::getClass() const { return "Mage"; }

// Methods
Mage* Mage::clone() const { return new Mage(*this); }
double Mage::strengthMultiplier() const { return 0.5; }
double Mage::agilityMultiplier() const { return 0.5; }
double Mage::intellectMultiplier() const { return 1.8; }
double Mage::spiritMultiplier() const { return 1.2; }
string Mage::armor() const { return "Cloth"; }
string Mage::role() const { return "Damage"; }


/******************** PRIEST ********************/
// Getters/setters
string Priest::getClass() const { return "Priest"; }

// Methods
Priest* Priest::clone() const { return new Priest(*this); }
double Priest::strengthMultiplier() const { return 0.5; }
double Priest::agilityMultiplier() const { return 0.5; }
double Priest::intellectMultiplier() const { return 1.4; }
double Priest::spiritMultiplier() const { return 1.6; }
string Priest::armor() const { return "Cloth"; }
string Priest::role() const { return "Healer, Damage"; }


/******************** ROGUE ********************/
// Getters/setters
string Rogue::getClass() const { return "Rogue"; }

// Methods
Rogue* Rogue::clone() const { return new Rogue(*this); }
double Rogue::strengthMultiplier() const { return 1.1; }
double Rogue::agilityMultiplier() const { return 1.9; }
double Rogue::intellectMultiplier() const { return 0.5; }
double Rogue::spiritMultiplier() const { return 0.5; }
string Rogue::armor() const { return "Leather"; }
string Rogue::role() const { return "Damage"; }


/******************** WARRIOR ********************/
// Getters/setters
string Warrior::getClass() const { return "Warrior"; }

// Methods
Warrior* Warrior::clone() const { return new Warrior(*this); }
double Warrior::strengthMultiplier() const { return 2.0; }
double Warrior::agilityMultiplier() const { return 1.0; }
double Warrior::intellectMultiplier() const { return 0.5; }
double Warrior::spiritMultiplier() const { return 0.5; }
string Warrior::armor() const { return "Plate"; }
string Warrior::role() const { return "Tank, Damage"; }
