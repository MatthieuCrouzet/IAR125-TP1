#include <fstream>
#include <time.h>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <mutex>

#include "Locations.h"
#include "Miner.h"
#include "Boozer.h"
#include "MinersWife.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "misc/ConsoleUtils.h"
#include "EntityNames.h"


std::ofstream os;


using namespace std;

#define NUM_THREADS     3

mutex m;

void task_bob()
{
	Miner* pPerso = new Miner(ent_Miner_Bob);
	EntityMgr->RegisterEntity(pPerso);
	for (int i = 0; i < 20; i++) {
		m.lock();
		pPerso->Update();
		Dispatch->DispatchDelayedMessages();
		m.unlock();
		Sleep(800);
	}
	delete pPerso;
}
void task_elsa()
{
	MinersWife* pPerso = new MinersWife(ent_Elsa);
	EntityMgr->RegisterEntity(pPerso);
	for (int i = 0; i < 20; i++) {
		m.lock();
		pPerso->Update();
		Dispatch->DispatchDelayedMessages();
		m.unlock();
		Sleep(1100);
	}
	delete pPerso;
}
void task_boozer()
{
	Boozer* pPerso = new Boozer(ent_Boozer);
	EntityMgr->RegisterEntity(pPerso);
	for (int i = 0; i < 20; i++) {
		m.lock();
		pPerso->Update();
		Dispatch->DispatchDelayedMessages();
		m.unlock();
		Sleep(500);
	}
	delete pPerso;
}




int main(int argc, char* argv[])
{
//define this to send output to a text file (see locations.h)
#ifdef TEXTOUTPUT
  os.open("output.txt");
#endif

  
  //seed random number generator
  srand((unsigned) time(NULL));

  thread T[NUM_THREADS];
  T[0] = thread(task_bob);
  T[1] = thread(task_elsa);
  T[2] = thread(task_boozer);

  for (int i = 0; i < NUM_THREADS; i++) {
	  if (T[i].joinable())
	  {
		  T[i].join();
	  }
  }
  for (int i = 0; i < NUM_THREADS; i++) {
	  T[i].~thread();
  }
  SetTextColor(FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
  cout << endl;
  system("PAUSE");
  return 0;
}






