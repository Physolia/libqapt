/***************************************************************************
 *   Copyright © 2010 Jonathan Thomas <echidnaman@kubuntu.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation; either version 2 of        *
 *   the License or (at your option) version 3 or any later version        *
 *   accepted by the membership of KDE e.V. (or its successor approved     *
 *   by the membership of KDE e.V.), which shall act as a proxy            *
 *   defined in Section 14 of version 3 of the license.                    *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "workeracquire.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QEventLoop>

#include <apt-pkg/error.h>
#include <apt-pkg/acquire-item.h>
#include <apt-pkg/acquire-worker.h>

WorkerAcquire::WorkerAcquire()
        : m_canceled(false)
        , m_questionResponse(QVariantMap())
{
}

WorkerAcquire::~WorkerAcquire()
{
}

void WorkerAcquire::Start()
{
    // Cleanup from old fetches
    m_canceled = false;

    pkgAcquireStatus::Start();
}

void WorkerAcquire::IMSHit(pkgAcquire::ItemDesc &item)
{
    QString message = QString::fromStdString(item.Description);
    emit downloadMessage((int) QApt::Globals::HitFetch, message);

    Update = true;
}

void WorkerAcquire::Fetch(pkgAcquire::ItemDesc &item)
{
    Update = true;
    if (item.Owner->Complete == true) {
        return;
    }

    QString message = QString::fromStdString(item.Description);
    emit downloadMessage((int) QApt::Globals::DownloadFetch, message);
}

void WorkerAcquire::Done(pkgAcquire::ItemDesc &item)
{
   Update = true;
};

void WorkerAcquire::Fail(pkgAcquire::ItemDesc &item)
{
    // Ignore certain kinds of transient failures (bad code)
    if (item.Owner->Status == pkgAcquire::Item::StatIdle) {
        return;
    }

    if (item.Owner->Status == pkgAcquire::Item::StatDone)
    {
        QString message = QString::fromStdString(item.Description);
        emit downloadMessage((int) QApt::Globals::IgnoredFetch, message);
    } else {
        // an error was found (maybe 404, 403...)
        // the item that got the error and the error text
        QVariantMap args;
        args["FailedItem"] = QString(item.Description.c_str());
        args["ErrorText"] = QString(item.Owner->ErrorText.c_str());
        emit fetchError(QApt::Globals::FetchError, args);
    }

    Update = true;
}

void WorkerAcquire::Stop()
{
   pkgAcquireStatus::Stop();
}

bool WorkerAcquire::MediaChange(string Media, string Drive)
{
    QVariantMap args;
    args["Media"] = QString::fromStdString(Media.c_str());
    args["Drive"] = QString::fromStdString(Drive.c_str());

    QVariantMap result = askQuestion(QApt::Globals::MediaChange, args);

    bool mediaChanged = result["MediaChanged"].toBool();

    return mediaChanged;
}

bool WorkerAcquire::Pulse(pkgAcquire *Owner)
{
    // FIXME: processEvents() is dangerous. Proper threading is needed
    QCoreApplication::processEvents();
    pkgAcquireStatus::Pulse(Owner);
    int percentage = int(int((CurrentBytes + CurrentItems)*100.0)/int(TotalBytes+TotalItems));

    int ETA = (int)((TotalBytes - CurrentBytes) / CurrentCPS);
    // if the ETA is greater than two weeks, show unknown time
    if (ETA > 14*24*60*60) {
        ETA = 0;
    }

    int speed = CurrentCPS;

    emit downloadProgress(percentage, speed, ETA);

    Update = false;

    return !m_canceled;
}

void WorkerAcquire::requestCancel()
{
    m_canceled = true;
}

QVariantMap WorkerAcquire::askQuestion(int questionCode, const QVariantMap &args)
{
    QEventLoop mediaBlock;
    connect(this, SIGNAL(answerReady()), &mediaBlock, SLOT(quit()));

    emit workerQuestion(questionCode, args);
    mediaBlock.exec(); // Process blocked, waiting for answerReady signal over dbus

    return m_questionResponse;
}

void WorkerAcquire::setAnswer(const QVariantMap &answer)
{
    m_questionResponse = answer;
    emit answerReady();
}
