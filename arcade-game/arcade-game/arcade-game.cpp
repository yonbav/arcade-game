// arcade-game.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <math.h>
#include <iostream>
#include <string>

#include "olcConsoleGameEngine.h"

using namespace std;

// Constants
const float MAX_SPEED = 1.0f;
const float MIN_SPEED = 0.0f;
const float TURNS_SPEED = 0.4f;
const float AUTO_BREAK_SPEED = 1.2f;
const float BREAKES_SPEED = 2.5f;
const float GRASS_WIDTH = 0.2f;
const float CLIP_PRESENTEGE = 0.1f;
const float CAR_HEIGHT = 0.9f;

const int CAR_WIDTH = 7;
const int CURVES_SHARPNESS = 4;
const int OFFTRACK_SLOWDOWN = 5;
class FormulaOne : public olcConsoleGameEngine
{
public:
	FormulaOne()
	{
		m_sAppName = L"Formula One";
	}
protected:
	// Called By Game engine
	virtual bool OnUserCreate()
	{
		m_vTrackVector.push_back(make_pair(0.0f, 150.0f));
		m_vTrackVector.push_back(make_pair(0.3f, 200.0f));
		m_vTrackVector.push_back(make_pair(-0.5f, 100.0f));
		m_vTrackVector.push_back(make_pair(0.0f, 100.0f));
		m_vTrackVector.push_back(make_pair(0.5f, 50.0f));
		m_vTrackVector.push_back(make_pair(0.3f, 180.0f));
		m_vTrackVector.push_back(make_pair(-0.25f, 200.0f));
		m_vTrackVector.push_back(make_pair(0.0f, 400.0f));
		m_vTrackVector.push_back(make_pair(-0.6f, 210.0f));
		m_vTrackVector.push_back(make_pair(0.3f, 230.0f));
		return true;
	}

	// Called By Game engine
	virtual bool OnUserUpdate(float fElapsedTime)
	{
		float fPrespective;
		float fMidPoint;
		float fRoadWidth;
		float fClipWidth;

		float fColorCalc;
		int iGrassColor;
		int iClipColor;

		int iLeftGrass;
		int iLeftClip;
		int iRightClip;
		int iRightGrass;
		int iCarPos;
		int iCarHeight;

		// whipes all the previous picture
		Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ', FG_BLACK);

		// Counting the time
		m_fPassedTime += fElapsedTime;

		// Turns left
		if (m_keys[VK_LEFT].bHeld)
		{
			m_fCarCurve -= TURNS_SPEED * fElapsedTime;

			// Minium car curve
			if ((m_fCarCurve - m_fTrackCurve) < -1.0f + (CAR_WIDTH / (ScreenWidth() / 2.0f)))
			{
				m_fCarCurve = -1.0 + m_fTrackCurve + (CAR_WIDTH / (ScreenWidth() / 2.0f));
			}
		}

		// Turns right
		if (m_keys[VK_RIGHT].bHeld)
		{
			m_fCarCurve += TURNS_SPEED * fElapsedTime;


			// Maxium car curve
			if ((m_fCarCurve - m_fTrackCurve) > (1.0f) - (CAR_WIDTH / (ScreenWidth() / 2.0f)))
			{
				m_fCarCurve = 1.0 + m_fTrackCurve - (CAR_WIDTH / (ScreenWidth() / 2.0f));
			}
		}

		iCarPos = (ScreenWidth() / 2) + ((m_fCarCurve - m_fTrackCurve) * (ScreenWidth() / 2.0f)) - CAR_WIDTH;
		// Accelerating 
		if (m_keys[VK_UP].bHeld && m_fSpeed < MAX_SPEED)
		{
			m_fSpeed += fElapsedTime;			
		}
		// Decelerating
		else
		{
			// Checking that the speed is greater thehn min speed
			if (m_keys[VK_DOWN].bHeld && m_fSpeed > MIN_SPEED)
			{
				m_fSpeed -= BREAKES_SPEED * fElapsedTime;

				if (m_fSpeed < MIN_SPEED)
				{
					m_fSpeed = MIN_SPEED;
				}
			}
			else
			{
				m_fSpeed -= AUTO_BREAK_SPEED * fElapsedTime;

				if (m_fSpeed < MIN_SPEED)
				{
					m_fSpeed = MIN_SPEED;
				}
			}
		}

		// Changes the distance on the track for view
		m_fDistance += m_fSpeed;

		// changes the Location on the trail
		m_fOffset += m_fSpeed;

		// Checking if the curve Type section have ended if does changes the section
		if (m_fOffset >= m_vTrackVector[m_iTrackSection].second)
		{
			m_fOffset = 0.0f;
			m_iTrackSection++;
		}

		// If the curve is bigger then current turn turns it more
		if (m_fTrackCurve < m_vTrackVector[m_iTrackSection].first)
		{
			m_fTrackCurve += m_fSpeed / 100;

		}

		// If the curve is smaller then current turn turns it less
		if (m_fTrackCurve > m_vTrackVector[m_iTrackSection].first)
		{
			m_fTrackCurve -= m_fSpeed / 100;
		}

		// Painting the board
		for (int y = ScreenHeight() / 2; y <= ScreenHeight(); y++)
		{
			fPrespective = (float)y / ScreenHeight();
			fRoadWidth = (fPrespective / 2.0f) - GRASS_WIDTH;
			fClipWidth = CLIP_PRESENTEGE * fRoadWidth;
			fColorCalc = sinf((float)ScreenHeight() * ((float)pow((1.0 - fPrespective), 2.0) + 0.01 * m_fDistance));
			fMidPoint = 0.5f + m_fTrackCurve * CURVES_SHARPNESS * pow(1.0f - fPrespective, 3.0f);

			iGrassColor = fColorCalc > 0 ? FG_GREEN : FG_DARK_GREEN;
			iClipColor = fColorCalc > 0 ? FG_RED: FG_WHITE;

			iLeftGrass = (fMidPoint - fRoadWidth - fClipWidth) * ScreenWidth();
			iLeftClip = (fMidPoint - fRoadWidth) * ScreenWidth();
			iRightClip = (fMidPoint + fRoadWidth) * ScreenWidth();
			iRightGrass = (fMidPoint + fRoadWidth + fClipWidth) * ScreenWidth();

			for (int x = 0; x < ScreenWidth(); x++)
			{

				// Painting the grass
				if ((x >= 0 && x < iLeftGrass) || (x > iRightGrass && x < ScreenWidth()))
				{
					Draw(x, y, PIXEL_SOLID, iGrassColor);
				}
				
				// Painting the Clips
				if ((x >= iLeftGrass && x <= iLeftClip) || (x >= iRightClip && x <= iRightGrass))
				{
					Draw(x, y, PIXEL_SOLID, iClipColor);
				}

				// Panting the Road
				if (x > iLeftClip && x < iRightClip)
				{
					Draw(x, y, PIXEL_SOLID, FG_GREY);
				}
			}
		}

		iCarHeight = CAR_HEIGHT * ScreenHeight();

		// Painting the car
		DrawStringAlpha(iCarPos, iCarHeight ,    L"    ||=||   ");
		DrawStringAlpha(iCarPos, iCarHeight + 1, L"      #      ");
		DrawStringAlpha(iCarPos, iCarHeight + 2, L"     ###     ");
		DrawStringAlpha(iCarPos, iCarHeight + 3, L"|||  ###  |||");
		DrawStringAlpha(iCarPos, iCarHeight + 4, L"|||=#####=|||");
		DrawStringAlpha(iCarPos, iCarHeight + 5, L"|||  ###  |||");

		// Printing data 
		DrawString(0, 0, L"Distance: " + to_wstring(m_fDistance));
		DrawStringAlpha(0, 1, L"TrackCurve: " + to_wstring(m_fTrackCurve));
		DrawStringAlpha(0, 2, L"speed: " + to_wstring(m_fSpeed * 230));
		DrawStringAlpha(0, 3, L"CarCurve: " + to_wstring(m_fCarCurve));
		DrawStringAlpha(0, 4, L"Time: " + to_wstring(m_fPassedTime));

		return true;
	}

private:
	float m_fTrackCurve = 0.0f;
	float m_fCarCurve = 0.0f;
	float m_fDistance = 0.0f;
	float m_fSpeed = 0.0f;
	float m_fPassedTime = 0.0f;
	vector<pair<float, float>> m_vTrackVector;
	float m_fOffset = 0.0f;
	int m_iTrackSection = 0;
};

int main()
{
	 // using the engine to start the game
	FormulaOne game;
	game.ConstructConsole(160, 100, 6, 6);
	game.Start();
    return 0;
}



