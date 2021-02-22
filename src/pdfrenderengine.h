#ifndef PDFRENDERENGINE_H
#define PDFRENDERENGINE_H

#include <poppler-qt5.h>

#include <QImage>
#include <QMutex>
#include <QQueue>
#include <QSemaphore>
#include <QThread>

class SafeThread : public QThread {
	Q_OBJECT

public:
	SafeThread();
	SafeThread(QObject *parent);
	void wait(unsigned long time = 60000);
	bool crashed;
};

class PDFQueue;

class RenderCommand {
public:
	explicit RenderCommand(int p, double xr = 72.0, double yr = 72.0,
	                       int x = -1, int y = -1, int w = -1, int h = -1);
	int pageNr;
	double xres;
	double yres;
	int x;
	int y;
	int w;
	int h;
	Poppler::Page::Rotation rotate;
	int ticket;
	bool priority;
};

class PDFRenderEngine : public SafeThread {
	Q_OBJECT

public:
	explicit PDFRenderEngine(QObject *parent, PDFQueue *mQueue);
	~PDFRenderEngine();

	QByteArray tempData;
	void setDocument(const QSharedPointer<Poppler::Document> &doc);

signals:
	void sendImage(QImage image, int page, int ticket);

public slots:

protected:
	void run();

private:
	QSharedPointer<Poppler::Document> document;
	int cachedNumPages;
	PDFQueue *queue;
};

#endif