#include"MySql.h"
#include<fstream>
int read_image(string filename,char*buffer,int len)
{
    std::ifstream in(filename.c_str(),std::ios::binary);
    if(!in.is_open()){
        std::cout<<"open failed"<<std::endl;
        return 0;
    }
    in.seekg(0,in.end);
    int total_len=in.tellg();
    in.seekg(0,in.beg);
    in.read(buffer,total_len);
    //std::cout<<in.gcount()<<"\n";
    in.close();
    return total_len;
}

void write_image(string filename,char*buffer,int len)
{
    std::ofstream out(filename.c_str(),std::ios::binary);
    out.write(buffer,len);
    out.close();
}

int main()
{
    MySql mysql;
    int ret = mysql.Connect("127.0.0.1","user1","123456","test",3306);
    //mysql.Create_Table(mysql.Create_Query("manager",MySql::Arg_List("name","age"),MySql::Arg_List("varchar(10) primary key","tinyint")));
    //mysql.Insert(mysql.Insert_Query("manager",MySql::Arg_List("name","age"),MySql::Arg_List("'zhang'","10")));
    //mysql.Update(mysql.Update_Query("manager",MySql::Arg_List("age"),MySql::Arg_List("30"),"name='zhang'"));
    //mysql.Insert(mysql.Insert_Query("manager",MySql::Arg_List("name","age"),MySql::Arg_List("'li'","10")));
    //mysql.Delete(mysql.Delete_Query("manager","name='li'"));
    //vector<vector<string>> res=mysql.Select("*","manager");
    //mysql.Alter(mysql.Alter_Query("manager",MySql::ADD,"image LONGBLOB"));
    //mysql.Alter(mysql.Alter_Query("manager",MySql::CHANGE,"address address varchar(10)"));
   // mysql.Alter("manager",MySql::DROP,"address");
    //mysql.Alter("manager",MySql::ADD,"name varchar(20)");
    //mysql.Drop_Table(mysql.Drop_Query("manager"));
    //mysql.Create_Table("",MySql::Arg_List(""),MySql::Arg_List(""));
    //mysql.Alter("",MySql::ADD,"");
    //mysql.Delete("","");
    //插入图片
    char *buffer=new char[40000];
    memset(buffer,0,40000);
    int len = read_image("/home/gong/projects/MySql/src/a.jpg",buffer,40000);
    std::cout<<"file size:"<<len<<"\n";
    mysql.Param_Send_Binary(mysql.Insert_Query("manager",MySql::Arg_List("name","image"),MySql::Arg_List("'gong'","?")),buffer,len);
    mysql.Param_Recv_Binary(mysql.Select_Query("image","manager","name='gong'"),buffer,len);
    write_image("/home/gong/projects/MySql/src/b.jpg",buffer,len);
    mysql.History();
    // for(auto &s:res){
    //     for(auto &v:s){
    //         std::cout<<v<<" ";
    //     }
    //     std::cout<<std::endl;
    // }
    return 0;
}