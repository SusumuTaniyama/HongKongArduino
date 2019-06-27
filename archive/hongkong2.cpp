#include <conio.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

const char portfile[] = "port.txt";
unsigned short PORT1_DOSV = 0x378;
unsigned short PORT2_DOSV = 0x379;
unsigned short PORT3_DOSV = 0x37a;
unsigned short PORT1_PC98 = 0x40;
unsigned short PORT2_PC98 = 0x42;
unsigned short PORT3_PC98 = 0x44;

unsigned short port1, port2, port3;

unsigned int sram;

enum ROMTYPE {
	SKIP=0, LINEAR=1, MADLO=2, MADHI=3
};
ROMTYPE romtype = LINEAR;

void GetPathFromName(char* apppath)
{
	int mark = 0;
	for(int i = 0; apppath[i] != '\0'; i++) {
		if (apppath[i] == '\\') mark = i;
	}
	if (mark != 0) apppath[mark+1] = '\0';
}

int HexToDec(const string& hex)
{
	int res = 0;
	for (int i = 0; i < hex.size(); i++) {
		res *= 16;
		if (hex[i] >= 'A' && hex[i] <= 'F') {
			res += hex[i] - 'A' + 10;
		} else if (hex[i] >= 'a' && hex[i] <= 'f') {
			res += hex[i] - 'a' + 10;
		} else if (hex[i] >= '0' && hex[i] <= '9') {
			res += hex[i] - '0';
		} else return -1;
	}
	return res;
}

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
	// クロックはLの状態
	_outp(port3, 0);
	_outp(port3, 32);
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
		return false;
	case 'D':
	case 'd':
		return true;
	default:
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

bool IsMAD()
{
	unsigned char data;
	OutCh4(0);
	Address(0x0);
	data = In();
	OutCh4(2);
	if (data != In()) return true;
	return false;
}

ROMTYPE SelectRomType()
{
	unsigned char data;
	char c;
	int type = 0; // rom type

	if (IsMAD()) {
		cout << "This ROM may have MAD Chip? [y/n]" << endl;
		cin >> c;
		if (c == 'y' || c== 'Y') type = MADLO;
	} else {
		cout << "This ROM may not have MAD Chip? [y/n]" << endl;
		cin >> c;
		if (c == 'n' || c== 'N') type = MADLO;
	}

	if (romtype & MADLO) OutCh4(2); // 読み込み可能
	else OutCh4(0);

	Address(0x8000);
	data = In();
	Address(0);
	if (data == In()) {
		cout << "Rom type is 32K skip rom? [y/n]" << endl;
		cin >> c;
		if (c == 'y' || c == 'Y') return ROMTYPE(type + SKIP);
		return ROMTYPE(type + LINEAR);
	} else {
		cout << "Rom type is linear rom? [y/n]" << endl;
		cin >> c;
		if (c == 'y' || c == 'Y') return ROMTYPE(type + LINEAR);
		return ROMTYPE(type + SKIP);
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

	if (romtype & LINEAR) { // HiRom
		add = 0xFFC0;
		if (romtype & MADLO) sram = 0x206000;
		else sram = 0x1C0000;
	} else {// LoRom
		add = 0x7FC0;
		if (romtype & MADLO) sram = 0x604000;
		else sram = 0x380000;
	}

	if (romtype & MADLO) OutCh4(2); // 読み込み可能
	else OutCh4(0);
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
	//OutCh4(0); // SRAM OE
	if (romtype == MADHI) OutCh4(6);
	else OutCh4(2); // SRAM CE (74139 RESET(26ピン）:0->1)

	suidashi(os, start+sram, end+sram);
	
	if (romtype & MADLO) OutCh4(2); // 読み込み可能
	else OutCh4(0);
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
		if (romtype == MADHI) OutCh4(7); // !OE !CE !CS
		else OutCh4(3); // !OE !CE
		Address(add);
		c = is.get();
		if (c == EOF) break;
		_outp(port1, c); // データセット
		_outp(port3, 2); // dir = out, !WE
		_outp(port3, 3); // dir = out, !WE:1->0
		while (_inp(port2) & (unsigned char)128 == 0);
		_outp(port3, 2); // dir = out, !WE
		if (romtype == MADHI) OutCh4(5); // CE (74139 RESET(26ピン）:0->1) !CS
		else OutCh4(1); // CE (74139 RESET(26ピン）:0->1)
		_outp(port3, 4);
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
		port1 = PORT1_DOSV;
		port2 = PORT2_DOSV;
		port3 = PORT3_DOSV;
    } else {
		port1 = PORT1_PC98;
		port2 = PORT2_PC98;
		port3 = PORT3_PC98;
    }
	if (romtype & MADLO) OutCh4(2); // 読み込み可能
	else OutCh4(0);

	return true;
}

int main(int argv, char* argc[])
{
	// ポートアドレスをファイルから取得
	char *apppath = argc[0];
	GetPathFromName(apppath);
	ifstream is((string(apppath) + portfile).c_str());
	string portname, port;
	while(is) {
		is >> portname >> port;
		if (portname == "PORT1_DOSV") {
			PORT1_DOSV = (unsigned short)HexToDec(port);
		} else if (portname == "PORT2_DOSV") {
			PORT2_DOSV = (unsigned short)HexToDec(port);
		} else if (portname == "PORT3_DOSV") {
			PORT3_DOSV = (unsigned short)HexToDec(port);
		} else if (portname == "PORT1_PC98") {
			PORT1_PC98 = (unsigned short)HexToDec(port);
		} else if (portname == "PORT2_PC98") {
			PORT2_PC98 = (unsigned short)HexToDec(port);
		} else if (portname == "PORT3_PC98") {
			PORT3_PC98 = (unsigned short)HexToDec(port);
		}
	}

	Init(AskIsDosV());
	romtype = SelectRomType();
	DumpRomInfo();
	while(ModeSelect());
	return 0;
}