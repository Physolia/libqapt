/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -c OrgKubuntuQaptworkerTransactionInterface -p transactiondbus.h:transactiondbus.cpp -i downloadprogress.h worker/org.kubuntu.qaptworker.transaction.xml
 *
 * qdbusxml2cpp is Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef TRANSACTIONDBUS_H_1347824719
#define TRANSACTIONDBUS_H_1347824719

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
#include "downloadprogress.h"

/*
 * Proxy class for interface org.kubuntu.qaptworker.transaction
 */
class OrgKubuntuQaptworkerTransactionInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.kubuntu.qaptworker.transaction"; }

public:
    OrgKubuntuQaptworkerTransactionInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~OrgKubuntuQaptworkerTransactionInterface();

    Q_PROPERTY(QString debconfPipe READ debconfPipe)
    inline QString debconfPipe() const
    { return qvariant_cast< QString >(property("debconfPipe")); }

    Q_PROPERTY(qulonglong downloadETA READ downloadETA)
    inline qulonglong downloadETA() const
    { return qvariant_cast< qulonglong >(property("downloadETA")); }

    Q_PROPERTY(QApt::DownloadProgress downloadProgress READ downloadProgress)
    inline QApt::DownloadProgress downloadProgress() const
    { return qvariant_cast< QApt::DownloadProgress >(property("downloadProgress")); }

    Q_PROPERTY(qulonglong downloadSpeed READ downloadSpeed)
    inline qulonglong downloadSpeed() const
    { return qvariant_cast< qulonglong >(property("downloadSpeed")); }

    Q_PROPERTY(int error READ error)
    inline int error() const
    { return qvariant_cast< int >(property("error")); }

    Q_PROPERTY(int exitStatus READ exitStatus)
    inline int exitStatus() const
    { return qvariant_cast< int >(property("exitStatus")); }

    Q_PROPERTY(bool isCancellable READ isCancellable)
    inline bool isCancellable() const
    { return qvariant_cast< bool >(property("isCancellable")); }

    Q_PROPERTY(bool isCancelled READ isCancelled)
    inline bool isCancelled() const
    { return qvariant_cast< bool >(property("isCancelled")); }

    Q_PROPERTY(bool isPaused READ isPaused)
    inline bool isPaused() const
    { return qvariant_cast< bool >(property("isPaused")); }

    Q_PROPERTY(QString locale READ locale)
    inline QString locale() const
    { return qvariant_cast< QString >(property("locale")); }

    Q_PROPERTY(QVariantMap packages READ packages)
    inline QVariantMap packages() const
    { return qvariant_cast< QVariantMap >(property("packages")); }

    Q_PROPERTY(int progress READ progress)
    inline int progress() const
    { return qvariant_cast< int >(property("progress")); }

    Q_PROPERTY(QString proxy READ proxy)
    inline QString proxy() const
    { return qvariant_cast< QString >(property("proxy")); }

    Q_PROPERTY(int role READ role)
    inline int role() const
    { return qvariant_cast< int >(property("role")); }

    Q_PROPERTY(int status READ status)
    inline int status() const
    { return qvariant_cast< int >(property("status")); }

    Q_PROPERTY(QString statusDetails READ statusDetails)
    inline QString statusDetails() const
    { return qvariant_cast< QString >(property("statusDetails")); }

    Q_PROPERTY(QString transactionId READ transactionId)
    inline QString transactionId() const
    { return qvariant_cast< QString >(property("transactionId")); }

    Q_PROPERTY(QStringList untrustedPackages READ untrustedPackages)
    inline QStringList untrustedPackages() const
    { return qvariant_cast< QStringList >(property("untrustedPackages")); }

    Q_PROPERTY(int userId READ userId)
    inline int userId() const
    { return qvariant_cast< int >(property("userId")); }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> cancel()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("cancel"), argumentList);
    }

    inline QDBusPendingReply<> provideMedium(const QString &medium)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(medium);
        return asyncCallWithArgumentList(QLatin1String("provideMedium"), argumentList);
    }

    inline QDBusPendingReply<> replyUntrustedPrompt(bool approved)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(approved);
        return asyncCallWithArgumentList(QLatin1String("replyUntrustedPrompt"), argumentList);
    }

    inline QDBusPendingReply<> run()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("run"), argumentList);
    }

    inline QDBusPendingReply<> setProperty(int property, const QDBusVariant &value)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(property) << QVariant::fromValue(value);
        return asyncCallWithArgumentList(QLatin1String("setProperty"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void finished(int exitStatus);
    void mediumRequired(const QString &label, const QString &mountPoint);
    void promptUntrusted(const QStringList &untrustedPackages);
    void propertyChanged(int role, const QDBusVariant &newValue);
};

namespace org {
  namespace kubuntu {
    namespace qaptworker {
      typedef ::OrgKubuntuQaptworkerTransactionInterface transaction;
    }
  }
}
#endif
