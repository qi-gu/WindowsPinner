#include "all.h"
#include <iostream>
#include <map>

const std::string qg = R"( ________  ___  ________  ___  ___     
|\   __  \|\  \|\   ____\|\  \|\  \    
\ \  \|\  \ \  \ \  \___|\ \  \\\  \   
 \ \  \\\  \ \  \ \  \  __\ \  \\\  \  
  \ \  \\\  \ \  \ \  \|\  \ \  \\\  \ 
   \ \_____  \ \__\ \_______\ \_______\
    \|___| \__\|__|\|_______|\|_______|
          \|__|             ����by ���)";

const std::map<int, std::string> key_map = {
	{0x08, "Backspace"},
	{0x09, "Tab"},
	{0x0D, "Enter"},
	{0x10, "Shift"},
	{0x11, "Ctrl"},
	{0x12, "Alt"},
	{0x1B, "Escape"},
	{0x20, "Space"},
	{0x21, "PageUp"},
	{0x22, "PageDown"},
	{0x23, "End"},
	{0x24, "Home"},
	{0x25, "LeftArrow"},
	{0x26, "UpArrow"},
	{0x27, "RightArrow"},
	{0x28, "DownArrow"},
	{0x2C, "PrintScreen"},
	{0x2D, "Insert"},
	{0x2E, "Delete"},
	{0x30, "0"},
    {0x31, "1"},
    {0x32, "2"},
    {0x33, "3"},
    {0x34, "4"},
    {0x35, "5"},
    {0x36, "6"},
    {0x37, "7"},
    {0x38, "8"},
    {0x39, "9"},
    {0x41, "A"},
    {0x42, "B"},
    {0x43, "C"},
    {0x44, "D"},
    {0x45, "E"},
    {0x46, "F"},
    {0x47, "G"},
    {0x48, "H"},
    {0x49, "I"},
    {0x4A, "J"},
    {0x4B, "K"},
    {0x4C, "L"},
    {0x4D, "M"},
    {0x4E, "N"},
    {0x4F, "O"},
    {0x50, "P"},
    {0x51, "Q"},
    {0x52, "R"},
    {0x53, "S"},
    {0x54, "T"},
    {0x55, "U"},
    {0x56, "V"},
    {0x57, "W"},
    {0x58, "X"},
    {0x59, "Y"},
    {0x5A, "Z"},
    {0x5B, "Win"},
	{0x70, "F1"},
	{0x71, "F2"},
	{0x72, "F3"},
	{0x73, "F4"},
	{0x74, "F5"},
	{0x75, "F6"},
	{0x76, "F7"},
	{0x77, "F8"},
	{0x78, "F9"},
	{0x79, "F10"},
	{0x7A, "F11"},
	{0x7B, "F12"},
    {0xBA, ";"},
    {0XBB, "="},
    {0xBC, ","},
    {0xBD, "-"},
    {0xBE, "."},
    {0xBF, "/"},
    {0xC0, "`"},
    {0xDB, "["},
    {0xDC, "\\"},
    {0xDD, "]"},
    {0xDE, "\'"},
};