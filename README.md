# DatabaseSystem
## 数据库的project作业
根据Sinew论文格式，实现下面三个功能：

1.insert filename    
  把filename文件的json内容，转换成论文要求的混合模型格式，并以8k大小一个页面存储
  
2.check catalog    
  显示catalog文件的内容（以表格形式）
  
3.find A = value   
  A为键名，value为键值，找出所有包含A = value的json数据并显示

## 下载使用
git clone https://github.com/zhushh/DatabaseSystem

cd DatabaseSystem/ && make

cd ./bin && ./app

### 说明
1.首先是帮助命令,直接在提示符'>> '后面输入'help'或者'?',就会出现跟刚登录时出现的那个命令说明命令;例如:

<pre>>> help</pre>

<pre>>> ?</pre>

2.在temp目录中有可以导入的json数据,运行app后可使用insert命令把数据导入;例如:

<pre>>> insert ../temp/nobench_data.json</pre>

3.然后可以使用check catalog来查看此时catalog是否已经生成,例如:

<pre>>> check catalog</pre>

4.再使用showJson命令可以查看复原为Json的数据,例如:

<pre>>> showJson</pre>

5.如果想要根据某些条件来查找数据,就使用find命令,例如:

<pre>>> find dyn2 = true</pre>

<pre>>> find dyn1 = 46608</pre>

<pre>>> find nested_obj.num = 8220</pre>

<pre>>> find bool = false</pre>

<pre>>> find bool = "true"</pre>

<pre>>> find str1 = "GBRDCMBRGAYTAMJQGEYTCMJRGAYTCMI="</pre>

6.内嵌有调用简单的不带参数的系统命令,例如:

<pre>>> ls</pre>

<pre>>> date</pre>
