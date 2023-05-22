#pragma once
#ifndef MYSQL_H
#define MYSQL_H
#include<iostream>
#include<mysql/mysql.h>
#include<string>
#include<vector>
#include<span>
#include<typeinfo>
#include<cxxabi.h>
#include<exception>
using std::string;
using std::vector;
using std::span;
using namespace abi;
#endif
class Type_Exception:public std::exception{
public:
    const char* what() {
        return "type error";
    }
};

class MySql//:public std::exception
{
public:
    enum Alter_Type{
        ADD,
        DROP,
        CHANGE,
    };

    MySql();
    MySql(const MySql&) = delete;
    MySql(MySql&&)=delete;
    MySql& operator=(const MySql&)=delete;
    MySql& operator=(const MySql&&)=delete;
    
    uint32_t Connect(string remote,string usrname,string passwd,string db_name);

    uint32_t Create_Table(string tb_name,vector<string>&& words,vector<string>&& types);

    uint32_t Drop_Table(string tb_name);

    //inline uint32_t Create_DB(string db_name);

    vector<vector<string>> Select(string field,string table,string condition="NULL");

    uint32_t Insert(string tb_name,vector<string> &&columns , vector<string> &&values);

    uint32_t Update(string tb_name,vector<string> &&columns,vector<string> &&values,string condition);

    uint32_t Delete(string tb_name,string condition);

    uint32_t Alter(string tb_name,Alter_Type type,string condition);

    template<typename ...Str>
    static inline vector<string> Arg_List(Str...colmuns){
        vector<string> ans;
        try{
            vector<string> res;
            arg_all(res,colmuns...);
            ans=res;
        }
        catch(Type_Exception* &e){
            std::cout<<e->what()<<std::endl;
            delete e;
        }
        return ans;
    }


    void  History(int num=0);

    ~MySql(){
        mysql_close(&this->handle);//关闭数据库连接
    }

private:
    
    string  All_Context(vector<string>& args);
    
    static inline void arg_sub(vector<string>& sub){
    }

    template<typename Arg,typename ... Args>
    static inline void arg_sub(vector<string>& sub,Arg value,Args...values){
        if(!IsString(value)) throw new Type_Exception;
        sub.push_back(value);
        arg_sub(sub,values...);
    }

    template<typename ... Args>
    static inline void arg_all(vector<string>& sub,Args...args){
        arg_sub(sub,args...);
    }

    template<typename T>
    static inline bool IsString(T& value){
        //string stringtype(__cxa_demangle(typeid(string).name(),NULL,NULL,NULL));
        string type(__cxa_demangle(typeid(value).name(),NULL,NULL,NULL));
        if(type.find("char")!=string::npos||type.find("string")!=string::npos) return true;
        return false;
    }
    inline span<string> Cmd_History(){
        return cmd_history;
    }

    inline uint32_t  Cmd_Num(){
        return cmd_history.size();
    }

    inline void Cmd_Add(string cmd){
        cmd_history.push_back(cmd);
    }

    MYSQL handle;//mysql连接句柄结构体
    vector<string> cmd_history;//历史mysql查询语句
};
