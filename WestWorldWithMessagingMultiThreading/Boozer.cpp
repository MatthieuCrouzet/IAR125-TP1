#include "Boozer.h"


bool Boozer::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}


void Boozer::Update()
{
	SetTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY);

	m_beveragesDrunk--;

	m_pStateMachine->Update();
}


