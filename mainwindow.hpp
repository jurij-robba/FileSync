#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QDir>
#include <QListWidgetItem>

#include "messagedialog.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

private:

	const QString setting_source_dir = "source_dir";
	const QString setting_destination_dir = "destination_dir";

	bool source_ok = false;
	bool destination_ok = false;

	MessageDialog _please_wait;

    Ui::MainWindow *ui;

    void set_progress_text(const QString &msg);

private slots:

	// 1
	void on_source_button_clicked();
	void on_destination_button_clicked();
	void on_go_button_clicked();
	void on_source_edit_textChanged(const QString &arg1);
	void on_destination_edit_textChanged(const QString &arg1);

	// 2
	void on_top_panel_button_1_clicked();
	void on_new_files_list_itemClicked(QListWidgetItem *item);
	void on_updated_files_list_itemClicked(QListWidgetItem *item);
	void on_deleted_files_list_itemClicked(QListWidgetItem *item);
	void on_new_files_checkbox_clicked(bool checked);
	void on_updated_files_checkbox_clicked(bool checked);
	void on_deleted_files_checkbox_clicked(bool checked);
	void on_transfer_button_clicked();

	// 3
	void on_close_button_clicked();

signals:

	void set_source(const QString& path);
	void set_destination(const QString& path);
	void build_lists();
	void synchronise(const QStringList& new_files, const QStringList& updated_files,
					 const QStringList& deleted_files);

	void close_wait_dialog();

public:

	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

public slots:

	void source_exists(const bool& status);
	void destinations_exists(const bool& status);

	void new_files(const QStringList& files);
	void update_files(const QStringList& files);
	void delete_files(const QStringList& files);

	void total_progress(const int& val);
	void current_progress(const int& val, const QString& message);
	void error(const QString& message);
	void done();
};

#endif
