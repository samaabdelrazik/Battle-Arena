#ifndef CHARACTERSELECT_H
#define CHARACTERSELECT_H

#include <QMainWindow>
#include <QTimer>
#include <QPixmap>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class CharacterSelect;
}
QT_END_NAMESPACE

struct PreviewClip
{
    int row;
    int frames;
    int holdTicks;
};

struct PreviewState
{
    QVector<PreviewClip> clips;
    int clipIndex = 0;
    int frameIndex = 0;
    int holdCounter = 0;
};

class CharacterSelect : public QMainWindow
{
    Q_OBJECT

public:
    explicit CharacterSelect(QWidget *parent = nullptr);
    ~CharacterSelect();

signals:
    void archerMaleSelected();
    void archerFemaleSelected();

    void mageMaleSelected();
    void mageFemaleSelected();

    void warriorMaleSelected();
    void warriorFemaleSelected();

private slots:
    void on_rangerSelectButton_clicked();
    void on_wizardSelectButton_clicked();
    void on_warriorSelectButton_clicked();

    void updatePreviewAnimations();

private:
    Ui::CharacterSelect *ui;

    QTimer previewTimer;

    QPixmap rangerSheet;
    QPixmap wizardSheet;
    QPixmap warriorSheet;

    PreviewState rangerPreview;
    PreviewState wizardPreview;
    PreviewState warriorPreview;

    void loadPreviewSheets();
    void setupPreviewSequences();

    QPixmap getFrame(const QPixmap &sheet,
                     int frameWidth,
                     int frameHeight,
                     int row,
                     int column,
                     int targetWidth,
                     int targetHeight);

    void advancePreview(PreviewState &state);

    int currentRow(const PreviewState &state) const;
    int currentFrame(const PreviewState &state) const;
};

#endif
