#include <QCoreApplication>
#include <QDebug>

/*! the parameter number pass by command line*/
const int PARAMER_COUNT = 4+1;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(PARAMER_COUNT == argc)
    {
        //accept
        qDebug() << "accept";
    }
    else
    {
        qDebug() << "error";
    }

    return a.exec();
}
