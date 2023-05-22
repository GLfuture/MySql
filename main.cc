#include"MySql.h"

int main()
{
    MySql mysql;
    int ret = mysql.Connect("127.0.0.1","user1","123456","test");
    mysql.Create_Table("manager",MySql::Arg_List("name","age"),MySql::Arg_List("varchar(10) primary key","tinyint"));
    mysql.Insert("user",MySql::Arg_List("name","age"),MySql::Arg_List("'zhang'","10"));
    vector<vector<string>> res=mysql.Select("*","user");
    mysql.Update("user",MySql::Arg_List("name","age"),MySql::Arg_List("'li'","30"),"name='zhang'");
    mysql.Delete("user","name='li'");
    mysql.Alter("manager",MySql::ADD,"address varchar(20)");
    mysql.Alter("manager",MySql::CHANGE,"address address varchar(10)");
    mysql.Alter("manager",MySql::DROP,"name");
    mysql.Alter("manager",MySql::ADD,"name varchar(20)");
    //mysql.Drop_Table("manager");
    mysql.History();
    for(auto &s:res){
        for(auto &v:s){
            std::cout<<v<<" ";
        }
        std::cout<<std::endl;
    }
    return 0;
}