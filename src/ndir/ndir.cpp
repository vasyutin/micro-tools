//---------------------------------------------------------------------------
// NDir
//
// Creates directory with the name which is constructed of current date (and time).
//
// Written by Sergey Vasyutin (sergey [at] vasyut.in).
//---------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include <ctime>

//---------------------------------------------------------------------------
int main(int Argc_, char *Argv_[])
{
	struct THelper {
		static void usage() {
			puts("NDIR. Create directory named as current date (and time)."
				"Copyright 2000-2020 by Sergey Vasyutin.\n"
				"Usage:\n"
				"\tndir <-t>\n"
				"Switches:\n"
				"\t-t - add time to the directory name;\n"
				"\t-m - year and month only;\n"
				"\t-r - append an underscore and a random number from 00001 to 32768 to the name;\n"
				"\t-p - only print the name for a new directory, the directory is not being created.");
		}
	};

	// ---
	bool PrintOnly = false, AddTime = false, YearAndMonthOnly = false, AddRandom = false;
	
	for(int i = 1; i < Argc_; ++i) {
		const char* Arg = Argv_[i];
		if(!strcmp(Arg, "-h") || !strcmp(Arg, "-H") || !strcmp(Arg, "/?")) {
			THelper::usage();
			return 0;
		}
		else if(!strcmp(Arg, "-t") || !strcmp(Arg, "-T")) {
			AddTime = true;
		}
		else if(!strcmp(Arg, "-m") || !strcmp(Arg, "-M")) {
			YearAndMonthOnly = true;
		}
		else if(!strcmp(Arg, "-p") || !strcmp(Arg, "-P")) {
			PrintOnly = true;
		}
		else if(!strcmp(Arg, "-r") || !strcmp(Arg, "-R")) {
			AddRandom = true;
		}
		else {
			printf("Invalid switch '%s'.\n", Arg);
			THelper::usage();
			return 1;
		}
	}

	const size_t BUFFER_SIZE = 128;
	char DirName[BUFFER_SIZE];
	SYSTEMTIME st;

	GetLocalTime(&st);
	GetDateFormatA(LOCALE_SYSTEM_DEFAULT, 0, &st, YearAndMonthOnly ? "yy-MM": "yy-MM-dd", DirName, (int)BUFFER_SIZE);
	size_t CurrentSize = strlen(DirName);
	if(AddTime) {
		CurrentSize += (size_t)GetTimeFormatA(LOCALE_SYSTEM_DEFAULT, TIME_NOSECONDS | TIME_FORCE24HOURFORMAT,
			&st, "_HH-mm", DirName + CurrentSize, int(BUFFER_SIZE - CurrentSize));
	}
	if(AddRandom) {
		size_t CurrentSize = strlen(DirName);
		#ifdef _MSC_VER
			#pragma warning(push)
			#pragma warning(disable: 4996)
		#endif
		srand((unsigned)time(NULL));
			sprintf(DirName + CurrentSize, "_%05d", int(rand() + 1));
		#ifdef _MSC_VER
			#pragma warning(pop)
		#endif
	}
	if(PrintOnly) {
		puts(DirName);
		return 0;
	}
	return (CreateDirectoryA(DirName, NULL) != 0)? 0: 1;
}