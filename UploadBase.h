#ifndef UPLOADBASE_H
#define UPLOADBASE_H

#include <QObject>
#include <QSet>
#include <QMap>
#include <QProcess>

/**
 * @brief 库引用信息
 */
struct LibraryReferenceInfor
{
	QString libName; /**< 库的名字(aa.h) */
	QString libPath;/**< 库所在路径(arduino/libraries/aa) */
	QSet<QString> libReference;/**< 该库所包含的其他库 */
};

/**
 * @brief The Board struct 开发版的基本信息
 */
struct Board
{
	Board(){}

	Board(const QString &name, const QString &mcu, const QString &type, const QString baud, int size)
		: fullName(name)
		, mcu(mcu)
		, mcuType(type)
		, baudrate(baud)
		, flashSIze(size)
	{
	}

	QString fullName;/**< 开发版的全名 */
	QString mcu;/**< mcu的名字 */
	QString mcuType;/**< mcu的类型,主要针对leonardo的特殊处理 */
	QString baudrate;/**< 波特率 */
	int flashSIze;/**< flash的大小,用于可能要处理的超出大小的判断 */
};

/*!
 * \brief Uploader的基类
 */
class UploadBase : public QObject
{
	Q_OBJECT
public:
	virtual ~UploadBase();
/*!
* \brief 错误类型
*/
	enum UploaderError
	{
		SetupError,
		CompileError,
		WriteProError,
		ClearError
	};
	void start();

signals:
	void signalSetupStart();
	void signalCompileStart();
	void signalWriteProStart();
	void signalClearStart();

	void signalSetupError();
	void signalCompileError(const QString &errorString);
	void signalWriteProError();
	void signalClearError();

	void signalSetupEnd();
	void signalCompileEnd();
	void signalWriteProEnd();
	void signalClearEnd();

	void signalError(UploaderError errorType);
	void signalCompileProgress(float current, float total, int sec);
	void signalWriteProProgress(float current, float total, int sec);

	///////////////////////////////////
	void signalTypeConversionError(const QString &error);
	void signalPortError(const QString &error);
	void signalBoardError(const QString &error);

public slots:
	void slotReadyReadStandardOutput();
	void slotreadyReadStandardError();

protected:
	//fucntion
	explicit UploadBase(const QString &codePath, const QString &serial, int boardIndex, QObject *parent = 0);

	//扫描库相关
	void scanAllLibraryHeaderFile(const QString &libraryPath);
	void scanAllheaderFile(const QString &path);
	//在这里得到所有的依赖
	QSet<QString> getAllReferenceHeaderFileSet(const QString &filePath);
	QSet<QString> getReferenceHeaderFilesFromSingleFile(const QString &filePath);
	//正则匹配抽取一个文本中#include的头文件, 这里正则匹配还可以改善
	QSet<QString> getAllMatchResults(const QString text, const QString regexp = "\\w+\\.h");

	//递归编译指定目录以及其子目录中所有*c,*cpp
	void compileLibrary(const QString &libraryDirPath, const QString &mcu);

	//给QProcess调用
	//编译
	QString getCompilerCommand(const QString &sketchPath, const QString &cpuType, const QList<QString> &libPaths, QString workPath = "./Temp", QString workingFrequency = "16000000");
	//链接
	void linkerCommand(const QString &filePath, const QString &cpuType, const QString &staticLibraryPath, QString workPath = "./Temp", QString workingFrequency = "16000000");
	QString create_elf_fileCommand(const QString &filePath, const QString &cpuType, const QString &staticLibraryPath, QString workPath = "./Temp", QString workingFrequency = "16000000");
	QString create_eep_fileCommand(const QString &toolPath, const QString &elfPath, const QString &eepPath);
	QString create_hex_fileCommand(const QString &toolPath, const QString &elfPath, const QString &hexPath);

	//上传
	QString getUploadCommand(const QString &avrdudePath, const QString &configPath, const QString &cpuType, const QString &serialPort, const QString &baudrate, const QString &hexPath);
	QSet<QString> getAllChildDirPath(const QString &parentDirPath);

	//复制库相关,为了防止有同名文件导致的覆盖问题.目前暂时不使用
	void copyDirectory(const QString &srcPath, const QString &desPath);
	bool copyFile(const QString &srcPath, const QString &desPath);

	Q_DISABLE_COPY(UploadBase)

	//interface
	virtual void prepare(); //! 准备
	virtual void compile(); //! 编译
	virtual void writePro() = 0;//! 烧写
	virtual void clear() = 0; //! 清理
	virtual void readStandardOutput() = 0;
	virtual void readStandardError() = 0;

protected:
	//data
	QProcess *pExternalProcess_;//! 调用的外部程序指针
	QString serialPort_;//! 串口号
	int boardIndex_;//! 板子类型
	QString compilerPath_;
	QString codePath_;
	QString cmd_;
	QSet<QString> alreadyCompile_;
	QSet<QString> libraryPaths_;
	QMap<QString, LibraryReferenceInfor> map_libName_infor_;
	QMultiMap<QString, QString> map_headerFile_path_;
	QMap<int, Board> map_boardIndex_infor_;
};

#endif // UPLOADBASE_H
