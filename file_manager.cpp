#include "file_manager.hpp"

#include <QDirIterator>
#include <QDateTime>

QStringList file_manager::list_files(const QDir& dir)
{
	QStringList files;
	QDirIterator it(dir, QDirIterator::Subdirectories);
	while(it.hasNext())
	{
		files << it.next().replace(dir.path(), "");
	}
	return files;
}


file_manager::file_manager(QObject *parent) : QObject(parent) { }

void file_manager::set_source(const QString& path)
{
	_source.setPath(path);
	emit source_exists(_source.exists());
}
void file_manager::set_destination(const QString& path)
{
	_destination.setPath(path);
	emit destinations_exists(_destination.exists());
}

void file_manager::build_lists()
{

	_source.setFilter(QDir::Files | QDir::Dirs | QDir::NoDot | QDir::NoDotDot);
	_destination.setFilter(QDir::Files | QDir::Dirs | QDir::NoDot | QDir::NoDotDot);

	QStringList source_files = list_files(_source);
	QStringList destination_files = list_files(_destination);

	{
		QStringList new_files_list;
		QStringList update_files_list;

		for(const QString& file : source_files)
		{
			if(!destination_files.contains(file))
			{
				new_files_list << file;
			}
			else if(QFileInfo(_source.path()+file).lastModified() >
                    QFileInfo(_destination.path()+file).lastModified() && QFileInfo(_destination.path()+file).isFile())
			{
				update_files_list << file;
			}
		}
		emit new_files(new_files_list);
		emit update_files(update_files_list);
	}

	QStringList deleted_files_list;

	for(const QString& file : destination_files)
	{
		if(!source_files.contains(file))
		{
			deleted_files_list << file;
		}
	}

	emit delete_files(deleted_files_list);
}

void file_manager::synchronise(const QStringList& new_files, const QStringList& updated_files,
				 const QStringList& deleted_files)
{
	emit total_progress(new_files.count() + updated_files.count() + deleted_files.count());
	int progress = 1;

	for(auto& file : new_files)
    {
        emit current_progress(progress, "Copying " + file);
        if(QFileInfo(_source.path()+file).isFile() && !QFile::copy(_source.path() + file, _destination.path()+file))
        {
			emit error("Unable to copy " + file + " to destination directory!");
		}
        else if(QFileInfo(_source.path()+file).isDir() && !QDir().mkdir(_destination.path()+file))
        {
            emit error("Unable to copy " + file + " to destination directory!");
        }
		++progress;
	}
	for(auto it = updated_files.rbegin(); it != updated_files.rend(); ++it)
	{
		emit current_progress(progress, "Updating " + *it);
		if(QFileInfo(_destination.path()+*it).isDir())
		{
			continue;
		}
		if(!QFile::remove(_destination.path()+*it))
		{
			emit error("Unable to override " + *it + " in destination directory!");
			continue;
		}
		if(!QFile::copy(_source.path() + *it, _destination.path()+*it))
		{
			emit error("Unable to copy " + *it + " to destination directory!");
		}
		++progress;
	}
	for(auto it = deleted_files.rbegin(); it != deleted_files.rend(); ++it)
	{
		emit current_progress(progress, "Deleting " + *it);
		if(QFileInfo(_destination.path()+*it).isFile() && !QFile::remove(_destination.path()+*it))
		{
			emit error("Unable to delete " + *it + " in destination directory!");
		}
		else if(QFileInfo(_destination.path()+*it).isDir() && !QDir(_destination.path()+*it).removeRecursively())
		{
			emit error("Unable to delete flder " + *it + " in destination directory!");
		}
		++progress;
	}
	emit done();
}
