#ifndef FLURRYAGENT_H
#define FLURRYAGENT_H

#include <QString>
#include <QMap>
#include <QList>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QObject>

class FlurryAgent : public QObject
{
    Q_OBJECT

    const static QString FLURRY_BASE_URL;
    static qint64 CURRENT_EVENT_ID;
    static qint64 CURRENT_ERROR_ID;
    const static int DEFAULT_SENDING_INTERVAL;

private:
    class FlurryEvent
    {
    public:
        FlurryEvent(QString eventName, const QMap<QString, QString>& params, qint64 deltaTime, bool isTimed = false);

        QString eventName() const;
        const QMap<QString, QString>& parameters() const;
        qint64 deltaTime() const;
        qint64 id() const;

        qint64 duration() const;
        void setDuration(const qint64 &duration);

        void setParameters(const QMap<QString, QString> &parameters);

        bool isReadyToSend() const;

    private:
        QString eventName_;
        QMap<QString, QString> parameters_;
        qint64 deltaTime_;
        qint64 duration_;
        qint64 id_;
        bool isTimed_;
        bool isReadyToSend_;
    };

    class ErrorEvent
    {
    public:
        ErrorEvent(QString errorName, QString desc, int lineNumber, qint64 timestamp);

        QString errorName() const;
        QString errorDesc() const;

        qint64 timestamp() const;
        qint64 id() const;

        int lineNumber() const;

    private:
        QString errorName_;
        QString errorDesc_;
        int lineNumber_;
        qint64 timestamp_;
        qint64 id_;
    };

public:
    FlurryAgent();

    void startSession(QString apiKey);
    void endSession();

    void setUserId(QString userId);
    void setLocation(double latitude, double longitude, float accuracy);
    void setAppVersion(QString appVersion);

    void setRequestInterval(int timeInSeconds);

    void logEvent(QString eventName, QMap<QString, QString> parameters = QMap<QString, QString>(), bool timedEvent = false);
    void logError(QString errorName, QString errorMessage, int lineNumber);
    void endTimedEvent(QString eventName, QMap<QString, QString> parameters = QMap<QString, QString>());

    void setSessionContinueSeconds(int seconds);

    QString formData();

signals:
    void isSendingChanged(bool isSending);

private slots:
    void sendData();

private:
    void clearData();

    QString formErrorToJson(const ErrorEvent& error);
    QString formEventToJson(const FlurryAgent::FlurryEvent& event);

private:
    QString apiKey_;    
    QString userIdHash_;
    QString appVersion_;
    qint64 sessionStartTime_;
    int sendingInterval_;

    QList<FlurryEvent> events_;

    QList<ErrorEvent> errorEvents_;

    QNetworkAccessManager networkManager_;
    QTimer sendTimer_;

    double latitude_;
    double longitude_;
    float locationAccuracy_;
    bool isSending_;
};

#endif // FLURRYAGENT_H
