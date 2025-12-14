#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QStatusBar>
#include <QMenuBar>
#include <QToolBar>
#include "RentalSystem.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Car management slots
    void onAddCar();
    void onEditCar();
    void onDeleteCar();
    void onUpdateCarStatus();
    void onCarFilterChanged(int index);

    // Transaction slots
    void onCreateRental();
    void onReturnCar();

    // Table selection slots
    void onCarTableSelectionChanged();
    void onTransactionTableSelectionChanged();

    // Menu slots
    void onAbout();
    void onRefresh();

private:
    // Setup methods
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupCarTab();
    void setupTransactionTab();
    void setupConnections();

    // Data refresh methods
    void refreshCarTable();
    void refreshTransactionTable();
    void updateStatusBar();

    // Helper methods
    QString getStatusString(CarStatus status);
    QString getTransactionStatusString(TransactionStatus status);
    void showMessage(const QString& title, const QString& message);
    void showError(const QString& title, const QString& message);

    // UI Components
    QTabWidget *tabWidget;

    // Car tab components
    QWidget *carTab;
    QTableWidget *carTable;
    QPushButton *btnAddCar;
    QPushButton *btnEditCar;
    QPushButton *btnDeleteCar;
    QPushButton *btnUpdateStatus;
    QComboBox *carFilterCombo;
    QLabel *carCountLabel;

    // Transaction tab components
    QWidget *transactionTab;
    QTableWidget *transactionTable;
    QPushButton *btnCreateRental;
    QPushButton *btnReturnCar;
    QComboBox *transactionFilterCombo;
    QLabel *transactionCountLabel;

    // Business logic
    RentalSystem *rentalSystem;
};

#endif // MAINWINDOW_H