#ifndef QT4JSON_H
#define QT4JSON_H
#include <QTextStream>
#include<QVariantMap>

class QJsonValue;

typedef QMap<QString, QJsonValue> QJsonObject;
typedef QList<QJsonValue> QJsonArray;
//typedef QJsonValue QJsonDocuddment;
#define QJsonDocument QJsonValue

class QJsonValue: public QVariant{
   QJsonObject m_object;
   QJsonArray m_array;

public:
    enum {
        Array=UserType+1,
        Object=UserType+2
    };
   QJsonArray toArray();
   QJsonObject toObject();

   QJsonObject object(){
       return toObject();
   }

   bool isArray()
   {
       return type()==Array;
   }
   bool isObject()
   {
       return type()==Object;
   }


   QJsonValue(const QJsonValue& other):
       QVariant(other),
       m_array(other.m_array),
       m_object(other.m_object)
   {

   }

   QJsonValue(QJsonObject &obj):
       QVariant(Object),
       m_object(obj)
   {


   }

   QJsonValue(bool val):
   QVariant(val){

   }
   QJsonValue(double val):
   QVariant(val){

   }
   QJsonValue(int val):
   QVariant(val){

   }

   QJsonValue(long int val):
   QVariant((qlonglong) val){

   }
   QJsonValue(QString str):
   QVariant(str){

   }


   QJsonValue(QJsonValue& other):
       QVariant(other),
       m_array(other.m_array),
       m_object(other.m_object)
   {

   }

   QJsonValue(){

   }

   QJsonValue(QJsonArray& arr):
       QVariant(Array),
       m_array(arr)
   {

   }


    static QJsonValue fromJson(QByteArray data);
    static QJsonValue fromJson(QTextStream &data);



};

/*
class QJsonDocument: public QJsonValue{

    QJsonObject m_object;
public:

    QJsonDocument(QJsonValue val):
        QJsonValue( val){

    }

    static QJsonDocument fromJson(QByteArray data);
    static QJsonDocument fromJson(QTextStream &data);


    QJsonObject object(){
        return m_object;
    }




};

//*/
#endif // QT4JSON_H
