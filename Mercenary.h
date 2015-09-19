/*
    ©2013-2015 EmuDevs <http://www.emudevs.com/>
    MaNGOS <http://getmangos.eu>
*/
#ifndef MERCENARY_H
#define MERCENARY_H

#include "Player.h"
#include "Chat.h"
#include "Pet.h"
#include "Spell.h"

#ifndef MANGOS
#include "SpellAuraEffects.h"
#else
#include "ObjectMgr.h"
#endif

struct MercenarySpells
{
    uint8 type;
    uint8 role;
    uint32 spellId;
    bool isActive;
};

struct MercenaryGear
{
    uint32 itemId;
    uint8 slot;

    MercenaryGear(uint32 itemEntry, uint8 slotId)
        : itemId(itemEntry), slot(slotId) { }
};

struct MercenaryStartGear
{
    uint8 mercenaryRole;
    uint32 headEntry;
    uint32 shoulderEntry;
    uint32 chestEntry;
    uint32 legEntry;
    uint32 handEntry;
    uint32 feetEntry;
    uint32 weaponEntry;
    uint32 offHandEntry;
    uint32 rangedEntry;
};

struct MercenaryTalking
{
    uint8 type;
    uint8 role;
    // Leave at 100 if you want to use it for random talking
    uint8 healthPercentageToTalk;
    std::string text;
};

struct RandomMercenary
{
    uint32 model;
    uint8 race;
    uint8 gender;
};

struct RandomMercenaryTypeRole
{
    uint8 type;
    uint8 role;
};

enum MercenaryType
{
    MERCENARY_TYPE_NONE,
    MERCENARY_TYPE_WARRIOR,
    MERCENARY_TYPE_PALADIN,
    MERCENARY_TYPE_PRIEST,
    MERCENARY_TYPE_DK,
    MERCENARY_TYPE_WARLOCK,
    MERCENARY_TYPE_MAGE,
    MERCENARY_TYPE_DRUID,
    MERCENARY_TYPE_HUNTER,
    MERCENARY_TYPE_ROGUE,
    MERCENARY_TYPE_SHAMAN,

    MAX_MERCENARY_TYPES
};

enum MercenaryRoles
{
    ROLE_NONE,
    ROLE_MELEE_DPS,
    ROLE_CASTER_DPS,
    ROLE_MARKSMAN_DPS,
    ROLE_HEALER,
    ROLE_TANK
};

enum RaceModels
{
    HUMAN_MALE_MODEL = 49,
    HUMAN_FEMALE_MODEL = 50,
    ORC_MALE_MODEL = 51,
    ORC_FEMALE_MODEL = 52,
    DWARF_MALE_MODEL = 53,
    DWARF_FEMALE_MODEL = 54,
    NIGHTELF_MALE_MODEL = 55,
    NIGHTELF_FEMALE_MODEL = 56,
    SCOURGE_MALE_MODEL = 57,
    SCOURGE_FEMALE_MODEL = 58,
    TAUREN_MALE_MODEL = 59,
    TAUREN_FEMALE_MODEL = 60,
    GNOME_MALE_MODEL = 1563,
    GNOME_FEMALE_MODEL = 1564,
    TROLL_MALE_MODEL = 1478,
    TROLL_FEMALE_MODEL = 1479,
    BLOODELF_MALE_MODEL = 15476,
    BLOODELF_FEMALE_MODEL = 15475,
    DRAENEI_MALE_MODEL = 16125,
    DRAENEI_FEMALE_MODEL = 16126
};

#define MERCENARY_DEFAULT_ENTRY 70000
#define MERCENARY_MAX_SPELLS 4

class Mercenary
{
public:
    Mercenary();
    Mercenary(uint32 model, uint8 race, uint8 gender, uint8 role, uint8 type);
    ~Mercenary();

    /*
    * Loads the Mercenary's gear from the database into a vector
    */
    void LoadGearFromDB();
    /*
    * Loads the Mercenary from the database into a map
    */
#ifndef MANGOS
    bool LoadFromDB(QueryResult result);
#else
    bool LoadFromDB(QueryResult* result);
#endif
    /*
    * Temporarily adds a new Mercenary being created into the Mercenary map
    * If creation of this temporary Mercenary isn't finished the Mercenary will NOT save
    */
    bool Create(Player* player, uint32 entry);
    /*
    * Creates and spawns the Mercenary
    */
    bool Create(Player* player, uint32 model, uint8 race, uint8 gender, uint8 mercenaryType, uint8 role);
    /*
    * Mercenary will learn the given spellId
    * Maximum spells Mercenary can have is 4
    */
    bool LearnSpell(Player* player, uint32 spellId);
    /*
    * Summons the Mercenary
    */
    bool Summon(Player* player);
    /*
    * Initializes Mercenary's stats
    */
    bool InitStats(Player* player, Pet* pet);
    /*
    * Updates Mercenary's stats
    */
    bool UpdateStats(Player* player, Stats /* stats */, Pet* pet);
    /*
    * Returns true if the Mercenary can equip the specified item
    */
    bool CanEquipItem(Player* player, Item* item);
    /*
    * Initializes Mercenary's stats, gear and other summon values
    */
    void Initialize(Player* player, Pet* pet, bool create);
    /*
    * Sets the displayId, race and gender values of the Mercenary
    */
    void SetValues(uint32 model, uint8 race, uint8 gender);
    /*
    * Sets the role of the Mercenary
    */
    void SetRole(const uint8 newRole) { role = newRole; }
    /*
    * Sets the type of the Mercenary
    */
    void SetType(const uint8 newType) { type = newType; }
    /*
    * Sets the equip slot the Mercenary owner is currently editing
    */
    void SetEditSlot(const uint8 slot) { editSlot = slot; }
    /*
    * Sets Mercenary's name
    */
    void SetName(std::string const& newName) { name = newName; }
    /*
    * Updates the Mercenary's Max Power
    */
    void UpdateMaxPower(Powers /* power */, Pet* pet);
    /*
    * Updates the Mercenary's Max Health
    */
    void UpdateMaxHealth(Pet* pet);
    /*
    * Updates the Mercenary's Armor
    */
    void UpdateArmor(Pet* pet);
    /*
    * Updates the Mercenary's strength
    */
    void UpdateStrength(Pet* pet, bool ranged = false);
    /*
    * Updates the Mercenary's physical damage
    */
    void UpdatePhysicalDamage(WeaponAttackType /* attackType */, Pet* pet);
    /*
    * Saves the Mercenary to the database
    */
    void SaveToDB();
    /*
    * Saves the Mercenary's gear to the database
    */
    void SaveGearToDB();
    /*
    * Sends the Mirror Image packet after equipping new armor
    */
    void SendMirrorImagePacket(Creature* creature);
    /*
    * Removes the Mercenary's off hand weapon
    */
    void RemoveOffHand(Creature* creature);
    /*
    * Updates the Mercenary's gear in the database
    */
    void UpdateGear();

    /*
    * Returns InventorySlot Id by supplying a CharacterSlot Id
    */
    uint8 GetInvTypeSlot(uint8 characterSlot) const;
    /*
    * Returns the item's icon
    * Thanks Rochet2 for the function
    */
    std::string GetItemIcon(uint32 entry) const;
    /*
    * Returns the slot icon, empty bag if no icon is found for a slot
    * Thanks Rochet2 for the function
    */
    std::string GetSlotIcon(uint8 slot) const;
    /*
    * Returns item link
    * Thanks Rochet2 for the function
    */
    std::string GetItemLink(uint32 entry, WorldSession* session) const;
    /*
    * Returns slot name
    * Thanks Rochet2 for the function
    */
    const char* GetSlotName(uint8 slot) const;
    /*
    * Returns spell icon & spell name
    */
    std::string GetSpellIcon(uint32 entry, WorldSession* session) const;

    uint32 GetId() const { return Id; }
    uint32 GetOwnerGUID() const { return ownerGUID; }
    uint8 GetRole() const { return role; }
    uint32 GetEntry() const { return entryId; }
    uint32 GetDisplay() const { return displayId; }
    uint8 GetRace() const { return race; }
    uint8 GetGender() const { return gender; }
    uint8 GetType() const { return type; }
    std::string GetName() const { return name; }
    uint8 GetLevel() const { return level; }
    uint32 GetXP() const { return xp; }
    float GetMinDamage() const { return minDamage; }
    float GetMaxDamage() const { return maxDamage; }
    uint32 GetAttackTime() const { return attackTime; }
    uint32 GetStrength() const { return strength; }
    uint32 GetAgility() const { return agility; }
    uint32 GetStamina() const { return stamina; }
    uint32 GetIntellect() const { return intellect; }
    uint32 GetSpirit() const { return spirit; }
    uint32 GetHealth() const { return health; }
    uint32 GetMana() const { return mana; }
    uint32 GetHappiness() const { return happiness; }
    bool IsSummoned() const { return summoned; }
    bool IsBeingCreated() const { return beingCreated; }
    uint8 GetEditSlot() const { return editSlot; }

    typedef std::vector<MercenaryGear> Gear;

    Gear::const_iterator GearBegin() const { return GearContainer.begin(); }
    Gear::const_iterator GearEnd() const { return GearContainer.end(); }

    /*
    * Returns an item entry Id by slotId. If no item is found, returns NULL
    * @slot: Requires CharacterSlot data type
    */
    uint32 GetItemBySlot(uint8 slot) const
    {
        for (auto& itr = GearBegin(); itr != GearEnd(); ++itr)
        {
            if (slot == itr->slot)
                return itr->itemId;
        }

        return NULL;
    }

    /*
    * Returns a vector of item entryIds if any items are found that can be equipped on your Mercenary.
    * @player: The player that gets their inventory checked. Most likely the Mercenary owner.
    * @slot: Requires CharacterSlot data type
    */
    std::vector<uint32> GetEquippableItems(Player* player, uint8 slot)
    {
        std::vector<uint32> tempVector;
        for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
        {
            if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
            {
#ifdef MANGOS
                const ItemPrototype* proto = item->GetProto();
#else
                const ItemTemplate* proto = item->GetTemplate();
#endif
                if (proto)
                {
                    uint8 invSlot = GetInvTypeSlot(slot);
                    if (proto->InventoryType == invSlot && slot != SLOT_MAIN_HAND && slot != SLOT_OFF_HAND)
                        tempVector.push_back(item->GetEntry());
                    if ((proto->InventoryType == invSlot || proto->InventoryType == INVTYPE_2HWEAPON ||
                        proto->InventoryType == INVTYPE_WEAPON) && slot == SLOT_MAIN_HAND)
                        tempVector.push_back(item->GetEntry());
                    if ((proto->InventoryType == invSlot || proto->InventoryType == INVTYPE_SHIELD ||
                        proto->InventoryType == INVTYPE_HOLDABLE) && slot == SLOT_OFF_HAND)
                        tempVector.push_back(item->GetEntry());
                }
            }
        }

        return tempVector;
    }

    /*
    * Returns true if the Mercenary has a weapon equipped, false if not.
    * @parameter: If true, Mercenary has an off hand weapon equipped.
    * If false it checks for a main hand weapon equipped.
    */
    bool HasWeapon(bool offhand)
    {
        for (auto& itr = GearBegin(); itr != GearEnd(); ++itr)
        {
            if (offhand)
            {
                if (itr->slot == SLOT_OFF_HAND)
                    return itr->itemId > 0;
            }
            else
            {
                if (itr->slot == SLOT_MAIN_HAND)
                    return itr->itemId > 0;
            }
        }

        return true;
    }

protected:
    Gear GearContainer;

private:
    uint32 Id;
    uint32 ownerGUID;
    uint8 role;
    uint32 entryId;
    uint32 displayId;
    uint8 race;
    uint8 gender;
    uint8 type;
    std::string name;
    uint8 level;
    uint32 xp;
    float minDamage;
    float maxDamage;
    uint32 attackTime;
    uint32 strength;
    uint32 agility;
    uint32 stamina;
    uint32 intellect;
    uint32 spirit;
    uint32 health;
    uint32 mana;
    uint32 happiness;
    bool summoned;
    bool beingCreated;
    uint8 editSlot;
};

typedef std::unordered_map<uint32, Mercenary*> MercenaryMap;

#endif