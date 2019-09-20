#ifndef CLASSES_H
#define CLASSES_H

#include "player.h"

/******************** MAGE ********************/
class Mage : public Player
{
public:
    // Constructors/destructors
    Mage() = default;
    virtual ~Mage() = default;

    // Getters/setters
    virtual string getClass() const;

    // Methods
    virtual Mage* clone() const;
    virtual double strengthMultiplier() const;
    virtual double agilityMultiplier() const;
    virtual double intellectMultiplier() const;
    virtual double spiritMultiplier() const;
    virtual string armor() const;
    virtual string role() const;
};


/******************** PRIEST ********************/
class Priest : public Player
{
public:
    // Constructors/destructors
    Priest() = default;
    virtual ~Priest() = default;

    // Getters/setters
    virtual string getClass() const;

    // Methods
    virtual Priest* clone() const;
    virtual double strengthMultiplier() const;
    virtual double agilityMultiplier() const;
    virtual double intellectMultiplier() const;
    virtual double spiritMultiplier() const;
    virtual string armor() const;
    virtual string role() const;
};


/******************** ROGUE ********************/
class Rogue : public Player
{    
public:
    // Constructors/destructors
    Rogue() = default;
    virtual ~Rogue() = default;

    // Getters/setters
    virtual string getClass() const;
    
    // Methods
    virtual Rogue* clone() const;
    virtual double strengthMultiplier() const;
    virtual double agilityMultiplier() const;
    virtual double intellectMultiplier() const;
    virtual double spiritMultiplier() const;
    virtual string armor() const;
    virtual string role() const;
};


/******************** WARRIOR ********************/
class Warrior : public Player
{
public:
    // Constructors/destructors
    Warrior() = default;
    virtual ~Warrior() = default;

    // Getters/setters
    virtual string getClass() const;

    // Methods
    virtual Warrior* clone() const;
    virtual double strengthMultiplier() const;
    virtual double agilityMultiplier() const;
    virtual double intellectMultiplier() const;
    virtual double spiritMultiplier() const;
    virtual string armor() const;
    virtual string role() const;
};

#endif // CLASSES_H
