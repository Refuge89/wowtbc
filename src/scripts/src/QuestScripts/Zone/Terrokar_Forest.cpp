/*
 * Moon++ Scripts for Ascent MMORPG Server
 * Copyright (C) 2005-2007 Ascent Team <http://www.ascentemu.com/>
 * Copyright (C) 2007-2008 Moon++ Team <http://www.moonplusplus.info/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.	If not, see <http://www.gnu.org/licenses/>.
 */
#include "StdAfx.h"
#include "../../Base/EAS/EasyFunctions.h"

#define SendQuickMenu(textid) objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), textid, plr); \
	Menu->SendTo(plr);

// Threat from Above
class ThreatFromAboveQAI : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ThreatFromAboveQAI);
		ThreatFromAboveQAI(Creature* pCreature) : CreatureAIScript(pCreature)	{}

	void OnDied(Unit * mKiller)
	{
		if (mKiller->IsPlayer()) 
		{
			QuestLogEntry *en = ((Player*)mKiller)->GetQuestLogForEntry(11096);
			if(en && en->GetMobCount(0) < en->GetQuest()->required_mobcount[0])
			{
				uint32 newcount = en->GetMobCount(0) + 1;
				en->SetMobCount(0, newcount);
				en->SendUpdateAddKill(0);
				en->UpdatePlayerFields();
				return;
			}
		}
	}
};

//The Infested Protectors
class TheInfestedProtectorsQAI : public CreatureAIScript
{
public:

  ADD_CREATURE_FACTORY_FUNCTION(TheInfestedProtectorsQAI);
  TheInfestedProtectorsQAI(Creature* pCreature) : CreatureAIScript(pCreature)  {}
      
  void OnDied(Unit * mKiller)
  {
  	if(!mKiller->IsPlayer())
	  return;
	  
  	Player *plr = (Player*)mKiller;
	
	if(!plr->GetQuestLogForEntry(10896))
	  return;
	
	sEAS.SpawnCreature(plr, 22419, _unit->GetPositionX()+RandomFloat(3.0f), _unit->GetPositionY()+RandomFloat(3.0f), _unit->GetPositionZ(), RandomFloat(1.0f), 60*200);
	sEAS.SpawnCreature(plr, 22419, _unit->GetPositionX()+RandomFloat(3.0f), _unit->GetPositionY()+RandomFloat(3.0f), _unit->GetPositionZ(), RandomFloat(1.0f), 60*200);
	sEAS.SpawnCreature(plr, 22419, _unit->GetPositionX()+RandomFloat(3.0f), _unit->GetPositionY()+RandomFloat(3.0f), _unit->GetPositionZ(), RandomFloat(1.0f), 60*200);
	sEAS.SpawnCreature(plr, 22419, _unit->GetPositionX()+RandomFloat(3.0f), _unit->GetPositionY()+RandomFloat(3.0f), _unit->GetPositionZ(), RandomFloat(1.0f), 60*200);
	sEAS.SpawnCreature(plr, 22419, _unit->GetPositionX()+RandomFloat(3.0f), _unit->GetPositionY()+RandomFloat(3.0f), _unit->GetPositionZ(), RandomFloat(1.0f), 60*200);
  }
};

// Taken in the Night
class TakenInTheNight : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(TakenInTheNight);

	TakenInTheNight(Creature* pCreature) : CreatureAIScript(pCreature) {}

	void OnLoad()
	{
		_unit->GetAIInterface()->m_canMove = false;
		_unit->GetAIInterface()->disable_combat = true;
	}
	
	void OnDied(Unit *mKiller)
	{
		if(!mKiller->IsPlayer())
		return;

		Player *plr = (Player*)mKiller;
		uint8 chance = (uint8)RandomUInt(5);
		uint32 spawn = 0;
		
		switch(chance)
		{
		case 0:
		case 1:
			spawn = 22459; //Freed Shat'ar Warrior
			break; 

		case 2:
			spawn = 21661; //Cabal Skirmisher
			break;

		case 3:
			spawn = 16805; //Broken Skeleton
			break;

		case 4:
			spawn = 18470; //Bonelasher
			break;

		case 5:
			spawn = 22045; //Vengeful Husk
			break;
		}

		Creature *creat = sEAS.SpawnCreature(plr, spawn, _unit->GetPositionX(), _unit->GetPositionY(), _unit->GetPositionZ(), 1*60*1000);

		if(spawn != 22459)
			return;

		creat->GetAIInterface()->m_canMove = false;
		creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "Finally! I'm free!");

		QuestLogEntry *qle = plr->GetQuestLogForEntry(10873);
		if(qle == NULL)
			return;

		if(qle->GetMobCount(0) == qle->GetQuest()->required_mobcount[0])
			return;

		qle->SetMobCount(0, qle->GetMobCount(0)+1);
		qle->SendUpdateAddKill(0);
		qle->UpdatePlayerFields();
	}
};

// Fumping
bool Fumping(uint32 i, Aura* pAura, bool apply)
{
  if(apply)
    return true;

  if(!pAura->GetUnitCaster()->IsPlayer())
    return true;

  Player *plr = (Player*)pAura->GetUnitCaster();
  QuestLogEntry *qle = plr->GetQuestLogForEntry(10929);
  
  if(qle == NULL)
    return true;

  uint8 chance = RandomUInt(1);
  uint32 entry = 0;

  switch(chance)
  {
    case 0:
      entry = 22482;
      break;
      
    case 1:
      entry = 22483;
      break;
  }

  Creature *creat = sEAS.SpawnCreature(plr, entry, plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 5*60*1000);
  
  if(entry == 22483) //Sand Gnomes ;)
    creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "YIEEEEEEEAA!");

  return true;
}

// The Big Bone Worm (Group)
bool TheBigBoneWorm(uint32 i, Aura* pAura, bool apply)
{
	if(apply)
		return true;

	if(!pAura->GetUnitCaster()->IsPlayer())
		return true;

	Player *plr = (Player*)pAura->GetUnitCaster();
	QuestLogEntry *qle = plr->GetQuestLogForEntry(10930);

	if(qle == NULL)
		return true;

	uint8 chance = RandomUInt(2);
	uint32 entry = 0;

	switch(chance)
	{
	case 0:
		entry = 22038;
		break;

	case 1:
		entry = 22483;
		break;

	case 2:
		entry = 22482;
		break;
	}

	Creature *creat = sEAS.SpawnCreature(plr, entry, plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 5*60*1000);

	if(entry == 22483) //Sand Gnomes ;)
		creat->SendChatMessage(CHAT_MSG_MONSTER_SAY, LANG_UNIVERSAL, "YIEEEEEEEAA!");

	return true;

}
// An Improper Burial
class AnImproperBurial : public CreatureAIScript
{
public:
  ADD_CREATURE_FACTORY_FUNCTION(AnImproperBurial);

  AnImproperBurial(Creature* pCreature) : CreatureAIScript(pCreature) {}

  void OnLoad()
  {
    _unit->SetStandState(7);
    _unit->setDeathState(CORPSE);
    _unit->GetAIInterface()->m_canMove = false;
  }
};

bool ShatariTorch(uint32 i, Spell* pSpell)
{
  if(pSpell->u_caster->IsPlayer() == false)
    return true;

  Player *plr = (Player*)pSpell->u_caster;
  Unit *unit_target = (Unit*)plr->GetMapMgr()->GetCreature(GET_LOWGUID_PART(plr->GetSelection()));
  
  if(unit_target == NULL)
    return true;

  if(!unit_target->IsCreature())
    return true;

  Creature *target = (Creature*)unit_target;

  QuestLogEntry *qle = plr->GetQuestLogForEntry(10913);
  if(qle == NULL)
    return true;
  
  GameObject *obj = NULL;

  if(target->GetEntry() == 21859)
  {
    if(qle->GetMobCount(0) == qle->GetQuest()->required_mobcount[0])
      return true;

    qle->SetMobCount(0, qle->GetMobCount(0)+1);
    qle->SendUpdateAddKill(0);

    obj = sEAS.SpawnGameobject(plr, 183816, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 1);
    sEAS.GameobjectDelete(obj, 1*60*1000);
  } 
  else if(target->GetEntry() == 21846)
  {
    if(qle->GetMobCount(1) == qle->GetQuest()->required_mobcount[1])
      return true;

    qle->SetMobCount(1, qle->GetMobCount(1)+1);
    qle->SendUpdateAddKill(1);

    obj = sEAS.SpawnGameobject(plr, 183816, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), target->GetOrientation(), 1);
    sEAS.GameobjectDelete(obj, 1*60*1000);
  }
  else 
    return true; 

  target->Despawn(0, 1*60*1000);
  qle->UpdatePlayerFields();
  plr->UpdateNearbyGameObjects();

  return true;
}

class TheMomentofTruth : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player* plr, bool AutoSend)
	{
		if(!plr)
			return;

		GossipMenu *Menu;
		Creature *doctor = (Creature*)(pObject);
		if (doctor == NULL)
			return;

		objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 1, plr);
		if(plr->GetQuestLogForEntry(10201) && plr->GetItemInterface()->GetItemCount(28500, 0))
			Menu->AddItem( 0, "Try this", 1);

		if(AutoSend)
			Menu->SendTo(plr);
	}

	void GossipSelectOption(Object* pObject, Player* plr, uint32 Id, uint32 IntId, const char * EnteredCode)
	{
		if(!plr)
			return;

		Creature *doctor = (Creature*)(pObject);
		if (doctor == NULL)
			return;

		switch (IntId)
		{
			case 0:
				GossipHello(pObject, plr, true);
				break;

			case 1:
			{
				plr->GetItemInterface()->RemoveItemAmt(2799, 1);
				QuestLogEntry *qle = plr->GetQuestLogForEntry(10201);
				if(qle && qle->GetMobCount(0) < qle->GetQuest()->required_mobcount[0])
				{
					qle->SetMobCount(0, qle->GetMobCount(0)+1);
					qle->SendUpdateAddKill(0);
					qle->UpdatePlayerFields();
				}
			}break;
		}
	}

	void Destroy()
	{
		delete this;
	}
};

bool EvilDrawsNear(uint32 i, Spell * pSpell)
{
  if(!pSpell->u_caster->IsPlayer())
    return true;

  Player *plr = (Player*)pSpell->u_caster;
  QuestLogEntry *qle = plr->GetQuestLogForEntry(10923);
  
  if(qle == NULL)
    return true;

  uint32 entry = 22441;

  Creature *creat = sEAS.SpawnCreature(plr, entry, plr->GetPositionX(), plr->GetPositionY(), plr->GetPositionZ(), 5*60*1000);

  return true;
}

//      Crackin' Some Skulls by Dj
#define Raliq 18585
#define Cooshcoosh 18586
#define Floon 18588

class CrackinSomeSkulls : public CreatureAIScript
{
public:
        ADD_CREATURE_FACTORY_FUNCTION(CrackinSomeSkulls);
         CrackinSomeSkulls(Creature * pCreature) : CreatureAIScript(pCreature) {}
        
        void onload()
        {
                _unit->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, 35 );
        }
};

class CrackinSmeSkullsGossip : public GossipScript
{
public:
        void GossipHello(Object* pObject, Player* plr, bool AutoSend)
        {
                if(plr->GetQuestLogForEntry(10009))
{
                Creature*  pCreature = (pObject->GetTypeId()==TYPEID_UNIT)?(TO_CREATURE(pObject)):NULL;
                if( pCreature == NULL )
                        return;
                {
                        pCreature->SetUInt32Value( UNIT_FIELD_FACTIONTEMPLATE, 14 );
                }
}
        }
                        
};

void SetupTerrokarForest(ScriptMgr * mgr)
{
	mgr->register_creature_script(22144, &ThreatFromAboveQAI::Create);
	mgr->register_creature_script(22143, &ThreatFromAboveQAI::Create);
	mgr->register_creature_script(22148, &ThreatFromAboveQAI::Create);
	mgr->register_creature_script(22355, &TakenInTheNight::Create);
	mgr->register_creature_script(21859, &AnImproperBurial::Create);
	mgr->register_creature_script(21846, &AnImproperBurial::Create);
	mgr->register_creature_script(22307, &TheInfestedProtectorsQAI::Create);
	mgr->register_creature_script(22095, &TheInfestedProtectorsQAI::Create);
	mgr->register_dummy_aura(39238, &Fumping);
	mgr->register_dummy_aura(39246, &TheBigBoneWorm);
	mgr->register_dummy_spell(39189, &ShatariTorch);
	mgr->register_dummy_spell(39239, &EvilDrawsNear);
	GossipScript * gossip1 = (GossipScript*) new TheMomentofTruth();
	mgr->register_gossip_script(19606, gossip1);

	GossipScript * CrackinSomeSkullsGossip = (GossipScript*) new CrackinSmeSkullsGossip;
	mgr->register_creature_script(Raliq, &CrackinSomeSkulls::Create);
	mgr->register_creature_script(Cooshcoosh, &CrackinSomeSkulls::Create);
	mgr->register_creature_script(Floon, &CrackinSomeSkulls::Create);
	mgr->register_gossip_script(Raliq , CrackinSomeSkullsGossip);   // Raliq the Drunk
	mgr->register_gossip_script(Cooshcoosh, CrackinSomeSkullsGossip);       // Coosh'coosh
	mgr->register_gossip_script(Floon, CrackinSomeSkullsGossip);    // Floon
}