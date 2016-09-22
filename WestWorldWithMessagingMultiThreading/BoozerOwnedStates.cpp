#include "BoozerOwnedStates.h"
#include "fsm/State.h"
#include "Boozer.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "Time/CrudeTimer.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif


//INSTANCE
Calm* Calm::Instance()
{
	static Calm instance;

	return &instance;
}
DrunkBoozer * DrunkBoozer::Instance()
{
	static DrunkBoozer instance;

	return &instance;
}
AngryBoozer * AngryBoozer::Instance()
{
	static AngryBoozer instance;

	return &instance;
}
Violent * Violent::Instance()
{
	static Violent instance;

	return &instance;
}

//ENTER
void Calm::Enter(Boozer* pBoozer)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pBoozer->ID()) << " is leaning on the bar quietly";
}
void DrunkBoozer::Enter(Boozer* pBoozer)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pBoozer->ID()) << " is sleeping on the bar, snoring";
}
void AngryBoozer::Enter(Boozer* pBoozer)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pBoozer->ID()) << " is standing next to the bar";
}
void Violent::Enter(Boozer* pBoozer)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pBoozer->ID()) << " is shouting words";
}

//EXECUTE
void Calm::Execute(Boozer* pBoozer)
{
	pBoozer->drink();
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pBoozer->ID()) << " drinks another glass";
	if (pBoozer->isDrunk())
	{
		pBoozer->GetFSM()->ChangeState(DrunkBoozer::Instance());
	}
}
void DrunkBoozer::Execute(Boozer* pBoozer)
{
	pBoozer->rest();
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pBoozer->ID()) << ": " << "Zzzz...";
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		pBoozer->ID(),        //ID of sender
		ent_Miner_Bob,            //ID of recipient
		Msg_ZZZZ,   //the message
		NO_ADDITIONAL_INFO);
	if (pBoozer->isClean())
	{
		pBoozer->GetFSM()->ChangeState(Calm::Instance());
	}
}
void AngryBoozer::Execute(Boozer* pBoozer)
{
	pBoozer->drink();
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pBoozer->ID()) << " drinks another glass and grumble";
}
void Violent::Execute(Boozer* pBoozer)
{
	pBoozer->drink();
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pBoozer->ID()) << " drinks another glass and taps his fist on the bar";
}

//EXIT
void Calm::Exit(Boozer* pBoozer)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pBoozer->ID()) << " falls tired on the bar";
}
void DrunkBoozer::Exit(Boozer* pBoozer)
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pBoozer->ID()) << ": " << "Wakin' up!";
}
void AngryBoozer::Exit(Boozer* pBoozer)
{
}
void Violent::Exit(Boozer* pBoozer)
{
}

//ONMESSAGE

bool Calm::OnMessage(Boozer* pBoozer, const Telegram& msg)
{
	return false;
}

bool DrunkBoozer::OnMessage(Boozer* pBoozer, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg)
	{
	case Msg_TooMuchNoise:

		cout << "\nMessage handled by " << GetNameOfEntity(pBoozer->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		pBoozer->GetFSM()->ChangeState(AngryBoozer::Instance());
		cout << "\n" << GetNameOfEntity(pBoozer->ID())
			<< ": Oh, guys I was sleeping!";
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pBoozer->ID(),        //ID of sender
			ent_Miner_Bob,            //ID of recipient
			Msg_Provocation,   //the message
			NO_ADDITIONAL_INFO);
		return true;

	}//end switch

	return false; //send message to global message handler
}

bool AngryBoozer::OnMessage(Boozer* pBoozer, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg)
	{
	case Msg_Provocation:

		cout << "\nMessage handled by " << GetNameOfEntity(pBoozer->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pBoozer->ID())
			<< ": What's the matter with you?";

		pBoozer->GetFSM()->ChangeState(Violent::Instance());

		return true;
	case Msg_CalmDown:

		cout << "\nMessage handled by " << GetNameOfEntity(pBoozer->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pBoozer->ID())
			<< ": Sorry, it's my fault, I'm drunk !";
		Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
			pBoozer->ID(),        //ID of sender
			ent_Miner_Bob,            //ID of recipient
			Msg_CalmDown,   //the message
			NO_ADDITIONAL_INFO);

		pBoozer->GetFSM()->ChangeState(Calm::Instance());

		return true;

	}//end switch

	return false; //send message to global message handler
}

bool Violent::OnMessage(Boozer* pBoozer, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_BLUE);

	switch (msg.Msg)
	{
	case Msg_CalmDown:

		cout << "\nMessage handled by " << GetNameOfEntity(pBoozer->ID())
			<< " at time: " << Clock->GetCurrentTime();

		SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

		cout << "\n" << GetNameOfEntity(pBoozer->ID())
			<< ": It's good you finished your childishness?";

		pBoozer->GetFSM()->ChangeState(AngryBoozer::Instance());

		return true;

	}//end switch

	return false; //send message to global message handler
}