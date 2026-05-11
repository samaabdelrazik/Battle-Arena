#include "CharacterSelect.h"
#include "ui_CharacterSelect.h"

#include <QRect>
#include <QPixmap>
#include <QCoreApplication>
#include <QScreen>
#include <QGuiApplication>

CharacterSelect::CharacterSelect(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CharacterSelect)
{
    ui->setupUi(this);

    setFixedSize(1000, 650);

    QRect screenGeometry =
        QGuiApplication::primaryScreen()->availableGeometry();

    move(screenGeometry.center() - rect().center());

    ui->rangerImage->setAlignment(Qt::AlignCenter);
    ui->wizardImage->setAlignment(Qt::AlignCenter);
    ui->warriorImage->setAlignment(Qt::AlignCenter);

    loadPreviewSheets();
    setupPreviewSequences();

    connect(&previewTimer,
            &QTimer::timeout,
            this,
            &CharacterSelect::updatePreviewAnimations);

    previewTimer.start(100);

    updatePreviewAnimations();
}

CharacterSelect::~CharacterSelect()
{
    delete ui;
}

void CharacterSelect::loadPreviewSheets()
{
    rangerSheet.load(":/sprites/ranger.png");
    wizardSheet.load(":/sprites/wizard.png");
    warriorSheet.load(":/sprites/warrior.png");

    if (rangerSheet.isNull())
    {
        rangerSheet.load("P:/CS2 project/Battle-Arena/Effects/ranger spritesheet calciumtrice.png");
    }

    if (wizardSheet.isNull())
    {
        wizardSheet.load("P:/CS2 project/Battle-Arena/Effects/wizard spritesheet calciumtrice.png");
    }

    if (warriorSheet.isNull())
    {
        warriorSheet.load("P:/CS2 project/Battle-Arena/Effects/warrior spritesheet calciumtrice.png");
    }
}

void CharacterSelect::setupPreviewSequences()
{
    rangerPreview.clips.clear();
    wizardPreview.clips.clear();
    warriorPreview.clips.clear();

    rangerPreview.clips.append({0, 10, 4});
    rangerPreview.clips.append({1, 10, 2});
    rangerPreview.clips.append({3, 10, 5});
    rangerPreview.clips.append({9, 10, 8});

    wizardPreview.clips.append({5, 10, 4});
    wizardPreview.clips.append({6, 10, 2});
    wizardPreview.clips.append({8, 10, 5});
    wizardPreview.clips.append({9, 10, 8});

    warriorPreview.clips.append({0, 10, 4});
    warriorPreview.clips.append({1, 10, 2});
    warriorPreview.clips.append({3, 10, 5});
    warriorPreview.clips.append({9, 10, 8});
}

QPixmap CharacterSelect::getFrame(const QPixmap &sheet,
                                  int frameWidth,
                                  int frameHeight,
                                  int row,
                                  int column,
                                  int targetWidth,
                                  int targetHeight)
{
    if (sheet.isNull())
    {
        return QPixmap();
    }

    QRect sourceRect(
        column * frameWidth,
        row * frameHeight,
        frameWidth,
        frameHeight
    );

    QPixmap frame =
        sheet.copy(sourceRect);

    return frame.scaled(targetWidth,
                        targetHeight,
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation);
}

int CharacterSelect::currentRow(const PreviewState &state) const
{
    if (state.clips.isEmpty())
    {
        return 0;
    }

    return state.clips[state.clipIndex].row;
}

int CharacterSelect::currentFrame(const PreviewState &state) const
{
    return state.frameIndex;
}

void CharacterSelect::advancePreview(PreviewState &state)
{
    if (state.clips.isEmpty())
    {
        return;
    }

    const PreviewClip &clip =
        state.clips[state.clipIndex];

    state.frameIndex++;

    if (state.frameIndex >= clip.frames)
    {
        state.frameIndex = clip.frames - 1;
        state.holdCounter++;

        if (state.holdCounter >= clip.holdTicks)
        {
            state.holdCounter = 0;
            state.frameIndex = 0;

            state.clipIndex++;

            if (state.clipIndex >= state.clips.size())
            {
                state.clipIndex = 0;
            }
        }
    }
}

void CharacterSelect::updatePreviewAnimations()
{
    ui->rangerImage->setPixmap(
        getFrame(rangerSheet,
                 32,
                 32,
                 currentRow(rangerPreview),
                 currentFrame(rangerPreview),
                 105,
                 105)
    );

    ui->wizardImage->setPixmap(
        getFrame(wizardSheet,
                 32,
                 32,
                 currentRow(wizardPreview),
                 currentFrame(wizardPreview),
                 105,
                 105)
    );

    ui->warriorImage->setPixmap(
        getFrame(warriorSheet,
                 32,
                 32,
                 currentRow(warriorPreview),
                 currentFrame(warriorPreview),
                 105,
                 105)
    );

    advancePreview(rangerPreview);
    advancePreview(wizardPreview);
    advancePreview(warriorPreview);
}

void CharacterSelect::on_rangerSelectButton_clicked()
{
    emit archerMaleSelected();
}

void CharacterSelect::on_wizardSelectButton_clicked()
{
    emit mageFemaleSelected();
}

void CharacterSelect::on_warriorSelectButton_clicked()
{
    emit warriorMaleSelected();
}
