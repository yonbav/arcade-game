// arcade-game.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "olcConsoleGameEngine.h"

using namespace std;

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
		return true;
	}

	// Called By Game engine
	virtual bool OnUserUpdate(float fElapsedTime)
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ', FG_BLACK);

		float fMidPoint = 0.5f;

		// Painting the board
		for (int y = ScreenHeight() / 2; y <= ScreenHeight(); y++)
		{
			float fPrespective = (float)y / ScreenHeight();
			float fRoadWidth = (fPrespective / 2.0f) - 0.2;
			float fClipWidth = 0.30f * fRoadWidth;


			int iLeftGrass = (fMidPoint - fRoadWidth - fClipWidth) * ScreenWidth();
			int iLeftClip = (fMidPoint - fRoadWidth) * ScreenWidth();
			int iRightClip = (fMidPoint + fRoadWidth) * ScreenWidth();
			int iRightGrass = (fMidPoint + fRoadWidth + fClipWidth) * ScreenWidth();

			for (int x = 0; x < ScreenWidth(); x++)
			{

				// Painting the grass
				if ((x >= 0 && x < iLeftGrass) || (x > iRightGrass && x < ScreenWidth()))
				{
					Draw(x, y, PIXEL_SOLID, FG_GREEN);
				}
				
				// Painting the Clips
				if ((x >= iLeftGrass && x <= iLeftClip) || (x >= iRightClip && x <= iRightGrass))
				{
					Draw(x, y, PIXEL_SOLID, FG_RED);
				}

				// Panting the Road
				if (x > iLeftClip && x < iRightClip)
				{
					Draw(x, y, PIXEL_SOLID, FG_GREY);
				}
			}
		}

		// Painting the car
		int nCarPos = ScreenWidth() / 2 + ((int)(m_fCarPosition * ScreenWidth()) / 2.0f)  - 7;
		int nCarHeight = ScreenHeight() * 0.9f;
		DrawStringAlpha(nCarPos, nCarHeight    , L"   ||===||   ");
		DrawStringAlpha(nCarPos, nCarHeight + 1, L"      #      ");
		DrawStringAlpha(nCarPos, nCarHeight + 2, L"     ###     ");
		DrawStringAlpha(nCarPos, nCarHeight + 3, L"|||  ###  |||");
		DrawStringAlpha(nCarPos, nCarHeight + 4, L"|||=#####=|||");
		DrawStringAlpha(nCarPos, nCarHeight + 5, L"|||  ###  |||");

		return true;
	}

private:
	float m_fCarPosition = 0.0f;
};

int main()
{
	 // using the engine to start the game
	FormulaOne game;
	game.ConstructConsole(160, 100, 6, 6);
	game.Start();
    return 0;
}



