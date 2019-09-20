#include "mainwindow.h"

/******************** CONSTRUCTORS/DESTRUCTORS ********************/
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    // Window icon and size
    setWindowIcon(QIcon(":/img/horde_icon.ico"));
    setFixedSize(980, 620);

    // Toolbar
    toolbar = new QToolBar;
    createToolbar();
    addToolBar(toolbar);

    // Components
    searchBox = new QLineEdit;
    showBox = new QComboBox;
    sortBox = new QComboBox;

    characterList = new QListWidget;

    // Stacked widget layouts and components
    infoLayout = new QVBoxLayout;
    insertLayout = new QVBoxLayout;
    modifyLayout = new QVBoxLayout;
    infoWidget = new QWidget;
    insertWidget = new QWidget;
    modifyWidget = new QWidget;
    stackedWidget = new QStackedWidget;

    // Layouts
    leftLayout = new QVBoxLayout;
    middleLayout = new QVBoxLayout;
    rightLayout = new QVBoxLayout;
    mainLayout = new QHBoxLayout;
    leftWidget = new QWidget;
    middleWidget = new QWidget;
    rightWidget = new QWidget;

    createLeftLayout();
    createMiddleLayout();
    createRightLayout();
    createMainLayout();

    // Main widget
    QWidget* mainWidget = new QWidget(this);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}


/******************** METHODS ********************/
void MainWindow::createToolbar()
{ 
    toolbar->setMovable(false);
    QAction* loadAction = toolbar->addAction("Load");
    QAction* saveAction = toolbar->addAction("Save");
    connect(loadAction, &QAction::triggered, [this] {
        load();
        resetFilters();
    });
    connect(saveAction, &QAction::triggered, [this] { save(); });
}

void MainWindow::createLeftLayout()
{
    // Logo
    QPixmap logo = QPixmap(":/img/horde_logo.png");
    logo = logo.scaledToWidth(150);
    QLabel* logoLabel = new QLabel;
    logoLabel->setPixmap(logo);
    logoLabel->setStyleSheet("margin-bottom: 2.7em");

    // Search
    QLabel* searchLabel = new QLabel("Search");
    connect(searchBox, &QLineEdit::textChanged, [this] {
        if (container.getSize() > 0)
            buildList(searchBox->text(), showBox->currentText(), sortBox->currentText());
    });

    // Show
    QLabel* showLabel = new QLabel("Show");
    showBox->addItem("All");
    showBox->insertSeparator(1);
    showBox->addItem("NPC");
    showBox->addItem("Player");
    showBox->insertSeparator(4);
    showBox->addItem("Mage");
    showBox->addItem("Priest");
    showBox->addItem("Rogue");
    showBox->addItem("Warrior");
    connect(showBox, &QComboBox::currentTextChanged, [this] {
        if (container.getSize() > 0)
            buildList(searchBox->text(), showBox->currentText(), sortBox->currentText());
    });

    // Sort
    QLabel* sortLabel = new QLabel("Sort");
    sortBox->addItem("None");
    sortBox->insertSeparator(1);
    sortBox->addItem("Name");
    sortBox->addItem("Gender");
    sortBox->addItem("Race");
    sortBox->addItem("Faction");
    sortBox->addItem("Level");
    sortBox->addItem("HP");
    sortBox->insertSeparator(8);
    sortBox->addItem("Status");
    sortBox->addItem("Rating");
    connect(sortBox, &QComboBox::currentTextChanged, [this] {
        if (container.getSize() > 0)
            buildList(searchBox->text(), showBox->currentText(), sortBox->currentText());
    });

    // Reset
    QPushButton* resetButton = new QPushButton("Reset filters");
    connect(resetButton, &QPushButton::clicked, [this] { resetFilters(); });

    leftLayout->addWidget(logoLabel);
    leftLayout->addWidget(searchLabel);
    leftLayout->addWidget(searchBox);
    leftLayout->addWidget(showLabel);
    leftLayout->addWidget(showBox);
    leftLayout->addWidget(sortLabel);
    leftLayout->addWidget(sortBox);
    leftLayout->addWidget(resetButton);
    leftLayout->setAlignment(logoLabel, Qt::AlignHCenter);
}

void MainWindow::createMiddleLayout()
{
    // List box
    QScrollArea* characterBox = new QScrollArea;
    characterBox->setWidget(characterList);
    characterBox->setWidgetResizable(true);
    characterBox->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    connect(characterList, &QListWidget::currentRowChanged, [this] {
        if (characterList->count() > 0) {
            clearLayout(infoLayout);
            createInfoLayout(searchItem(characterList->currentRow()));
        }
        if (stackedWidget->isHidden()) stackedWidget->show();
        stackedWidget->setCurrentWidget(infoWidget);
    });

    // Buttons
    QPushButton* insertButton = new QPushButton("Insert");
    QPushButton* modifyButton = new QPushButton("Modify");
    QPushButton* deleteButton = new QPushButton("Delete");
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(insertButton);
    buttonsLayout->addWidget(modifyButton);
    buttonsLayout->addWidget(deleteButton);
    connect(insertButton, &QPushButton::clicked, [this] {
        clearLayout(insertLayout);
        createInsertLayout();
        leftWidget->setDisabled(true);
        middleWidget->setDisabled(true);
        if (stackedWidget->isHidden()) stackedWidget->show();
        stackedWidget->setCurrentWidget(insertWidget);
    });
    connect(modifyButton, &QPushButton::clicked, [this] {
        if (characterList->count() > 0 && characterList->currentRow() != -1) {
            clearLayout(modifyLayout);
            createModifyLayout(searchItem(characterList->currentRow()));
            leftWidget->setDisabled(true);
            middleWidget->setDisabled(true);
            if (stackedWidget->isHidden()) stackedWidget->show();
            stackedWidget->setCurrentWidget(modifyWidget);
        }
    });
    connect(deleteButton, &QPushButton::clicked, [this] {
        if (characterList->count() > 0 && characterList->currentRow() != -1) {
            container.remove(searchItem(characterList->currentRow()));
            characterList->takeItem(characterList->currentRow());
        }
    });

    middleLayout->addWidget(characterBox);
    middleLayout->addLayout(buttonsLayout);
}

void MainWindow::createRightLayout()
{
    infoWidget->setLayout(infoLayout);
    insertWidget->setLayout(insertLayout);
    modifyWidget->setLayout(modifyLayout);

    stackedWidget->addWidget(infoWidget);
    stackedWidget->addWidget(insertWidget);
    stackedWidget->addWidget(modifyWidget);
    stackedWidget->hide();

    rightLayout->addWidget(stackedWidget);
}

void MainWindow::createMainLayout()
{
    leftWidget->setLayout(leftLayout);
    leftWidget->setFixedWidth(250);

    middleWidget->setLayout(middleLayout);
    middleWidget->setFixedWidth(350);

    rightWidget->setLayout(rightLayout);

    mainLayout->addWidget(leftWidget);
    mainLayout->addWidget(middleWidget);
    mainLayout->addWidget(rightWidget);
}

void MainWindow::updateList()
{
    if (characterList->count() != 0) {
        characterList->reset();
        characterList->clear();
    }
    for (unsigned int i = 0; i < container.getSize(); ++i)
        characterList->addItem(new QListWidgetItem(QString::fromStdString(container[i]->getName())));
}

void MainWindow::searchList(const QString& str)
{
    for (int i = 0; i < characterList->count(); ++i) {
        QListWidgetItem* listItem = characterList->item(i);
        if (!listItem->text().toUpper().contains(str.toUpper())) {
            characterList->takeItem(i);
            --i;    // count() gets updated by takeItem()
        }
    }
}

void MainWindow::showList(const QString& str, bool onlyPlayers)
{
    if (str == "All")
        updateList();
    else if (str == "NPC") {
        if (onlyPlayers) {
            characterList->reset();
            characterList->clear();
        }
        else {
            for (unsigned int i = 0; i < container.getSize(); ++i) {
                NPC* npc = dynamic_cast<NPC*>(container[i].operator->());
                if (npc) characterList->addItem(new QListWidgetItem(QString::fromStdString(container[i]->getName())));
            }
        }
    }
    else if (str == "Player") {
        for (unsigned int i = 0; i < container.getSize(); ++i) {
            Player* player = dynamic_cast<Player*>(container[i].operator->());
            if (player) characterList->addItem(new QListWidgetItem(QString::fromStdString(container[i]->getName())));
        }
    }
    else if (str == "Mage") {
        for (unsigned int i = 0; i < container.getSize(); ++i) {
            Mage* mage = dynamic_cast<Mage*>(container[i].operator->());
            if (mage) characterList->addItem(new QListWidgetItem(QString::fromStdString(container[i]->getName())));
        }
    }
    else if (str == "Priest") {
        for (unsigned int i = 0; i < container.getSize(); ++i) {
            Priest* priest = dynamic_cast<Priest*>(container[i].operator->());
            if (priest) characterList->addItem(new QListWidgetItem(QString::fromStdString(container[i]->getName())));
        }
    }
    else if (str == "Rogue") {
        for (unsigned int i = 0; i < container.getSize(); ++i) {
            Rogue* rogue = dynamic_cast<Rogue*>(container[i].operator->());
            if (rogue) characterList->addItem(new QListWidgetItem(QString::fromStdString(container[i]->getName())));
        }
    }
    else if (str == "Warrior") {
        for (unsigned int i = 0; i < container.getSize(); ++i) {
            Warrior* warrior = dynamic_cast<Warrior*>(container[i].operator->());
            if (warrior) characterList->addItem(new QListWidgetItem(QString::fromStdString(container[i]->getName())));
        }
    }
}

void MainWindow::sortList(const QString& str)
{
    if (str == "Name") {
        // Alphabetic order
        for (unsigned int i = 0; i < container.getSize() - 1; ++i) {
            unsigned int pos = i;
            for (unsigned int j = i + 1; j < container.getSize(); ++j) {
                if (container[j]->getName() < container[pos]->getName())
                    pos = j;
            }
            if (pos != i) {
                DeepPtr<Character> temp = container[i];
                container[i] = container[pos];
                container[pos] = temp;
            }
        }
    }
    else if (str == "Gender") {
        // Alphabetic order
        for (unsigned int i = 0; i < container.getSize() - 1; ++i) {
            unsigned int pos = i;
            for (unsigned int j = i + 1; j < container.getSize(); ++j) {
                if (container[j]->getGender() < container[pos]->getGender())
                    pos = j;
            }
            if (pos != i) {
                DeepPtr<Character> temp = container[i];
                container[i] = container[pos];
                container[pos] = temp;
            }
        }
    }
    else if (str == "Race") {
        // Alphabetic order
        for (unsigned int i = 0; i < container.getSize() - 1; ++i) {
            unsigned int pos = i;
            for (unsigned int j = i + 1; j < container.getSize(); ++j) {
                if (container[j]->getRace() < container[pos]->getRace())
                    pos = j;
            }
            if (pos != i) {
                DeepPtr<Character> temp = container[i];
                container[i] = container[pos];
                container[pos] = temp;
            }
        }
    }
    else if (str == "Faction") {
        // Alphabetic order
        for (unsigned int i = 0; i < container.getSize() - 1; ++i) {
            unsigned int pos = i;
            for (unsigned int j = i + 1; j < container.getSize(); ++j) {
                if (container[j]->getFaction() < container[pos]->getFaction())
                    pos = j;
            }
            if (pos != i) {
                DeepPtr<Character> temp = container[i];
                container[i] = container[pos];
                container[pos] = temp;
            }
        }
    }
    else if (str == "Level") {
        // Decreasing order
        for (unsigned int i = 0; i < container.getSize() - 1; ++i) {
            unsigned int pos = i;
            for (unsigned int j = i + 1; j < container.getSize(); ++j) {
                if (container[j]->getLevel() > container[pos]->getLevel())
                    pos = j;
            }
            if (pos != i) {
                DeepPtr<Character> temp = container[i];
                container[i] = container[pos];
                container[pos] = temp;
            }
        }
    }
    else if (str == "HP") {
        // Decreasing order
        for (unsigned int i = 0; i < container.getSize() - 1; ++i) {
            unsigned int pos = i;
            for (unsigned int j = i + 1; j < container.getSize(); ++j) {
                if (container[j]->getHP() > container[pos]->getHP())
                    pos = j;
            }
            if (pos != i) {
                DeepPtr<Character> temp = container[i];
                container[i] = container[pos];
                container[pos] = temp;
            }
        }
    }
    else if (str == "Status") {
        // Reverse alphabetic order
        for (unsigned int i = 0; i < container.getSize() - 1; ++i) {
            Player* player1 = dynamic_cast<Player*>(container[i].operator->());
            if (player1) {
                unsigned int pos = i;
                for (unsigned int j = i + 1; j < container.getSize(); ++j) {
                    Player* player2 = dynamic_cast<Player*>(container[j].operator->());
                    if (player2) {
                        if (player2->getStatus() > player1->getStatus())
                            pos = j;
                    }
                }
                if (pos != i) {
                    DeepPtr<Character> temp = container[i];
                    container[i] = container[pos];
                    container[pos] = temp;
                }
            }
        }
    }
    else if (str == "Rating") {
        // Decreasing order
        for (unsigned int i = 0; i < container.getSize() - 1; ++i) {
            Player* player1 = dynamic_cast<Player*>(container[i].operator->());
            if (player1) {
                unsigned int pos = i;
                for (unsigned int j = i + 1; j < container.getSize(); ++j) {
                    Player* player2 = dynamic_cast<Player*>(container[j].operator->());
                    if (player2) {
                        if (player2->getRating() > player1->getRating())
                            pos = j;
                    }
                }
                if (pos != i) {
                    DeepPtr<Character> temp = container[i];
                    container[i] = container[pos];
                    container[pos] = temp;
                }
            }
        }
    }
}

void MainWindow::buildList(const QString& searchStr, const QString& showStr, const QString& sortStr)
{
    if (characterList->count() != 0) {
        characterList->reset();
        characterList->clear();
    }
    sortList(sortStr);
    if (sortStr == "Status" || sortStr == "Rating")
        showList(showStr, true);
    else showList(showStr, false);
    searchList(searchStr);
}

int MainWindow::searchItem(int row)
{
    if (row != -1) {
        QListWidgetItem* listItem = characterList->item(row);
        string charName = listItem->text().toStdString();
        for (unsigned int i = 0; i < container.getSize(); ++i) {
            if (container[i]->getName() == charName)
                return static_cast<int>(i);
        }
    }
    return -1;
}

void MainWindow::resetFilters() const
{
    searchBox->clear();
    showBox->setCurrentIndex(0);
    sortBox->setCurrentIndex(0);
}

void MainWindow::createInfoLayout(int pos)
{   
    if (pos != -1) {
        unsigned int u_pos = static_cast<unsigned int>(pos);

        // Pixmaps
        if (container[u_pos]->getGender() == "Male") {
            if (container[u_pos]->getRace() == "Draenei") racePixmap.load(":/img/race_draenei_male.jpg");
            else if (container[u_pos]->getRace() == "Dwarf") racePixmap.load(":/img/race_dwarf_male.jpg");
            else if (container[u_pos]->getRace() == "Gnome") racePixmap.load(":/img/race_gnome_male.jpg");
            else if (container[u_pos]->getRace() == "Human") racePixmap.load(":/img/race_human_male.jpg");
            else if (container[u_pos]->getRace() == "Night Elf") racePixmap.load(":/img/race_nightelf_male.jpg");
            else if (container[u_pos]->getRace() == "Blood Elf") racePixmap.load(":/img/race_bloodelf_male.jpg");
            else if (container[u_pos]->getRace() == "Orc") racePixmap.load(":/img/race_orc_male.jpg");
            else if (container[u_pos]->getRace() == "Tauren") racePixmap.load(":/img/race_tauren_male.jpg");
            else if (container[u_pos]->getRace() == "Troll") racePixmap.load(":/img/race_troll_male.jpg");
            else if (container[u_pos]->getRace() == "Undead") racePixmap.load(":/img/race_undead_male.jpg");
        }
        else if (container[u_pos]->getGender() == "Female") {
            if (container[u_pos]->getRace() == "Draenei") racePixmap.load(":/img/race_draenei_female.jpg");
            else if (container[u_pos]->getRace() == "Dwarf") racePixmap.load(":/img/race_dwarf_female.jpg");
            else if (container[u_pos]->getRace() == "Gnome") racePixmap.load(":/img/race_gnome_female.jpg");
            else if (container[u_pos]->getRace() == "Human") racePixmap.load(":/img/race_human_female.jpg");
            else if (container[u_pos]->getRace() == "Night Elf") racePixmap.load(":/img/race_nightelf_female.jpg");
            else if (container[u_pos]->getRace() == "Blood Elf") racePixmap.load(":/img/race_bloodelf_female.jpg");
            else if (container[u_pos]->getRace() == "Orc") racePixmap.load(":/img/race_orc_female.jpg");
            else if (container[u_pos]->getRace() == "Tauren") racePixmap.load(":/img/race_tauren_female.jpg");
            else if (container[u_pos]->getRace() == "Troll") racePixmap.load(":/img/race_troll_female.jpg");
            else if (container[u_pos]->getRace() == "Undead") racePixmap.load(":/img/race_undead_female.jpg");
        }
        racePixmap = racePixmap.scaledToWidth(70);

        if (container[u_pos]->getClass() == "NPC") {
            NPC* npc = dynamic_cast<NPC*>(container[u_pos].operator->());
            if (npc->getType() == "Normal") classPixmap.load(":/img/mobtype_normal.jpg");
            else if (npc->getType() == "Elite") classPixmap.load(":/img/mobtype_elite.jpg");
            else if (npc->getType() == "Boss") classPixmap.load(":/img/mobtype_boss.jpg");
        }
        else if (container[u_pos]->getClass() == "Mage") classPixmap.load(":/img/classicon_mage.jpg");
        else if (container[u_pos]->getClass() == "Priest") classPixmap.load(":/img/classicon_priest.jpg");
        else if (container[u_pos]->getClass() == "Rogue") classPixmap.load(":/img/classicon_rogue.jpg");
        else if (container[u_pos]->getClass() == "Warrior") classPixmap.load(":/img/classicon_warrior.jpg");
        classPixmap = classPixmap.scaledToWidth(70);

        if (container[u_pos]->getFaction() == "Neutral") factionPixmap.load(":/img/pixmap_neutral.jpg");
        else if (container[u_pos]->getFaction() == "Alliance") factionPixmap.load(":/img/pixmap_alliance.jpg");
        else if (container[u_pos]->getFaction() == "Horde") factionPixmap.load(":/img/pixmap_horde.jpg");
        factionPixmap = factionPixmap.scaledToWidth(70);

        // Place pixmaps into labels
        QLabel* raceLabel = new QLabel;
        raceLabel->setPixmap(racePixmap);
        raceLabel->setToolTip(QString::fromStdString(container[u_pos]->getRace()));
        QLabel* classLabel = new QLabel;
        classLabel->setPixmap(classPixmap);
        classLabel->setToolTip(QString::fromStdString(container[u_pos]->getClass()));
        QLabel* factionLabel = new QLabel;
        factionLabel->setPixmap(factionPixmap);
        factionLabel->setToolTip(QString::fromStdString(container[u_pos]->getFaction()));

        // Pixmaps layout
        QHBoxLayout* pixmapsLayout = new QHBoxLayout;
        pixmapsLayout->addWidget(raceLabel);
        pixmapsLayout->addWidget(classLabel);
        pixmapsLayout->addWidget(factionLabel);
        pixmapsLayout->setAlignment(raceLabel, Qt::AlignLeft);
        pixmapsLayout->setAlignment(classLabel, Qt::AlignCenter);
        pixmapsLayout->setAlignment(factionLabel, Qt::AlignRight);

        // Basic info
        QFormLayout* formLayout = new QFormLayout;
        formLayout->setHorizontalSpacing(10);
        formLayout->addRow("Name:", new QLabel(QString::fromStdString(container[u_pos]->getName())));
        formLayout->addRow("Gender:", new QLabel(QString::fromStdString(container[u_pos]->getGender())));
        //formLayout->addRow("Race:", new QLabel(QString::fromStdString(container[u_pos]->getRace())));
        //formLayout->addRow("Class:", new QLabel(QString::fromStdString(container[u_pos]->getClass())));
        //formLayout->addRow("Faction:", new QLabel(QString::fromStdString(container[u_pos]->getFaction())));
        formLayout->addRow("Level:", new QLabel(QString::number(container[u_pos]->getLevel())));
        formLayout->addRow("HP:", new QLabel(QString::number(container[u_pos]->getHP())));
        formLayout->addRow(createLine());

        // NPC info
        NPC* npc = dynamic_cast<NPC*>(container[u_pos].operator->());
        if (npc) {
            formLayout->addRow("Type:", new QLabel(QString::fromStdString(npc->getType())));
            formLayout->addRow("Merchandise:", new QLabel(QString::fromStdString(npc->getMerchandise())));
            QLabel* questLabel = new QLabel(QString::fromStdString(npc->getQuest()));
            questLabel->setWordWrap(true);
            formLayout->addRow("Quest:", questLabel);
        }

        // Player info
        Player* player = dynamic_cast<Player*>(container[u_pos].operator->());
        if (player) {
            formLayout->addRow("Status:", new QLabel(QString::fromStdString(player->getStatus())));
            formLayout->addRow("Rating:", new QLabel(QString::number(player->getRating())));
            formLayout->addRow(createLine());
            formLayout->addRow("Strength:", new QLabel(QString::number(player->getStrength())));
            formLayout->addRow("Agility:", new QLabel(QString::number(player->getAgility())));
            formLayout->addRow("Intellect:", new QLabel(QString::number(player->getIntellect())));
            formLayout->addRow("Spirit:", new QLabel(QString::number(player->getSpirit())));
            formLayout->addRow(createLine());
            formLayout->addRow("Attack Power:", new QLabel(QString::number(player->attackPower())));
            formLayout->addRow("Spell Power:", new QLabel(QString::number(player->spellPower())));
            formLayout->addRow("Armor:", new QLabel(QString::fromStdString(player->armor())));
            formLayout->addRow("Role:", new QLabel(QString::fromStdString(player->role())));
        }

        infoLayout->addLayout(pixmapsLayout);
        infoLayout->addWidget(createLine());
        infoLayout->addLayout(formLayout);
        infoLayout->addStretch();
    }
}

void MainWindow::createInsertLayout()
{
    /******************** BASIC INFO ********************/
    // Class edit
    QStringList classList = {"", "NPC", "Mage", "Priest", "Rogue", "Warrior"};
    QComboBox* classEdit = new QComboBox;
    classEdit->addItems(classList);
    classEdit->insertSeparator(2);

    // Name edit
    QLineEdit* nameEdit = new QLineEdit();
    nameEdit->setMaxLength(20);

    // Gender edit
    QComboBox* genderEdit = new QComboBox;
    genderEdit->addItem("Male");
    genderEdit->addItem("Female");

    // Race edit
    QComboBox* raceEdit = new QComboBox;
    raceEdit->insertItem(0, "Draenei");
    raceEdit->insertItem(1, "Dwarf");
    raceEdit->insertItem(2, "Gnome");
    raceEdit->insertItem(3, "Human");
    raceEdit->insertItem(4, "Night Elf");
    raceEdit->insertSeparator(5);
    raceEdit->insertItem(6, "Blood Elf");
    raceEdit->insertItem(7, "Orc");
    raceEdit->insertItem(8, "Tauren");
    raceEdit->insertItem(9, "Troll");
    raceEdit->insertItem(10, "Undead");

    // Faction edit
    QComboBox* factionEdit = new QComboBox;
    factionEdit->addItem("Neutral");
    factionEdit->addItem("Alliance");
    factionEdit->addItem("Horde");

    // Level edit
    QLineEdit* levelEdit = new QLineEdit();
    QValidator* levelValidator = new QIntValidator(1, 80);
    levelEdit->setValidator(levelValidator);

    // HP edit
    QLineEdit* HPEdit = new QLineEdit();
    QValidator* HPValidator = new QIntValidator(1, 999999);
    HPEdit->setValidator(HPValidator);

    QFormLayout* formLayout = new QFormLayout;
    formLayout->insertRow(0, "Class", classEdit);
    formLayout->insertRow(1, "Name", nameEdit);
    formLayout->insertRow(2, "Gender", genderEdit);
    formLayout->insertRow(3, "Race", raceEdit);
    formLayout->insertRow(4, "Faction", factionEdit);
    formLayout->insertRow(5, "Level", levelEdit);
    formLayout->insertRow(6, "HP", HPEdit);
    formLayout->insertRow(7, createLine());

    /******************** NPC INFO ********************/
    QStringList typeList = {"Normal", "Elite", "Boss"};
    QStringList merchList = {"None", "Weapons", "Armors", "Containers", "Consumables", "Recipes", "Gems", "Miscellaneous"};

    // Type edit
    QComboBox* typeEdit = new QComboBox;
    typeEdit->addItems(typeList);

    // Merchandise edit
    QComboBox* merchEdit = new QComboBox;
    merchEdit->addItems(merchList);

    // Quest edit
    QTextEdit* questEdit = new QTextEdit();

    /******************** PLAYER INFO ********************/
    QStringList statusList = {"Offline", "Online"};

    // Status edit
    QComboBox* statusEdit = new QComboBox;
    statusEdit->addItems(statusList);

    // Rating edit
    QLineEdit* ratingEdit = new QLineEdit();
    QValidator* ratingValidator = new QIntValidator(0, 4000);
    ratingEdit->setValidator(ratingValidator);

    // Stats edit
    QValidator* statValidator = new QIntValidator(0, 2000);

    QLineEdit* strengthEdit = new QLineEdit();
    QLineEdit* agilityEdit = new QLineEdit();
    QLineEdit* intellectEdit = new QLineEdit();
    QLineEdit* spiritEdit = new QLineEdit();
    strengthEdit->setValidator(statValidator);
    agilityEdit->setValidator(statValidator);
    intellectEdit->setValidator(statValidator);
    spiritEdit->setValidator(statValidator);

    // Specific widgets
    QFormLayout* npcLayout = new QFormLayout;
    npcLayout->addRow("Type", typeEdit);
    npcLayout->addRow("Merchandise", merchEdit);
    npcLayout->addRow("Quest", questEdit);

    QFormLayout* playerLayout = new QFormLayout;
    playerLayout->addRow("Status", statusEdit);
    playerLayout->addRow("Rating", ratingEdit);
    playerLayout->addRow(createLine());
    playerLayout->addRow("Strength", strengthEdit);
    playerLayout->addRow("Agility", agilityEdit);
    playerLayout->addRow("Intellect", intellectEdit);
    playerLayout->addRow("Spirit", spiritEdit);

    QWidget* npcWidget = new QWidget;
    npcWidget->setLayout(npcLayout);
    npcWidget->setVisible(false);
    QWidget* playerWidget = new QWidget;
    playerWidget->setLayout(playerLayout);
    playerWidget->setVisible(false);

    // Buttons
    QPushButton* backButton = new QPushButton("Back");
    QPushButton* insertButton = new QPushButton("Insert");
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(backButton);
    buttonsLayout->addWidget(insertButton);

    // Connections
    connect(classEdit, &QComboBox::currentTextChanged, [classEdit, npcWidget, playerWidget] {
        if (classEdit->currentText().isEmpty()) {
            npcWidget->setVisible(false);
            playerWidget->setVisible(false);
        }
        else if (classEdit->currentText() == "NPC") {
            npcWidget->setVisible(true);
            playerWidget->setVisible(false);
        }
        else {
            npcWidget->setVisible(false);
            playerWidget->setVisible(true);
        }
    });
    connect(factionEdit, &QComboBox::currentTextChanged, [factionEdit, raceEdit] {
        if (factionEdit->currentText() == "Alliance" && raceEdit->currentIndex() > 5)
            raceEdit->setCurrentIndex(0);
        else if (factionEdit->currentText() == "Horde" && raceEdit->currentIndex() < 5)
            raceEdit->setCurrentIndex(6);
    });
    connect(raceEdit, &QComboBox::currentTextChanged, [factionEdit, raceEdit] {
       if (raceEdit->currentIndex() < 5 && factionEdit->currentText() == "Horde")
            factionEdit->setCurrentText("Alliance");
       else if (raceEdit->currentIndex() > 5 && factionEdit->currentText() == "Alliance")
           factionEdit->setCurrentText("Horde");
    });
    connect(backButton, &QPushButton::clicked, [this] {
        clearLayout(insertLayout);
        if (!container.getSize()) stackedWidget->hide();
        else stackedWidget->setCurrentWidget(infoWidget);
        leftWidget->setDisabled(false);
        middleWidget->setDisabled(false);
    });
    connect(insertButton, &QPushButton::clicked, [this, classEdit, nameEdit, genderEdit, raceEdit, factionEdit, levelEdit, HPEdit, typeEdit, merchEdit, questEdit, statusEdit, ratingEdit, strengthEdit, agilityEdit, intellectEdit, spiritEdit] {
        if (classEdit->currentText().isEmpty() || nameEdit->text().isEmpty() || levelEdit->text().isEmpty() || HPEdit->text().isEmpty()) {
            QMessageBox msgBox;
            if (classEdit->currentText().isEmpty()) msgBox.setText("Class field cannot be empty.");
            else if (nameEdit->text().isEmpty()) msgBox.setText("Name field cannot be empty.");
            else if (levelEdit->text().isEmpty()) msgBox.setText("Level field cannot be empty.");
            else if (HPEdit->text().isEmpty()) msgBox.setText("HP field cannot be empty.");
            msgBox.exec();
        }
        else {
            DeepPtr<Character> character;
            if (classEdit->currentText() == "NPC") {
                character = new NPC();
                character->setName(nameEdit->text().toStdString());
                character->setGender(genderEdit->currentText().toStdString());
                character->setRace(raceEdit->currentText().toStdString());
                character->setFaction(factionEdit->currentText().toStdString());
                character->setLevel(levelEdit->text().toInt());
                character->setHP(HPEdit->text().toInt());
                NPC* npc = dynamic_cast<NPC*>(character.operator->());
                if (npc) {
                    npc->setType(typeEdit->currentText().toStdString());
                    npc->setMerchandise(merchEdit->currentText().toStdString());
                    if (questEdit->toPlainText().length() > 100) {
                        QMessageBox msgBox;
                        msgBox.setText("Box maximum length is 100 characters.");
                        msgBox.exec();
                    }
                    else npc->setQuest(questEdit->toPlainText().toStdString());
                }
            }
            else {
                if (classEdit->currentText() == "Mage") character = new Mage();
                else if (classEdit->currentText() == "Priest") character = new Priest();
                else if (classEdit->currentText() == "Rogue") character = new Rogue();
                else if (classEdit->currentText() == "Warrior") character = new Warrior();

                character->setName(nameEdit->text().toStdString());
                character->setGender(genderEdit->currentText().toStdString());
                character->setRace(raceEdit->currentText().toStdString());
                character->setFaction(factionEdit->currentText().toStdString());
                character->setLevel(levelEdit->text().toInt());
                character->setHP(HPEdit->text().toInt());
                Player* player = dynamic_cast<Player*>(character.operator->());
                if (player) {
                    player->setStatus(statusEdit->currentText().toStdString());
                    player->setRating(ratingEdit->text().toInt());
                    player->setStrength(strengthEdit->text().toInt());
                    player->setAgility(agilityEdit->text().toInt());
                    player->setIntellect(intellectEdit->text().toInt());
                    player->setSpirit(spiritEdit->text().toInt());
                }
            }
            container.insert(character);
            updateList();
            resetFilters();
            clearLayout(insertLayout);
            clearLayout(infoLayout);
            characterList->setCurrentRow(characterList->count()-1);
            leftWidget->setDisabled(false);
            middleWidget->setDisabled(false);
        }
    });

    insertLayout->addLayout(formLayout);
    insertLayout->addWidget(npcWidget);
    insertLayout->addWidget(playerWidget);
    insertLayout->addLayout(buttonsLayout);
    insertLayout->addStretch();
}

void MainWindow::createModifyLayout(int pos)
{
    if (pos != -1) {
        unsigned int u_pos = static_cast<unsigned int>(pos);
        QString oldName = QString::fromStdString(container[u_pos]->getName());

        /******************** BASIC INFO ********************/
        // Class edit
        QLineEdit* classEdit = new QLineEdit(QString::fromStdString(container[u_pos]->getClass()));
        classEdit->setDisabled(true);

        // Name edit
        QLineEdit* nameEdit = new QLineEdit(QString::fromStdString(container[u_pos]->getName()));
        nameEdit->setMaxLength(20);

        // Gender edit
        QComboBox* genderEdit = new QComboBox;
        genderEdit->addItem("Male");
        genderEdit->addItem("Female");
        genderEdit->setCurrentText(QString::fromStdString(container[u_pos]->getGender()));

        // Race edit
        QComboBox* raceEdit = new QComboBox;
        raceEdit->insertItem(0, "Draenei");
        raceEdit->insertItem(1, "Dwarf");
        raceEdit->insertItem(2, "Gnome");
        raceEdit->insertItem(3, "Human");
        raceEdit->insertItem(4, "Night Elf");
        raceEdit->insertSeparator(5);
        raceEdit->insertItem(6, "Blood Elf");
        raceEdit->insertItem(7, "Orc");
        raceEdit->insertItem(8, "Tauren");
        raceEdit->insertItem(9, "Troll");
        raceEdit->insertItem(10, "Undead");
        raceEdit->setCurrentText(QString::fromStdString(container[u_pos]->getRace()));

        // Faction edit
        QComboBox* factionEdit = new QComboBox;
        factionEdit->addItem("Neutral");
        factionEdit->addItem("Alliance");
        factionEdit->addItem("Horde");
        factionEdit->setCurrentText(QString::fromStdString(container[u_pos]->getFaction()));

        // Level edit
        QLineEdit* levelEdit = new QLineEdit(QString::number(container[u_pos]->getLevel()));
        QValidator* levelValidator = new QIntValidator(1, 80);
        levelEdit->setValidator(levelValidator);

        // HP edit
        QLineEdit* HPEdit = new QLineEdit(QString::number(container[u_pos]->getHP()));
        QValidator* HPValidator = new QIntValidator(1, 999999);
        HPEdit->setValidator(HPValidator);

        QFormLayout* formLayout = new QFormLayout;
        formLayout->addRow("Class", classEdit);
        formLayout->addRow("Name", nameEdit);
        formLayout->addRow("Gender", genderEdit);
        formLayout->addRow("Race", raceEdit);
        formLayout->addRow("Faction", factionEdit);
        formLayout->addRow("Level", levelEdit);
        formLayout->addRow("HP", HPEdit);
        formLayout->addRow(createLine());

        /******************** NPC INFO ********************/
        QComboBox* typeEdit = nullptr;
        QComboBox* merchEdit = nullptr;
        QTextEdit* questEdit = nullptr;

        NPC* npc = dynamic_cast<NPC*>(container[u_pos].operator->());
        if (npc) {
            QStringList typeList = {"Normal", "Elite", "Boss"};
            QStringList merchList = {"None", "Weapons", "Armors", "Containers", "Consumables", "Recipes", "Gems", "Miscellaneous"};

            // Type edit
            typeEdit = new QComboBox;
            typeEdit->addItems(typeList);
            typeEdit->setCurrentText(QString::fromStdString(npc->getType()));

            // Merchandise edit
            merchEdit = new QComboBox;
            merchEdit->addItems(merchList);
            merchEdit->setCurrentText(QString::fromStdString(npc->getMerchandise()));

            // Quest edit
            questEdit = new QTextEdit(QString::fromStdString(npc->getQuest()));

            formLayout->addRow("Type", typeEdit);
            formLayout->addRow("Merchandise", merchEdit);
            formLayout->addRow("Quest", questEdit);
        }

        /******************** PLAYER INFO ********************/
        QComboBox* statusEdit = nullptr;
        QLineEdit* ratingEdit = nullptr;
        QLineEdit* strengthEdit = nullptr;
        QLineEdit* agilityEdit = nullptr;
        QLineEdit* intellectEdit = nullptr;
        QLineEdit* spiritEdit = nullptr;

        Player* player = dynamic_cast<Player*>(container[u_pos].operator->());
        if (player) {
            QStringList statusList = {"Offline", "Online"};

            // Status edit
            statusEdit = new QComboBox;
            statusEdit->addItems(statusList);
            statusEdit->setCurrentText(QString::fromStdString(player->getStatus()));

            // Rating edit
            ratingEdit = new QLineEdit(QString::number(player->getRating()));
            QValidator* ratingValidator = new QIntValidator(0, 3000);
            ratingEdit->setValidator(ratingValidator);

            // Stats edit
            QValidator* statValidator = new QIntValidator(0, 2000);

            strengthEdit = new QLineEdit(QString::number(player->getStrength()));
            agilityEdit = new QLineEdit(QString::number(player->getAgility()));
            intellectEdit = new QLineEdit(QString::number(player->getIntellect()));
            spiritEdit = new QLineEdit(QString::number(player->getSpirit()));
            strengthEdit->setValidator(statValidator);
            agilityEdit->setValidator(statValidator);
            intellectEdit->setValidator(statValidator);
            spiritEdit->setValidator(statValidator);

            formLayout->addRow("Status", statusEdit);
            formLayout->addRow("Rating", ratingEdit);
            formLayout->addRow(createLine());
            formLayout->addRow("Strength", strengthEdit);
            formLayout->addRow("Agility", agilityEdit);
            formLayout->addRow("Intellect", intellectEdit);
            formLayout->addRow("Spirit", spiritEdit);
        }

        // Buttons
        QPushButton* backButton = new QPushButton("Back");
        QPushButton* updateButton = new QPushButton("Update");
        QHBoxLayout* buttonsLayout = new QHBoxLayout;
        buttonsLayout->addWidget(backButton);
        buttonsLayout->addWidget(updateButton);

        // Connections
        connect(factionEdit, &QComboBox::currentTextChanged, [factionEdit, raceEdit] {
            if (factionEdit->currentText() == "Alliance" && raceEdit->currentIndex() > 5)
                raceEdit->setCurrentIndex(0);
            else if (factionEdit->currentText() == "Horde" && raceEdit->currentIndex() < 5)
                raceEdit->setCurrentIndex(6);
        });
        connect(raceEdit, &QComboBox::currentTextChanged, [factionEdit, raceEdit] {
           if (raceEdit->currentIndex() < 5 && factionEdit->currentText() == "Horde")
                factionEdit->setCurrentText("Alliance");
           else if (raceEdit->currentIndex() > 5 && factionEdit->currentText() == "Alliance")
               factionEdit->setCurrentText("Horde");
        });
        connect(backButton, &QPushButton::clicked, [this] {
            clearLayout(modifyLayout);
            stackedWidget->setCurrentWidget(infoWidget);
            leftWidget->setDisabled(false);
            middleWidget->setDisabled(false);
        });
        connect(updateButton, &QPushButton::clicked, [this, u_pos, oldName, nameEdit, genderEdit, raceEdit, factionEdit, levelEdit, HPEdit, npc, typeEdit, merchEdit, questEdit, player, statusEdit, ratingEdit, strengthEdit, agilityEdit, intellectEdit, spiritEdit] {
            if (nameEdit->text().isEmpty() || levelEdit->text().isEmpty() || HPEdit->text().isEmpty()) {
                QMessageBox msgBox;
                if (nameEdit->text().isEmpty()) msgBox.setText("Name field cannot be empty.");
                else if (levelEdit->text().isEmpty()) msgBox.setText("Level field cannot be empty.");
                else if (HPEdit->text().isEmpty()) msgBox.setText("HP field cannot be empty.");
                msgBox.exec();
            }
            else {
                container[u_pos]->setName(nameEdit->text().toStdString());
                container[u_pos]->setGender(genderEdit->currentText().toStdString());
                container[u_pos]->setRace(raceEdit->currentText().toStdString());
                container[u_pos]->setFaction(factionEdit->currentText().toStdString());
                container[u_pos]->setLevel(levelEdit->text().toInt());
                container[u_pos]->setHP(HPEdit->text().toInt());
                if (npc) {
                    npc->setType(typeEdit->currentText().toStdString());
                    npc->setMerchandise(merchEdit->currentText().toStdString());
                    if (questEdit->toPlainText().length() > 100) {
                        QMessageBox msgBox;
                        msgBox.setText("Box maximum length is 100 characters.");
                        msgBox.exec();
                    }
                    else npc->setQuest(questEdit->toPlainText().toStdString());
                }
                else if (player) {
                    player->setStatus(statusEdit->currentText().toStdString());
                    player->setRating(ratingEdit->text().toInt());
                    player->setStrength(strengthEdit->text().toInt());
                    player->setAgility(agilityEdit->text().toInt());
                    player->setIntellect(intellectEdit->text().toInt());
                    player->setSpirit(spiritEdit->text().toInt());
                }
                clearLayout(modifyLayout);
                clearLayout(infoLayout);
                if (nameEdit->text() != oldName) {
                    QListWidgetItem* listItem = characterList->currentItem();
                    listItem->setText(nameEdit->text());
                }
                createInfoLayout(searchItem(characterList->currentRow()));
                updateList();
                buildList(searchBox->text(), showBox->currentText(), sortBox->currentText());
                stackedWidget->setCurrentWidget(infoWidget);
                leftWidget->setDisabled(false);
                middleWidget->setDisabled(false);
            }
        });

        modifyLayout->addLayout(formLayout);
        modifyLayout->addLayout(buttonsLayout);
    }
}

void MainWindow::clearLayout(QLayout* layout){
    while(layout->count() > 0){
        QLayoutItem* item = layout->takeAt(0);
        if(QWidget* widget = item->widget()){
            widget->deleteLater();
        }
        else if (QLayout* innerLayout = item->layout()) {
            clearLayout(innerLayout);
        }
        delete item;
    }
}

void MainWindow::load()
{
    QString fileName = QFileDialog::getOpenFileName(this->toolbar, tr("Open container"), "../WoWDB/data", tr("JSON files (*.json)"));
    if (!fileName.isEmpty()) {
        if (!fileName.endsWith(".json")) {
            QMessageBox msgBox;
            msgBox.setText("Invalid format. Please select a .json file.");
            msgBox.exec();
        }
        else {
            QFile loadFile(fileName);
            loadFile.open(QIODevice::ReadOnly);
            QByteArray dataArray = loadFile.readAll();
            loadFile.close();
            QJsonDocument docJson = QJsonDocument::fromJson(dataArray);
            QJsonArray arrayJson = docJson.array();
            if (arrayJson.isEmpty()) {
                QMessageBox msgBox;
                msgBox.setText("The file is empty.");
                msgBox.exec();
            }
            else {
                if (container.getSize() != 0)
                    container.clear();
                foreach (const QJsonValue& value, arrayJson) {
                    QJsonObject obj = value.toObject();
                    if (obj.contains("class") && obj["class"].isString()) {
                        QString charClass = obj["class"].toString();
                        DeepPtr<Character> character;
                        if (charClass == "NPC") character = new NPC();
                        else if (charClass == "Mage") character = new Mage();
                        else if (charClass == "Priest") character = new Priest();
                        else if (charClass == "Rogue") character = new Rogue();
                        else if (charClass == "Warrior") character = new Warrior();
                        character->deserialize(obj);
                        container.insert(character);
                    }
                }
                clearLayout(infoLayout);
                updateList();
            }
        }
    }
}

void MainWindow::save() const
{
    if (container.getSize() != 0) {
        QString fileName = QFileDialog::getSaveFileName(this->toolbar, tr("Save container"), "../WoWDB/data", tr("JSON files (*.json)"));
        if (!fileName.endsWith(".json"))
            fileName.append(".json");
        QJsonArray arrayJson;
        for (unsigned int i = 0; i < container.getSize(); ++i)
            arrayJson.push_back(QJsonValue(container[i]->serialize()));
        QJsonDocument docJson(arrayJson);
        QString docString = docJson.toJson();
        QFile saveFile(fileName);
        saveFile.open(QIODevice::WriteOnly);
        saveFile.write(docString.toUtf8());
        saveFile.close();
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("The container is empty.");
        msgBox.exec();
    }
}

bool MainWindow::searchName(const string& str) const
{
    for (unsigned int i = 0; i < container.getSize(); ++i) {
        if (container[i]->getName() == str) return true;
    }
    return false;
}

QFrame* MainWindow::createLine()
{
    QFrame* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    return line;
}
