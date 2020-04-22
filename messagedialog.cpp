#include "messagedialog.hpp"
#include "ui_messagedialog.h"

MessageDialog::MessageDialog(const QString& message, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MessageDialog)
{
	ui->setupUi(this);
	ui->message_label->setText(message);
}

MessageDialog::~MessageDialog()
{
	delete ui;
}

void MessageDialog::on_ok_button_clicked()
{
	accept();
}
