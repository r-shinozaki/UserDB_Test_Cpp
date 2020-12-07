#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
//=========================
// グローバル定数
//=========================
const int MaxBufSize = 4096;

class CGI
{
public:
	char Buf[MaxBufSize];
	CGI()
	{
		printf("Content-type:text/html\r\n\r\n");
		GetData();
		// Decode();
	}
	void GetData()
	{
		char* p = fgets(Buf, sizeof(Buf), stdin);
		printf("%s", p);
	}
	void Decode(char* buf);
	void Div();
};

void Div()
{
	
}

void CGI::Decode(char* buf)
{
	if(! buf) return;
	char* p = buf;
	
}

class HTML
{
public:
	HTML()
	{
		printf("<html lang=\"ja\">\n<head>\n");
		Head();
		printf("</head>\n<body>HELLO WORLD!\n");
	}
	~HTML(){
		printf("</body>\n</html>\n");
	}
	void Head()
	{
		printf("<meta charset=\"UTF-8\">\n<title>Document</title>\n");
	}
};

class App : public CGI, public HTML
{

};

int main()
{
	App();
	return 0;
}