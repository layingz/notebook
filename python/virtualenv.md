**1、通过pip安装virtualenv:**
  
    pip install virtualenv
 
**2、virtualenv命令解析：**
  
    --system-site-packages  链接到系统的包

    --always-copy 完全拷贝库文件而不是链接

    --relocatable 移动虚拟环境重定位bin文件位置，体现在配置文件的第一行上

    --no-setuptools --no-pip  --no-wheel 不把系统自带的三个软件安装到虚拟环境

    --no-download 不从pypi上提前下载第三方包

    --download  从pypi提前下载第三方包
  
**3、简单使用：**
  
    创建虚拟环境-p是对应python的bin目录
    
    virtualenv -p /usr/bin/python2.7 my_project_env
    
    加载虚拟环境的env，进去虚拟环境
    
    source my_project_env/bin/activate
    
    退出虚拟环境
    
    deactivate
    
    从txt文件读取第三方包名字和版本，并从pip下载
  
    pip install -r requirements.txt
    
**4、关于virtualenvwrapper**
  
    只是简单对virtualenv进行简单封装，并没有很大用途，也需要激活配置文件， 使用上没更简单，反而多了一层，其作用主要体现在以下几个命令：
    
    lsvirtualenv    #列举所有的环境。

    cdvirtualenv    #导航到当前激活的虚拟环境的目录中，比如说这样您就能够浏览它的 site-packages。

    cdsitepackages   # 和上面的类似，但是是直接进入到 site-packages 目录中。

    lssitepackages     #显示 site-packages 目录中的内容。
  
