#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Model/npc.h"
#include "Model/classes.h"
#include "Template/deepptr.h"
#include "Template/container.h"
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIcon>
#include <QToolBar>
#include <QAction>
#include <QPixmap>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QScrollArea>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStackedWidget>
#include <QFormLayout>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Container<DeepPtr<Character>> container;

    // Toolbar
    QToolBar* toolbar;

    // Components
    QLineEdit* searchBox;
    QComboBox* showBox;
    QComboBox* sortBox;

    QListWidget* characterList;

    QPixmap racePixmap;
    QPixmap classPixmap;
    QPixmap factionPixmap;

    // Stacked widget layouts and components
    QVBoxLayout* infoLayout;
    QVBoxLayout* insertLayout;
    QVBoxLayout* modifyLayout;
    QWidget* infoWidget;
    QWidget* insertWidget;
    QWidget* modifyWidget;
    QStackedWidget* stackedWidget;

    // Layouts
    QVBoxLayout* leftLayout;
    QVBoxLayout* middleLayout;
    QVBoxLayout* rightLayout;
    QHBoxLayout* mainLayout;
    QWidget* leftWidget;
    QWidget* middleWidget;
    QWidget* rightWidget;

    // Methods
    void createToolbar();
    void createLeftLayout();
    void createMiddleLayout();
    void createRightLayout();
    void createMainLayout();

    void updateList();
    void searchList(const QString& str);
    void showList(const QString& str, bool onlyPlayers);
    void sortList(const QString& str);
    void buildList(const QString& searchStr, const QString& showStr, const QString& sortStr);

    int searchItem(int row);
    void resetFilters() const;

    void createInfoLayout(int pos);
    void createInsertLayout();
    void createModifyLayout(int pos);
    void clearLayout(QLayout* layout);

    void load();
    void save() const;

    bool searchName(const string& str) const;

    QFrame* createLine();

public:
    // Constructors/destructors
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;
};

#endif // MAINWINDOW_H
