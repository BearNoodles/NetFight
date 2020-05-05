#ifndef MESSAGE_H
#define MESSAGE_H

//Structure of messages sent between clients
struct Message {

	bool pingSend;
	bool pingReply;

	bool input1;
	bool input2;
	bool input3;
	bool input4;
	bool input5;
	bool input6;
	bool input7;
	bool set;
	int frame;
};

#endif