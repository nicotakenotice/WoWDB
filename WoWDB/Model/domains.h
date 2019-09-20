#ifndef DOMAINS_H
#define DOMAINS_H

namespace Domains {
    enum Race {
        Draenei, Dwarf, Gnome, Human, Night_Elf,
        Blood_Elf, Orc, Tauren, Troll, Undead
    };
    enum Gender {
        Male, Female
    };
    enum Faction {
        Neutral, Alliance, Horde
    };
    enum Mob {
        Normal, Elite, Boss
    };
    enum Merchandise {
        None, Weapons, Armors, Containers, Consumables, Recipes, Gems, Miscellaneous
    };
    enum Status {
        Offline, Online
    };
}

#endif // DOMAINS_H
