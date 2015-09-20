#READ CAREFULLY!

These edits are important. Make sure you add them all. If you need help view the diff file(s).
mercenary_bot is the Mercenary's Script Name in this project. It should not be changed unless you know what you are doing.

This particular README is for manually implementing this system.

Apply the sqls!
Don't forget to setup mercenary_gossip.cpp in the ScriptLoader!

Following files goes in src\game\Object
* Mercenary.cpp
* Mercenary.h
* MercenaryMgr.cpp
* MercenaryMgr.h

Following files goes in src\modules\SD2\scripts\world
* mercenary_gossip.cpp

##World.cpp

* Go to: src/game/WorldHandlers/World.cpp
* Include MercenaryMgr header: #include "MercenaryMgr.h"

* Find: void World::SetInitialWorldSettings()
* Add the following code above: 'sLog.outString("WORLD: World initialized");':

        sMercenaryMgr->LoadMercenaries();

##Player.cpp

* Go to: src/game/Object/Player.cpp
* Include MercenaryMgr header: #include "MercenaryMgr.h"

* Find: void Player::SaveToDB()
* Search for:

         if (Pet* pet = GetPet())
             pet->SavePetToDB(PET_SAVE_AS_CURRENT);
		 
* And replace it with:

        if (Pet* pet = GetPet())
        {
            pet->SavePetToDB(PET_SAVE_AS_CURRENT);
            sMercenaryMgr->OnSave(this, pet);
        }


##Pet.cpp

* Go to: src/game/Object/Pet.cpp
* Include MercenaryMgr header: #include "MercenaryMgr.h"
* Search for: bool Pet::LoadPetFromDB(Player* owner, uint32 petentry, uint32 petnumber, bool current))
* Add the following code:

        sMercenaryMgr->OnSummon(owner);

* UNDER (VERY IMPORTANT):

        owner->SetPet(this);                                    // in DB stored only full controlled creature
		
		
* Still in: src/game/Object/Pet.cpp
* Search for: void Pet::DeleteFromDB(uint32 guidlow, bool separate_transaction)
* At the bottom of the function, add the following code:

        sMercenaryMgr->OnDelete(guidlow);
		
ABOVE (IMPORTANT):

        if (separate_transaction)
            { CharacterDatabase.CommitTransaction(); }


##CreatureAISelector.cpp

* Go to src\server\game\Object\CreatureAISelector.cpp
* Find: CreatureAI* selectAI(Creature* creature)
* Search for the code below:

        if ((!creature->IsPet() || !((Pet*)creature)->isControlled()) && !creature->IsCharmed())
            if (CreatureAI* scriptedAI = sScriptMgr.GetCreatureAI(creature))
                { return scriptedAI; }

* And change it to:

        if (creature->GetScriptName() == "mercenary_bot" || (!creature->IsPet() || !((Pet*)creature)->isControlled()) && !creature->IsCharmed())
            if (CreatureAI* scriptedAI = sScriptMgr.GetCreatureAI(creature))
                { return scriptedAI; }


##ItemDisplayInfo Addition (DBCStores.cpp, DBCStores.h, DBCStructure.h, DBCFmt.h)

* Go to: src/game/Server/DBCStores.cpp
* Uncomment: DBCStorage <ItemDisplayInfoEntry> sItemDisplayInfoStore(ItemDisplayTemplateEntryfmt);
* Next, uncomment: LoadDBC(availableDbcLocales, bar, bad_dbc_files, sItemDisplayInfoStore,     dbcPath,"ItemDisplayInfo.dbc");

* After uncommenting "LoadDBC" remove "dbcCount" argument. It should look like this now:

        LoadDBC(availableDbcLocales, bar, bad_dbc_files, sItemDisplayInfoStore,     dbcPath,"ItemDisplayInfo.dbc");

* Go to: src/game/Server/DBCStores.h
* Uncomment: extern DBCStorage <ItemDisplayInfoEntry>      sItemDisplayInfoStore;

* Go to: src/game/Server/DBCStructure.h
* Search for: struct ItemDisplayInfoEntry
* Once you found it add: 'char* inventoryIcon;' under 'uint32      ID;'

* Go to: src/game/Server/DBCfmt.h
* Uncomment: char const ItemDisplayTemplateEntryfmt[] = "nxxxxxxxxxxixxxxxxxxxxx";
* And replace the entire line of code with: char const ItemDisplayTemplateEntryfmt[] = "nxxxxsxxxxxxxxxxxxxxxxxxx";


##SpellIconEntry Addition (DBCStores.cpp, DBCStores.h, DBCStructure.h, DBCFmt.h)

* Go to: src/game/Server/DBCStores.cpp
* Add the following code on top of the file where the rest of the similar code is: DBCStorage <SpellIconEntry> sSpellIconStore(SpellIconEntryfmt);
* Still in the same file, find: void LoadDBCStores(const std::string& dataPath)
* Add the following code in the function: LoadDBC(availableDbcLocales, bad_dbc_files, sSpellIconStore,              dbcPath, "SpellIcon.dbc");

* Go to: src/game/Server/DBCStores.h
* Add the following code: extern DBCStorage <SpellIconEntry>               sSpellIconStore;

* Go to: src/game/Server/DBCStructure.h
* Add the following code:

        struct SpellIconEntry
        {
            uint32 ID;                                              // 0
            char* spellIcon;                                        // 1
        };

* Go to: src/game/Server/DBCfmt.h
* Add the following code: char const SpellIconEntryfmt[] = "ns";


##Creature.cpp
* Go to: src/game/Object/Creature.cpp
* Search for: bool Creature::InitEntry(uint32 Entry, CreatureData const* data /*=NULL*/, GameEventCreatureData const* eventData /*=NULL*/)
* Once you've found the function, search for: CreatureModelInfo const* minfo = * sObjectMgr.GetCreatureModelRandomGender(display_id);
* And add the following code above it:

        if (GetScriptName() == "mercenary_bot")
            SetFlag(UNIT_FIELD_FLAGS_2, 16);
        else
            RemoveFlag(UNIT_FIELD_FLAGS_2, 16);
		
* Go to: src/game/Object/Creature.cpp
* Search for: bool Creature::UpdateEntry(uint32 Entry, Team team, const CreatureData* data /*=NULL*/, GameEventCreatureData const* eventData /*=NULL*/, bool preserveHPAndPower /*=true*/)
* Once you've found that function, search for (in the function above): SetUInt32Value(UNIT_FIELD_FLAGS, unitFlags);
* And add the following code below it:

        if (GetScriptName() == "mercenary_bot")
            SetUInt32Value(UNIT_FIELD_FLAGS_2, 16);
        else
            SetUInt32Value(UNIT_FIELD_FLAGS_2, 0);

		
##SpellHandler.cpp
* Go to: src/game/WorldHandlers/SpellHandler.cpp
* Include MercenaryMgr header: #include "MercenaryMgr.h"
* Search for: void WorldSession::HandleMirrorImageDataRequest(WorldPacket& recvData)
* And on line 657, above:

        Unit::AuraList const& images = pCreature->GetAurasByType(SPELL_AURA_MIRROR_IMAGE);

* Paste the following code:

        if (pCreature->IsPet())
        {
            Mercenary* mercenary = sMercenaryMgr->GetMercenary(pCreature->GetCharmInfo()->GetPetNumber());
            if (mercenary)
            {
                WorldPacket data(SMSG_MIRRORIMAGE_DATA, 68);
                data << pCreature->GetObjectGuid();
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


##StatSystem.cpp
* Go to: src/game/Object/StatSystem.cpp
* Search for these functions:
 * bool Guardian::UpdateStats(Stats stat)
 * bool Guardian::UpdateAllStats()
 * void Guardian::UpdateArmor()
 * void Guardian::UpdateMaxHealth()
 * void Guardian::UpdateMaxPower(Powers power)
 * void Guardian::UpdateAttackPowerAndDamage(bool ranged)
 * void Guardian::UpdateDamagePhysical(WeaponAttackType attType)

* And add the code below to the BOOLEAN (bool) functions on top of the functions:

        if (GetScriptName() == "mercenary_bot")
            return false;

* And lastly add the code below to the VOID (void) functions on top of the functions:

        if (GetScriptName() == "mercenary_bot")
            return;
