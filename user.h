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

const char*  LevelLabel[] = {
  "管理者", "プレミア会員", "一般会員", "お試し会員", NULL
};

//================================================================
// グローバル定数：
//================================================================
const int    MaxBufSize = 4096;
const int    MaxNameSize = 1024;
const int    MaxValueSize = 2048;
//
const int    MaxStrItem = 500;
//
const char*  ProgName = "index.cgi";  // 実行ファイル名
//
const int    MaxIDLen  = 40;
const int    MaxStrLen = 256;
const int    MaxUsers  = 1000;
const char*  UserDBFile = "db.dat";


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
	   : Level == GEN ? "GEN" : "Try"
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
  void  SetLevel(t_Level lv)
  {
    Level = lv;
  }
  void  Remove()
  {
    // 文字列の初期化
    ID[0] = '-';
    Name[0] = 0;
    Pass[0] = 0;

    // フラグの初期化
    Avail = AVAIL;
    Level = GEN;
  }
};

//================================================================
// クラス：UserDB
//================================================================
class  UserDB
{
public:
  int    Num;          // 登録ユーザ数
  User   U[MaxUsers];  // ユーザページ

  int    FNum;         // バッファ登録数
  int    F[MaxUsers];  // 検索バッファ
public:
  UserDB()
  {
    Reset();
  }
  ~UserDB()
  {
  }
  void  Add(User& u);
  void  Add(const char* name, const char* pass, t_Level level);
  void  AddBuf(int nth)
  {
    if(FNum < MaxUsers){
      F[FNum] = nth;
      FNum++;
    }
  }
  void  Reset()
  {
    Num = 0;
    memset(U, 0, sizeof(User) * MaxUsers);
    ResetBuf();
  }
  void  ResetBuf()
  {
    FNum = 0;
    // memset(F, 0, sizeof(int) * MaxUsers);
  }
  void  Show();
  void  ShowFound();
  void  Save();
  void  Load();
  //
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
  void  Remove(int nth)
  {
    if(InRange(nth))
      U[nth].Remove();
  }
  void  SetLevel(int nth, t_Level lv)
  {
    if(InRange(nth))
      U[nth].SetLevel(lv);
  }
  int  Search(const char* name, int is_patial = false);
  void  GenForm(int nth);
  //
  void  SaveDemo();
};

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
void  UserDB::Add(const char* name, const char* pass, t_Level level)
{
  User  u;

  sprintf(u.ID, "s%04d", Num);
  strcpy(u.Name, name);
  strcpy(u.Pass, pass);
  u.SetLevel(level);
  Add(u);
}

//-------------------------------------------------------
// メンバ関数：Show
// ----------------
// ページに登録されているユーザをすべて表示する
//-------------------------------------------------------
void  UserDB::Show()
{
  for(int i=0; i<Num; i++){
    printf("[%4d] ", i);
    U[i].Out();
  }
}

//-------------------------------------------------------
// メンバ関数：ShowFound
// ---------------------
// 検索されたユーザをすべて表示する
//-------------------------------------------------------
void  UserDB::ShowFound()
{
  for(int i=0; i<FNum; i++){
    printf("[%4d] ", i);
    U[F[i]].Out();
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
//-------------------------------------------------------
void  UserDB::Load()
{
  FILE*  fp = fopen(UserDBFile, "rb+");
  if(! fp){
    perror("UserDB::Load()");
    return;
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
// ------------------
// ユーザ名が一致したレコードの検索フラグをセットし，
// 一致したデータ数を返す
//-------------------------------------------------------
int  UserDB::Search(const char* name, int is_partial)
{
  if(! name || ! *name)
    return  0;

  ResetBuf();
  for(int i=0; i<Num; i++){
    // 部分一致
    if(is_partial && strstr(U[i].Name , name)){
      AddBuf(i);
    }
    // 完全一致
    else if(strcmp(U[i].Name, name) == 0){
      AddBuf(i);
    }
  }
  return  FNum;
}

//-------------------------------------------------------
// メンバ関数：GenForm
// -------------------
// 指定したレコード番号のユーザページから読み込み，
// フォーム画面を生成する
//-------------------------------------------------------
void  UserDB::GenForm(int nth)
{
  if(nth < 0 || nth >= Num){
    printf("Invalid record number (%d).\n", nth);
    return;
  }

  printf("<table border cellpadding=1 cellspacing=1 bgcolor=#cce>\n"
	 "<tr><td>氏名<td><input name=name value='%s'>\n"
	 "<tr><td>パスワード<td><input type=password name=pass value='%s'>\n"
	 "<tr><td>有効フラグ<td><input type=radio name=avail value='1' %s>有効\n"
	 "<input type=radio name=avail value='0' %s>無効\n"
	 "<tr><td>会員種別<td><select name=level>"
	 , U[nth].Name
	 , U[nth].Pass
	 , U[nth].Avail ? "checked" : ""
	 , U[nth].Avail ? "" : "checked"
	 );
  for(int i=0; i<EOD_LEVEL; i++){
    printf("<option value=%d %s>%s"
	   , i
	   , U[nth].Level == i ? "selected" : ""
	   , LevelLabel[i]);
  }
  printf("</select>"
	 "<tr><td colspan=2><input type=submit name=doadd value='登録'>"
	 "</table>"
	 );
}

//-------------------------------------------------------
// メンバ関数：SaveDemo
// --------------------
// n 人のユーザを登録後，データ保存する．
//-------------------------------------------------------
void  UserDB::SaveDemo()
{
  const char*  name[] = {
    "田中一郎", "佐藤次郎", "小林次郎", "中尾静香",
    "遠藤太郎", "小林香織", "前田太郎", "佐久太郎",
    NULL
  };
  char  pass[MaxStrLen];

  Reset();
  for(int i=0; name[i]; i++){
    sprintf(pass, "p%08d", i);
    t_Level  level = (t_Level)(rand() % EOD_LEVEL);
    Add(name[i], pass, level);

    // ユーザレベル（会員種別）を乱数で設定
    int  lv = rand() % EOD_LEVEL;
    SetLevel(i, (t_Level)lv);
  }
  if(! Num){
    printf("Num is 0.\n");
    return;
  }
  Save();
}



#endif