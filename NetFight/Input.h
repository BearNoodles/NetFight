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

	void SetInput(FrameInput frameInput);

	FrameInput GetInput(int frameNo);

	void SetCurrentInput(int frame);

	void UpdateInputs(int frameNo);

	void UpdateAll();

	void SetCurrentFrame(int frame);

protected:
	std::vector<FrameInput>* player1Inputs;
	std::vector<FrameInput>* player2Inputs;

	int m_currentFrame;

	FrameInput noInput;
};

