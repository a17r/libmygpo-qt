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

#include <QVariant>

#include "parser.h"

#include "JsonParser.h"

using namespace mygpo;

QList< Podcast > JsonParser::toPodcastList(const QByteArray& jsonData)
{
    QJson::Parser parser;
    QList<QVariant>  podcastVarList = parser.parse(jsonData).toList();
    QList<Podcast> podcastList;
    foreach (QVariant podcastVar, podcastVarList)
    {
        podcastList.append(qvariantToPodcast(podcastVar));
    }
    return podcastList;
}

Podcast JsonParser::toPodcast(const QByteArray& jsonData)
{
    QJson::Parser parser;
    QVariant podcastVar = parser.parse(jsonData);
    return qvariantToPodcast(podcastVar);
}


QList<Episode> JsonParser::toEpisodeList(const QByteArray& jsonData)
{
    QJson::Parser parser;
    QList<QVariant>  episodeVarList = parser.parse(jsonData).toList();
    QList<Episode> episodeList;
    foreach (QVariant episodeVar, episodeVarList)
    {
        episodeList.append(qvariantToEpisode(episodeVar));
    }
    return episodeList;
}

Episode JsonParser::toEpisode(const QByteArray& jsonData)
{
    QJson::Parser parser;
    QVariant episodeVar = parser.parse(jsonData);
    return qvariantToEpisode(episodeVar);
}

QList< Tag > JsonParser::toTagList(const QByteArray& jsonData)
{
    QJson::Parser parser;
    QList<QVariant>  tagVarList = parser.parse(jsonData).toList();
    QList<Tag> tagList;
    foreach (QVariant tagVar, tagVarList)
    {
        tagList.append(qvariantToTag(tagVar));
    }
    return tagList;
}


Podcast JsonParser::qvariantToPodcast(QVariant& variantData)
{
    QVariantMap podcastMap = variantData.toMap();
    QUrl url = podcastMap.value(QLatin1String("url")).toUrl();
    QString title = podcastMap.value(QLatin1String("title")).toString();
    QString description = podcastMap.value(QLatin1String("description")).toString();
    uint subscribers = podcastMap.value(QLatin1String("subscribers")).toUInt();
    QUrl logoUrl = podcastMap.value(QLatin1String("logo_url")).toUrl();
    QUrl website = podcastMap.value(QLatin1String("website")).toUrl();
    QUrl mygpoLink = podcastMap.value(QLatin1String("mygpo_link")).toUrl();
    mygpo::Podcast podcast(url,title,description,subscribers,logoUrl,website,mygpoLink);
    return podcast;
}


Episode JsonParser::qvariantToEpisode(QVariant& variantData)
{
    QVariantMap episodeMap = variantData.toMap();
    QUrl url = episodeMap.value(QLatin1String("url")).toUrl();
    QString title = episodeMap.value(QLatin1String("title")).toString();
    QUrl podcastUrl = episodeMap.value(QLatin1String("podcast_url")).toUrl();
    QString podcastTitle = episodeMap.value(QLatin1String("podcast_title")).toString();
    QString description = episodeMap.value(QLatin1String("description")).toString();
    QUrl website = episodeMap.value(QLatin1String("website")).toUrl();
    QUrl mygpoLink = episodeMap.value(QLatin1String("mygpo_link")).toUrl();
    mygpo::Episode episode(url,title,podcastUrl,podcastTitle,description,website,mygpoLink);
    return episode;
}

Tag JsonParser::qvariantToTag(QVariant& variantData)
{
    QVariantMap tagMap = variantData.toMap();
    QString tagString = tagMap.value(QLatin1String("tag")).toString();
    uint usage = tagMap.value(QLatin1String("usage")).toUInt();
    mygpo::Tag tag(tagString,usage);
    return tag;
}

QByteArray JsonParser::addRemoveSubsToJSON(const QList< QUrl >& add, const QList< QUrl >& remove)
{
    QString jsonData = QString(QLatin1String("{\"add\":[%1],\"remove\":[%2]}")).arg(urlListToString(add)).arg(urlListToString(remove));
    QByteArray jsonByteArray;
    jsonByteArray.append(jsonData.toAscii());
    return jsonByteArray;
}


QString JsonParser::urlListToString(const QList< QUrl >& urls)
{
    QString urlsString;
    int max = urls.size();
    for (int i=0;i<max;i++)
    {
        urlsString.append(QString(QLatin1String("\"%1\"")).arg(urls.at(i).toString()));
        if (i!=(max-1))
        {
            urlsString.append(QLatin1String(","));
        }
    }
    return urlsString;
}

AddRemoveResult JsonParser::toAddRemoveResult(const QByteArray& jsonData)
{
    QJson::Parser parser;
    QVariantMap resultMap = parser.parse(jsonData).toMap();
    qulonglong timestamp = resultMap.value(QLatin1String("timestamp")).toULongLong();
    QVariant updateVar = resultMap.value(QLatin1String("update_urls"));
    AddRemoveResult result(timestamp,updateVar);
    return result;
}

QList< QPair< QUrl, QUrl > > JsonParser::toUrlPairList(const QVariant& variantData)
{
    QVariantList updateVarList = variantData.toList();
    QList<QPair<QUrl, QUrl > > updateUrls;
    foreach (QVariant urlVar, updateVarList)
    {
        updateUrls.append(toUpdatePair(urlVar));
    }
    return updateUrls;
}


QPair< QUrl, QUrl > mygpo::JsonParser::toUpdatePair(QVariant& variantData)
{
    QVariantList urlList = variantData.toList();
    QUrl first = QUrl(urlList.at(0).toString());
    QUrl second = QUrl(urlList.at(1).toString());
    return qMakePair(first,second);
}
