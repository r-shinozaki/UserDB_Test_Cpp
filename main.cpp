#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include "user.h"

//================================================================
// 関数：main
//================================================================
int  main()
{
  srand(time(NULL));
  UserDB  db;

  //db.Demo1();
  //db.Demo2();
  //db.Demo3();
  //db.Demo4();
  db.Demo5_user_search();
  return  0;
}
