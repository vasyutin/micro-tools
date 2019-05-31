//---------------------------------------------------------------------------
// ToHex
// Convert binary file to C/C++ header file containing source file as a char array.
// Written by Sergey Vasyutin (sergey [at] vasyut.in).
//---------------------------------------------------------------------------

#include <stdio.h>
#include <string>
#include <algorithm>

//---------------------------------------------------------------------------
void Usage(void)
{
printf(
"ToHex. Convert binary file to C/C++ header file containing source file as a char array.\n"
"Usage:\n"
"   tohex <source filename>\n\n"
"The generated source code is written to the standard output stream.");
}

//---------------------------------------------------------------------------
void ReplaceSymbols(std::string &String_)
{
for(std::string::iterator it = String_.begin(); it != String_.end(); ++it) {
	if(*it == '-') *it = '_';
	}
}

//---------------------------------------------------------------------------
int main(int Argc_, char *Argv_[])
{
long Size;
FILE *fp;
int SymbolInStringCount = 0;
int c;
int Count = 0;

if(Argc_ < 2) {
   Usage();
   return 1;
   }

std::string FileName(Argv_[1]);

fp = fopen(FileName.c_str(), "rb");
if(!fp) {
	printf("Error opening file '%s'.", FileName.c_str());
	return 1;
	}

fseek(fp, 0, SEEK_END);
Size = ftell(fp);
fseek(fp, 0, SEEK_SET);

// Extracting filename without extension.
struct THelper {
	static bool isSeparator(const char C_) {return C_ == '\\' || C_ == '/';} 
	};
std::string::reverse_iterator it = std::find_if(FileName.rbegin(), FileName.rend(), 
	THelper::isSeparator);
//
std::string::iterator Start = it.base();
std::string::iterator Last = std::find(Start, FileName.end(), '.');
FileName.assign(Start, Last);
ReplaceSymbols(FileName);
//
printf("/* This file was generated automatically. Do not edit. */\n"
   "#ifndef DATA_DECLARATION_ONLY\n\n");
printf("unsigned char %s[%ld] = {", FileName.c_str(), Size);
while((c = fgetc(fp)) != EOF) {
   Count++;
   if(SymbolInStringCount == 16) SymbolInStringCount = 0;
   if(!SymbolInStringCount) printf("\n\t");
   printf("0x%02x%c ", c, (Count == Size)? ' ': ',');
   SymbolInStringCount++;
   }
printf("\n\t};\n#else\n");
printf("extern unsigned char %s[%ld];\n", FileName.c_str(), Size);
printf("#endif\n\n/* EOF */\n");

return 0;
}
