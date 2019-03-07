/*
 * SEGS - Super Entity Game Server
 * http://www.segs.io/
 * Copyright (c) 2006 - 2019 SEGS Team (see AUTHORS.md)
 * This software is licensed under the terms of the 3-clause BSD License. See LICENSE.md for details.
 */

#include "ClientOptionService.h"
#include "GameData/Entity.h"
#include "GameData/playerdata_definitions.h"
#include "GameData/map_definitions.h"
#include "GameData/EntityHelpers.h"
#include "Messages/Map/MapEvents.h"
#include <QtCore/QDebug>

using namespace SEGSEvents;

void ClientOptionService::on_set_keybind(Entity* ent, Event *ev)
{
    SetKeybind* casted_ev = static_cast<SetKeybind *>(ev);
    KeyName key = static_cast<KeyName>(casted_ev->key);
    ModKeys mod = static_cast<ModKeys>(casted_ev->mods);

    ent->m_player->m_keybinds.setKeybind(casted_ev->profile, key, mod, casted_ev->command, casted_ev->is_secondary);
    //qCDebug(logMapEvents) << "Setting keybind: " << ev->profile << QString::number(ev->key) << QString::number(ev->mods) << ev->command << ev->is_secondary;
}


void ClientOptionService::on_remove_keybind(Entity* ent, Event *ev)
{
    RemoveKeybind* casted_ev = static_cast<RemoveKeybind *>(ev);
    ent->m_player->m_keybinds.removeKeybind(casted_ev->profile,(KeyName &)casted_ev->key,(ModKeys &)casted_ev->mods);
    //qCDebug(logMapEvents) << "Clearing Keybind: " << ev->profile << QString::number(ev->key) << QString::number(ev->mods);
}

void ClientOptionService::on_reset_keybinds(Entity* ent, Event *ev)
{
    // the event is used to merely get the session and then the kbSettings, which we already found in MapInstance
    const GameDataStore &data(getGameData());
    const Parse_AllKeyProfiles &default_profiles(data.m_keybind_profiles);

    ent->m_player->m_keybinds.resetKeybinds(default_profiles);
    //qCDebug(logMapEvents) << "Resetting Keybinds to defaults.";
}

void ClientOptionService::on_select_keybind_profile(Entity* ent, Event *ev)
{
    SelectKeybindProfile* casted_ev = static_cast<SelectKeybindProfile *>(ev);
    ent->m_player->m_keybinds.setKeybindProfile(casted_ev->profile);
    //qCDebug(logMapEvents) << "Saving currently selected Keybind Profile. Profile name: " << ev->profile;
}

void ClientOptionService::on_client_options(Entity* ent, Event* ev)
{
    SaveClientOptions* casted_ev = static_cast<SaveClientOptions *>(ev);

    markEntityForDbStore(ent,DbStoreFlags::PlayerData);
    ent->m_player->m_options = casted_ev->data;
}

void ClientOptionService::on_switch_viewpoint(Entity* ent, Event *ev)
{
    SwitchViewPoint* casted_ev = static_cast<SwitchViewPoint *>(ev);

    ent->m_player->m_options.m_first_person_view = casted_ev->new_viewpoint_is_firstperson;
    qCDebug(logMapEvents) << "Saving viewpoint mode to ClientOptions" << casted_ev->new_viewpoint_is_firstperson;
}

