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

#ifndef QAPT_BACKEND_H
#define QAPT_BACKEND_H

#include <QtCore/QVariantMap>

#include "globals.h"
#include "package.h"

class pkgSourceList;

/**
 * The QApt namespace is the main namespace for LibQApt. All classes in this
 * library fall under this namespace.
 */
namespace QApt {

class BackendPrivate;

/**
 * @brief The main entry point for performing operations with the dpkg database
 *
 * Backend encapsulates all the needed logic to perform most apt operations.
 * It implements the initializing of the database and all requests to/for the
 * database. Please note that you \b _MUST_ call init() before doing any
 * further operations to the backend, or else risk encountering undefined
 * behavior.
 *
 * @author Jonathan Thomas
 */
class Backend : public QObject
{
    Q_OBJECT
public:
     /**
      * Default constructor
      */
    explicit Backend();

     /**
      * Default destructor
      */
    virtual ~Backend();

    /**
     * Initializes the Apt database for usage. It sets up everything the backend
     * will need to perform all operations. Please note that you @b _MUST_ call
     * this function before doing any further operations in the backend, or else
     * risk encountering undefined behavior.
     *
     * @return @c true if initialization was successful
     * @return @c false if there was a problem initializing
     */
    bool init();

    /**
     * Returns whether the undo stack is empty
     */
    bool isUndoStackEmpty() const;

    /**
     * Returns whether the redo stack is empty
     *
     */
    bool isRedoStackEmpty() const;

    /**
     * Repopulates the internal package cache, package list, and group list.
     * Mostly used internally, like after an update or a package installation
     * or removal.
     */
    void reloadCache();

    /**
     * Takes a snapshot of the current state of the package cache. (E.g.
     * which packages are marked for removal, install, etc)
     *
     * \return The current state of the cache as a @c CacheState
     */
    CacheState currentCacheState() const;

    /**
     * Queries the backend for a Package object for the specified name.
     *
     * @b _WARNING_ :
     * Note that if a package with a given name cannot be found, a null pointer
     * will be returned. Also, please note that certain actions like reloading
     * the cache may invalidate the pointer.
     *
     * @param name name used to specify the package returned
     *
     * @return A pointer to a @c Package defined by the specified name
     */
    Package *package(const QString &name) const;

    /**
     * Queries the backend for a Package object that installs the specified
     * file.
     *
     * @b _WARNING_ :
     * Note that if a package with a given name cannot be found, a null pointer
     * will be returned. Also, please note that certain actions like reloading
     * the cache may invalidate the pointer.
     *
     * @param file The file used to search for the package
     *
     * @return A pointer to a @c Package defined by the specified name
     */
    Package *packageForFile(const QString &file) const;

    /**
     * Returns a list of all package origins, as user readable strings.
     *
     * @return The list of human-readable origin labels
     */
    QStringList originLabels() const;

    /**
     * Returns the human-readable name for the origin repository of the given
     * the machine-readable name.
     *
     * @return The human-readable origin label
     */
    QString originLabel(const QString &origin) const;

    /**
     * Returns the machine-readable name for the origin repository of the given
     * the human-readable name.
     *
     * @return The machine-readable origin label
     */
    QString origin(QString originLabel) const;

    /**
     * Queries the backend for the total number of packages in the Apt
     * database, discarding no-longer-existing packages that linger on in the
     * status cache (That have a version of 0)
     *
     * @return The total number of packages in the Apt database
     */
    int packageCount() const;

    /**
     * Queries the backend for the total number of packages in the Apt
     * database, discarding no-longer-existing packages that linger on in the
     * status cache (That have a version of 0)
     *
     * @param states The package state(s) for which you wish to count packages for
     *
     * @return The total number of packages of the given PackageState in the
     *         APT database
     */
    int packageCount(const Package::States &states) const;

    /**
     * Returns the total amount of data that will be downloaded if the user
     * commits changes. Cached packages will not show up in this count.
     *
     * @return The total amount that will be downloaded in bytes.
     */
    qint64 downloadSize() const;

    /**
     * Returns the total amount of disk space that will be consumed or
     * freed once the user commits changes. Freed space will show up as a
     * negative number.
     *
     * @return The total disk space to be used in bytes.
     */
    qint64 installSize() const;

    /**
     * Returns a list of all available packages. This includes essentially all
     * packages, excluding now-nonexistent packages that have a version of 0.
     *
     * \return A @c PackageList of all available packages in the Apt database
     */
    PackageList availablePackages() const;

    /**
     * Returns a list of all upgradeable packages
     *
     * \return A @c PackageList of all upgradeable packages
     */
    PackageList upgradeablePackages() const;

    /**
     * Returns a list of all packages that have been marked for change. (To be
     * installed, removed, etc)
     *
     * \return A @c PackageList of all packages marked to be changed
     */
    PackageList markedPackages() const;

    /**
     * Searches through the internal package list and returns a list of packages
     * based on the given input.
     *
     * @param searchString The string to narrow the search by.
     *
     * \return A @c PackageList of all packages matching the search string.
     */
    PackageList search(const QString &searchString) const;

    /**
     * Returns a list of all available groups
     *
     * \return A @c GroupList of all available groups in the Apt database
     */
    GroupList availableGroups() const;

    /**
     * Returns whether the search index needs updating
     */
    bool xapianIndexNeedsUpdate() const;

   /**
    * Attempts to open the APT Xapian index, needed for searching
    *
    * \returns true if opening was successful
    * \returns false otherwise
    */
    bool openXapianIndex();

    /**
     * Returns whether the cache has broken packages or has a null dependency
     * cache
     */
    bool isBroken() const;

protected:
    BackendPrivate *const d_ptr;

    /**
     * Returns a pointer to the internal package source list. Mainly used for
     * internal purposes in QApt::Package.
     *
     * @return @c pkgSourceList The package source list used by the backend
     */
    pkgSourceList *packageSourceList() const;

private:
    Q_DECLARE_PRIVATE(Backend);
    friend class Package;
    friend class PackagePrivate;

    Package *package(pkgCache::PkgIterator &iter) const;

Q_SIGNALS:
    /**
     * Emitted whenever a backend error occurs. You should listen to this
     * signal and present the error/clean up when your app receives it.
     *
     * @param error QApt::ErrorCode enum member indicating error type
     * @param details A QVariant map containing info about the error, if
     *                available
     */
    void errorOccurred(QApt::ErrorCode error, const QVariantMap &details);

    /**
     * Emitted whenever a backend warning occurs. You should listen to this
     * signal and present the warning when your app receives it.
     *
     * @param error QApt::WarningCode enum member indicating error type
     * @param details A QVariant map containing info about the warning, if
     *                available
     */
    void warningOccurred(QApt::WarningCode warning, const QVariantMap &details);

    /**
     * Emitted whenever the worker asks a question. You should listen to this
     * signal and present the question to the user when your app receives it.
     *
     * You should send the response back to the worker as a QVariantMap
     * using the Backend's answerWorkerQuestion() function.
     *
     * @param question A QApt::WorkerQuestion enum member indicating question type
     * @param details A QVariant map containing info about the question, if available
     */
    void questionOccurred(QApt::WorkerQuestion question, const QVariantMap &details);

    /**
     * Emitted whenever a package changes state. Useful for knowning when to
     * react to state changes.
     */
    void packageChanged();

    /**
     * Emitted whenever a backend event occurs.
     *
     * @param event A QApt::WorkerEvent enum member indicating event type
     */
    void workerEvent(QApt::WorkerEvent event);

    /**
     * Emits total progress information while the QApt Worker is downloading
     * packages.
     *
     * @param percentage Total percent complete
     * @param speed Current download speed in bytes
     * @param ETA Current estimated download time
     */
    void downloadProgress(int percentage, int speed, int ETA);

    /**
     * Emits per-package progress information while the QApt Worker is
     * downloading packages.
     *
     * @param name Name of the package currently being downloaded
     * @param percentage Percentage of the package downloaded
     * @param URI The URI of the download location
     * @param size The size of the download in bytes
     * @param flag Fetch type (is a QApt::Global enum member)
     */
    void packageDownloadProgress(const QString &name, int percentage, const QString &URI,
                                 double size, int flag);

    /**
     * Emitted whenever an item has been downloaded.
     *
     * This signal is deprecated. You should connect to packageDownloadProgress
     * which provides a lot more information about the fetch.
     *
     * @param flag Fetch type (is a QApt::Global enum member)
     * @param message Usually the URI of the item that's being downloaded
     */
    QT_DEPRECATED void downloadMessage(int flag, const QString &message);

    /**
     * Emits the progress of a current package installation/removal/
     * operation.
     *
     * @param status Current status retrieved from dpkg
     * @param percentage Total percent complete
     */
    void commitProgress(const QString &status, int percentage);

   /**
    * Emits the progress of the Apt Xapian Indexer
    *
    * @param progress The progress percentage of the indexer
    */
    void xapianUpdateProgress(int percentage);

public Q_SLOTS:
    /**
     * Takes the current state of the cache and puts it on the undo stack
     */
    void saveCacheState();

    /**
     * Restores the package cache to the given state.
     *
     * @param state The state to restore the cache to
     */
    void restoreCacheState(const CacheState &state);

    /**
     * Un-performs the last action performed to the package cache
     */
    void undo();

    /**
     * Re-performs the last un-done action to the package cache.
     */
    void redo();

    /**
     * Marks all upgradeable packages for upgrading, without marking new
     * packages for installation.
     */
    void markPackagesForUpgrade();

    /**
     * Marks all upgradeable packages for upgrading, including updates that
     * would require marking new packages for installation.
     */
    void markPackagesForDistUpgrade();

    /**
     * Marks a package for install.
     *
     * @param name The name of the package to be installed
     */
    void markPackageForInstall(const QString &name);

    /**
     * Marks a package for removal.
     *
     * @param name The name of the package to be removed
     */
    void markPackageForRemoval(const QString &name);

    /**
     * Commits all pending package state changes that have been made.
     */
    void commitChanges();

    /**
     * A slot that Packages use to tell the backend they've changed.
     * (Used internally by QApt::Package. You likely will never use this)
     */
    void packageChanged(Package *package);

    /**
     * Checks for and downloads new package source lists.
     */
    void updateCache();

    /**
     * Cancels download operations initialized by the updateCache() or
     * commitChanges() functions.
     */
    void cancelDownload();

    /**
     * This function should be used to return the answer the user has given
     * to a worker question.
     */
    void answerWorkerQuestion(const QVariantMap &response);

    /**
     * Writes a list of packages that have been marked for install, removal or
     * upgrade
     *
     * @param path The path to save the selection list to
     *
     * \return @c true if saving succeeded
     * \return @c false if the saving failed
     */
    bool saveSelections(const QString &path) const;

    /**
     * Reads and applies selections from a text file generated from either
     * saveSelections() or from Synaptic
     *
     * @param path The path from which to read the selection list
     *
     * \return @c true if reading/marking succeeded
     * \return @c false if the reading/marking failed
     */
    bool loadSelections(const QString &path);

    void updateXapianIndex();

private Q_SLOTS:
    void serviceOwnerChanged(const QString &name, const QString &oldOwner, const QString &newOwner);
    void workerStarted();
    void workerFinished(bool result);

    void emitErrorOccurred(int errorCode, const QVariantMap &details);
    void emitWarningOccurred(int warningCode, const QVariantMap &details);
    void emitWorkerEvent(int event);
    void emitWorkerQuestionOccurred(int question, const QVariantMap &details);
};

}

#endif
