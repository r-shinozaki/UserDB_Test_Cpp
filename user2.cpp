#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "user.h"

//-------------------------------------------------------
// メンバ関数：Add
// ---------------
// ユーザ u をページに登録する
//-------------------------------------------------------
void  UserDB::Add(User& u)
{
  if(Num < MaxUsers){
    U[Num].Set(u);
    Num++;
  }
  else {
    printf("UserDB::Add(): User Num is Overflow.\n");
  }
}

//-------------------------------------------------------
// メンバ関数：Add
// ---------------
// name, pass のユーザをページに登録する
//-------------------------------------------------------
void  UserDB::Add(const char* name, const char* pass)
{
  User  u;

  sprintf(u.ID, "s%04d", Num);
  strcpy(u.Name, name);
  strcpy(u.Pass, pass);
  Add(u);
}

//-------------------------------------------------------
// メンバ関数：Show
// ----------------
// ページに登録されているユーザをすべて表示する
// User型の変数Uに現在格納されているデータをすべて出力。
//-------------------------------------------------------
void  UserDB::Show()
{
  for(int i=0; i<Num; i++){
    printf("[%4d] ", i);
    U[i].Out();
  }
}

//-------------------------------------------------------
// メンバ関数：Save
// ----------------
// ページデータをファイルに保存する
//-------------------------------------------------------
void  UserDB::Save()
{
  FILE*  fp = fopen(UserDBFile, "rb+");
  if(! fp){
    fp = fopen(UserDBFile, "wb");
  }
  if(! fp){
    perror("UserDB::Save()");
    exit(-1);
  }
  fwrite(U, sizeof(User), MaxUsers, fp);
  fclose(fp);
}

//-------------------------------------------------------
// メンバ関数：Load
// ----------------
// ファイルからページデータを読み込む
// Numに現在のユーザ数を格納する。
//-------------------------------------------------------
void  UserDB::Load()
{
  FILE*  fp = fopen(UserDBFile, "rb+");
  if(! fp){
    perror("UserDB::Load()");
    exit(-1);
  }
  fread(U, sizeof(User), MaxUsers, fp);
  fclose(fp);

  Num = 0;
  for(int i=0; i<MaxUsers; i++){
    if(*U[i].ID)
      Num++;
  }
}

//-------------------------------------------------------
// メンバ関数：Search
//--------------------
// ファイルからデータを検索する
//-------------------------------------------------------
int  UserDB::Search(const char* user_id, int patial_search)
{
  if(!user_id || *user_id) return 0;

  int num = 0;
  ResetBuf();
  for(int i=0; i<Num; i++)
  {
    if(patial_search && strstr(U[i].ID, user_id))
    {
      AddBuf(i);
    }
    else if(strcmp(U[i].ID, user_id) == 0){
      AddBuf(i);
    }
  }
}

//-------------------------------------------------------
// メンバ関数：Edit
//---------------------------
// ファイルのユーザ情報を編集する
//-------------------------------------------------------
void UserDB::Edit(int num, char* edit_text)
{
  if(InRange(num)){
    strcpy(U[num].Name, edit_text);
  }
}

//-------------------------------------------------------
// メンバ関数：Demo1
// -----------------
// ３名のユーザをページに登録し表示する
//-------------------------------------------------------
void  UserDB::Demo1()
{
  Add("taro", "123");
  Add("hana", "257");
  Add("jiro", "369");
  Show();
}

//-------------------------------------------------------
// メンバ関数：Demo2
// -----------------
// n 人のユーザをページに登録し，セーブ，リセット，ロード
// を行ってからページの表示を行う
//-------------------------------------------------------
void  UserDB::Demo2()
{
  char  name[MaxStrLen];
  char  pass[MaxStrLen];

  int  n = 5; 
  //rand() % MaxUsers;
  
  for(int i=0; i<n; i++){
    sprintf(name, "n%08d", i);
    sprintf(pass, "p%08d", i);
    Add(name, pass);
  }
  Save();
  Reset();
  Load();
  Show();
}

//-------------------------------------------------------
// メンバ関数：Demo3
// -----------------
// n 人のユーザを登録後，指定したレコード番号のユーザを
// 有効化，無効化，クリアの各場合のページ一覧を行う
//-------------------------------------------------------
void  UserDB::Demo3()
{
  char  name[MaxStrLen];
  char  pass[MaxStrLen];

  int  n = 8;
  
  for(int i=0; i<n; i++){
    sprintf(name, "n%08d", i);
    sprintf(pass, "p%08d", i);
    Add(name, pass);
  }
  if(! Num){
    printf("Num is 0.\n");
    return;
  }

  int  nth;
  while(1){
    do {
      printf("Clear Record No.? (0-%d or 99 for QUIT) :", Num-1);
      scanf("%d", &nth);
      if(nth == 99)
	      return; // for QUIT
    } while(! InRange(nth));

    printf("\nRec [%d] is Disabled.\n", nth);
    Disable(nth);
    Show();

    printf("\nRec [%d] is Enabled.\n", nth);
    Enable(nth);
    Show();

    printf("\nRec [%d] is Cleared.\n", nth);
    Clear(nth);
    Show();
  }
}

void UserDB::Demo4(){
  char name[MaxStrLen];
  char pass[MaxStrLen];
  char temp_name[8];
  char temp_pass[8];

  int  n = 8;
  
  for(int i=0; i<n; i++){
    sprintf(name, "n%08d", i);
    sprintf(pass, "p%08d", i);
    Add(name, pass);
  }
  if(! Num){
    printf("Num is 0.\n");
    return;
  }

  int n_in;
  while(1){
    do{
      printf("Type number 0 or 1. (0: Add user, 1: Show users 99: Quit) : ");
      scanf("%d", &n_in);
    }while( n_in != 0 && n_in != 1 && n_in != 99);
    switch (n_in)
    {
    case 0:
      printf("name: ");
      scanf("%08s", temp_name);
      printf("password: ");
      scanf("%08s", temp_pass);
      snprintf(name, sizeof(name), "%8s", temp_name);
      snprintf(pass, sizeof(pass), "%8s", temp_pass);
      Add(name, pass);
      break;
    case 1:
      Show();
      break;
    case 99:
      return;
    default:
      break;
    }
  }
}

void UserDB::Demo5()
{
  char  pass[MaxStrLen];
  char  buf[MaxStrLen];
  int   n = 6;
  int   num = 0;
  int  mode_num;

  const char* name[] = 
  {
    "神奈川太郎", "神奈川太鳳", "ホゲ太郎", "佐藤真知子", NULL
  };

  for(int i=0; name[i]; i++)
  {
    sprintf(pass, "p%08d", i);
    Add(name[i], pass);
  }

  Save();
  Reset();
  Load();
  while(1)
  {
    Show();
    printf("Select Mode : (Serch:1 Edit:2 Quit:99) : ");
    fgets(mode_num, sizeof(mode_num), stdin);
    printf("error");
    switch (mode_num)
    {
    case 1 :
      printf("Who Serch? (Return for Quit) : ");
      fgets(buf, sizeof(buf), stdin);
      strtok(buf, "\r\n");
      if(! *buf || *buf == '\n') return;
      // Search(search_id); //全文一致
      num = Search(buf, true); //部分一致
      if (num)
      {
        printf("%d date found!", num);
        for(int i=0; i<num; i++)
        {
          U[F[i]].Out();
        }
        printf("\n");
      }
      break;
    case 2 :
      printf("Whose name edit? : ");
      break;
    default:
      return;
    }
  }
}