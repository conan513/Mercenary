/*
    _______ _____  _______ _______ _______
    |    ___|     \|    ___|   |   |   |   |
    |    ___|  --  |    ___|       |   |   |
    |_______|_____/|_______|__|_|__|_______|
    ©2013-2015 EmuDevs <http://www.emudevs.com/>
    DO NOT REDISTRIBUE! IF I CATCH ANYONE GIVING THIS AWAY, EVERYONE WILL BE REMOVED.
*/
#include "MercenaryMgr.h"
#include "PetAI.h"
#ifndef MANGOS
#include "Config.h"
#endif

#ifndef MANGOS
class mercenary_world_load : public WorldScript
{
public:
    mercenary_world_load() : WorldScript("mercenary_world_load") { }

    void OnStartup() override
    {
        sMercenaryMgr->LoadMercenaries();
    }
};

class mercenary_player : public PlayerScript
{
public:
    mercenary_player() : PlayerScript("mercenary_player") { }

    void OnLogin(Player* player, bool /*firstLogin*/) override
    {
        sMercenaryMgr->OnSummon(player);
    }

    void OnUpdateZone(Player* player, uint32 newZone, uint32 newArea) override
    {
        if ((newZone || newArea) && !player->GetPet())
            sMercenaryMgr->OnSummon(player);
    }
};
#endif

class mercenary_npc_gossip : public CreatureScript
{
public:
    mercenary_npc_gossip() : CreatureScript("mercenary_npc_gossip") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        SendToHello(player, creature);
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 actions) override
    {
        player->PlayerTalkClass->ClearMenus();
        Mercenary* mercenary = sMercenaryMgr->GetMercenaryByOwner(player->GetGUIDLow());
        WorldSession* session = player->GetSession();
        if (!mercenary)
        {
            mercenary = new Mercenary;
            if (!mercenary->Create(player, MERCENARY_DEFAULT_ENTRY))
            {
                session->SendNotification("Failed to create Mercenary!");
                return false;
            }
        }

        switch (actions)
        {
            case 1: // Hire Mercenary
                SendHireOrOptionalList(player, creature);
                break;
            case 4:
                player->CLOSE_GOSSIP_MENU();
                break;
            case 5: // Hire random Mercenary
            {
                RandomMercenary randomMercenary = GetRandomMercenary();
                RandomMercenaryTypeRole randomClass = GetRandomTypeAndRole();
                mercenary->SetValues(randomMercenary.model, randomMercenary.race, randomMercenary.gender);
                mercenary->SetType(randomClass.type);
                mercenary->SetRole(randomClass.role);
                CreateMercenary(player, creature, mercenary, MERCENARY_DEFAULT_ENTRY, mercenary->GetDisplay(), mercenary->GetRace(), 
                    mercenary->GetGender(), mercenary->GetRole(), mercenary->GetType());
                player->CLOSE_GOSSIP_MENU();
            }break;
            case 94: // Hire Mercenary and choose what's best for it (and you)
                SendHireList(player, creature);
                break;
            case 15:
                SendFeatureList(player, creature, true);
                break;
            case 6:
                mercenary->SetType(MERCENARY_TYPE_WARRIOR);
                SendFeatureList(player, creature);
                break;
            case 7:
                mercenary->SetType(MERCENARY_TYPE_PALADIN);
                SendFeatureList(player, creature);
                break;
            case 8:
                mercenary->SetType(MERCENARY_TYPE_HUNTER);
                SendFeatureList(player, creature);
                break;
            case 9:
                mercenary->SetType(MERCENARY_TYPE_ROGUE);
                SendFeatureList(player, creature);
                break;
            case 10:
                mercenary->SetType(MERCENARY_TYPE_DK);
                SendFeatureList(player, creature);
                break;
            case 11:
                mercenary->SetType(MERCENARY_TYPE_PRIEST);
                SendFeatureList(player, creature);
                break;
            case 12:
                mercenary->SetType(MERCENARY_TYPE_SHAMAN);
                SendFeatureList(player, creature);
                break;
            case 13:
                mercenary->SetType(MERCENARY_TYPE_WARLOCK);
                SendFeatureList(player, creature);
                break;
            case 14:
                mercenary->SetType(MERCENARY_TYPE_DRUID);
                SendFeatureList(player, creature);
                break;
            case 16:
                mercenary->SetValues(BLOODELF_MALE_MODEL, RACE_BLOODELF, GENDER_MALE);
                SendRoleList(player, creature, mercenary->GetType());
                break;
            case 17:
                mercenary->SetValues(BLOODELF_FEMALE_MODEL, RACE_BLOODELF, GENDER_FEMALE);
                SendRoleList(player, creature, mercenary->GetType());
                break;
            case 18:
                mercenary->SetValues(DRAENEI_MALE_MODEL, RACE_DRAENEI, GENDER_MALE);
                SendRoleList(player, creature, mercenary->GetType());
                break;
            case 19:
                mercenary->SetValues(DRAENEI_FEMALE_MODEL, RACE_DRAENEI, GENDER_FEMALE);
                SendRoleList(player, creature, mercenary->GetType());
                break;
            case 20:
                mercenary->SetValues(DWARF_MALE_MODEL, RACE_DWARF, GENDER_MALE);
                SendRoleList(player, creature, mercenary->GetType());
                break;
            case 21:
                mercenary->SetValues(DWARF_FEMALE_MODEL, RACE_DWARF, GENDER_FEMALE);
                SendRoleList(player, creature, mercenary->GetType());
                break;
            case 22:
                mercenary->SetValues(GNOME_MALE_MODEL, RACE_GNOME, GENDER_MALE);
                SendRoleList(player, creature, mercenary->GetType());
                break;
            case 23:
                mercenary->SetValues(GNOME_FEMALE_MODEL, RACE_GNOME, GENDER_FEMALE);
                SendRoleList(player, creature, mercenary->GetType());
                break;
            case 24:
                mercenary->SetValues(HUMAN_MALE_MODEL, RACE_HUMAN, GENDER_MALE);
                SendRoleList(player, creature, mercenary->GetType());
                break;
            case 25:
                mercenary->SetValues(HUMAN_FEMALE_MODEL, RACE_HUMAN, GENDER_FEMALE);
                SendRoleList(player, creature, mercenary->GetType());
                break;
            case 26:
                mercenary->SetValues(NIGHTELF_MALE_MODEL, RACE_NIGHTELF, GENDER_MALE);
                SendRoleList(player, creature, mercenary->GetType());
                break;
            case 27:
                mercenary->SetValues(NIGHTELF_FEMALE_MODEL, RACE_NIGHTELF, GENDER_FEMALE);
                SendRoleList(player, creature, mercenary->GetType());
                break;
            case 28:
                mercenary->SetValues(ORC_MALE_MODEL, RACE_ORC, GENDER_MALE);
                SendRoleList(player, creature, mercenary->GetType());
                break;
            case 29:
                mercenary->SetValues(ORC_FEMALE_MODEL, RACE_ORC, GENDER_FEMALE);
                SendRoleList(player, creature, mercenary->GetType());
                break;
            case 30:
                mercenary->SetValues(TAUREN_MALE_MODEL, RACE_TAUREN, GENDER_MALE);
                SendRoleList(player, creature, mercenary->GetType());
                break;
            case 31:
                mercenary->SetValues(TAUREN_FEMALE_MODEL, RACE_TAUREN, GENDER_FEMALE);
                SendRoleList(player, creature, mercenary->GetType());
                break;
             case 32:
                mercenary->SetValues(TROLL_MALE_MODEL, RACE_TROLL, GENDER_MALE);
                SendRoleList(player, creature, mercenary->GetType());
                break;
            case 33:
                mercenary->SetValues(TROLL_FEMALE_MODEL, RACE_TROLL, GENDER_FEMALE);
                SendRoleList(player, creature, mercenary->GetType());
                break;
            case 34:
                mercenary->SetRole(ROLE_MELEE_DPS);
                SendConfirmation(player, creature);
                break;
            case 35:
                mercenary->SetRole(ROLE_CASTER_DPS);
                SendConfirmation(player, creature);
                break;
            case 36:
                mercenary->SetRole(ROLE_MARKSMAN_DPS);
                SendConfirmation(player, creature);
                break;
            case 37:
                mercenary->SetRole(ROLE_HEALER);
                SendConfirmation(player, creature);
                break;
            case 38:
                mercenary->SetRole(ROLE_TANK);
                SendConfirmation(player, creature);
                break;
            case 39:
                CreateMercenary(player, creature, mercenary, MERCENARY_DEFAULT_ENTRY, mercenary->GetDisplay(), mercenary->GetRace(),
                    mercenary->GetGender(), mercenary->GetRole(), mercenary->GetType());
                player->CLOSE_GOSSIP_MENU();
                break;
            case 40:
                SendToHello(player, creature);
                break;
            case 41:
                player->CLOSE_GOSSIP_MENU();
                break;
            case 96:
                SendFeatureList(player, creature, true);
                break;
            case 97:
                SendFeatureList(player, creature);
                break;
            case 98:
                SendHireList(player, creature);
                break;
            case 99:
                SendToHello(player, creature);
                break;
        }

        return true;
    }

    RandomMercenary GetRandomMercenary()
    {
        RandomMercenary rndMerc[] =
        {
            { HUMAN_MALE_MODEL, RACE_HUMAN, GENDER_MALE },
            { HUMAN_FEMALE_MODEL, RACE_HUMAN, GENDER_FEMALE },
            { ORC_MALE_MODEL, RACE_ORC, GENDER_MALE },
            { ORC_FEMALE_MODEL, RACE_ORC, GENDER_FEMALE },
            { DWARF_MALE_MODEL, RACE_DWARF, GENDER_MALE },
            { DWARF_FEMALE_MODEL, RACE_DWARF, GENDER_FEMALE },
            { NIGHTELF_MALE_MODEL, RACE_NIGHTELF, GENDER_MALE },
            { NIGHTELF_FEMALE_MODEL, RACE_NIGHTELF, GENDER_FEMALE },
            { TAUREN_MALE_MODEL, RACE_TAUREN, GENDER_MALE },
            { TAUREN_FEMALE_MODEL, RACE_TAUREN, GENDER_FEMALE },
            { GNOME_MALE_MODEL, RACE_GNOME, GENDER_MALE },
            { GNOME_FEMALE_MODEL, RACE_GNOME, GENDER_FEMALE },
            { TROLL_MALE_MODEL, RACE_TROLL, GENDER_MALE },
            { TROLL_FEMALE_MODEL, RACE_TROLL, GENDER_FEMALE },
            { BLOODELF_MALE_MODEL, RACE_BLOODELF, GENDER_MALE },
            { BLOODELF_FEMALE_MODEL, RACE_BLOODELF, GENDER_FEMALE },
            { DRAENEI_MALE_MODEL, RACE_DRAENEI, GENDER_MALE },
            { DRAENEI_FEMALE_MODEL, RACE_DRAENEI, GENDER_FEMALE }
        };
        return rndMerc[rand() % sizeof(rndMerc) / sizeof(*rndMerc)];
    }

    RandomMercenaryTypeRole GetRandomTypeAndRole()
    {
        RandomMercenaryTypeRole rndTypeRole[] =
        {
            { MERCENARY_TYPE_WARRIOR, ROLE_MELEE_DPS },
            { MERCENARY_TYPE_WARRIOR, ROLE_TANK },
            { MERCENARY_TYPE_PALADIN, ROLE_MELEE_DPS },
            { MERCENARY_TYPE_PALADIN, ROLE_TANK },
            { MERCENARY_TYPE_PALADIN, ROLE_HEALER },
            { MERCENARY_TYPE_PRIEST, ROLE_CASTER_DPS },
            { MERCENARY_TYPE_PRIEST, ROLE_HEALER },
            { MERCENARY_TYPE_DK, ROLE_MELEE_DPS },
            { MERCENARY_TYPE_DK, ROLE_TANK },
            { MERCENARY_TYPE_WARLOCK, ROLE_CASTER_DPS },
            { MERCENARY_TYPE_MAGE, ROLE_CASTER_DPS },
            { MERCENARY_TYPE_HUNTER, ROLE_MARKSMAN_DPS },
            { MERCENARY_TYPE_ROGUE, ROLE_MELEE_DPS },
            { MERCENARY_TYPE_DRUID, ROLE_MELEE_DPS },
            { MERCENARY_TYPE_DRUID, ROLE_TANK },
            { MERCENARY_TYPE_DRUID, ROLE_HEALER },
            { MERCENARY_TYPE_DRUID, ROLE_CASTER_DPS },
            { MERCENARY_TYPE_SHAMAN, ROLE_MELEE_DPS },
            { MERCENARY_TYPE_SHAMAN, ROLE_CASTER_DPS },
            { MERCENARY_TYPE_SHAMAN, ROLE_HEALER }
        };
        return rndTypeRole[rand() % sizeof(rndTypeRole) / sizeof(*rndTypeRole)];
    }

    void SendToHello(Player* player, Creature* creature)
    {
        Mercenary* mercenary = sMercenaryMgr->GetMercenaryByOwner(player->GetGUIDLow());
        if (!mercenary)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I want a mercenary.", 0, 1);
        else
        {
            if (mercenary->IsBeingCreated())
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Continue creating your Mercenary", 0, 1);
        }
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nevermind", 0, 4);
#ifndef MANGOS
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
#endif
    }

    void SendHireOrOptionalList(Player* player, Creature* creature)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Hire random Mercenary", 0, 5);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Hire Mercenary and choose what's best for it", 0, 94);
#ifndef MANGOS
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
#endif
    }

    void SendHireList(Player* player, Creature* creature)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Warrior", 0, 6);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Paladin", 0, 7);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Hunter", 0, 8);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Rogue", 0, 9);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Death Knight", 0, 10);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Priest", 0, 11);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Shaman", 0, 12);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Warlock", 0, 13);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Druid", 0, 14);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nevermind", 0, 99);
#ifndef MANGOS
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
#endif
    }

    void SendFeatureList(Player* player, Creature* creature, bool races = false)
    {
        if (!races)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Race", 0, 15);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "<- Back <-", 0, 98); // Send to SendHireList
        }
        else
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Male Blood Elf", 0, 16);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Female Blood Elf", 0, 17);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Male Draenei", 0, 18);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Female Draenei", 0, 19);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Male Dwarf", 0, 20);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Female Dwarf", 0, 21);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Male Gnome", 0, 22);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Female Gnome", 0, 23);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Male Human", 0, 24);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Female Human", 0, 25);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Male Night Elf", 0, 26);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Female Night Elf", 0, 27);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Male Orc", 0, 28);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Female Orc", 0, 29);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Male Tauren", 0, 30);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Female Tauren", 0, 31);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Male Troll", 0, 32);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Female Troll", 0, 33);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "<- Back <-", 0, 97); // Send to SendFeatureList
        }
#ifndef MANGOS
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
#endif
    }

    void SendRoleList(Player* player, Creature* creature, uint8 type)
    {
        bool isMelee = (type == MERCENARY_TYPE_DRUID || type == MERCENARY_TYPE_DK || type == MERCENARY_TYPE_WARRIOR
            || type == MERCENARY_TYPE_ROGUE || type == MERCENARY_TYPE_SHAMAN || type == MERCENARY_TYPE_PALADIN);
        bool isCaster = (type == MERCENARY_TYPE_DRUID || type == MERCENARY_TYPE_MAGE || type == MERCENARY_TYPE_WARLOCK
            || type == MERCENARY_TYPE_PRIEST || type == MERCENARY_TYPE_SHAMAN);
        bool isHealer = (type == MERCENARY_TYPE_DRUID || type == MERCENARY_TYPE_PRIEST || type == MERCENARY_TYPE_PALADIN
            || type == MERCENARY_TYPE_SHAMAN);
        bool isTank = (type == MERCENARY_TYPE_WARRIOR || type == MERCENARY_TYPE_DK || type == MERCENARY_TYPE_PALADIN
            || type == MERCENARY_TYPE_DRUID);
        if (isMelee)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Melee DPS", 0, 34);
        if (isCaster)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Caster DPS", 0, 35);
        if (type == MERCENARY_TYPE_HUNTER)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Marksman DPS", 0, 36);
        if (isHealer)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Healer", 0, 37);
        if (isTank)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tank", 0, 38);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "<- Back <-", 0, 96); // Back to all races & genders
#ifndef MANGOS
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
#endif
    }

    void SendConfirmation(Player* player, Creature* creature)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Confirm?", 0, 39);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Start over again", 0, 40);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I don't want to make a mercenary now!", 0, 41);
#ifndef MANGOS
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
#endif
    }

    void CreateMercenary(Player* player, Creature* creature, Mercenary* mercenary, uint32 entryId, uint32 model, uint8 race, uint8 gender, uint8 role, uint8 type)
    {
        if (!mercenary->Create(player, entryId, model, race, gender, type, role))
        {
            player->GetSession()->SendNotification("Could not create your Mercenary!");
            SendToHello(player, creature);
            return;
        }
        player->CLOSE_GOSSIP_MENU();

        return;
    }
};

class mercenary_bot : public CreatureScript
{
public:
    mercenary_bot() : CreatureScript("mercenary_bot") { }

    bool OnGossipHello(Player* player, Creature* creature) override
    {
        SendToHello(player, creature);
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 actions) override
    {
        player->PlayerTalkClass->ClearMenus();
        Mercenary* mercenary = sMercenaryMgr->GetMercenaryByOwner(player->GetGUIDLow());
        WorldSession* session = player->GetSession();
        if (mercenary)
        {
            std::vector<uint32> tempVector = mercenary->GetEquippableItems(player, mercenary->GetEditSlot());
            for (auto& itr = tempVector.begin(); itr != tempVector.end(); ++itr)
            {
                if (Item* item = player->GetItemByEntry(*itr))
                {
                    if (actions == item->GetEntry())
                    {
                        player->CLOSE_GOSSIP_MENU();
                        if (!mercenary->CanEquipItem(player, item))
                            return false;
                        else
                        {
                            ChatHandler(session).PSendSysMessage("Successfully equipped %s to your Mercenary!", mercenary->GetItemLink(item->GetEntry(), session));
                            return false;
                        }
                    }
                }
            }

            if (!mercenary->IsProcessingAction())
            {
                int spellAction = 100;
                for (auto it = sMercenaryMgr->MercenarySpellsBegin(); it != sMercenaryMgr->MercenarySpellsEnd(); ++it)
                {
                    if (mercenary->GetType() == it->first && mercenary->GetRole() == it->second.role)
                    {
                        if (actions == spellAction)
                        {
                            mercenary->SetSpell(it->second.spellId);
                            SendSetupAction(player, creature, mercenary);
                            return false;
                        }
                        spellAction++;
                    }
                }
            }
        }

        switch (actions)
        {
            case 1:
                SendEquipGear(player, creature, mercenary);
                break;
            case 2:
                for (auto& itr = mercenary->GearBegin(); itr != mercenary->GearEnd(); ++itr)
                {
#ifndef MANGOS
                    const ItemTemplate* proto = sObjectMgr->GetItemTemplate(itr->itemId);
#else
                    const ItemPrototype* proto = sObjectMgr.GetItemPrototype(itr->itemId);
#endif
                    if (proto)
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, mercenary->GetItemIcon(itr->itemId) + mercenary->GetItemLink(itr->itemId, session), 0, 99);
                }
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<- Back <-", 0, 99);
#ifndef MANGOS
                player->SEND_GOSSIP_MENU(1, creature->GetGUID());
#else
                player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
#endif
                break;
            case 3:
                SendSpellList(player, creature, mercenary);
                break;
            case 4:
                player->CLOSE_GOSSIP_MENU();
                break;
            case 5:
                mercenary->RemoveOffHand(player->GetPet());
                player->CLOSE_GOSSIP_MENU();
                break;
            case 6: // Equip Ranged
                SendItemList(player, creature, mercenary, SLOT_RANGED);
                break;
            case 7: // Equip Off Hand
                SendItemList(player, creature, mercenary, SLOT_OFF_HAND);
                break;
            case 8: // Equip Main Hand
                SendItemList(player, creature, mercenary, SLOT_MAIN_HAND);
                break;
            case 9: // Equip Chest
                SendItemList(player, creature, mercenary, SLOT_CHEST);
                break;
            case 10: // Equip Legs
                SendItemList(player, creature, mercenary, SLOT_LEGS);
                break;
            case 11: // Equip Feet
                SendItemList(player, creature, mercenary, SLOT_FEET);
                break;
            case 12: // Equip Hands
                SendItemList(player, creature, mercenary, SLOT_HANDS);
                break;
            case 13: // Equip Shoulders
                SendItemList(player, creature, mercenary, SLOT_SHOULDERS);
                break;
            case 14: // Equip Head
                SendItemList(player, creature, mercenary, SLOT_HEAD);
                break;
            case 15:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Cast every 5 seconds", 0, 19);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Cast every 8 seconds", 0, 20);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Cast every 12 seconds", 0, 21);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Cast every 20 seconds", 0, 22);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Cast every 25 seconds", 0, 23);
#ifndef MANGOS
                player->SEND_GOSSIP_MENU(1, creature->GetGUID());
#else
                player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
#endif
                break;
            case 16:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Cast at 5%", 0, 25);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Cast at 10%", 0, 26);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Cast at 20%", 0, 27);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Cast at 30% ", 0, 28);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Cast at 40%", 0, 29);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Cast at 50%", 0, 30);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Cast at 60%", 0, 31);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Cast at 70%", 0, 32);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Cast at 80%", 0, 33);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Cast at 90%", 0, 34);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Cast at 100%", 0, 35);
#ifndef MANGOS
                player->SEND_GOSSIP_MENU(1, creature->GetGUID());
#else
                player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
#endif
                break;
            case 18:
                if (mercenary->CreateAction(player))
                    ChatHandler(session).SendSysMessage("Successfully created an action for your Mercenary!");
                else
                    session->SendNotification("Failed to create action!");
                player->CLOSE_GOSSIP_MENU();
                break;
            case 19:
                mercenary->SetCastTime(5000);
                SendSetupAction(player, creature, mercenary);
                break;
            case 20:
                mercenary->SetCastTime(8000);
                SendSetupAction(player, creature, mercenary);
                break;
            case 21:
                mercenary->SetCastTime(12000);
                SendSetupAction(player, creature, mercenary);
                break;
            case 22:
                mercenary->SetCastTime(20000);
                SendSetupAction(player, creature, mercenary);
                break;
            case 23:
                mercenary->SetCastTime(25000);
                SendSetupAction(player, creature, mercenary);
                break;
            case 24:
                mercenary->SetSpell(0);
                player->CLOSE_GOSSIP_MENU();
                break;
            case 25:
                mercenary->SetCastPct(5);
                SendSetupAction(player, creature, mercenary);
                break;
            case 26:
                mercenary->SetCastPct(10);
                SendSetupAction(player, creature, mercenary);
                break;
            case 27:
                mercenary->SetCastPct(20);
                SendSetupAction(player, creature, mercenary);
                break;
            case 28:
                mercenary->SetCastPct(30);
                SendSetupAction(player, creature, mercenary);
                break;
            case 29:
                mercenary->SetCastPct(40);
                SendSetupAction(player, creature, mercenary);
                break;
            case 30:
                mercenary->SetCastPct(50);
                SendSetupAction(player, creature, mercenary);
                break;
            case 31:
                mercenary->SetCastPct(60);
                SendSetupAction(player, creature, mercenary);
                break;
            case 32:
                mercenary->SetCastPct(70);
                SendSetupAction(player, creature, mercenary);
                break;
            case 33:
                mercenary->SetCastPct(80);
                SendSetupAction(player, creature, mercenary);
                break;
            case 34:
                mercenary->SetCastPct(90);
                SendSetupAction(player, creature, mercenary);
                break;
            case 35:
                mercenary->SetCastPct(100);
                SendSetupAction(player, creature, mercenary);
                break;
            case 99:
                SendToHello(player, creature);
                break;
        }

        return true;
    }

#ifndef MANGOS
    bool OnGossipSelectCode(Player* player, Creature* creature, uint32 /*sender*/, uint32 actions, const char* code) override
#else
    bool OnGossipSelectWithCode(Player* player, Creature* creature, uint32 /*sender*/, uint32 actions, const char* code) override
#endif
    {
        player->PlayerTalkClass->ClearMenus();
        if (actions == 98)
        {
            Mercenary* mercenary = sMercenaryMgr->GetMercenaryByOwner(player->GetGUIDLow());
            if (!mercenary)
                return false;

            std::string name = code;
            WorldSession* session = player->GetSession();
            if (name.empty())
            {
                session->SendNotification("Name is empty! Failed to change your Mercenary's name.");
                return false;
            }

            if (name.length() <= 3)
            {
                session->SendNotification("Name must contain more than 3 characters. Failed to change your Mercenary's name.");
                return false;
            }

            if (name.length() > 21)
            {
                session->SendNotification("Name is too long. Failed to change your Mercenary's name.");
                return false;
            }

            for (uint8 i = 0; i < strlen(name.c_str()); ++i)
            {
                if (!isalpha(name[i]) && name[i] != ' ')
                {
                    session->SendNotification("Name must contain letters only. Failed to change your Mercenary's name.");
                    return false;
                }
            }

            mercenary->SetName(name);
            creature->SetName(name);

#ifndef MANGOS
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_MERCENARY_NAME);
            stmt->setString(0, name);
            stmt->setUInt32(1, mercenary->GetId());
            stmt->setUInt32(2, mercenary->GetOwnerGUID());
            CharacterDatabase.Execute(stmt);
#else
            CharacterDatabase.PExecute("UPDATE mercenaries SET name=%s WHERE Id=%u AND ownerGUID=%u", name.c_str(), player->GetGUIDLow());
#endif
        }

        player->CLOSE_GOSSIP_MENU();
        return true;
    }

    void SendToHello(Player* player, Creature* creature)
    {
        Mercenary* mercenary = sMercenaryMgr->GetMercenaryByOwner(player->GetGUIDLow());
        if (mercenary && mercenary->GetOwnerGUID() == player->GetGUIDLow())
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Change Gear", 0, 1);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "View equipped Gear", 0, 2);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Change Actions", 0, 3);
            player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, "Change Name", 0, 98, "", 0, true);
        }
        else
            player->CLOSE_GOSSIP_MENU();
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nevermind", 0, 4);
#ifndef MANGOS
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
#endif
    }

    void SendEquipGear(Player* player, Creature* creature, Mercenary* mercenary)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, mercenary->GetSlotIcon(SLOT_HEAD) + mercenary->GetSlotName(SLOT_HEAD), 0, 14);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, mercenary->GetSlotIcon(SLOT_SHOULDERS) + mercenary->GetSlotName(SLOT_SHOULDERS), 0, 13);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, mercenary->GetSlotIcon(SLOT_HANDS) + mercenary->GetSlotName(SLOT_HANDS), 0, 12);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, mercenary->GetSlotIcon(SLOT_FEET) + mercenary->GetSlotName(SLOT_FEET), 0, 11);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, mercenary->GetSlotIcon(SLOT_LEGS) + mercenary->GetSlotName(SLOT_LEGS), 0, 10);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, mercenary->GetSlotIcon(SLOT_CHEST) + mercenary->GetSlotName(SLOT_CHEST), 0, 9);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, mercenary->GetSlotIcon(SLOT_MAIN_HAND) + mercenary->GetSlotName(SLOT_MAIN_HAND), 0, 8);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, mercenary->GetSlotIcon(SLOT_OFF_HAND) + mercenary->GetSlotName(SLOT_OFF_HAND), 0, 7);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, mercenary->GetSlotIcon(SLOT_RANGED) + mercenary->GetSlotName(SLOT_RANGED), 0, 6);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nevermind", 0, 99);
#ifndef MANGOS
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
#endif
    }

    void SendItemList(Player* player, Creature* creature, Mercenary* mercenary, uint8 slot)
    {
        mercenary->SetEditSlot(slot);
        std::vector<uint32> tempVector = mercenary->GetEquippableItems(player, slot);
        for (auto& itr = tempVector.begin(); itr != tempVector.end(); ++itr)
        {
            if (Item* item = player->GetItemByEntry(*itr))
            {
                std::ostringstream ss;
                if (mercenary->GetItemBySlot(slot) != item->GetEntry())
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, mercenary->GetItemIcon(item->GetEntry()) + mercenary->GetItemLink(item->GetEntry(), player->GetSession()), 0, item->GetEntry());
                else
                {
                    ss << mercenary->GetItemLink(item->GetEntry(), player->GetSession()) << " [|cff990000Already Equipped|r]";
                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, mercenary->GetItemIcon(item->GetEntry()) + ss.str().c_str(), 0, 99);
                }
            }
        }

        if (slot == SLOT_OFF_HAND && mercenary->HasWeapon(true))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Remove Off Hand Weapon", 0, 5);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Back to Main Menu", 0, 99);
#ifndef MANGOS
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
#endif
    }

    void SendSpellList(Player* player, Creature* creature, Mercenary* mercenary)
    {
        if (mercenary->IsProcessingAction())
            mercenary->SetSpell(0);

        int spellAction = 100;
        for (auto& it = sMercenaryMgr->MercenarySpellsBegin(); it != sMercenaryMgr->MercenarySpellsEnd(); ++it)
        {
            if (mercenary->GetType() == it->first && mercenary->GetRole() == it->second.role)
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, mercenary->GetSpellIcon(it->second.spellId, player->GetSession()), 0, spellAction++);
        }
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Back to Main Menu", 0, 99);
#ifndef MANGOS
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
#endif
    }

    void SendSetupAction(Player* player, Creature* creature, Mercenary* mercenary)
    {
        MercenarySpells spell;
        for (auto it = sMercenaryMgr->MercenarySpellsBegin(); it != sMercenaryMgr->MercenarySpellsEnd(); ++it)
        {
            if (it->second.spellId == mercenary->GetSelectedSpell())
                spell = it->second;
        }

        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, mercenary->GetSpellIcon(spell.spellId, player->GetSession()) + "[Setup Action]", 0, 0);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Set Cast Time", 0, 15);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Set Cast at Pct", 0, 16);
        if (spell.isHeal)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Set Heal at Pct", 0, 17);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Done", 0, 18);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Quit", 0, 24);
#ifndef MANGOS
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
#endif
    }

    struct mercenary_bot_AI : public PetAI
    {
        mercenary_bot_AI(Creature* creature) : PetAI(creature) { }

        uint32 talkTimer;
        std::string lastMessage;
        uint32 eventTimer[MERCENARY_MAX_ACTIONS];

        void Reset() override
        {
            talkTimer = urand(35000, 120000);
            lastMessage = "";

#ifndef MANGOS
            if (owner = (Player*)me->GetOwner())
#else
            if (owner = (Player*)m_creature->GetOwner())
#endif
            {
                mercenary = sMercenaryMgr->GetMercenaryByOwner(owner->GetGUIDLow());
#ifndef MANGOS
                me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, me->GetFollowAngle());
#else
                m_creature->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
#endif
            }
        }

        void EnterCombat(Unit* /*who*/) override
        {
            if (mercenary)
            {
                for (int i = 0; i < sizeof(eventTimer) / sizeof(*eventTimer); ++i)
                {
                    MercenaryActions action = mercenary->GetActionById(i);
                    eventTimer[i] = action.castTimer;
                }
            }
        }

#ifndef MANGOS
        void UpdateAI(uint32 diff) override
#else
        void UpdateAI(const uint32 diff) override
#endif
        {
            if (mercenary)
            {
                if (talkTimer <= diff)
                {
                    std::vector<MercenaryTalking> tempVector = sMercenaryMgr->GetTalk(mercenary->GetType(), mercenary->GetRole());
                    if (tempVector.size() > 0)
                    {
                        int rnd = sMercenaryMgr->random.Next(0, tempVector.size() - 1);
                        int rnd2 = sMercenaryMgr->random.Next(0, 100);
                        mercenaryTalk = tempVector[rnd];
                        if (rnd2 <= 50 && mercenaryTalk.healthPercentageToTalk == 100 && lastMessage != mercenaryTalk.text)
                        {
#ifndef MANGOS
                            me->Say(mercenaryTalk.text.c_str(), LANG_UNIVERSAL);
#else
                            m_creature->MonsterSay(mercenaryTalk.text.c_str(), LANG_UNIVERSAL);
#endif
                            lastMessage = mercenaryTalk.text;
                        }
                    }
                    talkTimer = urand(35000, 120000);
                }
                else
                    talkTimer -= diff;

#ifndef MANGOS
                if (!UpdateVictim())
                    return;
#else
                if (!m_creature->getVictim())
                    return;
#endif

#ifndef MANGOS
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;
#endif

                for (int i = 0; i < sizeof(eventTimer) / sizeof(*eventTimer); ++i)
                {
                    MercenaryActions action = mercenary->GetActionById(i);
                    if (eventTimer[i] <= diff)
                    {
#ifndef MANGOS
                        if (me->GetHealthPct() <= action.castAtPct)
                            me->CastSpell(me->GetVictim(), action.castTimer);
                        eventTimer[i] = action.castTimer;
#else
                        if (m_creature->GetHealthPercent() <= action.castAtPct)
                            if (DoCastSpellIfCan(m_creature->getVictim(), action.spellId) == CAST_OK)
                                eventTimer[i] = action.castTimer;
#endif
                    }
                    else
                        eventTimer[i] -= diff;
                }
            }
            DoMeleeAttackIfReady();
        }
    private:
        Mercenary* mercenary;
        MercenaryTalking mercenaryTalk;
        Player* owner;
    };

#ifndef MANGOS
    CreatureAI* GetAI(Creature* creature) const override
#else
    CreatureAI* GetAI(Creature* creature) override
#endif
    {
        return new mercenary_bot_AI(creature);
    }
};

void MercenarySetup()
{
#ifndef MANGOS
    new mercenary_npc_gossip;
    new mercenary_bot;
    new mercenary_world_load;
    new mercenary_player;
#else
    Script* s;
    s = new mercenary_npc_gossip;
    s->RegisterSelf();
    s = new mercenary_bot;
    s->RegisterSelf();
#endif
}