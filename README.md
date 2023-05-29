### 编译环境
cmake 3.0版本以上

g++ 11.0以上

mysql C API

### 编译
mkdir build

cd build

cmake ..
### 注意事项
可执行文件在build目录下，注意文件的相对路径

### 接口
```c++
template<typename ...Str>
static inline vector<string> Arg_List(Str...colmuns)
利用可变参数和模板组织Query的参数，与多个Query函数连用

uint32_t Create_Table(string query)
参数是CREATE的SQL命令
```
#### query生成接口
```c++
string Create_Query(string tb_name,vector<string>&& words,vector<string>&& types)
tb_name为表名，words和types参数可利用Arg_List静态成员函数生成
```
例：mysql.Create_Table(mysql.Create_Query("manager",MySql::Arg_List("name","age"),MySql::Arg_List("varchar(10) primary key","tinyint")))
```c++
string Drop_Query(string tb_name)
参数为需要删除的表名
```
```c++
string Select_Query(vector<string>&& field,string table,string condition="NULL")
filed为列名，可以通过Arg_List生成，table是表名，condition是条件，默认为NULL
```
```c++
string Insert_Query(string tb_name,vector<string> &&fields , vector<string> &&values);
tb_name为表名，fields为列名，values为值
```
```c++
string Update_Query(string tb_name,vector<string> &&fields,vector<string> &&values,string condition);
tb_name为表名，fields为列名，values为值,condition为条件
```
```c++
string Delete_Query(string tb_name,string condition);
tb_name为表名，condition为条件
```
```c++
string Alter_Query(string tb_name,Alter_Type type,string condition);
tb_name为表名，type为Alter的操作，condition为条件
```
#### 功能接口
参数为query，使用对应的query方法生成

