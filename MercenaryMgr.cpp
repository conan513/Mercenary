/*
    _______ _____  _______ _______ _______
    |    ___|     \|    ___|   |   |   |   |
    |    ___|  --  |    ___|       |   |   |
    |_______|_____/|_______|__|_|__|_______|
    ©2013-2015 EmuDevs <http://www.emudevs.com/>
    DO NOT REDISTRIBUE! IF I CATCH ANYONE GIVING THIS AWAY, EVERYONE WILL BE REMOVED.
*/
#include "MercenaryMgr.h"

MercenaryMgr::MercenaryMgr() { }

MercenaryMgr::~MercenaryMgr()
{
    for (auto itr = MercenaryContainer.begin(); itr != MercenaryContainer.end(); ++itr)
        delete itr->second;

    MercenaryContainer.clear();
    MercenarySpellsContainer.clear();
    MercenaryStartGearContainer.clear();
    MercenaryTalkContainer.clear();
}

void MercenaryMgr::SaveToList(Mercenary* mercenary)
{
    MercenaryContainer[mercenary->GetId()] = mercenary;
}

void MercenaryMgr::LoadMercenaries()
{
#ifndef MANGOS
    TC_LOG_INFO("server.loading", "Loading Mercenaries..");
#else
    sLog.outBasic("Loading Mercenaries...");
#endif
    for (auto& itr = MercenaryContainer.begin(); itr != MercenaryContainer.end(); ++itr)
        delete itr->second;

    MercenaryContainer.clear();
    MercenarySpellsContainer.clear();
    MercenaryStartGearContainer.clear();
    MercenaryTalkContainer.clear();

#ifndef MANGOS
    QueryResult result = CharacterDatabase.Query("SELECT mercenaryType, mercenaryRole, headEntry, shoulderEntry, chestEntry, legEntry, handEntry, feetEntry, weaponEntry, "
        "offHandEntry, rangedEntry FROM mercenary_start_gear");
#else
    QueryResult* result = CharacterDatabase.Query("SELECT mercenaryType, mercenaryRole, headEntry, shoulderEntry, chestEntry, legEntry, handEntry, feetEntry, weaponEntry, "
        "offHandEntry, rangedEntry FROM mercenary_start_gear");
#endif
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            MercenaryStartGear startGear;
            startGear.mercenaryRole = fields[1].GetUInt8();
            startGear.headEntry = fields[2].GetUInt32();
            startGear.shoulderEntry = fields[3].GetUInt32();
            startGear.chestEntry = fields[4].GetUInt32();
            startGear.legEntry = fields[5].GetUInt32();
            startGear.handEntry = fields[6].GetUInt32();
            startGear.feetEntry = fields[7].GetUInt32();
            startGear.weaponEntry = fields[8].GetUInt32();
            startGear.offHandEntry = fields[9].GetUInt32();
            startGear.rangedEntry = fields[10].GetUInt32();

            MercenaryStartGearContainer[fields[0].GetUInt8()] = startGear;
        } while (result->NextRow());
    }

    result = CharacterDatabase.Query("SELECT entry, type, role, healthpct, message FROM mercenary_talk");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            MercenaryTalking mercenaryTalking;
            mercenaryTalking.type = fields[1].GetUInt8();
            mercenaryTalking.role = fields[2].GetUInt8();
            mercenaryTalking.healthPercentageToTalk = fields[3].GetUInt8();
            mercenaryTalking.text = fields[4].GetString();

            MercenaryTalkContainer[fields[0].GetInt32()] = mercenaryTalking;
        } while (result->NextRow());
    }

    result = CharacterDatabase.Query("SELECT type, role, spellId, isHeal, isActive FROM mercenary_spells");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            MercenarySpells spells;
            spells.role = fields[1].GetUInt8();
            spells.spellId = fields[2].GetUInt32();
            spells.isHeal = fields[3].GetBool();
            spells.isActive = fields[4].GetBool();

            MercenarySpellsContainer[fields[0].GetUInt8()] = spells;
        } while (result->NextRow());
    }

    result = CharacterDatabase.Query("SELECT Id, ownerGUID, role, entryId, displayId, race, gender, type, name, level, xp, minDamage, maxDamage, attackTime, "
        "strength, agility, stamina, intellect, spirit, health, mana, happiness, summoned FROM mercenaries");
    if (result)
    {
        do
        {
            Mercenary* mercenary = new Mercenary;
            if (!mercenary->LoadFromDB(result))
            {
                delete mercenary;
                continue;
            }

            MercenaryContainer[mercenary->GetId()] = mercenary;
        } while (result->NextRow());
   }

#ifndef MANGOS
    TC_LOG_INFO("server.loading", "Loading Mercenaries Completed..");
#else
    sLog.outBasic("Loading Mercenaries Completed..");
#endif
}

void MercenaryMgr::UpdateSummoned(uint32 Id, bool summoned)
{
#ifndef MANGOS
    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_MERCENARY_SUMMON);
    stmt->setBool(0, summoned);
    stmt->setUInt32(1, Id);
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);
#endif
}

void MercenaryMgr::UpdateGear(Player* player)
{
    Mercenary* mercenary = GetMercenaryByOwner(player->GetGUIDLow());
    if (!mercenary)
        return;

    if (!mercenary->IsBeingCreated())
        mercenary->UpdateGear();
}

void MercenaryMgr::OnSummon(Player* player)
{
    if (Mercenary* mercenary = sMercenaryMgr->GetMercenaryByOwner(player->GetGUIDLow()))
    {
        uint32 entry = mercenary->GetEntry();
        uint32 model = mercenary->GetDisplay();
        uint8 type = mercenary->GetType();
        mercenary->Summon(player, entry, model, type);
    }
}