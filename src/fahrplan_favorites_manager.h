#ifndef FAHRPLAN_FAVORITES_MANAGER_H
#define FAHRPLAN_FAVORITES_MANAGER_H

#include <QSettings>
#include <QStringList>

class FahrplanFavoritesManager : public QObject
{
    Q_OBJECT

    public:
        explicit FahrplanFavoritesManager(QObject *parent = 0);

    public slots:
        void addFavorite(QString name);
        void removeFavorite(QString name);
        bool isFavorite(QString name);
        QStringList getFavorites();

    signals:
        void favoritesChanged(QStringList);

    private:
        QStringList favorites;
        QSettings *settings;
};

#endif // FAHRPLAN_FAVORITES_MANAGER_H
