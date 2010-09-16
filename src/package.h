/***************************************************************************
 *   Copyright © 2010 Jonathan Thomas <echidnaman@kubuntu.org>             *
 *   Heavily inspired by Synaptic library code ;-)                         *
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

#ifndef QAPT_PACKAGE_H
#define QAPT_PACKAGE_H

#include <QtCore/QFlags>
#include <QtCore/QUrl>
#include <QtCore/QDateTime>
#include <QtCore/QVariantMap>

#include <apt-pkg/pkgcache.h>

#include "globals.h"

class pkgRecords;
class pkgDepCache;

namespace QApt {

class Backend;

/**
 * PackagePrivate is a class containing all private members of the Package class
 */
class PackagePrivate;

/**
 * The Package class is an object for referencing a software package in the Apt
 * package database. You will be getting most of your information about your
 * packages from this class.
 *
 * @author Jonathan Thomas
 */
class Package
{

public:
   /**
    * Default constructor
    *
    * @param parent The backend that this package is being made a child of
    * @param depcache The underlying dependency cache for fetching some info
    * @param records The underlying package records for fetching some info
    * @param packageIter The underlying object representing the package in APT
    */
    Package(QApt::Backend* parent, pkgDepCache *depcache,
            pkgRecords *records, pkgCache::PkgIterator &packageIter);

   /**
    * Default destructor
    */
    virtual ~Package();

   /**
    * Returns the internal APT representation of the package
    *
    * \return The interal APT package pointer
    */
    pkgCache::PkgIterator *packageIterator() const;

   /**
    * Returns the name of the package
    *
    * \return The name of the package
    */
    QString name() const;

    // TODO QApt2: Replace name() with latin1Name (but still call it name())
   /**
    * Returns the name of the package. This is the better choice over the
    * regular name() since a QLatin1String is much cheaper to construct
    * than a QString, since we don't have to convert from ascii
    *
    * \return The name of the package as a \c QLatin1String
    */
    QLatin1String latin1Name() const;

   /**
    * Returns the unique internal identifier for the package
    *
    * \return The identifier of the package
    */
    int id() const;

   /**
    * Returns the version of the package, regardless of whether it is installed
    * or not. If not installed, it returns the version of the candidate for
    * installation, which may not necessarily be the latest. (If the version has
    * been changed with setVersion())
    *
    * \return The version of the package
    *
    * \sa Package::setVersion()
    */
    QString version() const;

   /**
    * Returns a list of all available versions of the package in the form of
    * "version, release" (E.g. "0.2-0ubuntu1, maverick")
    *
    * \return All available versions of the package
    */
    QStringList availableVersions() const;

   /**
    * Returns the categorical section where the package resides
    *
    * \return The section of the package
    */
    QString section() const;

   /**
    * Returns the source package corresponding to the package
    *
    * \return The source package of the package
    */
    QString sourcePackage() const;

   /**
    * Returns the short description (or "summary" in libapt-pkg terms) of the
    * package
    *
    * \return The short description of the package
    */
    QString shortDescription() const;

   /**
    * Returns the maintainer of the package
    *
    * \return The maintainer of the package
    */
    QString maintainer() const;

   /**
    * Returns the homepage of the package
    *
    * \return The homepage of the package
    */
    QString homepage() const;

   /**
    * Returns the installed version of the package.
    *
    * \return The installed version of the package. If this package is not
    *         installed, this function will return a null @c QString
    */
    QString installedVersion() const;

   /**
    * Returns the newest available version of the package if it is not
    * installed.
    *
    * \return The available version of the package. If this package is
    *         installed, this function will return a null @c QString.
    */
    QString availableVersion() const;

   /**
    * Returns the priority of the package
    *
    * \return The priority of the package
    */
    QString priority() const;

   /**
    * Returns the files that this package has installed. 
    *
    * \return The file list of the package. If the package is not installed, it
    *         will return an empty list.
    */
    QStringList installedFilesList() const;

   /**
    * Returns the long description of the package.
    *
    * \return The long description of the package
    */
    QString longDescription() const;

   /**
    * Returns the origin of the package.
    * (Usually Ubuntu or Debian)
    *
    * \return The origin of the package
    */
    QString origin() const;

   /**
    * Returns the archive component of the package. (E.g. main, restricted,
    * universe, contrib, etc)
    *
    * \return The archive component of the package
    */
    QString component() const;

   /**
    * Returns the url to the location of a package's changelog.
    *
    * \return The location of the package's changelog
    */
    QUrl changelogUrl() const;

   /**
    * Returns the url of the package's screenshot over the Internet.
    *
    * @param type The type of screenshot to be fetched as a QApt::ScreenshotType
    *
    * \return The url of the package changelog
    */
    QUrl screenshotUrl(QApt::ScreenshotType type) const;

   /**
    * Returns the date when Canonical's support of the package ends.
    *
    * \return The date that the package is supported until. If it is not
    *         supported now, then it will return an empty QString. The date
    *         will be localized in the "month year" format.
    */
    QString supportedUntil() const;

   /**
    * Returns the amount of hard drive space that the currently-installed
    * version of this package takes up.
    * This is human-unreadable, so KDE applications may wish to run this
    * through the KGlobal::locale()->formatByteSize() function to get a
    * localized, human-readable number.
    *
    * \return The installed size of the package
    */
    qint64 currentInstalledSize() const;

   /**
    * Returns the amount of hard drive space that this package will take up
    * once installed.
    * This is human-unreadable, so KDE applications may wish to run this
    * through the KGlobal::locale()->formatByteSize() function to get a
    * localized, human-readable number.
    *
    * \return The installed size of the package
    */
    qint64 availableInstalledSize() const;

   /**
    * Returns the download size of the package archive in bytes.
    * This is human-unreadable, so KDE applications may wish to run this
    * through the KGlobal::locale()->formatByteSize() function to get a
    * localized, human-readable number.
    *
    * \return The installed size of the package
    */
    qint64 downloadSize() const;

   /**
    * Returns the state of a package, using the @b PackageState enum to define
    * states.
    *
    * \return The PackageState flags of the package as an @c int
    */
    int state() const;

   /**
    * Returns whether the Package is installed
    */
    bool isInstalled() const;

   /**
    * Returns whether the package is supported by Canonical
    */
    bool isSupported() const;

   /**
    * Returns a list of the names of all the dependencies of this package.
    *
    * \return A list of packages that this package depends on
    */
    QStringList dependencyList(bool useCandidateVersion) const;

   /**
    * Returns a list of the names of all the packages that depend on this
    * package. (Reverse dependencies)
    *
    * \return A list of packages that depend on this package
    */
    QStringList requiredByList() const;

   /**
    * Returns a list of the names of all the virtual packages that this package
    * provides.
    *
    * \return A list of packages that this package provides
    */
    QStringList providesList() const;

   /**
    * If a package is in a broke state, this function returns why the package
    * is broken by showing all errors in the dependency cache that marking the
    * package has caused.
    *
    * The format is a bit complex. The QHash contains a QHash corresponding to
    * each QApt::BrokenReason. This internal QHash is a QString corresponding to
    * a QVariantMap. The QString in this case is a package that is broken by this
    * QApt::Package, and the QVariantMap contails details on why the corresponding
    * package is broken. These details may vary based on what QApt::BrokenState
    * the QHash corresponds to.
    *
    * \return A @c QHash of reasons why the package is broken, corresponding to a
    * QApt::BrokenReason
    */
    QHash<int, QHash<QString, QVariantMap> > brokenReason() const;

   /**
    * Returns whether the package is signed with a trusted GPG signature.
    */
    bool isTrusted() const;

   /**
    * Returns whether the package would break if the current potential changes
    * are committed
    */
    bool wouldBreak() const;

   /**
    * Sets and unsets the auto-install flag
    */
    void setAuto(bool flag = true);

   /**
    * Marks the package to be kept
    */
    void setKeep();

   /**
    * Marks the package for installation
    */
    void setInstall();

   /**
    * Member function that sets whether or not the package needs
    * reinstallation, based on a boolean value passed to it.
    */
    void setReInstall();

   /**
    * Marks the package for removal.
    */
    void setRemove();

   /**
    * Marks the package for complete removal, including config files.
    */
    void setPurge();

   /**
    * Overrides the candidate version, setting it to the version string
    */
    bool setVersion(const QString & version);

    // "//" == TODO
   /**
    * An enumerator for various states that a @c Package may hold. A package
    * may hold several states at once.
    */
    enum State {
        /// The package will not be changed
        ToKeep              = 1 << 0,
        /// The package has been marked for install
        ToInstall           = 1 << 1,
        /// The package is a new install, never have been installed before
        NewInstall          = 1 << 2,
        /// The package has been marked for reinstall
        ToReInstall         = 1 << 3,
        /// The package has been marked for upgrade
        ToUpgrade           = 1 << 4,
        /// The package has been marked for downgrade
        ToDowngrade         = 1 << 5,
        /// The package has been marked for removal
        ToRemove            = 1 << 6,
        /// The package has been held from beinig upgraded
        Held                = 1 << 7,
        /// The package is currently installed
        Installed           = 1 << 8,
        /// The package is currently upgradeable
        Upgradeable         = 1 << 9,
        /// The package is currently broken
        NowBroken           = 1 << 10,
        /// The package's install is broken
        InstallBroken       = 1 << 11,
        /// This package is a dependency of another package that is not installed
        Orphaned            = 1 << 12,//
        /// The package has been manually prevented from upgrade
        Pinned              = 1 << 13,//
        /// The package is new in the archives
        New                 = 1 << 14,//
        /// The package still has residual config. (Was not purged)
        ResidualConfig      = 1 << 15,
        /// The package is no longer downloadable
        NotDownloadable     = 1 << 16,
        /// The package has been marked for purging
        ToPurge             = 1 << 17,
        /// The package is essential for a base installation
        IsImportant         = 1 << 18,
        /// The package has had its candidate version overridden by setVersion()
        OverrideVersion     = 1 << 19,
        /// The package was automatically installed as a dependency
        IsAuto              = 1 << 20,
        /// The package is invalid
        IsGarbage           = 1 << 21,
        /// The package's policy is broken
        NowPolicyBroken     = 1 << 22,
        /// The package's install policy is broken
        InstallPolicyBroken = 1 << 23,
        /// The package is not installed
        NotInstalled        = 1 << 24
    };
    Q_DECLARE_FLAGS(States, State);

private:
    PackagePrivate *const d;
};

}

#endif
