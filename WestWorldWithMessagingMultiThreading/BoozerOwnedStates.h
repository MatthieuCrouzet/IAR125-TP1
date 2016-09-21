#ifndef BOOZER_OWNED_STATES_H
#define BOOZER_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   MinerOwnedStates.h
//
//  Desc:   All the states that can be assigned to the Miner class.
//          Note that a global state has not been implemented.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include "fsm/State.h"


class Boozer;
struct Telegram;




class Calm : public State<Boozer>
{
private:

	Calm() {}

	//copy ctor and assignment should be private
	Calm(const Calm&);
	Calm& operator=(const Calm&);

public:

	//this is a singleton
	static Calm* Instance();

	virtual void Enter(Boozer* boozer);

	virtual void Execute(Boozer* boozer);

	virtual void Exit(Boozer* boozer);

	virtual bool OnMessage(Boozer* agent, const Telegram& msg);
};

class DrunkBoozer : public State<Boozer>
{
private:

	DrunkBoozer() {}

	//copy ctor and assignment should be private
	DrunkBoozer(const DrunkBoozer&);
	DrunkBoozer& operator=(const DrunkBoozer&);

public:

	//this is a singleton
	static DrunkBoozer* Instance();

	virtual void Enter(Boozer* boozer);

	virtual void Execute(Boozer* boozer);

	virtual void Exit(Boozer* boozer);

	virtual bool OnMessage(Boozer* agent, const Telegram& msg);
};

class AngryBoozer : public State<Boozer>
{
private:

	AngryBoozer() {}

	//copy ctor and assignment should be private
	AngryBoozer(const AngryBoozer&);
	AngryBoozer& operator=(const AngryBoozer&);

public:

	//this is a singleton
	static AngryBoozer* Instance();

	virtual void Enter(Boozer* boozer);

	virtual void Execute(Boozer* boozer);

	virtual void Exit(Boozer* boozer);

	virtual bool OnMessage(Boozer* agent, const Telegram& msg);
};


class Violent : public State<Boozer>
{
private:

	Violent() {}

	//copy ctor and assignment should be private
	Violent(const Violent&);
	Violent& operator=(const Violent&);

public:

	//this is a singleton
	static Violent* Instance();

	virtual void Enter(Boozer* boozer);

	virtual void Execute(Boozer* boozer);

	virtual void Exit(Boozer* boozer);

	virtual bool OnMessage(Boozer* agent, const Telegram& msg);
};

#endif