#include "pch.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	string tmpath = argv[1];
	TM *t = new TM(tmpath);
	t->control();
	return 0;
}

