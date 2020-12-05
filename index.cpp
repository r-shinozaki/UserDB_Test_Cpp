#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

class HTML
{
public:
	HTML()
	{
		printf("Content-type:text/html\r\n\r\n");
		printf("<html lang=\"en\">\n<head>\n");
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

int main()
{
	HTML();
	return 0;
}