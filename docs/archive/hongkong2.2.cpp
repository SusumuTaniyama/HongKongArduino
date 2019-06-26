#include <conio.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

const unsigned short PORT_DOSV = 0x378;
short STEP_DOSV1 = 1;
short STEP_DOSV2 = 2;
const unsigned short PORT_PC98 = 0x40;
short STEP_PC981 = 2;
short STEP_PC982 = 6;

unsigned short g_mode;
short step1, step2;
unsigned short port1, port2, port3;

unsigned int sram;

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

void OutCh1(unsigned char data)
{
	_outp(port3, 10);
	_outp(port1, data);
	_outp(port3, 11);
	_outp(port3, 10);
}

void OutCh2(unsigned short data)
{
	_outp(port3, 8);
	_outp(port1, data);
	_outp(port3, 9);
	_outp(port3, 8);
}

void OutCh3(unsigned short data)
{
	_outp(port3, 14);
	_outp(port1, data);
	_outp(port3, 15);
	_outp(port3, 14);
}

void OutCh4(unsigned short data)
{
	_outp(port3, 12);
	_outp(port1, data);
	_outp(port3, 13);
	_outp(port3, 12);
}

unsigned short In()
{
	unsigned char data;
	_outp(port3, 33);
	while (_inp(port2) & (unsigned char)128 == 0);
	data = _inp(port1);
	return data;
}

bool AskIsDosV()
{
	char c;
	// Select machine
	cout << "Which is this machine PC98 or DOS/V? [9/D]" << endl;
	cin >> c;
	switch (c) {
	case '9':
		g_mode = PORT_PC98;
		step1 = STEP_PC981;
		step2 = STEP_PC982;
		port1 = g_mode;
		port2 = g_mode + STEP_PC981;
		port3 = g_mode + STEP_PC982;
		return false;
	case 'D':
	case 'd':
		g_mode = PORT_DOSV;
		step1 = STEP_DOSV1;
		step2 = STEP_DOSV2;
		port1 = g_mode;
		port2 = g_mode + STEP_DOSV1;
		port3 = g_mode + STEP_DOSV2;
		return true;
	default:
		g_mode = PORT_DOSV;
		step1 = STEP_DOSV1;
		step2 = STEP_DOSV2;
		port1 = g_mode;
		port2 = g_mode + STEP_DOSV1;
		port3 = g_mode + STEP_DOSV2;
		return true;
	}
}

void LinearAddress(ADDRESS add)
// アドレスの設定
{
	//add.inverse();
	OutCh1(add.ch1);
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

void suidashi(ofstream& os, unsigned int start, unsigned int end)
{
	unsigned short indata;
	unsigned int i;
	for (i = start; i <= end; i++) {
		Address(i);
		indata = In();
		//cout << setbase(16) << i << ":" << indata << " ";
		os.put(char(indata));
	}
}

unsigned short convport(int port)
{
	switch (port) {
	case 1:
		return port1;
	case 2:
		return port2;
	case 3:
		return port3;
	}
	return port1;
}

void DumpRomInfo()
{
	unsigned int add;
	string title;
	int romsize, sramsize;

	if (romtype == LINEAR) { // HiRom
		add = 0xFFC0;
		sram = 0x1C0000;
	} else {// LoRom
		add = 0x7FC0;
		sram = 0x380000;
	}

	OutCh4(0); // 読み込み可能
	for (int i = 0; i < 22; i++) {
		Address(add+i);
		title += char(In());
	}
	// ROMサイズを判別
	Address(add+i+1);
	switch (In()) {
	case 0x08:
		romsize = 2;
		break;
	case 0x09:
		romsize = 4;
		break;
	case 0x0A:
		romsize = 8;
		break;
	case 0x0B:
		romsize = 16;
		break;
	case 0x0C:
		romsize = 32;
		break;
	default:
		romsize = -1;
		break;
	}

	// SRAMサイズを判別
	Address(add+i);
	switch (In()) {
	case 0x00:
		sramsize = 0;
		break;
	case 0x01:
	case 0x02:
		sramsize = 16;
		break;
	default:
		sramsize = -1;
		break;
	}

	// ROM情報を表示
	cout << title << "   ROM SIZE:" << setbase(10) << romsize << "MBit";
	cout << "   SRAM SIZE" << setbase(10) << sramsize  << "KBit" << endl;
	cout << "ADRESS 0~" << setbase(16) << romsize*0x100000/8-1;
	cout << "   SRAM 0~" << setbase(16) << sramsize*1024/8-1 << endl;
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
		cout << setbase(16) << unsigned int(_inp(convport(port))) << endl;
		break;
	case 'O':
	case 'o':
		cin >> port >> setbase(16) >> databyte;
		_outp(convport(port), databyte);
		break;
	case 'Q':
	case 'q':
		return false;
	default:
		break;
	}
	return true;
}

void LoadROM() {
	string filename;
	ofstream os;
	unsigned int start, end;

	DumpRomInfo();
	cout << "(start address) (end address) <-hex" << endl;
	cin >> setbase(16) >> start >> end;
	cout << "Save file name?" << endl;
	cin >> filename;
	os.open(filename.c_str(), ios::out | ios::binary); // ファイルオープン
	suidashi(os, start, end);
	os.close();
}

void LoadSRAM() {
	string filename;
	ofstream os;
	unsigned int start, end;

	DumpRomInfo();
	cout << "(start address) (end address) <-hex" << endl;
	cin >> setbase(16) >> start >> end;
	cout << "Save file name?" << endl;
	cin >> filename;
	os.open(filename.c_str(), ios::out | ios::binary); // ファイルオープン
	OutCh4(0); // SRAM OE
	OutCh4(2); // SRAM CE (74139 RESET(26ピン）:0->1)

	suidashi(os, start+sram, end+sram);
	
	OutCh4(0); // SRAM OE
	os.close();
}

void WriteSRAM() {
	string filename;
	ifstream is;
	unsigned int add;
	unsigned int c;

	cout << "write file name?" << endl;
	cin >> filename;
	is.open(filename.c_str(), ios::in | ios::binary);
	for (add = sram; ; add++) {
		OutCh4(3); // !OE !CE
		Address(add);
		c = is.get();
		if (c == EOF) break;
		_outp(port1, c); // データセット
		_outp(port3, 3); // dir = out, !WE
		_outp(port3, 2); // dir = out, !WE:1->0
		while (_inp(port2) & (unsigned char)128 == 0);
		_outp(port3, 3); // dir = out, !WE
		OutCh4(1); // CE (74139 RESET(26ピン）:0->1)
		_outp(port3, 5);
	}
}

bool ModeSelect()
{
	char c;
	// mode select
	cout << "Debug mode or Run mode or Load SRAM or Write SRAM or Quit?" << endl;
	cout << "Input [D/R/L/W/Q]" << endl;
	cin >> c;
	switch (c) {
	case 'd':
	case 'D':
		while (debug());
		break;
	case 'r':
	case 'R':
		LoadROM();
		break;
	case 'l':
	case 'L':
		LoadSRAM();
		break;
	case 'w':
	case 'W':
		WriteSRAM();
		break;
	default:
		return false;
	}
	return true;
}

bool Init(bool IsDosV)
{
	if (IsDosV) {
		g_mode = PORT_DOSV;
		port1 = g_mode;
		port2 = g_mode + STEP_DOSV1;
		port3 = g_mode + STEP_DOSV2;
    } else {
		g_mode = PORT_PC98;
		port1 = g_mode;
		port2 = g_mode + STEP_PC981;
		port3 = g_mode + STEP_PC982;
    }
	OutCh4(0); // OE

	return true;
}

int main()
{
	Init(AskIsDosV());
	romtype = SelectRomType();
	DumpRomInfo();
	while(ModeSelect());
	return 0;
}