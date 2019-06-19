搭建需求：在与外网隔绝的局域网下，多台设备需要使用python第三方包或者内部项目包

[git项目地址](https://github.com/pypiserver/pypiserver)

环境centos7+python2.7

1、安装pypiserver

    pip install pypiserver

2、密码设置-用于代码上传

    way1：关闭密码保护  pypi-server -P . -a .

    way2：使用密码
        1、安装必要组件
        pip install htpasswd 
        pip install passlib
        2、生成使用密码和添加密码文件
        htpasswd -c /root/.pypipasswd user101
        pypi-server -P ~/.pypipasswd
        3、修改配置~/.pypirc
        [distutils]
        index-servers = localhost
        
        [localhost]
        repository: http://localhost:9090
        username: user101
        password: 123456
        4、上传代码
        python setup.py sdist upload -r localhost

3、配置nginx

4、启动pypiserver

    pypi-server -p 9090 -P /opt/pypiserver/packages

5、其他端使用源

    在~/.pip/pip.conf配置文件下添加
        [global]
        extra-index-url = http://localhost:9090
    or
    pip install  --extra-index-url = http://localhost:9090

6、pip使用

    1、导出当前环境的pip安装包：
    pip freeze > requirements.txt
    2、新环境一键导入pip包：
    pip install -r requirement.txt
    





