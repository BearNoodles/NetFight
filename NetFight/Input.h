#pragma once
#include "SFML/Graphics.hpp"

struct FrameInput
{
	int frameNumber;
	bool inputs[7];
};
class Input
{
public:
	Input();
	~Input();

	FrameInput ReadLocalInput(int frameNo);

	void SetInput(FrameInput frameInput);

	FrameInput GetInput(int frameNo);

	FrameInput GetNoInput(int frameNo);

	void SetCurrentInput(int frame);

	FrameInput GetCurrentInput(int frame);

	void UpdateInputP1(FrameInput p1Input);
	void UpdateInputP2(FrameInput p2Input);

	void UpdateAll();

	void SetCurrentFrame(int frame);

protected:
	std::vector<FrameInput>* player1Inputs;
	std::vector<FrameInput>* player2Inputs;

	int m_currentFrame;

	FrameInput noInput;
};

