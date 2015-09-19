/*
    ©2013-2015 EmuDevs <http://www.emudevs.com/>
    MaNGOS <http://getmangos.eu>
*/
#ifndef MERCENARYMGR_H
#define MERCENARYMGR_H

#include "Mercenary.h"
#include <random>

typedef std::vector<MercenarySpells> MercenarySpellsMap;
typedef std::unordered_map<uint8, MercenaryStartGear> MercenaryStartGearMap;
typedef std::unordered_map<int, MercenaryTalking> MercenaryTalkMap;

class Random
{
public:
    Random() = default;
    Random(std::mt19937::result_type seed) : eng(seed) { }

    int Next(int min, int max)
    {
        return std::uniform_int_distribution < int > {min, max}(eng);
    }

private:
    std::mt19937 eng{ std::random_device{}() };
};

class MercenaryMgr
{
public:
    static MercenaryMgr* GetInstance()
    {
        static MercenaryMgr instance;
        return &instance;
    }

    /*
    * Loads Mercenaries from Database
    */
    void LoadMercenaries();
    /*
    * Saves Mercenary to a container
    */
    void SaveToList(Mercenary* mercenary);
    /*
    * Sets if the Mercenary is currently summoned
    */
    void UpdateSummoned(uint32 Id, bool summoned);
    /*
    * On Mercenary save to database
    * Also updates gear and saves Mercenary spells
    * Cals when players saves
    */
    void OnSave(Player* player, Pet* pet);
    /*
    * On Mercenary summon
    * Calls when you change zones
    * If your Mercenary isn't with you, you can change zones to summon it back automatically
    */
    void OnSummon(Player* player);

    MercenaryMap::const_iterator MercenaryBegin() const { return MercenaryContainer.begin(); }
    MercenaryMap::const_iterator MercenaryEnd() const { return MercenaryContainer.end(); }
    MercenaryStartGearMap::const_iterator MercenaryStartGearBegin() const { return MercenaryStartGearContainer.begin(); }
    MercenaryStartGearMap::const_iterator MercenaryStartGearEnd() const { return MercenaryStartGearContainer.end(); }
    MercenarySpellsMap::const_iterator MercenarySpellsBegin() const { return MercenarySpellsContainer.begin(); }
    MercenarySpellsMap::const_iterator MercenarySpellsEnd() const { return MercenarySpellsContainer.end(); }
    MercenaryTalkMap::const_iterator MercenaryTalkBegin() const { return MercenaryTalkContainer.begin(); }
    MercenaryTalkMap::const_iterator MercenaryTalkEnd() const { return MercenaryTalkContainer.end(); }

    /*
    * Returns Mercenary's class by search for pet's guid
    */
    Mercenary* GetMercenary(uint32 Id)
    {
        auto itr = MercenaryContainer.find(Id);
        if (itr != MercenaryEnd())
            return itr->second;

        return nullptr;
    }

    /*
    * Returns Mercenary's class by search for owner's guid
    */
    Mercenary* GetMercenaryByOwner(uint32 ownerGUID)
    {
        for (auto& itr = MercenaryBegin(); itr != MercenaryEnd(); ++itr)
            if (itr->second->GetOwnerGUID() == ownerGUID)
                return itr->second;

        return nullptr;
    }

    /*
    *  Returns a vector of the correct talk sayings for that Mercenary type and role
    */
    std::vector<MercenaryTalking> GetTalk(uint8 type, uint8 role)
    {
        std::vector<MercenaryTalking> tempTalk;
        for (auto& itr = MercenaryTalkBegin(); itr != MercenaryTalkEnd(); ++itr)
            if (itr->second.type == type && itr->second.role == role)
                tempTalk.push_back(itr->second);

        return tempTalk;
    }

    /*
    * Returns item's displayId by itemEntry
    */
    uint32 GetItemDisplayId(uint32 itemEntry)
    {
#ifndef MANGOS
        const ItemTemplate* proto = sObjectMgr->GetItemTemplate(itemEntry);
#else
        const ItemPrototype* proto = sObjectMgr.GetItemPrototype(itemEntry);
#endif
        if (proto)
            return proto->DisplayInfoID;

        return 0;
    }

    Random random;

protected:
    MercenarySpellsMap MercenarySpellsContainer;
    MercenaryStartGearMap MercenaryStartGearContainer;
    MercenaryMap MercenaryContainer;
    MercenaryTalkMap MercenaryTalkContainer;

private:
    MercenaryMgr();
    ~MercenaryMgr();
};

#define sMercenaryMgr MercenaryMgr::GetInstance()

#endif