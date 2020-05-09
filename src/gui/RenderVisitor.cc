#include "EditVisitor.hh"

NodeItem::NodeItem(QMenu *contextMenu, QPixmap icon, Node *n, Properties *prop, bool isChild) :
QGraphicsRectItem(n->getPosition().x(), n->getPosition().y(), CARD_X, CARD_Y),
contextMenu(contextMenu), icon(icon), n(n), prop(prop), child(isChild)
{
	setFlag(ItemIsSelectable);
}

bool NodeItem::isChild()
{
	return (child);
}

Node *NodeItem::node()
{
	return (n);
}

void NodeItem::setProperties(Properties *prop)
{
	this->prop = prop;
	scene()->update();
}

QRectF NodeItem::boundingRect() const
{
	return QRectF(n->getPosition().x(), n->getPosition().y(), CARD_X, CARD_Y);
}

void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	(void)option;
	(void)widget;
	QRectF r = boundingRect();
	r.setHeight(r.height() - ICON_RSIZE);
	QPen pen(Qt::black, BORDER_SIZE);
	painter->setPen(pen);
	painter->setRenderHint(QPainter::Antialiasing);
	QPainterPath path;
	path.addRoundedRect(r, ICON_RSIZE / 4, ICON_RSIZE / 4);
	path.addRect(r.x(), r.y() + r.height() / 4 - BORDER_SIZE / 2, r.width(), 0);
	painter->setPen(pen);
	painter->fillPath(path, (isSelected() ? QColor(213, 236, 255) : Qt::white));
	painter->drawPath(path);
	QPointF p(r.x() + ICON_RSIZE, r.y() + 1.89 * ICON_RSIZE + BORDER_SIZE - 1);
	painter->drawPixmap(p, icon);
	if (prop)
	{
		QFont font = painter->font();
		font.setPixelSize(ICON_RSIZE / 4);
		painter->setFont(font);
		QRect r2(r.x() + ICON_RSIZE / 4, r.y() + ICON_RSIZE / 8,
		r.width() - ICON_RSIZE / 2, r.height() / 4 - BORDER_SIZE);
		painter->drawText(r2, Qt::AlignLeft, prop->getName());
		r2.setY(r.y() + r.height() / 4);
		r2.setHeight(r.height() - r.height() / 4 - BORDER_SIZE);
		painter->drawText(r2, Qt::TextWordWrap, prop->getDesc());
	}
}

void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
		setSelected(true);
	// else ignore
	QGraphicsItem::mouseReleaseEvent(event);
}

void NodeItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	setSelected(true);
	contextMenu->exec(event->screenPos()); // Calls MainWindow dedicated context menu
}

RenderVisitor::RenderVisitor(MainWindow &win, Node *selection) :
win(win), selection(selection)
{}

// Visits the children of gate recursively
static void visitChildren(Gate &gate, RenderVisitor &visitor)
{
	QList<Node*> &children = gate.getChildren();
	QList<Node*>::const_iterator i;
	for (i = children.constBegin(); i != children.constEnd(); ++i)
		(*i)->accept(visitor);
}

void RenderVisitor::visit(And &gate)
{
	visitChildren(gate, *this);
	static QPixmap icon(QPixmap(":objects/and.png").scaled(ICON_RSIZE, ICON_RSIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	auto n = new NodeItem(win.itemsContextMenu(), icon, &gate, &gate.getProperties());
	if (&gate == selection) n->setSelected(true);
	win.getScene()->addItem(n);
}

void RenderVisitor::visit(Or &gate)
{
	visitChildren(gate, *this);
	static QPixmap icon(QPixmap(":objects/or.png").scaled(ICON_RSIZE, ICON_RSIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	auto n = new NodeItem(win.itemsContextMenu(), icon, &gate, &gate.getProperties());
	if (&gate == selection) n->setSelected(true);
	win.getScene()->addItem(n);
}

void RenderVisitor::visit(Xor &gate)
{
	visitChildren(gate, *this);
	static QPixmap icon(QPixmap(":objects/xor.png").scaled(ICON_RSIZE, ICON_RSIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	auto n = new NodeItem(win.itemsContextMenu(), icon, &gate, &gate.getProperties());
	if (&gate == selection) n->setSelected(true);
	win.getScene()->addItem(n);
}

void RenderVisitor::visit(VotingOR &gate)
{
	visitChildren(gate, *this);
	static QPixmap icon(QPixmap(":objects/kn.png").scaled(ICON_RSIZE, ICON_RSIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	auto n = new NodeItem(win.itemsContextMenu(), icon, &gate, &gate.getProperties());
	if (&gate == selection) n->setSelected(true);
	win.getScene()->addItem(n);
}

void RenderVisitor::visit(Inhibit &gate)
{
	visitChildren(gate, *this);
	static QPixmap icon(QPixmap(":objects/inhibit.png").scaled(ICON_RSIZE, ICON_RSIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	auto n = new NodeItem(win.itemsContextMenu(), icon, &gate, &gate.getProperties());
	if (&gate == selection) n->setSelected(true);
	win.getScene()->addItem(n);
}

void RenderVisitor::visit(Transfert &gate)
{
	Properties *prop = gate.getLink() ? &gate.getLink()->getProperties() : nullptr;
	static QPixmap icon(QPixmap(":objects/transfert.png").scaled(ICON_RSIZE, ICON_RSIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	auto n = new NodeItem(win.childItemsContextMenu(), icon, &gate, prop, true);
	if (&gate == selection) n->setSelected(true);
	win.getScene()->addItem(n);
}

void RenderVisitor::visit(Container &cont)
{
	static QPixmap icon(QPixmap(":objects/basicEvent.png").scaled(ICON_RSIZE, ICON_RSIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	auto n = new NodeItem(win.childItemsContextMenu(), icon, &cont, &cont.getEvent()->getProperties(), true);
	if (&cont == selection) n->setSelected(true);
	win.getScene()->addItem(n);
}