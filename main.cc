#include"MySql.h"

int main()
{
    MySql mysql;
    int ret = mysql.Connect("127.0.0.1","user1","123456","test",3306);
    mysql.Create_Table("manager",MySql::Arg_List("name","age"),MySql::Arg_List("varchar(10) primary key","tinyint"));
    mysql.Insert("manager",MySql::Arg_List("name","age"),MySql::Arg_List("'zhang'","10"));
    vector<vector<string>> res=mysql.Select("*","user");
    mysql.Update("manager",MySql::Arg_List("name","age"),MySql::Arg_List("'li'","30"),"name='zhang'");
    mysql.Insert("manager",MySql::Arg_List("name","age"),MySql::Arg_List("'zhang'","10"));
    mysql.Delete("manager","name='li'");
    mysql.Alter("manager",MySql::ADD,"address varchar(20)");
    mysql.Alter("manager",MySql::CHANGE,"address address varchar(10)");
    mysql.Alter("manager",MySql::DROP,"address");
    mysql.Alter("manager",MySql::ADD,"name varchar(20)");
    //mysql.Drop_Table("manager");
    mysql.Create_Table("",MySql::Arg_List(""),MySql::Arg_List(""));
    mysql.Alter("",MySql::ADD,"");
    mysql.Delete("","");
    mysql.History();
    for(auto &s:res){
        for(auto &v:s){
            std::cout<<v<<" ";
        }
        std::cout<<std::endl;
    }
    return 0;
}