/*
    ©2013-2015 EmuDevs <http://www.emudevs.com/>
    MaNGOS <http://getmangos.eu>
    TrinityCore <http://www.trinitycore.org>
*/
#include "MercenaryMgr.h"

MercenaryMgr::MercenaryMgr() { }

MercenaryMgr::~MercenaryMgr()
{
    Clear();
}

void MercenaryMgr::SaveToList(Mercenary* mercenary)
{
    MercenaryContainer[mercenary->GetId()] = mercenary;
}

void MercenaryMgr::DeleteFromList(Mercenary* mercenary)
{
    MercenaryContainer.erase(mercenary->GetId());
}

void MercenaryMgr::LoadMercenaries()
{
    Clear();
#ifndef MANGOS
    TC_LOG_INFO("server.loading", "Loading Mercenaries..");
#else
    sLog.outBasic("Loading Mercenaries...");
#endif

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

            MercenaryStarterGear starterGear;
            starterGear.mercenaryType = fields[0].GetUInt8();
            starterGear.mercenaryRole = fields[1].GetUInt8();
            starterGear.headEntry = fields[2].GetUInt32();
            starterGear.shoulderEntry = fields[3].GetUInt32();
            starterGear.chestEntry = fields[4].GetUInt32();
            starterGear.legEntry = fields[5].GetUInt32();
            starterGear.handEntry = fields[6].GetUInt32();
            starterGear.feetEntry = fields[7].GetUInt32();
            starterGear.weaponEntry = fields[8].GetUInt32();
            starterGear.offHandEntry = fields[9].GetUInt32();
            starterGear.rangedEntry = fields[10].GetUInt32();

            MercenaryStartGearContainer.push_back(starterGear);
        } while (result->NextRow());
    }

    result = CharacterDatabase.Query("SELECT type, role, healthpct, message FROM mercenary_talk");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            MercenaryTalking mercenaryTalking;
            mercenaryTalking.type = fields[0].GetUInt8();
            mercenaryTalking.role = fields[1].GetUInt8();
            mercenaryTalking.healthPercentageToTalk = fields[2].GetUInt8();
            mercenaryTalking.text = fields[3].GetString();

            MercenaryTalkContainer.push_back(mercenaryTalking);
        } while (result->NextRow());
    }

    result = CharacterDatabase.Query("SELECT type, role, spellId, isDefaultAura, isActive FROM mercenary_spells");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            MercenarySpell spells;
            spells.type = fields[0].GetUInt8();
            spells.role = fields[1].GetUInt8();
            spells.spellId = fields[2].GetUInt32();
            spells.isDefaultAura = fields[3].GetBool();
            spells.isActive = fields[4].GetBool();

            MercenarySpellsContainer.push_back(spells);
        } while (result->NextRow());
    }

    result = CharacterDatabase.Query("SELECT Id, ownerGUID, role, displayId, race, gender, type, strength, agility, stamina, intellect, spirit, summoned FROM mercenaries");
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

void MercenaryMgr::Clear()
{
    for (auto& itr = MercenaryContainer.begin(); itr != MercenaryContainer.end(); ++itr)
        delete itr->second;

    MercenaryContainer.clear();
    MercenarySpellsContainer.clear();
    MercenaryStartGearContainer.clear();
    MercenaryTalkContainer.clear();
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

void MercenaryMgr::OnSave(Player* player, Pet* pet)
{
    Mercenary* mercenary = GetMercenaryByOwner(player->GetGUIDLow());
    if (!mercenary)
        return;

    if (!mercenary->IsBeingCreated())
    {
        mercenary->UpdateGear();
#ifndef MANGOS
        SQLTransaction trans = CharacterDatabase.BeginTransaction();
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_MERCENARY);
        stmt->setUInt32(0, mercenary->GetId());
        trans->Append(stmt);
        CharacterDatabase.CommitTransaction(trans);
#else
        CharacterDatabase.BeginTransaction();
        static SqlStatementID delMerc;

        SqlStatement stmt = CharacterDatabase.CreateStatement(delMerc, "DELETE FROM mercenaries WHERE Id=?");
        stmt.PExecute(mercenary->GetId());

        CharacterDatabase.CommitTransaction();
#endif
        mercenary->SaveToDB();
    }
}

void MercenaryMgr::OnDelete(uint32 guidLow)
{
    Mercenary* mercenary = GetMercenary(guidLow);
    if (!mercenary)
        return;

    mercenary->DeleteFromDB();
}

void MercenaryMgr::OnSummon(Player* player)
{
    if (Mercenary* mercenary = GetMercenaryByOwner(player->GetGUIDLow()))
        mercenary->Summon(player);
}