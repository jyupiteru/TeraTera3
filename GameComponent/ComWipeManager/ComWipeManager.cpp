/**
 * @file ComWipeManager.cpp
 * @author jupiter ()
 * @brief ComWipeManagerのcppが記載されたcpp
 */

#include "ComWipeManager.h"

ComWipeManager *ComWipeManager::m_instance = nullptr;

void ComWipeManager::Init()
{
	m_gameObject->m_drawLayer.SetValue(10);
	m_instance = this;
}

//================================================================================================
//================================================================================================

void ComWipeManager::Uninit()
{
	m_instance = nullptr;
}

//================================================================================================
//================================================================================================

void ComWipeManager::Ready()
{
	m_comWipe = m_gameObject->GetComponent<ComWipe>();
	m_comWipe->m_enable.SetValue(false);
}

//================================================================================================
//================================================================================================

void ComWipeManager::Update()
{
	switch (m_typeWipe)
	{

	case E_TYPE_WIPE::RESULT_WIPE_CLOSE:
	case E_TYPE_WIPE::GAME_WIPE_CLOSE:

		UpdateWipeClose();

		break;

	case E_TYPE_WIPE::GAME_WIPE_OPEN:
	case E_TYPE_WIPE::RESULT_WIPE_OPEN:

		UpdateWipeOpen();

		break;
	default:
		break;
	}
}

//================================================================================================
//================================================================================================

ComWipeManager &ComWipeManager::GetInstance()
{
	return *m_instance;
}

//================================================================================================
//================================================================================================

void ComWipeManager::StartWipe(E_TYPE_WIPE _type)
{
	float maxsize = SCREEN_WIDTH;
	m_typeWipe = _type;

	m_comWipe->m_enable.SetValue(true);
	switch (m_typeWipe)
	{
	case E_TYPE_WIPE::GAME_WIPE_CLOSE:
	case E_TYPE_WIPE::GAME_WIPE_OPEN:

		if (m_typeWipe == E_TYPE_WIPE::GAME_WIPE_CLOSE)
		{
			m_comWipe->m_wipeSize.SetValue(0.0f);
			m_comWipe->m_wipeFlag.SetValue(false);
			m_timeCounter = 0.0f;
			m_sizeCount = 0.0f;

			m_comWipe->m_wipeVector.SetValue(1.0f, 0.0f);
		}
		else
		{
			m_comWipe->m_wipeSize.SetValue(0.0f);
			m_comWipe->m_wipeFlag.SetValue(true);
			m_comWipe->m_wipeVector.SetValue(1.0f, 1.0f);
		}

		m_comWipe->LoadTexture("Operation.png");
		break;

	case E_TYPE_WIPE::RESULT_WIPE_CLOSE:
	case E_TYPE_WIPE::RESULT_WIPE_OPEN:

		if (m_typeWipe == E_TYPE_WIPE::RESULT_WIPE_CLOSE)
		{
			m_comWipe->m_wipeSize.SetValue(0.0f);
			m_comWipe->m_wipeFlag.SetValue(false);
			m_timeCounter = 0.0f;
			m_sizeCount = 0.0f;

			m_comWipe->m_wipeVector.SetValue(1.0f, 0.0f);
		}
		else
		{
			m_comWipe->m_wipeSize.SetValue(0.0f);
			m_comWipe->m_wipeFlag.SetValue(true);
			m_comWipe->m_wipeVector.SetValue(1.0f, 1.0f);
		}

		m_comWipe->LoadTexture("Point.png");
		break;
	default:
		break;
	}
}

//================================================================================================
//================================================================================================

bool ComWipeManager::IsEnd()
{
	if (m_typeWipe == E_TYPE_WIPE::END)
	{
		return true;
	}
	return false;
}

//================================================================================================
//================================================================================================

E_TYPE_WIPE ComWipeManager::GetNowWipeType()
{
	return m_typeWipe;
}

//================================================================================================
//================================================================================================

void ComWipeManager::UpdateWipeClose()
{
	float addsize = static_cast<float>(m_wipeSpeed * CTimer::GetInstance().m_deltaTime.GetValue());

	if (m_sizeCount < SCREEN_WIDTH)
	{
		m_sizeCount += addsize;
		m_comWipe->m_wipeSpeed.SetValue(m_wipeSpeed);
	}
	else
	{
		m_comWipe->m_wipeSpeed.SetValue(0.0f);

		m_timeCounter = 0.0f;
		if (m_typeWipe == E_TYPE_WIPE::GAME_WIPE_CLOSE)
		{
			StartWipe(E_TYPE_WIPE::GAME_WIPE_OPEN);
		}
		else
		{
			StartWipe(E_TYPE_WIPE::RESULT_WIPE_OPEN);
		}
	}
}

//================================================================================================
//================================================================================================

void ComWipeManager::UpdateWipeOpen()
{

	float maxtime = 2.0f;

	m_timeCounter += static_cast<float>(CTimer::GetInstance().m_deltaTime.GetValue());

	if (m_timeCounter > maxtime)
	{
		float addsize = static_cast<float>(m_wipeSpeed * CTimer::GetInstance().m_deltaTime.GetValue());


		if (m_sizeCount > 0)
		{
			m_comWipe->m_wipeSpeed.SetValue(m_wipeSpeed);
			m_sizeCount += addsize;
		}
		else
		{
			m_comWipe->m_wipeSpeed.SetValue(0.0f);
			m_typeWipe = E_TYPE_WIPE::END;
		}
	}
}