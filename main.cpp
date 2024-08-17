#include<sqlite3.h>
//TODO: اضافة لتعريف النظام لاضافة ملف قاعدة البيانات اذا لم يوجد  
#include<iostream>

using namespace std;

class connection {
    private:
      sqlite3* DB;
      char* errMsg = 0;
      int exit = sqlite3_open("hospital.db", &DB);
    
    public:
      void connect(){
      if(exit) {
        cout<< "Error open DB " << sqlite3_errmsg(DB) << endl;
        return;
        }
        cout<< "connection done ";
      }

      void disoncect(){
        sqlite3_close(DB);
        cout<< "connection closed ";
      }
};

int main(){

  connection db ;

  db.connect();
  db.disoncect(); 


    return 0;
}