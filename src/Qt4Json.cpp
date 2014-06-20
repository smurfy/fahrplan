
#include "Qt4Json.h"
#include <QTextStream>
#include <QDebug>
#include <math.h>

QString readUntil(QTextStream& stream, QChar delim){
   QString res;
   QChar ch;
   stream >> ch;
   while (ch!=delim) {
      res+=ch;
      stream >> ch;
   }
return res;
}

char peek(QTextStream& stream){
    qint64 pos =stream.pos();
    char res;
    stream >> res;
    // jump back
    stream.seek(pos);
    return res;
}

QJsonValue QJsonValue::fromJson(QByteArray data)
{

        QTextStream source(data, QIODevice::ReadOnly);
        return fromJson(source);

}

QJsonValue QJsonValue::fromJson(   QTextStream& source)
{
    char curchar=1;
    while (!source.atEnd() && curchar){
        source.skipWhiteSpace();
        source >> curchar;
        switch(curchar){
			case ']':
			case '}':
                return QJsonValue();
			case '[': // we're dealing with an array;
			{
                QJsonArray newArr;
				
				do{
                    QJsonValue arrayMember = fromJson(source);
                    if(arrayMember.isValid())
                        newArr.push_back(arrayMember);
					else// a ']' has been read
						return newArr;
                    source.skipWhiteSpace();
                source >> curchar;
				}while(curchar== ',');
				
				if(curchar == ']')
					return newArr;
				else 
					throw "Error, falsely terminated array";
					
			}
			case '{':// we're dealing with a map. Json/ javascript calls this an object, but this is confusing, because everyghing is objects. So we'll call it a map
			{
                QJsonObject* res = new QJsonObject;
				while(1){
                    QString name;
                    source.skipWhiteSpace();
                    source >> curchar;
					if(curchar !='"')
						throw "Error, '\"' expected." ;
                    name=readUntil(source, '"');
                    source.skipWhiteSpace();
                    source >>  curchar;
					if(curchar!=':')
						throw "error, ':' expected";
					
                    QJsonValue obj(fromJson(source));
                    res->insert(name, obj);
                    source.skipWhiteSpace();
                    char nxt=peek(source);
					if(nxt == ','){
						source >> curchar;
						continue;
					}
					if(nxt == '}'){
						source>>curchar;
                        return *res;
					}
					else
						throw "error, unexpected token";
				}
			}
			case '"':
			case '\'':
			//string
			{
                QString res = readUntil(source, curchar);
                return res;
			}
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '-':
			case '+':
			//number
			{
				long intpart=0;
				bool intneg=0;
				bool expneg=0;
				intneg = (curchar=='-');
				if(curchar >'0' && curchar <= '9')
					intpart=curchar-'0';
				double fractpart=0;
				double fractMult=0.1;
				int exppart=0;

                char nxt;
				intp:
                nxt=peek(source);
				if(nxt>='0' && nxt <='9'){
					intpart=intpart*10+(nxt-'0');
					source>>curchar;
					goto intp;
				}else if(nxt == '.'){
					source >> curchar;
					goto fractp;
				}
				else if((nxt|0x20)=='e'){
					source >> curchar;
					goto expp;
				}
                return QJsonValue(intpart);// no point detected, so integer
				fractp:
                nxt=peek(source);
				if(nxt>='0' && nxt <='9'){
					fractpart+=(nxt-'0')*fractMult;
					fractMult/=10;
					source>>curchar;
					goto fractp;
				}
				else if((nxt|0x20)=='e'){
					source >> curchar;
					goto expp;
				}
				
				if(intneg)
                    return QJsonValue(0-intpart-fractpart);
				else
                    return QJsonValue(intpart + fractpart);
					
				expp:
                nxt=peek(source);
				if(nxt>='0' && nxt <='9'){
					exppart=exppart*10+(nxt-'0');
					source>>curchar;
					goto expp;
				}else if(nxt=='-'){
					expneg=true;
					source>>curchar;
					goto expp;
				}
				else if(nxt=='+'){
					source>>curchar;
					goto expp;
				}
				
				if(expneg)
					exppart=-exppart;
				if(intneg)
                    return QJsonValue((-intpart-fractpart) * pow(10.0,(double)exppart));
				else
                    return new QJsonValue(( intpart+fractpart) * 	pow(10.0,(double)exppart));
			}
			case 'T':
			case 't': //true
			{
				char check[5];
                char* n=check;
				*n=curchar;    //t
				source >> *++n;//r;
				*n |= 0x20;
				source >> *++n;//u
				*n |= 0x20;
				source >> *++n;//e
				*n |= 0x20;
				*++n=0;
				if(strcmp(check, "true"))
					throw "Error, unexpected token: " ;
                return QJsonValue(true);
			}
			case 'F':
			case 'f'://false
			{
				char check[6];
				char* n=check;
				*n=curchar;    //f
				source >> *++n;//a;
				*n |= 0x20;
				source >> *++n;//l;
				*n |= 0x20;
				source >> *++n;//s
				*n |= 0x20;
				source >> *++n;//e
				*n |= 0x20;
				*++n=0;
				if(strcmp(check, "false"))
					throw "Error, unexpected token: " ;
                return QJsonValue(false);
			}
			
			case 'N':
				curchar='n';
			case 'n':
			// null;
			{
				char check[5];
				check[0]=curchar;
				check[4]=0;
				int n=0;
				source >> check[++n];
				check[n] |= 0x20;
				source >> check[++n];
				check[n] |= 0x20;
				source >> check[++n];
				check[n] |= 0x20;
				if(strcmp("null", check))
					throw "Json error: unexpected token after n";
				else 
                    return QJsonValue();
				
			}
			  
        default:
            continue;

		}
	}
	return 0;
}


QJsonArray QJsonValue::toArray()
{
    return m_array;
}

QJsonObject QJsonValue::toObject()
{
    return m_object;
}

