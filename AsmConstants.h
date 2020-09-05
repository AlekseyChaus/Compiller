#pragma once
#include <string>
// файл содержит набор констант для удобства работы


using namespace std;


const string asmHeader = ".586\n"
".model flat, stdcall\n\n"
"include <\\masm32\\include\\msvcrt.inc>\n"
"include <\\masm32\\include\\kernel32.inc>\n"
"includelib <\\masm32\\lib\\msvcrt.lib>\n"
"includelib <\\masm32\\lib\\kernel32.lib>\n";

const string startData = "data segment\n";
const string endData = "data ends\n";

const string textStart = "text segment\n";
const string textEnd = "text ends\n";

const string labelStart = "__start:\n";
const string labelEnd = "end __start\n";

const string procProlog = "   enter ";

const string procEpilogue = "   leave\n";

const string functionReturn = "   ret\n";

const string tab = "   ";



// registers
const string eax("eax");
const string ebx("ebx");
const string ecx("ecx");
const string edx("edx");
const string esi("esi");





const string null("0");
const string one("1");
const string minus_one("-1");
