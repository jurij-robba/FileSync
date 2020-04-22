#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <QObject>
#include <QDir>

class file_manager : public QObject
{
	Q_OBJECT

private:

	QDir _source;
	QDir _destination;

	QStringList list_files(const QDir& dir);

signals:

	void source_exists(const bool& status);
	void destinations_exists(const bool& status);

	void new_files(const QStringList& files);
	void update_files(const QStringList& files);
	void delete_files(const QStringList& files);

	void total_progress(const int& val);
	void current_progress(const int& val, const QString& message);
	void error(const QString& message);
	void done();

public:

	explicit file_manager(QObject *parent = nullptr);

public slots:

	void set_source(const QString& path);
	void set_destination(const QString& path);
	void build_lists();
	void synchronise(const QStringList& new_files, const QStringList& updated_files,
					 const QStringList& deleted_files);

};

#endif
