#include "MySql.h"
MySql::MySql()
{
    if (mysql_init(&this->handle) == NULL)
    {
        exit(0);
    }
    //设置属性
    int reconnect=1;
    mysql_options(&this->handle,MYSQL_OPT_RECONNECT,&reconnect);
    mysql_options(&this->handle,MYSQL_SET_CHARSET_NAME,"utf8mb4");
}

uint32_t MySql::Connect(string remote, string usrname, string passwd, string db_name)
{
    if (!mysql_real_connect(&this->handle, remote.c_str(), usrname.c_str(), passwd.c_str(), db_name.c_str(), 3306, NULL, 0))
    {
        return mysql_errno(&this->handle);
    }
    return 0;
}

uint32_t MySql::Create_Table(string tb_name,vector<string>&& words,vector<string>&& types)
{
    if(tb_name.empty()||(words.size()!=types.size()))
        return 0;
    mysql_ping(&this->handle);
    string query="CREATE TABLE IF NOT EXISTS " + tb_name+"(";
    vector<string> contexts;
    for(int i=0;i<words.size();i++){
        string context=words[i]+" "+types[i];
        contexts.push_back(context);
    }
    string res = All_Context(contexts);
    query = query + res + ");";
    Cmd_Add(query);
    int ret = mysql_real_query(&this->handle,query.c_str(),query.size());
    return ret;
}

uint32_t MySql::Drop_Table(string tb_name)
{
    if(tb_name.empty())
        return 0;
    mysql_ping(&this->handle);
    string query="DROP TABLE IF EXISTS "+tb_name+";";
    Cmd_Add(query);
    int ret=mysql_real_query(&this->handle,query.c_str(),query.size());
    return ret;
}

vector<vector<string>> MySql::Select(string field, string table, string condition)
{
    vector<vector<string>> results;
    if(field.empty()||table.empty()||condition.empty())
        return results;
    mysql_ping(&this->handle);
    // res.assign(0,"");
    string query = "SELECT " + field + " FROM " + table;
    if (condition != "NULL")
    {
        query += " WHERE " + condition;
    }
    query += ";";
    Cmd_Add(query); // 加入历史命令
    int ret = mysql_real_query(&this->handle, query.c_str(),query.size());
    if (ret)
        return results;
    MYSQL_RES *m_res = mysql_store_result(&this->handle);
    MYSQL_FIELD *fd; // 列名
    vector<string> names;
    while ((fd = mysql_fetch_field(m_res)) != NULL)
    {
        names.push_back(string(fd->name));
    }
    results.push_back(names);
    int field_num = mysql_num_fields(m_res); // 列数
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(m_res)) != NULL)
    {
        vector<string> result;
        for (int i = 0; i < field_num; i++)
        {
            if (row[i] == NULL)
            {
                result.push_back("NULL");
            }
            else
            {
                result.push_back(row[i]);
            }
        }
        results.push_back(result);
    }
    mysql_free_result(m_res);
    return results;
}

uint32_t MySql::Insert(string table, vector<string> &&columns, vector<string> &&values)
{
    if (columns.empty() || values.empty())
        return 0;
    mysql_ping(&this->handle);
    string col = All_Context(columns);
    string val = All_Context(values);
    string query = "INSERT INTO " + table + "(" + col + ") values (" + val + ");";
    Cmd_Add(query);
    int ret = mysql_real_query(&this->handle, query.c_str(),query.size());
    return ret;
}

uint32_t MySql::Update(string table, vector<string> &&columns, vector<string> &&values, string condition)
{
    if (columns.size() != values.size())
        return 0;
    mysql_ping(&this->handle);
    string query = "UPDATE " + table + " SET ";
    int n = columns.size();
    for (int i = 0; i < n; i++)
    {
        string part = columns[i] + "=" + values[i];
        if (i != n - 1)
            part += ",";
        query += part;
    }
    query = query + " WHERE " + condition + ";";
    Cmd_Add(query);
    int ret = mysql_real_query(&this->handle, query.c_str(),query.size());
    return ret;
}

uint32_t MySql::Delete(string table, string condition)
{
    if(table.empty()||condition.empty())
        return 0;
    mysql_ping(&this->handle);
    string query = "DELETE FROM " + table + " WHERE " + condition + ";";
    Cmd_Add(query);
    int ret = mysql_real_query(&this->handle, query.c_str(),query.size());
    return ret;
}

uint32_t MySql::Alter(string tb_name,Alter_Type type,string condition)
{
    if(tb_name.empty()||condition.empty())
        return 0;
    mysql_ping(&this->handle);
    string query="ALTER TABLE "+tb_name+" ";
    switch(type)
    {
        case ADD:
            query+="ADD "+condition+";";
            break;
        case DROP:
            query+="DROP "+condition+";";
            break;
        case CHANGE:
            query+="CHANGE "+condition+";";
            break;
    };
    Cmd_Add(query);
    int ret=mysql_real_query(&this->handle,query.c_str(),query.size());
    return ret;
}

void MySql::History(int num)
{
    span<string> cmd = Cmd_History();
    if (num == 0 || num > Cmd_Num())
    {
        for (int i = 0; i < cmd.size(); i++)
        {
            std::cout << cmd[i] << std::endl;
        }
    }
    else
    {
        for (int i = 0; i < num; i++)
        {
            std::cout << cmd[i] << std::endl;
        }
    }
}

string MySql::All_Context(vector<string> &args)
{
    string res = "";
    int n = args.size();
    for (int i = 0; i < n; i++)
    {
        res += args[i];
        if (i != n - 1)
        {
            res += ",";
        }
    }
    return res;
}