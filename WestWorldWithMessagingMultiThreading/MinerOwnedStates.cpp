#include "MinerOwnedStates.h"
#include "fsm/State.h"
#include "Miner.h"
#include "Locations.h"
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


//------------------------------------------------------------------------methods for EnterMineAndDigForNugget
EnterMineAndDigForNugget* EnterMineAndDigForNugget::Instance()
{
  static EnterMineAndDigForNugget instance;

  return &instance;
}


void EnterMineAndDigForNugget::Enter(Miner* pMiner)
{
  //if the miner is not already located at the goldmine, he must
  //change location to the gold mine
  if (pMiner->Location() != goldmine)
  {
	  SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Walkin' to the goldmine";

    pMiner->ChangeLocation(goldmine);
  }
}


void EnterMineAndDigForNugget::Execute(Miner* pMiner)
{  
  //Now the miner is at the goldmine he digs for gold until he
  //is carrying in excess of MaxNuggets. If he gets thirsty during
  //his digging he packs up work for a while and changes state to
  //gp to the saloon for a whiskey.
  pMiner->AddToGoldCarried(1);

  pMiner->IncreaseFatigue();
  SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Pickin' up a nugget";

  //if enough gold mined, go and put it in the bank
  if (pMiner->PocketsFull())
  {
    pMiner->GetFSM()->ChangeState(VisitBankAndDepositGold::Instance());
  }

  if (pMiner->Thirsty())
  {
    pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
  }
}


void EnterMineAndDigForNugget::Exit(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
       << "Ah'm leavin' the goldmine with mah pockets full o' sweet gold";
}


bool EnterMineAndDigForNugget::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}

//------------------------------------------------------------------------methods for VisitBankAndDepositGold

VisitBankAndDepositGold* VisitBankAndDepositGold::Instance()
{
  static VisitBankAndDepositGold instance;

  return &instance;
}

void VisitBankAndDepositGold::Enter(Miner* pMiner)
{  
  //on entry the miner makes sure he is located at the bank
  if (pMiner->Location() != bank)
  {
	  SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Goin' to the bank. Yes siree";

    pMiner->ChangeLocation(bank);
  }
}


void VisitBankAndDepositGold::Execute(Miner* pMiner)
{
  //deposit the gold
  pMiner->AddToWealth(pMiner->GoldCarried());
    
  pMiner->SetGoldCarried(0);
  SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
       << "Depositing gold. Total savings now: "<< pMiner->Wealth();

  //wealthy enough to have a well earned rest?
  if (pMiner->Wealth() >= ComfortLevel)
  {
	  SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
         << "WooHoo! Rich enough for now. Back home to mah li'lle lady";
      
    pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());      
  }

  //otherwise get more gold
  else 
  {
    pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }
}


void VisitBankAndDepositGold::Exit(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Leavin' the bank";
}


bool VisitBankAndDepositGold::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}
//------------------------------------------------------------------------methods for GoHomeAndSleepTilRested

GoHomeAndSleepTilRested* GoHomeAndSleepTilRested::Instance()
{
  static GoHomeAndSleepTilRested instance;

  return &instance;
}

void GoHomeAndSleepTilRested::Enter(Miner* pMiner)
{
  if (pMiner->Location() != shack)
  {
	  SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Walkin' home";

    pMiner->ChangeLocation(shack); 

    //let the wife know I'm home
	SetTextColor(BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
    Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pMiner->ID(),        //ID of sender
                              ent_Elsa,            //ID of recipient
                              Msg_HiHoneyImHome,   //the message
                              NO_ADDITIONAL_INFO);    
  }
}

void GoHomeAndSleepTilRested::Execute(Miner* pMiner)
{ 
  //if miner is not fatigued start to dig for nuggets again.
  if (!pMiner->Fatigued())
  {
	  SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
     cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " 
          << "All mah fatigue has drained away. Time to find more gold!";

     pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }

  else 
  {
    //sleep
    pMiner->DecreaseFatigue();
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "ZZZZ... ";
  } 
}

void GoHomeAndSleepTilRested::Exit(Miner* pMiner)
{ 
}


bool GoHomeAndSleepTilRested::OnMessage(Miner* pMiner, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);

   switch(msg.Msg)
   {
   case Msg_StewReady:

     cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID()) 
     << " at time: " << Clock->GetCurrentTime();

     SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

     cout << "\n" << GetNameOfEntity(pMiner->ID()) 
          << ": Okay Hun, ahm a comin'!";

     pMiner->GetFSM()->ChangeState(EatStew::Instance());
      
     return true;

   }//end switch

   return false; //send message to global message handler
}

//------------------------------------------------------------------------QuenchThirst

QuenchThirst* QuenchThirst::Instance()
{
  static QuenchThirst instance;

  return &instance;
}

void QuenchThirst::Enter(Miner* pMiner)
{
  if (pMiner->Location() != saloon)
  {    
    pMiner->ChangeLocation(saloon);
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Boy, ah sure is thusty! Walking to the saloon";
  }
}

void QuenchThirst::Execute(Miner* pMiner)
{
  pMiner->BuyAndDrinkAWhiskey();
  SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "That's mighty fine sippin' liquer";
  pMiner->GetFSM()->ChangeState(Drunk::Instance());  
}

void QuenchThirst::Exit(Miner* pMiner)
{ 
}


bool QuenchThirst::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}

//------------------------------------------------------------------------EatStew

EatStew* EatStew::Instance()
{
  static EatStew instance;

  return &instance;
}


void EatStew::Enter(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Smells Reaaal goood Elsa!";
}

void EatStew::Execute(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
  cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Tastes real good too!";

  pMiner->GetFSM()->RevertToPreviousState();
}

void EatStew::Exit(Miner* pMiner)
{ 
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Thankya li'lle lady. Ah better get back to whatever ah wuz doin'";
}


bool EatStew::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}


Drunk* Drunk::Instance()
{
	static Drunk instance;

	return &instance;
}


void Drunk::Enter(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "I reckon' I drank too much.";
}

void Drunk::Execute(Miner* pMiner)
{
	pMiner->DrinkAWhiskey();
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << " drink another glass.";
	if (pMiner->Location() == saloon && pMiner->getNbBeverages()>4) {
		pMiner->GetFSM()->ChangeState(Cleanse::Instance());
	}
}

void Drunk::Exit(Miner* pMiner)
{
}


bool Drunk::OnMessage(Miner* pMiner, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_RED);
	switch (msg.Msg)
	{
	case Msg_Provocation:
		if (pMiner->Location() == saloon)
		{
			
			cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
				<< " at time: " << Clock->GetCurrentTime();
			SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": You should be ashamed !";
			Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
				pMiner->ID(),        //ID of sender
				ent_Boozer,            //ID of recipient
				Msg_Provocation,   //the message
				NO_ADDITIONAL_INFO);
			pMiner->GetFSM()->ChangeState(Angry::Instance());
			return true;
		}
		break;
		
	case Msg_ZZZZ:
		if (pMiner->Location() == saloon && pMiner->GetFSM()->CurrentState() == Drunk::Instance())
		{
			cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
				<< " at time: " << Clock->GetCurrentTime();
			SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": AHAHAH THIS MAN IS SLEEPIN' ON THE BAR!";
			Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
				pMiner->ID(),        //ID of sender
				ent_Boozer,            //ID of recipient
				Msg_TooMuchNoise,   //the message
				NO_ADDITIONAL_INFO);
			return true;
		}
		break;
	}//end switch

	return false; //send message to global message handler
}


Angry* Angry::Instance()
{
	static Angry instance;

	return &instance;
}

void Angry::Enter(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": YOU, cuz' you're drunk !";
}

void Angry::Execute(Miner* pMiner)
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": Just calm down now !";
	Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
		pMiner->ID(),        //ID of sender
		ent_Boozer,            //ID of recipient
		Msg_CalmDown,   //the message
		NO_ADDITIONAL_INFO);
}

void Angry::Exit(Miner* pMiner)
{
	cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "I think...hic... I've to go...*hic*...to work !";
}


bool Angry::OnMessage(Miner* pMiner, const Telegram& msg)
{
	SetTextColor(BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | FOREGROUND_RED);

	switch (msg.Msg)
	{
	case Msg_CalmDown:

		cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
			<< " at time: " << Clock->GetCurrentTime();
		SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

		pMiner->GetFSM()->ChangeState(Cleanse::Instance());

		return true;

	}//end switch

	return false; //send message to global message handler
}

Cleanse* Cleanse::Instance() {
	static Cleanse instance;
	return &instance;
}

void Cleanse::Enter(Miner* miner) {
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "\n" << GetNameOfEntity(miner->ID()) << ": Before I.. *hic* go to work, I should drink a looot of water";
}

void Cleanse::Execute(Miner* miner) {
	cout << "\n" << GetNameOfEntity(miner->ID()) << " drinks 5 whole bottle of water";
	miner->DrunkCleanse();
	miner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
}

void Cleanse::Exit(Miner* miner) {
	cout << "\n" << GetNameOfEntity(miner->ID()) << ": My thirst an' mah heavy head is gon'! Let's head back to da mine!";

}

bool Cleanse::OnMessage(Miner* agent, const Telegram& msg) {
	return false;
}