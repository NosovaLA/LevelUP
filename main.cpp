#include <QFile>
#include <QTextStream>
#include <QDebug>


class ReadWriteFile {
public:
    bool ReadFile (const QString &NameFile){ //ф-ия чтения текстового файла
        QFile file (NameFile);               // создание объекта
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) //проверка
            return false;

        QTextStream in(&file); // создание текстового потока
        in.setCodec("UTF-8");  // кодировка

        while (!in.atEnd())   // чтение из файла
        {
            QString line = in.readLine();
            qDebug()<<line;
        }
        file.close(); // закрытие файла
        return true;
    } // конец ф-ии чтения

    bool WrireFile (const QString &NameFile,const QString &StringLine){ //ф-ия записи текстового файла
        QFile file (NameFile);  // создание объекта
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) //проверка
            return false;

        QTextStream out(&file); // создание текстового потока
        out.setCodec("UTF-8");  // кодировка

        out << StringLine << endl; // запись
        file.close(); // закрытие файла
        return true;
    } // конец ф-ии записи
};


int main() {
    ReadWriteFile txt;
    txt.WrireFile("Test.txt","Hello world");
    txt.ReadFile("Test.txt");
    return 0;
};


