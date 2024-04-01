#ifndef SPRITEEDITOR_H
#define SPRITEEDITOR_H

#include <QObject>
#include <QImage>
#include <QColor>
#include <QPen>
#include <QVector>
#include <QJsonObject>
#include <QWidget>
#include <QMouseEvent>
#include<QJsonObject>
#include<QJsonArray>
#include <QVariant>
#include <QBuffer>
#include <QTimer>

class SpriteEditor : public QWidget
{

	Q_OBJECT

private:
	QVector<QImage> frames;
    QTimer *timer;
	int currentFrame;

    int index = 0;

	QColor currentColor;
    QColor previewColor;

    // max scaled values
    const QPoint maxImageSize = QPoint(448, 448);
    const QPoint maxPreviewSize = QPoint(164,164);

    // the ratio of how much to scale upwards
    float previewRatio;
    float ratio;

    // the current canvas position to for mouse translation
    QPoint canvasPosition;

    // for canvas brush preview
    QPoint lastMousePosition;
    QColor lastPreviewColor;

    // toolbar
	bool drawing;
	bool erasing;
    bool mousePressed;

    // Playback Modes
    bool reverse;
    bool forward;
    bool boomerang;
    bool boomerangDirection;
    bool isScaledPreview;

    // helpers
	void displayCurrentFrame();
    bool translateAndDraw(int x, int y, bool draw); // true if mouse was in bounds
    void showCursorPreview(int x, int y);
    void forwardPlayback();
    void reversePlayback();
    void boomerangPlayback();

    // onion skin
    bool isOnionSkinOn = false;
    QImage* onionSkin;
    QImage generateOnionSkin();

public:
	explicit SpriteEditor(QWidget *parent = nullptr);

    // actual project info
	int width;
	int height;
	QString name;
public slots:
	void erasePixel(int x, int y);
    void drawPixel(int x, int y, const QColor &color);
	void setCurrentColor(const QColor &newColor);
	void addFrame();
	void deleteFrame();
	void onNewProject(int width, int height, QString name);
    void adjustFrame(int value);
    void toggleOnionSkin(int state);
    void framePreview();
    void adjustFPS(int);
    void toJson();
    void toggleForward();
    void toggleReverse();
    void toggleBoomerang();
    void toggleActualSize(int state);

	// mouse

	void onMousePressed(int x, int y, bool pressed);
	void onMouseMoved(int x, int y);

	void currentCanvasPosition(int x, int y);

	// toolbar

	void onDrawPressed(bool pressed);
	void onErasePressed(bool pressed);

signals:
	void updateMaxFrames(int max);
	void updateFrameBox(int value);
	void displayFrame(QImage *frame);
    void displayPreview(QImage *frame);
    void updateCanvasSize(int x, int y);
    void deleteModelFrame(int frameIndex);
    void jsonObject(QJsonObject json);

};

#endif // SPRITEEDITOR_H
