#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QVariantMap>

class QTcpSocket;

class NetWork : public QObject
{
	Q_OBJECT
public:
	explicit NetWork(QObject *parent = 0);

	/**
	 * @brief getGrammaticalError 语法错误(编译错误)
	 * @param str
	 */
	QVariantMap getGrammaticalError(const QString &str);

	/**
	 * @brief getBoradError 板子类型错误(不一定准确)
	 * @param str
	 */
	QVariantMap getBoradError(const QString &str);

	/**
	 * @brief getSerialPortError 串口号错误
	 * @param str
	 */
	QVariantMap getSerialPortError(const QString &str);

	/**
	 * @brief getCompileSuccess 编译成功
	 * @param str
	 */
	QVariantMap getCompileSuccess(const QString &str);

	/**
	 * @brief getUploaderSuccess 上传成功
	 * @param str
	 */
	QVariantMap getUploaderSuccess(const QString &str);

	/**
	 * @brief getOtherInfor 其他信息
	 * @param str
	 */
	QVariantMap getOtherInfor(const QString &str);

private:
	void initData();

private:
	QTcpSocket *pTcpSocket_;
};

#endif // NETWORK_H
