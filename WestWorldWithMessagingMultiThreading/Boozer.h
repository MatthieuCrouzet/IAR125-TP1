#ifndef BOOZER_H
#define BOOZER_H
//------------------------------------------------------------------------
//
//  Name:   Miner.h
//
//  Desc:   A class defining a goldminer.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <string>
#include <cassert>
#include <iostream>

#include "BaseGameEntity.h"
#include "misc/ConsoleUtils.h"
#include "BoozerOwnedStates.h"
#include "fsm/StateMachine.h"

template <class entity_type> class State; //pre-fixed with "template <class entity_type> " for vs8 compatibility

struct Telegram;

//the amount of beverages must have be drink before he feels drunk
const int DrunkLevel = 5;



class Boozer : public BaseGameEntity
{
private:

	//an instance of the state machine class
	StateMachine<Boozer>*  m_pStateMachine;

	//how many beverages the boozer has drunk
	int		m_beveragesDrunk;
	bool	m_disturbed;

public:

	Boozer(int id) :
		m_beveragesDrunk(0),
		m_disturbed(false),
		BaseGameEntity(id)
	{
		//set up state machine
		m_pStateMachine = new StateMachine<Boozer>(this);
		m_pStateMachine->SetCurrentState(Calm::Instance());

	}

	~Boozer() { delete m_pStateMachine; }

	//this must be implemented
	void Update();

	//so must this
	virtual bool  HandleMessage(const Telegram& msg);


	StateMachine<Boozer>* GetFSM()const { return m_pStateMachine; }



	//-------------------------------------------------------------accessors
	int		getBeveragesDrunk()const { return m_beveragesDrunk; }
	bool	isDrunk()const { return m_beveragesDrunk >= DrunkLevel; }
	bool	isClean()const { return m_beveragesDrunk == 0; }
	void    drink() { m_beveragesDrunk += 2; }
	void    rest() { m_beveragesDrunk--; }

};



#endif
