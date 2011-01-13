/***************************************************************************
* This file is part of libmygpo-qt                                         *
* Copyright (c) 2010 - 2011 Stefan Derkits <stefan@derkits.at>             *
* Copyright (c) 2010 - 2011 Christian Wagner <christian.wagner86@gmx.at>   *
* Copyright (c) 2010 - 2011 Felix Winter <ixos01@gmail.com>                *
*                                                                          *
* This library is free software; you can redistribute it and/or            *
* modify it under the terms of the GNU Lesser General Public               *
* License as published by the Free Software Foundation; either             *
* version 2.1 of the License, or (at your option) any later version.       *
*                                                                          *
* This library is distributed in the hope that it will be useful,          *
* but WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU        *
* Lesser General Public License for more details.                          *
*                                                                          *
* You should have received a copy of the GNU Lesser General Public         *
* License along with this library; if not, write to the Free Software      *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 *
* USA                                                                      *
***************************************************************************/

#include <parser.h>
#include <QSharedPointer>

#include "TagList.h"

namespace mygpo
{

class TagListPrivate : public QObject
{
    Q_OBJECT

public:
    TagListPrivate( TagList* qq, QNetworkReply* reply );
    virtual ~TagListPrivate();
    QList<TagPtr> list() const;
    QVariant tags() const;
private:
    TagList* const q;
    QNetworkReply* m_reply;
    QVariant m_tags;

    QNetworkReply::NetworkError m_error;

    bool parse( const QVariant& data );
    bool parse( const QByteArray& data );
private slots:
    void parseData();
    void error( QNetworkReply::NetworkError error );
};

}

using namespace mygpo;

TagListPrivate::TagListPrivate( TagList* qq, QNetworkReply* reply ) : q( qq ), m_reply( reply ), m_tags( QVariant() ), m_error( QNetworkReply::NoError )
{
    QObject::connect( m_reply, SIGNAL( finished() ), this, SLOT( parseData() ) );
    QObject::connect( m_reply, SIGNAL( error( QNetworkReply::NetworkError ) ), this, SLOT( error( QNetworkReply::NetworkError ) ) );
}

TagListPrivate::~TagListPrivate()
{
    delete m_reply;
}

QList<TagPtr> TagListPrivate::list() const
{
    QList<TagPtr> list;
    QVariantList varList = m_tags.toList();
    foreach( QVariant var, varList )
    {
        list.append( var.value<mygpo::TagPtr>() );
    }
    return list;
}

QVariant TagListPrivate::tags() const
{
    return m_tags;
}

bool TagListPrivate::parse( const QVariant& data )
{
    if( !data.canConvert( QVariant::List ) )
        return false;
    QVariantList varList = data.toList();
    QVariantList tagList;
    foreach( QVariant var, varList )
    {
        QVariant v;
        v.setValue<mygpo::TagPtr>( TagPtr( new Tag( var ) ) );
        tagList.append( v );
    }
    m_tags = QVariant( tagList );
    return true;
}

bool TagListPrivate::parse( const QByteArray& data )
{
    QJson::Parser parser;
    bool ok;
    QVariant variant = parser.parse( data, &ok );
    if( ok )
    {
        ok = ( parse( variant ) );
    }
    return ok;
}


void TagListPrivate::parseData()
{
    if( m_reply->error() == QNetworkReply::NoError )
    {
        if( parse( m_reply->readAll() ) )
        {
            emit q->finished();
        }
        else
        {
            emit q->parseError();
        }
    }
}

void TagListPrivate::error( QNetworkReply::NetworkError error )
{
    this->m_error = error;
    emit q->requestError( error );
}

TagList::TagList( QNetworkReply* reply, QObject* parent ) : QObject( parent ), d( new TagListPrivate( this, reply ) )
{

}

TagList::~TagList()
{
    delete d;
}

QList<TagPtr> TagList::list() const
{
    return d->list();
}

QVariant TagList::tags() const
{
    return d->tags();
}

#include "TagList.moc"
