目录结构:
-----------------------------------------------------------------------
	|----------team_10_report.pdf
	|----------README.txt
	|----------Makefile
	|----------bin
		|-------------- app (目标执行程序)
		|-------------- catalog (用来存catalog data，可能不存在)
		|-------------- serilization.data (用来存数据，可能不存在)
	|----------obj
		|-------------- app.o
		|-------------- catalog.o
	|----------src
		|-------------- app.cpp
		|-------------- buffer.h
		|-------------- catalog.cpp
		|-------------- catalog.h
		|-------------- check.h
		|-------------- extract.h
		|-------------- find.h
		|-------------- insert.h
		|-------------- record_data.h
		|-------------- recovery_data.h
		|-------------- show_record.h
		|-------------- vector.h
	|----------temp
		|-------------- nobench_data.json
		|-------------- find_fast_show.h(可替换掉src/find.h,替换后find查询输出一次性)
		|-------------- find_slow_show.h(可替换掉src/find.h,替换后find查询时与用户交互输出)

注:
	因为打包文件里面的bin目录下已经存放了我们之前实验的数据,
所以如果想重新从头开始,执行如下命令(先进入到打包的目录文件):
	~$ rm -rf bin/* && make
	~$ cd bin/ && ./app
或者直接执行：
    ~$ make clean && make
    ~$ cd bin/ && ./app

命令使用说明:
1.首先是帮助命令,直接在提示符'>> '后面输入'help'或者'?',就会出现跟刚登录时出现的那个命令说明命令;例如:
	>> help
	>> ?

2.在temp目录中有可以导入的json数据,运行app后可使用insert命令把数据导入;例如:
	>> insert ../temp/nobench_data.json

3.然后可以使用check catalog来查看此时catalog是否已经生成,例如:
	>> check catalog

4.再使用showJson命令可以复原为Json数据进行查看,例如:
	>> showJson

5.如果想要根据某些条件来查找数据,就使用find命令,例如:
	>> find dyn2 = true
	>> find dyn1 = 46608
	>> find nested_obj.num = 8220
	>> find bool = false
	>> find bool = "true"
	>> find str1 = "GBRDCMBRGAYTAMJQGEYTCMJRGAYTCMI="

6.内嵌有调用简单的不带参数的系统命令,例如:
查看当前目录下的文件:
	>> ls

清屏:
	>> clear

查看时间:
	>> date
