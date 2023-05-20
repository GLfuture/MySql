#include"MySql.h"

int main()
{
    MySql mysql;
    int ret = mysql.Connect("127.0.0.1","user1","123456","test");
    mysql.Insert("user",MySql::Arg_List("name","age"),MySql::Arg_List("'zhang'","10"));
    vector<vector<string>> res=mysql.Select("*","user");
    mysql.Update("user",MySql::Arg_List("name","age"),MySql::Arg_List("'li'","30"),"name='zhang'");
    mysql.Delete("user","name='zhang'");
    mysql.History();
    for(auto &s:res){
        for(auto &v:s){
            std::cout<<v<<" ";
        }
        std::cout<<std::endl;
    }
    return 0;
}