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

    MySql();

    inline uint32_t Connect(string remote,string usrname,string passwd,string db_name);


    inline vector<vector<string>> Select(string field,string table,string condition="NULL");

    int inline Insert(string table,vector<string> &&columns , vector<string> &&values);

    int inline Update(string table,vector<string> &&columns,vector<string> &&values,string condition);

    int inline Delete(string table,string condition);

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


    void inline History(int num=0);

    ~MySql(){
        mysql_close(&this->handle);//关闭数据库连接
    }

private:
    
    string inline All_Context(vector<string>& args);
    
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