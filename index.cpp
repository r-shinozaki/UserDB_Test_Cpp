#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
//=========================
// グローバル定数
//=========================
const int MaxBufSize = 4096;
const int MaxNameSize = 1024;
const int MaxValueSize = 2048;
const int MaxStrItem = 500;
const char* Progname = "index.cgi";

//=========================
// クラス：StrItem
//=========================
class StrItem
{
public:
	char* Name;
	char* Value;
public:
	StrItem()
	{
		Name = new char[MaxNameSize];
		Value = new char[MaxValueSize];
	}
	~StrItem()
	{
		if(Name) delete []Name;
		if(Value) delete []Value;
	}

	void Set(const char* name, const char* value)
	{
		if(name && *name){
			strncpy(Name, name, MaxNameSize);
		}else{
			Name[0] = 0;
		}
		if(value && *value){
			strncpy(Value, value, MaxValueSize);
		}else{
			Value[0] = 0;
		}
	}
	void Clear()
	{
		Name[0] = 0;
		Value[0] = 0;
	}
};

//==========================
// クラス：StrTable
//==========================
class StrTable
{
public:
	int Num;
	StrItem* Item;
public:
	StrTable()
	{
		Num = 0;
		Item = new StrItem[MaxStrItem];
	}
	~StrTable()
	{
		if(Item) delete []Item;
	}
	int Add(const char* name, const char* value){
		if(Num < MaxStrItem){
			Item[Num].Set(name, value);
			Num++;
			return 1;
		}
		return 0;
	}
	char* Search(const char* name);
	void Show();
};

char* StrTable::Search(const char* name)
{
	if(! name || ! *name) return NULL;
	for(int i=0; i<Num; i++){
		if(strcmp(Item[i].Name, name) == 0)
			return Item[i].Value;
	}
	return NULL;
}

void StrTable::Show()
{
	printf("<table border=1 cellpadding=1 cellspacing=1>\n");
  for(int i=0; i<Num; i++){
    printf("<tr><td>%d<td>%s<td>%s\n"
	   , i
	   , Item[i].Name
	   , Item[i].Value
	   );
  }
  printf("</table>\n");
}

//=========================
// クラス：CGI
//=========================
class CGI
{
public:
	char Buf[MaxBufSize];
	CGI()
	{
		printf("Content-type:text/html\r\n\r\n");
		GetData();
		Div();
	}
	void GetData()
	{
		char* p = fgets(Buf, sizeof(Buf), stdin);
	}
	void Decode(char* buf);
	void Div();
};

void CGI::Div()
{
	char* p = Buf;
	char* q;
	if(q = strtok(p, "&")){
		int Num = 0;
		printf("strtok後の変数q : %s\n", q);
		do{
			char* r;
			if(r = strchr(q, '=')){
				*r = 0;
				printf("到達 : %s",r+1);
			}
		} while(q = strtok(NULL, "&")); //NULLは続きから
	}
}

void CGI::Decode(char* buf)
{
	if(! buf) return;
	char* p = buf;
	char decode[MaxBufSize];
	int code; 
	int i=0;
	while(*p){
		switch(*p){
			case '%' :
				sscanf(p+1, "%02X", &code);
				decode[i++] = code;
				p+=3;
				break;
			case '&' :
				decode[i++] = ' ';
				p++;
				break;
			default :
				decode[i++] = p[0];
				p++;
				break;
		}
	}	
	strcpy(buf, decode);
}

class HTML
{
public:
	HTML()
	{
		printf("<html lang=\"ja\">\n<head>\n");
		Head();
		printf("</head>\n<body>\n");
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
