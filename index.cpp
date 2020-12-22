#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "user.h"

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
	UserDB	UDb;
public:
	App()
	{
		UDb.Load();
	}
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
	tbl.Add("add", "新規登録");
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

	// ユーザ登録
  if(Search("add")){
    printf("add<br>\n");
    StartForm();
    {
      printf("<table border cellpadding=1 cellspacing=1 bgcolor=#cce>\n"
	     "<tr><td>氏名<td><input name=name >\n"
	     "<tr><td>パスワード<td><input type=password name=pass >\n"
	     "<tr><td>有効フラグ<td><input type=radio name=avail value='1' checked>有効\n"
	     "<input type=radio name=avail value='0'>無効\n"
	     "<tr><td>会員種別<td><select name=level>"
	     );
      for(int i=0; i<EOD_LEVEL; i++){
	      printf("<option value=%d %s>%s"
	       , i
	       , i == GEN ? "selected" : ""
	       , LevelLabel[i]);
      }
      printf("</select>"
	     "<tr><td colspan=2><input type=submit name=doadd value='登録'>"
	     "</table>"
	     );
    }
    EndForm();
  }
  else if(Search("doadd")){
    char*  name = Search("name");
    char*  pass = Search("pass");
    char*  avail = Search("avail");
    char*  level = Search("level");
    char*  id = Search("id");
    if(name && *name && pass && *pass && avail && *avail && level && *level){
      if(! id){
	UDb.Add(name, pass, (t_Level)atoi(level));
      }
      else {
	int  nth = atoi(id);
	strcpy(UDb.U[nth].Name, name);
	strcpy(UDb.U[nth].Pass, pass);
	if(UDb.U[nth].ID[0] == '-')
	  UDb.U[nth].ID[0] = 's';
	UDb.U[nth].Avail = (t_Avail)atoi(avail);
	UDb.U[nth].Level = (t_Level)atoi(level);
      }
      printf("<textarea cols=100 rows=10 readonly>");
      UDb.Show();
      printf("</textarea>");
      UDb.Save();
    }
    else {
      printf("すべてのデータを入力してください．");
    }
  }
  // ユーザ編集
  else if(Search("edit")){
    printf("edit<br>\n");
    StartForm();
    {
      printf("<select name=id>");
      for(int i=0; i<UDb.Num; i++){
	printf("<option value=%d>%d", i, i);
      }
      printf("</select>"
	     "<input type=submit name=doedit value='編集'>"
	     );
    }
    EndForm();
  }
  // ユーザ編集
  else if(Search("doedit")){
    char*  id = Search("id");
    if(! id || ! *id)
      ;
    else {
      int  nth = atoi(id);

      printf("nth = [%d]<br>\n", nth);
      StartForm();
      {
	printf("<input type=hidden name=id value='%d'>", nth);
	UDb.GenForm(nth);
      }
      EndForm();
    }
  }
  // ユーザ削除
  else if(Search("del")){
    printf("del<br>\n");
    StartForm();
    {
      printf("<select name=id>");
      for(int i=0; i<UDb.Num; i++){
	printf("<option value=%d>%d", i, i);
      }
      printf("</select>"
	     "<input type=submit name=dodel value='削除'>"
	     );
    }
    EndForm();
  }
  // ユーザ削除
  else if(Search("dodel")){
    char*  id = Search("id");
    if(! id || ! *id)
      ;
    else {
      int  nth = atoi(id);

      printf("nth = [%d]<br>\n", nth);
      UDb.Remove(nth);
      UDb.Save();
    }
  }
  // ユーザ一覧
  else if(Search("show")){
    printf("show<br>\n");
    printf("<textarea cols=100 rows=10 readonly>");
    UDb.Show();
    printf("</textarea>");
  }
  // ユーザ検索
  else if(Search("search")){
    printf("search<br>\n");
    StartForm();
    {
      printf("<input name=key>"
	     "<input type=submit name=dosearch value='検索'>"
	     );
    }
    EndForm();
  }
  // ユーザ検索
  else if(Search("dosearch")){
    printf("search<br>\n");
    char*  key = Search("key");
    UDb.Search(key, true);
    printf("<textarea cols=100 rows=10 readonly>");
    UDb.ShowFound();
    printf("</textarea>");
  }
  // ファイル読込
  else if(Search("load")){
    printf("load<br>\n");
    UDb.Load();
    printf("<textarea cols=100 rows=10 readonly>");
    UDb.Show();
    printf("</textarea>");
  }
  // ファイル保存
  else if(Search("save")){
    printf("save<br>\n");
    UDb.Save();
  }
  // デモデータ保存
  else if(Search("savedemo")){
    printf("savedemo<br>\n");
    UDb.SaveDemo();
  }
}

//=====================================
// 関数：main
//=====================================
int main()
{
	App().Run();
	return 0;
}
