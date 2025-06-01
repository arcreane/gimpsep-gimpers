//
// Created by Ines Mansour on 01/06/2025.
//

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <QJsonObject>

class ConfigManager {
public:
    static ConfigManager& instance();

    bool loadConfig(const QString& filePath = "config.json");

    QString getFaceCascadePath() const;
    QString getFaceDatasetPath() const;

private:
    ConfigManager() = default;
    QJsonObject configData;
};


#endif //CONFIGMANAGER_H
