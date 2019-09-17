#include "ui_CPuzzleWidget.h"

#include "CPuzzleWidget.hpp"
#include "ColorVector.hpp"

#include <QPainter>
#include <QDebug>

bool rectItemListCompare( RectItemList &A, RectItemList &B)
{
    RectItemList::iterator it1, it2;

    if(A.size() != B.size())
        return false;

    for( it1 = A.begin(), it2 = B.begin(); it1 != A.end() && it2 != B.end(); ++it1, ++it2)
    {
        if( *(*it1) != *(*it2))
            return false;
    }
    return true;
}

//-----
int pos2index( int x, int y, int size)
{    return x * size + y; }

// Puzzle widget
CPuzzleWidget::CPuzzleWidget(QWidget *parent) : QWidget(parent),
    m_ui(new Ui::CPuzzleWidget),
    m_scene( new QGraphicsScene())
{
    m_ui->setupUi(this);
    m_ui->m_puzzle_view->setScene( m_scene );

	m_puzzle.m_size = 0;
}

CPuzzleWidget::~CPuzzleWidget()
{
    this->clearRectList();
    delete m_ui;
    delete m_scene;
}


//------------------ cleanup

void CPuzzleWidget::clearRectList()
{
    m_scene->clear();

    if( !m_rect_list.size())
    {
        RectItemList::iterator it;

        for( it = m_rect_list.begin(); it != m_rect_list.end(); ++it)
            delete (*it);

        m_rect_list.clear();

        m_rect_list_answer.clear();
    }
}

//------------------ puzzle

void CPuzzleWidget::resizeRects()
{
    if( m_rect_list.size() != (m_puzzle.m_size * m_puzzle.m_size))
        return;

    RectItemList::iterator it = m_rect_list.begin();

    qreal rect_width = m_ui->m_puzzle_view->width() / m_puzzle.m_size;
    qreal rect_height = m_ui->m_puzzle_view->height() / m_puzzle.m_size;

    QSizeF	rect_size( rect_width, rect_height);

    for(int i = 0; i < m_puzzle.m_size; ++i)
    {
        for(int j = 0; j < m_puzzle.m_size; ++j)
        {
            QPointF top_left( rect_width * i, rect_height * j);

            (*it)->setRect( QRectF( top_left, rect_size) );
            ++it;
        }
    }
}

void CPuzzleWidget::setPuzzleInfo(const PuzzleInfo &puzzle_new)
{
    m_puzzle = puzzle_new;
    m_ui->m_puzzle_name_label->setText( m_puzzle.m_name );

    // clear rect list
    this->clearRectList();

    // setup color:
    ColorVector v_tl = m_puzzle.m_colors[0];	//top left
    ColorVector v1 = ColorVector( m_puzzle.m_colors[1]) - v_tl;
    ColorVector v2 = ColorVector( m_puzzle.m_colors[3]) - v_tl;
    v1 = v1 / m_puzzle.m_size;
    v2 = v2 / m_puzzle.m_size;

	for(int i = 0; i < m_puzzle.m_size; ++i)
	{
        for(int j = 0; j < m_puzzle.m_size; ++j)
		{
            CPuzzleRectItem* p = new CPuzzleRectItem();
            ColorVector tmp = v_tl + v1 * j + v2 * i;

            p->setColor( tmp.toQColor() );

            m_scene->addItem( p );

            m_rect_list.append( p );
            m_rect_list_answer.append( p );
		}
	}

    this->resizeRects();

    /*
    m_rect_list[ pos2index(0, 0, m_puzzle.m_size) ]->setColor( m_puzzle.m_colors[0] );
    m_rect_list[ pos2index(0, m_puzzle.m_size - 1, m_puzzle.m_size) ]->setColor( m_puzzle.m_colors[1] );
    m_rect_list[ pos2index(m_puzzle.m_size - 1, m_puzzle.m_size - 1, m_puzzle.m_size) ]->setColor( m_puzzle.m_colors[2] );
    m_rect_list[ pos2index(m_puzzle.m_size - 1, 0, m_puzzle.m_size) ]->setColor( m_puzzle.m_colors[3] );
    */
}

PuzzleInfo CPuzzleWidget::getPuzzleInfo() const
{
	return m_puzzle;
}

//------------------

void CPuzzleWidget::resizeEvent(QResizeEvent *event)
{
    this->resizeRects();
    QWidget::resizeEvent(event);
}

void CPuzzleWidget::mousePressEvent(QMouseEvent *event)
{
    QList<QGraphicsItem*>tmp_list = m_scene->selectedItems();

    if( tmp_list.size() == 2)
    {

    }
    else if(tmp_list.size() == 3)
    {

    }

    QWidget::mousePressEvent( event );
}
