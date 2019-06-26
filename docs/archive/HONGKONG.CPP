#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

// ウエイトをかけるときはdefineでWAIT_ONを定義する
#ifdef WAIT_ON
	#include <windows.h>
	const DWORD WAIT = 1; // ウエイトの設定(MillSec)
#endif // WAIT_ON

const unsigned short PORT_DOSV = 0x378;
short STEP_DOSV1 = 1;
short STEP_DOSV2 = 2;
const unsigned short PORT_PC98 = 0x40;
short STEP_PC981 = 2;
short STEP_PC982 = 6;

unsigned short mode;
short step1, step2;
unsigned short port1, port2, port3;

string filename;
ofstream os;

enum ROMTYPE {
	SKIP, LINEAR
};
ROMTYPE romtype = LINEAR;

class ADDRESS {
public:
	unsigned char ch1, ch2, ch3, dummy;
	ADDRESS(unsigned int& add) {
		memcpy(this, &add, sizeof(add));
	}
	void inverse() {
		ch1 = ~ch1;
		ch2 = ~ch2;
		ch3 = ~ch3;
	}
};

extern void outp(unsigned short port, unsigned char databyte);
extern unsigned char inp(unsigned short port);

void OutCh1(unsigned char data)
{
	outp(port3, 3);
	outp(port1, data);
	outp(port3, 2);
}

void OutCh2(unsigned short data)
{
	outp(port3, 1);
	outp(port1, data);
	outp(port3, 0);
}

void OutCh3(unsigned short data)
{
	outp(port3, 7);
	outp(port1, data);
	outp(port3, 6);
}

void OutCh4(unsigned short data)
{
	outp(port3, 5);
	outp(port1, data);
	outp(port3, 4);
}

void Init()
{
	OutCh4(57);
}

void SelectMachineType()
{
	char c;
	// Select machine
	cout << "Which is this machine PC98 or DOS/V? [9/D]" << endl;
	cin >> c;
	switch (c) {
	case '9':
		mode = PORT_PC98;
		step1 = STEP_PC981;
		step2 = STEP_PC982;
		port1 = mode;
		port2 = mode + STEP_PC981;
		port3 = mode + STEP_PC982;
		break;
	case 'D':
	case 'd':
		mode = PORT_DOSV;
		step1 = STEP_DOSV1;
		step2 = STEP_DOSV2;
		port1 = mode;
		port2 = mode + STEP_DOSV1;
		port3 = mode + STEP_DOSV2;
		break;
	default:
		mode = PORT_DOSV;
		step1 = STEP_DOSV1;
		step2 = STEP_DOSV2;
		port1 = mode;
		port2 = mode + STEP_DOSV1;
		port3 = mode + STEP_DOSV2;
		break;
	}
}

void LinearAddress(ADDRESS add)
// アドレスの設定
{
	add.inverse();
	OutCh4(57);
	OutCh1(add.ch1);
	OutCh4(56);
	OutCh2(add.ch2);
	OutCh3(add.ch3);
}

void Address(unsigned int add)
{
	if (romtype == SKIP) {
		unsigned int upper, lower;
		upper = add / 0x8000;
		lower = add % 0x8000;
		add = upper * 2 * 0x8000 + lower;
	}
	LinearAddress(add);
}

unsigned short In()
{
	unsigned char data;
	//OutCh1(0);
	outp(port1, 0);
	outp(port3, 3);
	outp(port3, 2);
	OutCh4(0x3a);
	outp(port3, 0x20);
	outp(port3, 0x2b); // ch1の入力許可
	data = inp(port1);
	return data;
}

ROMTYPE SelectRomType()
{
	unsigned char data;
	char c;
	Address(0x8000);
	data = In();
	Address(0);
	if (data == In()) {
		cout << "Rom type is 32K skip rom? [y/n]" << endl;
		cin >> c;
		if (c == 'y' | c == 'Y') return SKIP;
		return LINEAR;
	} else {
		cout << "Rom type is linear rom? [y/n]" << endl;
		cin >> c;
		if (c == 'y' | c == 'Y') return LINEAR;
		return SKIP;
	}
}
	
void suidashi(unsigned int start, unsigned int end)
{
	unsigned short indata;
	DWORD counter = GetTickCount();
	unsigned int i;
	for (i = start; i <= end; i++) {
		Address(i);
#ifdef WAIT_ON
		while (GetTickCount() - counter < WAIT);
		counter = GetTickCount();
#endif // WAIT_ON
		indata = In();
		//cout << setbase(16) << i << ":" << indata << " ";
		os.put(indata);
	}
}

unsigned short convport(int port)
{
	switch (port) {
	case 1:
		return mode;
	case 2:
		return mode + step1;
	case 3:
		return mode + step2;
	}
	return mode;
}

bool debug()
{
	char func;
	int port;
	unsigned int databyte;
	unsigned int add;

	cout << "[I | O | Q] port(1|2|3) [data]" << endl;
	cout << "Address:A address, Input:B, ch1:1,...,ch4:4 data" << endl;
	cout << ">";
	cin >> func;

	switch (func) {
	case 'A':
	case 'a':
		cin >> setbase(16) >> add;
		Address(add);
		break;
	case 'B':
	case 'b':
		cout << setbase(16) << unsigned int(In()) << endl;
		break;
	case '1':
		cin >> databyte;
		OutCh1(databyte);
		break;
	case '2':
		cin >> databyte;
		OutCh2(databyte);
		break;
	case '3':
		cin >> databyte;
		OutCh3(databyte);
		break;
	case '4':
		cin >> databyte;
		OutCh4(databyte);
		break;
	case 'I':
	case 'i':
		cin >> port;
		cout << setbase(16) << unsigned int(inp(convport(port))) << endl;
		break;
	case 'O':
	case 'o':
		cin >> port >> setbase(16) >> databyte;
		outp(convport(port), databyte);
		break;
	case 'Q':
	case 'q':
		return false;
	default:
		break;
	}
	return true;
}

bool ModeSelect()
{
	char c;
	// mode select
	cout << "Debug mode or Run mode or Quit? [D/R/Q]" << endl;
	cin >> c;
	switch (c) {
	case 'd':
	case 'D':
		while (debug());
		break;
	case 'r':
	case 'R':
		unsigned int start, end;
		cout << "(start address) (end address) <-hex" << endl;
		cin >> setbase(16) >> start >> end;
		cout << "Save file name?" << endl;
		cin >> filename;
		os.open(filename.c_str(), ios::out | ios::binary); // ファイルオープン
		suidashi(start, end);
		os.close();
		break;
	default:
		return false;
	}
	return true;
}

int main()
{
	Init();
	SelectMachineType();
	romtype = SelectRomType();
	while (ModeSelect());

	return true;
}
