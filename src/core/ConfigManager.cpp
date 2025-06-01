//
// Created by Ines Mansour on 01/06/2025.
//
#include "ConfigManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QDebug>

ConfigManager& ConfigManager::instance() {
    static ConfigManager instance;
    return instance;
}

bool ConfigManager::loadConfig(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open config file:" << filePath;
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format in config file.";
        return false;
    }

    configData = doc.object();
    return true;
}

QString ConfigManager::getFaceCascadePath() const {
    return configData.value("face_cascade").toString();
}

QString ConfigManager::getFaceDatasetPath() const {
    return configData.value("face_dataset").toString();
}

