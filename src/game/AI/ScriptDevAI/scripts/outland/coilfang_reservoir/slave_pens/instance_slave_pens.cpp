/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* ScriptData
SDName: Instance_Slave_Pens
SD%Complete:
SDComment:
SDCategory: Coilfang Resevoir, Slave Pens
EndScriptData */

#include "AI/ScriptDevAI/include/precompiled.h"
#include "slave_pens.h"

instance_slave_pens::instance_slave_pens(Map* map) : ScriptedInstance(map), m_naturalistYelled(false)
{
    Initialize();
}

void instance_slave_pens::Initialize()
{

}

void instance_slave_pens::SetData(uint32 type, uint32 data)
{
    if (type == DATA_NATURALIST)
        m_naturalistYelled = bool(data);
}

uint32 instance_slave_pens::GetData(uint32 type) const
{
    if (type == DATA_NATURALIST)
        return uint32(m_naturalistYelled);

    return 0;
}

void instance_slave_pens::OnCreatureCreate(Creature * creature)
{
    switch (creature->GetEntry())
    {
        case NPC_NATURALIST_BITE:
        case NPC_NATURALIST_BITE_H:
            m_npcEntryGuidStore[creature->GetEntry()] = creature->GetObjectGuid();
            break;
    }
}

InstanceData* GetInstanceData_instance_slave_pens(Map* map)
{
    return new instance_slave_pens(map);
}

/*######
## at_naturalist_bite
######*/

enum
{
    SAY_AREATRIGGER = -1547000,

    GOSSIP_CAGED    = 7520,
    GOSSIP_RELEASED = 7540,

    FACTION_RELEASED = 113,
};

bool AreaTrigger_at_naturalist_bite(Player* player, AreaTriggerEntry const* /*pAt*/)
{
    ScriptedInstance* instance = (ScriptedInstance*)player->GetMap()->GetInstanceData();
    if (instance->GetData(DATA_NATURALIST) == 0)
    {
        if (Unit* naturalist = instance->GetSingleCreatureFromStorage(player->GetMap()->IsRegularDifficulty() ? NPC_NATURALIST_BITE : NPC_NATURALIST_BITE_H))
        {
            DoScriptText(SAY_AREATRIGGER, naturalist, player);
        }
        instance->SetData(DATA_NATURALIST, 1);
    }
    return true;
}

bool GossipHello_npc_naturalist_bite(Player* player, Creature* creature)
{
    // custom code required because it utilizes two entries
    uint32 gossipId = creature->getFaction() == FACTION_RELEASED ? GOSSIP_RELEASED : GOSSIP_CAGED;
    player->PrepareGossipMenu(creature, gossipId);
    player->SendPreparedGossip(creature);

    return true;
}

void AddSC_instance_slave_pens()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_slave_pens";
    pNewScript->GetInstanceData = &GetInstanceData_instance_slave_pens;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "at_naturalist_bite";
    pNewScript->pAreaTrigger = &AreaTrigger_at_naturalist_bite;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_naturalist_bite";
    pNewScript->pGossipHello = &GossipHello_npc_naturalist_bite;
    pNewScript->RegisterSelf();
}

