#ifndef USER_H_INCLUDED
#define USER_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <time.h>

//================================================================
// 列挙型：t_Avail
//================================================================
enum  t_Avail
{
  UNAVAIL, // 無効
  AVAIL,   // 有効
  //
  EOD_AVAIL
};

//================================================================
// 列挙型：t_Level
//================================================================
enum  t_Level
{
  ADMIN, // 管理者
  PREM,  // プレミア会員
  GEN,   // 一般会員
  TRY,   // お試し会員
  //
  EOD_LEVEL
};

//================================================================
// グローバル定数：
//================================================================
const int    MaxIDLen  = 40;
const int    MaxStrLen = 256;
const int    MaxUsers  = 1000;
static const char*  UserDBFile = "db.dat";

//================================================================
// クラス：User
//================================================================
class  User
{
public:
  char      ID[MaxIDLen];     // ユーザID
  char      Name[MaxStrLen];  // 氏名
  char      Pass[MaxStrLen];  // パスワード
  t_Avail   Avail;            // 有効フラグ
  t_Level   Level;            // ユーザレベル
public:
  User()  // 専用の初期化関数
  {
    Clear();
  }
  User(User& u)
  {
    Set(u);
  }
  void  Clear()
  {
    // 文字列の初期化
    ID[0] = 0;
    Name[0] = 0;
    Pass[0] = 0;

    // フラグの初期化
    Avail = AVAIL;
    Level = GEN;
  }
  void  Set(User& u) 
  {
    strcpy(ID, u.ID);
    strcpy(Name, u.Name);
    strcpy(Pass, u.Pass);
    Avail = u.Avail;
    Level = u.Level;
  }
  void  Out()
  {
    printf("%-8s | %-30s | %-20s | %-8s | %-8s\n"
	   , ID
	   , Name
	   , Pass
	   , Avail == AVAIL ? "Avail" : "Unavail"
	   , Level == ADMIN ? "Admin" : Level == PREM ? "Prem"
	   : Level == GEN ? "GEN" : ""
	   );
  }
  void  Disable()
  {
    Avail = UNAVAIL;
  }
  void  Enable()
  {
    Avail = AVAIL;
  }
};


//================================================================
// クラス：UserDB
//================================================================
class  UserDB
{
public:
  int    Num;          // 登録ユーザ数が増えたらインクリメントする変数
  User   U[MaxUsers];  // ユーザページ, 最大数分確保しておく

  int    FNum;
  int    F[MaxUsers];  
public:
  UserDB()
  {
    Reset();
  }
  ~UserDB()
  {
  }
  void  Add(User& u);
  void  Add(const char* name, const char* pass);
  void  Reset()
  {
    Num = 0;
    memset(U, 0, sizeof(User) * MaxUsers);
  }
  void  Show();
  void  Save();
  void  Load();
  void  Search(char user_id[]);
  void  Edit();
  
  int   InRange(int nth)
  {
    return  (0 <= nth && nth < Num) ? true : false;
  }
  void  Disable(int nth)
  {
    if(InRange(nth))
      U[nth].Disable();
  }
  void  Enable(int nth)
  {
    if(InRange(nth))
      U[nth].Enable();
  }
  void  Out(int nth)
  {
    if(InRange(nth))
      U[nth].Out();
  }
  void  Clear(int nth)
  {
    if(InRange(nth))
      U[nth].Clear();
  }
  //
  void  Demo1();
  void  Demo2();
  void  Demo3();
  void  Demo4();
  void  Demo5_user_search();
};

#endif