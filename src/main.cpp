#include <glad/glad.h>
#include <iostream>
#include "OBJDisplayer.h"
#include "objmodel.h"
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		cout << "Usage: " << argv[0] << " <obj> <texture>" << endl;
		return -1;
	}
	//OBJmodel obj;
	//obj.load( string(argv[1]) );
	//cout << argv[1] << endl;
	OBJDisplayer objDisplayer( argv[1], argv[2] );
	objDisplayer.run();	
}

