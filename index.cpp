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
const char* ProgName = "index.cgi";

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
	   , i+1
	   , Item[i].Name
	   , Item[i].Value
	   );
  }
  printf("</table>\n");
}

//=========================
// クラス：CGI
//=========================
class CGI : public StrTable
{
public:
	char Buf[MaxBufSize];
	CGI()
	{
		printf("Content-type:text/html\n\n");
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
		Num = 0;
		do{
			char* r;
			if(r = strchr(q, '=')){
				*r = 0;
				Add(q, r+1);
				Decode(Item[Num-1].Value);
			}
		} while(q = strtok(NULL, "&")); 
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
			case '+' :
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
	virtual void Run() = 0;
};

//=======================================
// クラス：App
//=======================================
class App : public CGI, public HTML
{
public:
	virtual void Run();
	void StartForm()
	{
		printf("<form action %s method=post>\n", ProgName);
	}
	void EndForm()
	{
		printf("</form>");
	}
};

void App::Run()
{
	Show();

	StrTable tbl;
	tbl.Add("Add", "新規登録");
	tbl.Add("edit", "ユーザ編集");
	tbl.Add("del", "ユーザ削除");
  tbl.Add("show", "ユーザ一覧");
  tbl.Add("search", "ユーザ検索");
  tbl.Add("load", "読込");
  tbl.Add("save", "保存");
  tbl.Add("savedemo", "デモデータ保存");

	char name[256];
	StartForm();
	{
		for(int i=0; i<tbl.Num; i++){
      printf("<input type=submit name=%s value='%s'>\n"
	     , tbl.Item[i].Name
	     , tbl.Item[i].Value
	     );
    }
	}
	EndForm();
}

//=====================================
// 関数：main
//=====================================
int main()
{
	App().Run();
	return 0;
}
