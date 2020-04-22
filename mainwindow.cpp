#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDirIterator>
#include <QDateTime>
#include <QSettings>
#include <QTimer>
#include <QFontMetrics>

void MainWindow::set_progress_text(const QString& msg)
{
    QFontMetrics metrix(ui->progress_status->font());
    int width = ui->progress_status->width() - 2;
    QString clipped = metrix.elidedText(msg, Qt::ElideRight, width);
    ui->progress_status->setText(clipped);
}

// 1
void MainWindow::on_source_button_clicked()
{
	ui->source_edit->setText(
	QFileDialog::getExistingDirectory(this, ("Select Source Folder"),
									  ui->source_edit->text()));
}

void MainWindow::on_destination_button_clicked()
{
	ui->destination_edit->setText(
	QFileDialog::getExistingDirectory(this, ("Select Destination Folder"),
									  ui->destination_edit->text()));
}

void MainWindow::on_go_button_clicked()
{
	emit build_lists();
	_please_wait.show();
}

void MainWindow::on_source_edit_textChanged(const QString &arg1)
{
	emit set_source(arg1);
}

void MainWindow::on_destination_edit_textChanged(const QString &arg1)
{
	emit set_destination(arg1);
}

// 2
void MainWindow::on_top_panel_button_1_clicked()
{
	ui->main_stack->setCurrentIndex(0);
	ui->top_panel_button_2->setChecked(false);
	ui->top_panel_button_3->setChecked(false);
	ui->top_panel_button_4->setChecked(false);
	ui->top_panel_button_1->setEnabled(false);
	ui->top_panel_button_1->setChecked(true);
}

void MainWindow::on_new_files_list_itemClicked(QListWidgetItem *item)
{
	if(item->checkState() == Qt::Checked)
	{
		for(int i = 0; i<ui->new_files_list->count(); ++i)
		{
			if(ui->new_files_list->item(i)->checkState() == Qt::Unchecked)
			{
				ui->new_files_checkbox->setCheckState(Qt::Unchecked);
				return;
			}
		}
		ui->new_files_checkbox->setCheckState(Qt::Checked);
	}
	else
	{
		ui->new_files_checkbox->setCheckState(Qt::Unchecked);
	}
}

void MainWindow::on_updated_files_list_itemClicked(QListWidgetItem *item)
{
	if(item->checkState() == Qt::Checked)
	{
		for(int i = 0; i<ui->updated_files_list->count(); ++i)
		{
			if(ui->updated_files_list->item(i)->checkState() == Qt::Unchecked)
			{
				ui->updated_files_checkbox->setCheckState(Qt::Unchecked);
				return;
			}
		}
		ui->updated_files_checkbox->setCheckState(Qt::Checked);
	}
	else
	{
		ui->updated_files_checkbox->setCheckState(Qt::Unchecked);
	}
}

void MainWindow::on_deleted_files_list_itemClicked(QListWidgetItem *item)
{
	if(item->checkState() == Qt::Checked)
	{
		for(int i = 0; i<ui->deleted_files_list->count(); ++i)
		{
			if(ui->deleted_files_list->item(i)->checkState() == Qt::Unchecked)
			{
				ui->deleted_files_checkbox->setCheckState(Qt::Unchecked);
				return;
			}
		}
		ui->deleted_files_checkbox->setCheckState(Qt::Checked);
	}
	else
	{
		ui->deleted_files_checkbox->setCheckState(Qt::Unchecked);
	}
}

void MainWindow::on_new_files_checkbox_clicked(bool checked)
{
	if(checked)
	{
		for(int i = 0; i<ui->new_files_list->count(); ++i)
		{
			ui->new_files_list->item(i)->setCheckState(Qt::Checked);
		}
	}
	else
	{
		for(int i = 0; i<ui->new_files_list->count(); ++i)
		{
			ui->new_files_list->item(i)->setCheckState(Qt::Unchecked);
		}
	}
}

void MainWindow::on_updated_files_checkbox_clicked(bool checked)
{
	if(checked)
	{
		for(int i = 0; i<ui->updated_files_list->count(); ++i)
		{
			ui->updated_files_list->item(i)->setCheckState(Qt::Checked);
		}
	}
	else
	{
		for(int i = 0; i<ui->updated_files_list->count(); ++i)
		{
			ui->updated_files_list->item(i)->setCheckState(Qt::Unchecked);
		}
	}
}

void MainWindow::on_deleted_files_checkbox_clicked(bool checked)
{
	if(checked)
	{
		for(int i = 0; i<ui->deleted_files_list->count(); ++i)
		{
			ui->deleted_files_list->item(i)->setCheckState(Qt::Checked);
		}
	}
	else
	{
		for(int i = 0; i<ui->deleted_files_list->count(); ++i)
		{
			ui->deleted_files_list->item(i)->setCheckState(Qt::Unchecked);
		}
	}
}

void MainWindow::on_transfer_button_clicked()
{
	QStringList new_files;
	for(int i=0; i< ui->new_files_list->count(); ++i)
	{
		if(ui->new_files_list->item(i)->checkState() == Qt::Checked)
		{
			new_files << ui->new_files_list->item(i)->text();
		}
	}
	QStringList updated_files;
	for(int i=0; i< ui->updated_files_list->count(); ++i)
	{
		if(ui->updated_files_list->item(i)->checkState() == Qt::Checked)
		{
			updated_files << ui->updated_files_list->item(i)->text();
		}
	}
	QStringList deleted_files;
	for(int i=0; i< ui->deleted_files_list->count(); ++i)
	{
		if(ui->deleted_files_list->item(i)->checkState() == Qt::Checked)
		{
			deleted_files << ui->deleted_files_list->item(i)->text();
		}
	}

	emit synchronise(new_files, updated_files, deleted_files);
	ui->main_stack->setCurrentIndex(2);
	ui->top_panel_button_1->setEnabled(false);
	ui->top_panel_button_3->setChecked(true);
}

// 3
void MainWindow::on_close_button_clicked()
{
	QCoreApplication::quit();
}


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), _please_wait("Comparing folders, please wait!"),
	  ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->main_stack->setCurrentIndex(0);

	QObject::connect(this, &MainWindow::close_wait_dialog,
					 &_please_wait, &MessageDialog::on_ok_button_clicked);

	QSettings settings;
	ui->source_edit->setText(settings.value(setting_source_dir).toString());
	ui->destination_edit->setText(settings.value(setting_destination_dir).toString());

	QTimer::singleShot(0, [&](){ emit set_source(ui->source_edit->text()); });
	QTimer::singleShot(0, [&](){ emit set_destination(ui->destination_edit->text()); });

	resize(settings.value("size", QSize(400, 400)).toSize());
	move(settings.value("pos", QPoint(200, 200)).toPoint());
}

MainWindow::~MainWindow()
{
	QSettings settings;
	settings.setValue("size", size());
	settings.setValue("pos", pos());
	delete ui;
}

void MainWindow::source_exists(const bool& status)
{
	source_ok = status;
	ui->go_button->setEnabled(source_ok && destination_ok);
	if(status)
	{
		ui->source_edit->setStyleSheet("color:white;");
		QSettings settings;
		settings.setValue(setting_source_dir, ui->source_edit->text());
	}
	else
	{
		ui->source_edit->setStyleSheet("color:red;");
	}
}

void MainWindow::destinations_exists(const bool& status)
{
	destination_ok = status;
	ui->go_button->setEnabled(source_ok && destination_ok);
	if(status)
	{
		ui->destination_edit->setStyleSheet("color:white;");
		QSettings settings;
		settings.setValue(setting_destination_dir, ui->destination_edit->text());
	}
	else
	{
		ui->destination_edit->setStyleSheet("color:red;");
	}
}

void MainWindow::new_files(const QStringList& files)
{
	ui->new_files_list->clear();
	for(const QString& file : files)
	{
		ui->new_files_list->addItem(file);
		QListWidgetItem* item = ui->new_files_list->item(ui->new_files_list->count()-1);
		item->setSizeHint(QSize(item->sizeHint().width(), 32));
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		item->setCheckState(Qt::Checked);
	}
	if(ui->new_files_list->count() == 0)
	{
		ui->new_files_widget->setVisible(false);
	}
	else
	{
		ui->new_files_widget->setVisible(true);
		ui->new_files_list->setMinimumHeight(ui->new_files_list->count()*32
											 +5);
	}
}

void MainWindow::update_files(const QStringList& files)
{
	ui->updated_files_list->clear();
	for(const QString& file : files)
	{
		ui->updated_files_list->addItem(file);
		QListWidgetItem* item = ui->updated_files_list->item(ui->updated_files_list->count()-1);
		item->setSizeHint(QSize(item->sizeHint().width(), 32));
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		item->setCheckState(Qt::Checked);
	}
	if(ui->updated_files_list->count() == 0)
	{
		ui->updated_files_widget->setVisible(false);
	}
	else
	{
		ui->updated_files_widget->setVisible(true);
		ui->updated_files_list->setMinimumHeight(ui->updated_files_list->count()*32
											 +5);
	}
}

void MainWindow::delete_files(const QStringList& files)
{
	ui->deleted_files_list->clear();
	for(const QString& file : files)
	{
		ui->deleted_files_list->addItem(file);
		QListWidgetItem* item = ui->deleted_files_list->item(ui->deleted_files_list->count()-1);
		item->setSizeHint(QSize(item->sizeHint().width(), 32));
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		item->setCheckState(Qt::Checked);
	}
	if(ui->deleted_files_list->count() == 0)
	{
		ui->deleted_files_widget->setVisible(false);
	}
	else
	{
		ui->deleted_files_widget->setVisible(true);
		ui->deleted_files_list->setMinimumHeight(ui->deleted_files_list->count()*32
											 +5);
	}

	if(ui->new_files_list->count() + ui->updated_files_list->count() +
	   ui->deleted_files_list->count() > 0)
	{
		ui->no_files_text->setVisible(false);
		ui->transfer_button->setEnabled(true);
	}
	else
	{
		ui->no_files_text->setVisible(true);
		ui->transfer_button->setEnabled(false);
	}

	ui->main_stack->setCurrentIndex(1);
	ui->top_panel_button_1->setEnabled(true);
	ui->top_panel_button_2->setChecked(true);
	emit close_wait_dialog();
}

void MainWindow::total_progress(const int& val)
{
	ui->progressBar->setRange(0, val);
}
void MainWindow::current_progress(const int& val, const QString& message)
{
	ui->progressBar->setValue(val);
    set_progress_text(message);
}
void MainWindow::error(const QString& message)
{
	MessageDialog* error_message = new MessageDialog(message, this);
	error_message->show();
}
void MainWindow::done()
{
	ui->main_stack->setCurrentIndex(3);
	ui->top_panel_button_4->setChecked(true);
	ui->top_panel_button_1->setEnabled(true);
}
