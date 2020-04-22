#include "mainwindow.hpp"
#include "file_manager.hpp"

#include <QThread>

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);


	QCoreApplication::setOrganizationName("Vernocte laboratories");
	QCoreApplication::setOrganizationDomain("vernocte.org");
	QCoreApplication::setApplicationName("FolderSync");
	QCoreApplication::setApplicationVersion("0.8.0");

	QThread file_thread;
	file_manager files;
	files.moveToThread(&file_thread);
	QObject::connect(&a, &QCoreApplication::aboutToQuit,
	[&]()
	{
		file_thread.quit();
		file_thread.wait();
	});

	MainWindow w;

	QObject::connect(&files, &file_manager::source_exists,
					 &w, &MainWindow::source_exists);
	QObject::connect(&files, &file_manager::destinations_exists,
					 &w, &MainWindow::destinations_exists);
	QObject::connect(&files, &file_manager::new_files,
					 &w, &MainWindow::new_files);
	QObject::connect(&files, &file_manager::update_files,
					 &w, &MainWindow::update_files);
	QObject::connect(&files, &file_manager::delete_files,
					 &w, &MainWindow::delete_files);
	QObject::connect(&files, &file_manager::total_progress,
					 &w, &MainWindow::total_progress);
	QObject::connect(&files, &file_manager::current_progress,
					 &w, &MainWindow::current_progress);
	QObject::connect(&files, &file_manager::error,
					 &w, &MainWindow::error);
	QObject::connect(&files, &file_manager::done,
					 &w, &MainWindow::done);

	QObject::connect(&w, &MainWindow::set_source,
					 &files, &file_manager::set_source);
	QObject::connect(&w, &MainWindow::set_destination,
					 &files, &file_manager::set_destination);
	QObject::connect(&w, &MainWindow::build_lists,
					 &files, &file_manager::build_lists);
	QObject::connect(&w, &MainWindow::synchronise,
					 &files, &file_manager::synchronise);

	file_thread.start();
	w.show();


	return a.exec();
}
