#include "WidgetLinker.hh"

DoubleSpinBox::DoubleSpinBox(QWidget *parent) :
QDoubleSpinBox(parent)
{
	setDecimals(12);
	setSingleStep(0.01);
	setAccelerated(true);
	setLocale(QLocale("C"));
	connect(this, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
	[=](double d){ this->setToolTip(toStringNotFilled(d, 'f', 12)); });
}

QString DoubleSpinBox::textFromValue(double d) const
{
	return (toStringNotFilled(d, 'e', 12));
}

QString DoubleSpinBox::toStringNotFilled(double d, char f, int prec)
{
	QLocale l("C");
	QString s(l.toString(d, f, prec));
	int start = s.indexOf(l.decimalPoint()) + 2, end = s.indexOf('e'), i;
	if (end < 0) end = s.size();
	i = end - 1;
	while (i > start && s[i] == '0')
		--i;
	i += s[i] != '0';
	if (s[end] == 'e' && s[end + 2] == '0')
		s.remove(end + 2, 1); // e+0x
	return (s.remove(i, end - i));
}

WidgetLinker::WidgetLinker(QWidget *parent, QBoxLayout *layout) :
parent(parent), layout(layout)
{}

WidgetLinker::~WidgetLinker() {}

QCheckBox *WidgetLinker::addCheckBox(const QString &content)
{
	auto checkBox = new QCheckBox(content, parent);
	layout->addWidget(checkBox);
	return (checkBox);
}

QComboBox *WidgetLinker::addComboBox()
{
	auto comboBox = new QComboBox(parent);
	layout->addWidget(comboBox);
	return (comboBox);
}

DoubleSpinBox *WidgetLinker::addDoubleSpinBox()
{
	auto spinBox = new DoubleSpinBox(parent);
	layout->addWidget(spinBox);
	return (spinBox);
}

QLabel *WidgetLinker::addLabel(const QString &content)
{
	auto label = new QLabel(parent);
	label->setText(content);
	layout->addWidget(label);
	return (label);
}

QLayoutItem *WidgetLinker::addLayoutItem(QLayoutItem *item)
{
	layout->addItem(item);
	return (item);
}

QLineEdit *WidgetLinker::addLineEdit(const QString &content)
{
	auto lineEdit = new QLineEdit(parent);
	lineEdit->setText(content);
	layout->addWidget(lineEdit);
	return (lineEdit);
}

QPushButton *WidgetLinker::addOKButton(bool link)
{
	layout->addItem(new QSpacerItem(0, 20, QSizePolicy::Minimum, QSizePolicy::Maximum));
	auto hLayout = new QHBoxLayout();
	layout->addLayout(hLayout);
	hLayout->addItem(new QSpacerItem(20, 0, QSizePolicy::Expanding, QSizePolicy::Maximum));
	auto button = new QPushButton(parent);
	button->setText("OK");
	hLayout->addWidget(button);
	button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	if (link)
		parent->connect(button, SIGNAL(released()), parent, SLOT(close()));
	return (button);
}

QPushButton *WidgetLinker::addPushButton(const QString &content)
{
	auto button = new QPushButton(parent);
	button->setText(content);
	layout->addWidget(button);
	return (button);
}

QRadioButton *WidgetLinker::addRadioButton(const QString &content)
{
	auto button = new QRadioButton(parent);
	button->setText(content);
	layout->addWidget(button);
	return (button);
}

QSpinBox *WidgetLinker::addSpinBox()
{
	auto spinBox = new QSpinBox(parent);
	layout->addWidget(spinBox);
	return (spinBox);
}

QTabWidget *WidgetLinker::addTabWidget()
{
	auto widget = new QTabWidget(parent);
	layout->addWidget(widget);
	return (widget);
}

QTableWidget *WidgetLinker::addTableWidget()
{
	auto widget = new QTableWidget(parent);
	layout->addWidget(widget);
	return (widget);
}

QTextEdit *WidgetLinker::addTextEdit(const QString &content)
{
	auto textEdit = new QTextEdit(parent);
	textEdit->setText(content);
	layout->addWidget(textEdit);
	return (textEdit);
}

void WidgetLinker::set(QWidget *parent)
{
	this->parent = parent;
}

void WidgetLinker::set(QBoxLayout *layout)
{
	if (!layout->parent())
		this->layout->addLayout(layout);
	this->layout = layout;
}

void WidgetLinker::replace(QBoxLayout *layout, QBoxLayout *with)
{
	with->addLayout(layout);
	this->layout = layout;
}

void ListWidget::contextMenuEvent(QContextMenuEvent *event)
{
	bool select = true;
	if (!itemAt(event->pos()))
	{
		setCurrentItem(nullptr);
		select = false;
	}
	QMenu menu;
	QAction *act = menu.addAction(select ? "Clear" : "Clear All");
	act->setIcon(QIcon(":icons/remove.png"));
	if (!count()) act->setEnabled(false);
	if (menu.exec(event->globalPos()))
	{
		if (select)
		{
			qDeleteAll(selectedItems());
			setCurrentItem(nullptr);
		}
		else
			clear();
	}
}

void ListWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && !itemAt(event->pos()))
		setCurrentItem(nullptr);
}

ListWidget::ListWidget(QWidget *parent) :
QListWidget(parent)
{}

GateToolButton::GateToolButton(QWidget *parent) :
QToolButton(parent)
{
	setPopupMode(QToolButton::MenuButtonPopup);
	QObject::connect(this, SIGNAL(triggered(QAction*)), this, SLOT(setDefaultAction(QAction*)));
}

GraphicsView::GraphicsView(QWidget *parent) :
QGraphicsView(parent)
{
	setDragMode(QGraphicsView::ScrollHandDrag);
	setMouseTracking(true);
	viewport()->setMouseTracking(true);
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
	qreal f = 1;

	if (event->delta() < 0 && (f -= ZOOM_SCROLL) * zoom() < ZOOM_MIN)
		setZoom(ZOOM_MIN);
	else if (event->delta() > 0 && (f += ZOOM_SCROLL) * zoom() > ZOOM_MAX)
		setZoom(ZOOM_MAX);
	else
		scale(f, f);
}

void GraphicsView::setZoom(qreal factor)
{
	scale(0.1, 0.1);
	resetMatrix();
	scale(factor, factor);
}

qreal GraphicsView::zoom()
{
	return transform().m11();
}