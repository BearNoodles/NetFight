#pragma once

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

protected:
	FrameInput player1Inputs[1000];
	FrameInput player2Inputs[1000];
};

