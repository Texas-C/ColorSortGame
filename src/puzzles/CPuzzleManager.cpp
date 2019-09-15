#include "CPuzzleManager.hpp"
#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define NUM2COLOR(value) (value < 0? 0 : (value > 255? 255 : value))

//---------- ColorVector
ColorVector::ColorVector() : m_r(0), m_g(0), m_b(0)
{}

ColorVector::ColorVector( int r, int g, int b) : m_r(r), m_g(g), m_b(b)
{}

ColorVector::ColorVector(const QColor &color)
{
    m_r = color.red();
    m_g = color.green();
    m_b = color.blue();
}

QColor ColorVector::toQColor()
{	return QColor( NUM2COLOR(m_r), NUM2COLOR(m_g), NUM2COLOR(m_b));	}

ColorVector ColorVector::operator + (const ColorVector &other)
{	return ColorVector( m_r + other.m_r, m_g + other.m_g, m_b + other.m_b);}

ColorVector ColorVector::operator - (const ColorVector &other)
{	return ColorVector( m_r - other.m_r, m_g - other.m_g, m_b - other.m_b);}

ColorVector ColorVector::operator * ( const double &k)
{	return ColorVector( m_r * k, m_g * k,  m_b * k);	}

ColorVector ColorVector::operator / ( const double &k)
{	return ColorVector( m_r / k, m_g / k,  m_b / k);	}

//---------- CPuzzleInfo
PuzzleInfo::PuzzleInfo() : m_name("Unknow"), m_size(3)
{
	for(int i = 0; i < PuzzleInfo::KNOWED_COLOR_CNT; ++i)
		m_colors[i] = Qt::white;
}

//---------- CPuzzleManager --- functions
QByteArray read_json_from_file( const QString &filename)
{
	QFile file(filename);

	if( !file.open( QIODevice::ReadOnly ))
	{
		qDebug() << "Read" << filename << "failed";
		return QByteArray();
	}

	QByteArray buff = file.readAll();
	file.close();
	return buff;
}

// QString to QColor
QColor qstr2qcolor( QString str)
{
	QByteArray buf = str.toUtf8();
	if( buf.size() )
	{
		int r, g, b;
		sscanf(buf.data(), "#%2x%2x%2x", &r, &g, &b);
		return QColor(r, g, b);
	}
	return Qt::white;	// return white as default
}

bool init_puzzles_from_json( QList<PuzzleInfo> &puzzle_list, const QJsonDocument &json_colors, const QJsonDocument &json_puzzles)
{
	if( (!json_colors.isObject()) || (!json_puzzles.isArray()) )
		return false;

	QJsonObject	json_color_container;

	QJsonArray	json_puzzle_array;
	QJsonArray::iterator it;

	json_color_container	= json_colors.object();
	json_puzzle_array		= json_puzzles.array();

	// load puzzle infomation to puzzle list
	for(it = json_puzzle_array.begin(); it != json_puzzle_array.end(); ++it)
	{
		PuzzleInfo	tmp_puzzle;
		QJsonObject	tmp_object	= it->toObject();

		tmp_puzzle.m_name = tmp_object.value("puzzleName").toString();
		tmp_puzzle.m_size = tmp_object.value("size").toInt();

		QJsonArray	tmp_array	= json_color_container.find( tmp_puzzle.m_name)->toArray();

		for(int i = 0; i < tmp_array.size(); ++i)
			tmp_puzzle.m_colors[i] = qstr2qcolor( tmp_array[i].toString() );

		puzzle_list.push_back( tmp_puzzle );
	}

	return true;
}

//---------- CPuzzleManager --- member methods
CPuzzleManager::CPuzzleManager()
{
    // load original puzzle list from color.jsons and puzzles.json
	QByteArray array_colors = read_json_from_file( ":/res/puzzles/colors.json");
	QByteArray array_puzzles = read_json_from_file( ":/res/puzzles/puzzles.json");

	QJsonDocument json_colors = QJsonDocument::fromJson( array_colors);
	QJsonDocument json_puzzles = QJsonDocument::fromJson( array_puzzles);

	init_puzzles_from_json( m_puzzle_list, json_colors, json_puzzles);
}

// get puzzle by index
PuzzleInfo CPuzzleManager::getPuzzle(int index) const
{
	if( index >= 0 && index < m_puzzle_list.size())
		return m_puzzle_list[index];
	else
	{
		qDebug() << "invalid puzzle request";
		return PuzzleInfo();
	}
}
