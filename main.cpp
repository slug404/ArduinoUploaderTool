#include <QCoreApplication>
#include <QDebug>

/*! the parameter number pass by command line*/
const int PARAMER_COUNT = 1+3;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(PARAMER_COUNT == argc)
    {
        //accept
		QString fileName = argv[1];
		QString board = argv[2];
		QString serialPort = argv[3];
    }
    else
    {
        qDebug() << "error";
    }

    return a.exec();
}
