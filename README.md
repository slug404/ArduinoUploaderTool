ArduinoUploaderTool
===================
这是一个支持多平台的Arduino命令行编译上传工具.目前用于Arduino可视化集成开发环境开发环境Mind+中.

这工具有什么用处?
    Arduino官方IDE太慢太烂了,如果有人希望自己写一个Arduino的IDE或者仅仅是写一个Arduino的代码编辑器.这时候可以使用本工具对代码进行编译和上传,最为重要的是此工具是跨平台的(基于Qt,将来会完全使用IOS C++不依赖于其他框架).使用的时候仅仅需要传递要编译上传的文件名以及Arduino开发板的类型还有串口号通过命令行传入即可.
  
这东西怎么使用?  
    很简单,仅仅需要从命令行传入一些参数,参数命令如下:
    ArduinoUploader 文件名 开发板类型 串口号
    文件名:这里是带后缀的,而且后缀必须是.cpp
    开发板类型:如UNO, Leonardo各种,与官方所支持的一致
    串口号:Windows下就是是COMx这种, MacOS和Linux就是/dev里面的
    
这东西目前有什么问题?
    嗯目前发现比较大的问题便是MacOS下Leonardo的支持问题.这个问题计划在将来一段事件之后解决.
    
该工具的构建参考了http://www.arduino.cc/en/Hacking/BuildProcess, Arduino官方IDE的源码以及stanley前辈写的Arduino命令行上传工具(https://github.com/stanleyhuangyc/ArduinoUploader)
    
如有疑问可以随时给我发邮件与我取得联系
slug404@hotmail.com
