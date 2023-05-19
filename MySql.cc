#include "MySql.h"
MySql::MySql()
{
    if (mysql_init(&this->handle) == NULL)
    {
        exit(0);
    }
}

inline uint32_t MySql::Connect(string remote, string usrname, string passwd, string db_name)
{
    if (!mysql_real_connect(&this->handle, remote.c_str(), usrname.c_str(), passwd.c_str(), db_name.c_str(), 3306, NULL, 0))
    {
        return mysql_errno(&this->handle);
    }
    return 0;
}

inline vector<vector<string>> MySql::Select(string field, string table, string condition)
{
    vector<vector<string>> results;
    // res.assign(0,"");
    string query = "SELECT " + field + " FROM " + table;
    if (condition != "NULL")
    {
        query += " WHERE " + condition;
    }
    query += ";";
    Cmd_Add(query); // 加入历史命令
    int ret = mysql_query(&this->handle, query.c_str());
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

int inline MySql::Insert(string table, vector<string> &&columns, vector<string> &&values)
{
    if (columns.size() == 0 || values.size() == 0)
        return 0;
    string col = All_Context(columns);
    string val = All_Context(values);
    string query = "INSERT INTO " + table + "(" + col + ") values (" + val + ");";
    Cmd_Add(query);
    int ret = mysql_query(&this->handle, query.c_str());
    return ret;
}

int inline MySql::Update(string table, vector<string> &&columns, vector<string> &&values, string condition)
{
    if (columns.size() != values.size())
        return 0;
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
    int ret = mysql_query(&this->handle, query.c_str());
    return ret;
}

int inline MySql::Delete(string table, string condition)
{
    string query = "DELETE FROM " + table + " WHERE " + condition + ";";
    Cmd_Add(query);
    int ret = mysql_query(&this->handle, query.c_str());
    return ret;
}

void inline MySql::History(int num)
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

string inline MySql::All_Context(vector<string> &args)
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