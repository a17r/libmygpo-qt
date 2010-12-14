/***************************************************************************
* This file is part of libmygpo-qt                                         *
* Copyright (c) 2010 Stefan Derkits <stefan@derkits.at>                    *
* Copyright (c) 2010 Christian Wagner <christian.wagner86@gmx.at>          *
* Copyright (c) 2010 Felix Winter <ixos01@gmail.com>                       *
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

#ifndef ADDREMOVERESULT_H
#define ADDREMOVERESULT_H

#include <QObject>
#include <QList>
#include <QUrl>
#include <QVariant>
#include <QNetworkReply>

#include "mygpo_export.h"

namespace mygpo
{
class AddRemoveResultPrivate;
class MYGPO_EXPORT AddRemoveResult : public QObject
{
    Q_OBJECT
    Q_PROPERTY ( qulonglong timestamp READ timestamp CONSTANT )
    Q_PROPERTY ( QVariant updateUrls READ updateUrls CONSTANT )
public:
    AddRemoveResult ( QNetworkReply* reply ,QObject* parent = 0 );
    AddRemoveResult ( qulonglong timestamp, const QVariant& updateUrls ,QObject* parent = 0 );
    AddRemoveResult ( const AddRemoveResult& other );
    AddRemoveResult ( );
    virtual ~AddRemoveResult();
    AddRemoveResult operator= ( const AddRemoveResult& other );
    QVariant updateUrls() const;
    qulonglong timestamp() const;
    QList<QPair<QUrl, QUrl> > updateUrlsList() const;
private:
    AddRemoveResultPrivate* const d;
    friend class AddRemoveResultPrivate;
signals:
    /**Gets emitted when the data is ready to read*/
    void finished();
    /**Gets emitted when an parse error ocurred*/
    void parseError();
    /**Gets emitted when an request error ocurred*/
    void requestError ( QNetworkReply::NetworkError error );
};

}

Q_DECLARE_METATYPE ( mygpo::AddRemoveResult );

#endif // ADDREMOVERESULT_H
