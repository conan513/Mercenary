#READ CAREFULLY!

These edits are important. Make sure you add them all. If you need help view the diff files.
mercenary_bot is the Mercenary's Script Name in this project. It should not be changed unless you know what you are doing.

Apply the sqls!
Don't forget to setup mercenary_gossip.cpp in the ScriptLoader!

Following files goes in src\server\game\AI
* Mercenary.cpp
* Mercenary.h
* MercenaryMgr.cpp
* MercenaryMgr.h

Following files goes in src\server\scripts\Custom
* mercenary_gossip.cpp

Put this in src/server/shared/Database/Implementation/CharacterDatabase.h (above MAX_CHARACTERDATABASE_STATEMENTS):

    CHAR_INS_MERCENARY,
    CHAR_INS_MERCENARY_ACTION,
    CHAR_UPD_MERCENARY_SUMMON,
    CHAR_INS_MERCENARY_GEAR,
    CHAR_UPD_MERCENARY_GEAR,
    CHAR_UPD_MERCENARY_NAME

Put this in src/server/shared/Database/Implementation/CharacterDatabase.cpp:

	// Mercenary
    PrepareStatement(CHAR_INS_MERCENARY, "INSERT INTO mercenaries VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", CONNECTION_BOTH);
    PrepareStatement(CHAR_INS_MERCENARY_ACTION, "INSERT INTO mercenary_actions (guid, ownerGUID, isCast, spellId, castTimer, healPct, castPct) VALUES(?, ?, ?, ?, ?, ?, ?)", CONNECTION_BOTH);
    PrepareStatement(CHAR_UPD_MERCENARY_SUMMON, "UPDATE mercenaries SET summoned=? WHERE Id=?", CONNECTION_ASYNC);
    PrepareStatement(CHAR_INS_MERCENARY_GEAR, "INSERT INTO mercenary_gear (guid, itemId, slot) VALUES (?, ?, ?)", CONNECTION_BOTH);
    PrepareStatement(CHAR_UPD_MERCENARY_GEAR, "UPDATE mercenary_gear SET itemId=? WHERE guid=? AND slot=?", CONNECTION_ASYNC);
    PrepareStatement(CHAR_UPD_MERCENARY_NAME, "UPDATE mercenaries SET name=? WHERE Id=? and ownerGUID=?", CONNECTION_ASYNC);

-----------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
* Go to: src/server/game/Entities/Player.cpp
- Include MercenaryMgr header: #include "MercenaryMgr.h"

* Find: void Player::SaveToDB(bool create /*=false*/)
- At the bottom of the function add the following code:
    sMercenaryMgr->UpdateGear(this);

-----------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
* Go to: src/server/game/Entities/Pet.cpp
* Search for: void Pet::SavePetToDB(PetSaveMode mode)
- Add the code below, below 'GetEntry()' if statement: 

    if (GetScriptName() == "mercenary_bot")
        return;

-----------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
* Find CreatureAISelector.cpp (src\server\game\AI\)
* Search for this code: if (creature->IsPet())
- And change it to: if (creature->IsPet() && creature->GetScriptName() != "mercenary_bot")

-----------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
* Go to: src/server/game/DataStores/DBCStores.cpp
- Uncomment: DBCStorage <ItemDisplayInfoEntry> sItemDisplayInfoStore(ItemDisplayTemplateEntryfmt);
- Next, uncomment: LoadDBC(dbcCount, availableDbcLocales, bad_dbc_files, sItemDisplayInfoStore,        dbcPath, "ItemDisplayInfo.dbc");

* After uncommenting "LoadDBC" remove "dbcCount" argument. It should look like this now:
- LoadDBC(availableDbcLocales, bad_dbc_files, sItemDisplayInfoStore,        dbcPath, "ItemDisplayInfo.dbc");

* Go to: src/server/game/DataStores/DBCStores.h
- Uncomment: extern DBCStorage <ItemDisplayInfoEntry>      sItemDisplayInfoStore;

* Go to: src/server/game/DataStores/DBCStructure.h
* Search for: struct ItemDisplayInfoEntry
- Once you found it add: char* inventoryIcon; under uint32      ID;

* Go to: src/server/game/DataStores/DBCfmt.h
- Uncomment: char const ItemDisplayTemplateEntryfmt[] = "nxxxxxxxxxxixxxxxxxxxxx";
- And replace the entire line of code with: char const ItemDisplayTemplateEntryfmt[] = "nxxxxsxxxxxxxxxxxxxxxxxxx";

-----------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
* Go to: src/server/game/DataStores/DBCStores.cpp
- Add the following code on top of the file where the rest of the similar code is: DBCStorage <SpellIconEntry> sSpellIconStore(SpellIconEntryfmt);
* Still in the same file, find this function: void LoadDBCStores(const std::string& dataPath)
- Add the following code in the function: LoadDBC(availableDbcLocales, bad_dbc_files, sSpellIconStore,              dbcPath, "SpellIcon.dbc");

* Go to: src/server/game/DataStores/DBCStores.h
- Add the following code: extern DBCStorage <SpellIconEntry>               sSpellIconStore;

* Go to: src/server/game/DataStores/DBCStructure.h
- Add the following code:

struct SpellIconEntry
{
    uint32 ID;                                              // 0
    char* spellIcon;                                        // 1
};

Go to: src/server/game/DataStores/DBCfmt.h
Add the following code: char const SpellIconEntryfmt[] = "ns";

-----------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
* Go to: src/server/game/Entities/Creature/Creature.cpp
* Search for: bool Creature::InitEntry(uint32 entry, CreatureData const* data /*= nullptr*/)
- Once you've found that function, go to line 311 and add:

    if (GetScriptName() == "mercenary_bot")
        SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_MIRROR_IMAGE);
    else
        RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_MIRROR_IMAGE);
		
* Go to: src/server/game/Entities/Creature/Creature.cpp
* Search for: bool Creature::UpdateEntry(uint32 entry, CreatureData const* data /*= nullptr*/)
- Once you've found that function, search for (in the function above): SetUInt32Value(UNIT_FIELD_FLAGS_2, cInfo->unit_flags2);
- And replace it with:

    if (GetScriptName() == "mercenary_bot")
        SetUInt32Value(UNIT_FIELD_FLAGS_2, cInfo->unit_flags2 | UNIT_FLAG2_MIRROR_IMAGE);
    else
        SetUInt32Value(UNIT_FIELD_FLAGS_2, cInfo->unit_flags2);
		
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
* Go to: src/server/game/Handlers/SpellHandler.cpp
* Search for: void WorldSession::HandleMirrorImageDataRequest(WorldPacket& recvData)
- And on line 586, below:
    if (!unit)
        return;

- Paste the following code:

    if (Pet* pet = unit->ToPet())
    {
        Mercenary* mercenary = sMercenaryMgr->GetMercenary(pet->GetCharmInfo()->GetPetNumber());
        if (mercenary)
        {
            WorldPacket data(SMSG_MIRRORIMAGE_DATA, 68);
            data << uint64(pet->GetGUID());
            data << uint32(mercenary->GetDisplay());
            data << uint8(mercenary->GetRace());
            data << uint8(mercenary->GetGender());
            data << uint8(1);
            data << uint8(0); // Skin
            data << uint8(0); // Face
            data << uint8(0); // Hair
            data << uint8(0); // Hair color
            data << uint8(0); // Facial hair
            data << uint32(0);
            data << uint32(sMercenaryMgr->GetItemDisplayId(mercenary->GetItemBySlot(SLOT_HEAD)));
            data << uint32(sMercenaryMgr->GetItemDisplayId(mercenary->GetItemBySlot(SLOT_SHOULDERS)));
            data << uint32(0); // Shirt?
            data << uint32(sMercenaryMgr->GetItemDisplayId(mercenary->GetItemBySlot(SLOT_CHEST)));
            data << uint32(0); // Waist
            data << uint32(sMercenaryMgr->GetItemDisplayId(mercenary->GetItemBySlot(SLOT_LEGS)));
            data << uint32(sMercenaryMgr->GetItemDisplayId(mercenary->GetItemBySlot(SLOT_FEET)));
            data << uint32(0); // Wrists
            data << uint32(sMercenaryMgr->GetItemDisplayId(mercenary->GetItemBySlot(SLOT_HANDS)));
            data << uint32(0); // Cloak
            data << uint32(0); // Tabard

            SendPacket(&data);
            return;
        }
    }

* Lastly, go to: src/server/game/Server/WorldSession.cpp
* Press Ctrl+G and put in line number 1368. You should jump to that line.
- Afterwards, insert this case:

        case CMSG_GET_MIRRORIMAGE_DATA:                 // not profiled

-----------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
* Go to: src/server/game/Handlers/MiscHandler.cpp
* Search for: void WorldSession::HandleGossipSelectOptionOpcode(WorldPacket& recvData)
- Inside of that function, search for: if (guid.IsCreatureOrVehicle())
- And change it to: if ((guid.IsCreatureOrVehicle() || guid.IsCreatureOrPet()))

-----------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
* Go to: src/server/game/Entities/Unit/StatSystem.cpp
* Search for these functions:
 * bool Guardian::UpdateStats(Stats stat)
 * bool Guardian::UpdateAllStats()
 * void Guardian::UpdateArmor()
 * void Guardian::UpdateMaxHealth()
 * void Guardian::UpdateMaxPower(Powers power)
 * void Guardian::UpdateAttackPowerAndDamage(bool ranged)
 * void Guardian::UpdateDamagePhysical(WeaponAttackType attType)

- And add the code below to the BOOLEAN (bool) functions on top of the functions:

    if (GetScriptName() == "mercenary_bot")
        return false;

- And lastly add the code below to the VOID (void) functions on top of the functions:

    if (GetScriptName() == "mercenary_bot")
        return;