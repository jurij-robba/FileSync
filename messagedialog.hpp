#ifndef PLEASEWAITDIALOG_HPP
#define PLEASEWAITDIALOG_HPP

#include <QDialog>

namespace Ui {
	class MessageDialog;
}

class MessageDialog : public QDialog
{
	Q_OBJECT

private:
	Ui::MessageDialog *ui;

public:
	explicit MessageDialog(const QString& message, QWidget *parent = nullptr);
	~MessageDialog();

public slots:

	void on_ok_button_clicked();
};

#endif
