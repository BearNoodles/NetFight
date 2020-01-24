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

	FrameInput GetOpponentInput(int frameNo);
	FrameInput GetLocalInput(int frameNo);

	void SetOpponentInput(FrameInput input);
	void SetLocalInput(FrameInput input);

	std::vector<FrameInput>* GetAllInputs(int player);

	FrameInput GetNoInput(int frameNo);


	FrameInput GetCurrentInput(int frame);

	void UpdateAll();

	void SetCurrentFrame(int frame);

	bool BothInputsReady(int frame);

protected:
	std::vector<FrameInput>* localInputs;
	std::vector<FrameInput>* opponentInputs;

	int m_currentFrame;

	FrameInput noInput;
};

