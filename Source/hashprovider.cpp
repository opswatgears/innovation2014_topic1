#include "hashprovider.h"

HashProvider::HashProvider(MoFile* file)
{
    mofile = file;
}


void HashProvider::run()
{
    QByteArray buffer;
    const int buff_size = 65536;
    QCryptographicHash md5_hash(QCryptographicHash::Md5);
    QCryptographicHash sha1_hash(QCryptographicHash::Sha1);
    QCryptographicHash sha256_hash(QCryptographicHash::Sha256);

    // qDebug() << "HashProvider::run thread " << QThread::currentThread()->objectName().toStdString().c_str();

    buffer.resize(buff_size);

    if  (!mofile) {
        return;
    }

    if(!mofile->open(QIODevice::ReadOnly)) {
        // error opening
        // qDebug() << "CalcFileHashesTask Error opening file " << mofile->errorString().toStdString().c_str();
        return;
    }


    QCryptographicHash cryptoHash(QCryptographicHash::Md5);
    while(!mofile->atEnd()){
        buffer = mofile->read(buff_size);
        md5_hash.addData(buffer);
        sha1_hash.addData(buffer);
        sha256_hash.addData(buffer);
    }


    mofile->setMd5(md5_hash.result());
    mofile->setSha1(sha1_hash.result());
    mofile->setSha256(sha256_hash.result());

    mofile->getLbMD5()->setText(md5_hash.result().toHex().toUpper());

    qDebug() << "md5 = " << md5_hash.result().toHex().toUpper();
    qDebug() << "sha1 = " << sha1_hash.result().toHex().toUpper();
    qDebug() << "sha256 = " << sha256_hash.result().toHex().toUpper();

    mofile->close();
}
